package main

import (
	"errors"
	"fmt"
	"math"
	"sync"
	"time"
)

type VehicleSize int
type SpotType int

const (
	Small VehicleSize = iota
	Medium
	Large
)

const (
	Compact SpotType = iota
	Regular
	Oversized
)

func (v VehicleSize) String() string {
	return [...]string{"Small", "Medium", "Large"}[v]
}

func (s SpotType) String() string {
	return [...]string{"Compact", "Regular", "Oversized"}[s]
}


type Vehicle struct {
	LicensePlate string
	Size         VehicleSize
}

type ParkingSpot struct {
	ID             int
	Type           SpotType
	IsOccupied     bool
	CurrentVehicle *Vehicle
}

func (ps *ParkingSpot) CanFit(v *Vehicle) bool {
	return int(ps.Type) >= int(v.Size)
}

// Occupy assigns a vehicle to the spot.
func (ps *ParkingSpot) Occupy(v *Vehicle) {
	ps.IsOccupied = true
	ps.CurrentVehicle = v
}

// Vacate removes the vehicle from the spot.
func (ps *ParkingSpot) Vacate() {
	ps.IsOccupied = false
	ps.CurrentVehicle = nil
}

// Ticket represents the contract for the parking session.
type Ticket struct {
	ID           string
	VehiclePlate string
	SpotID       int
	EntryTime    time.Time
}

// ==========================================
// 3. Strategy Pattern for Fees
// ==========================================

// FareStrategy defines the interface for calculating parking fees.
type FareStrategy interface {
	Calculate(t Ticket, vSize VehicleSize, durationHours float64) float64
}

// BaseFareStrategy implements standard hourly rates.
type BaseFareStrategy struct{}

func (b *BaseFareStrategy) Calculate(t Ticket, vSize VehicleSize, durationHours float64) float64 {
	rate := 0.0
	switch vSize {
	case Small:
		rate = 10.0
	case Medium:
		rate = 20.0
	case Large:
		rate = 30.0
	}
	return rate * durationHours
}

type PeakHoursFareStrategy struct{}

func (p *PeakHoursFareStrategy) Calculate(t Ticket, vSize VehicleSize, durationHours float64) float64 {
	baseStrategy := &BaseFareStrategy{}
	baseFee := baseStrategy.Calculate(t, vSize, durationHours)
	
	currentHour := time.Now().Hour()
	if currentHour >= 17 && currentHour <= 20 {
		return baseFee * 1.5 
	}
	return baseFee
}


type FareCalculator struct {
	strategy FareStrategy
}

func (fc *FareCalculator) CalculateFee(t Ticket, vSize VehicleSize) float64 {
	duration := time.Since(t.EntryTime)
	hours := math.Ceil(duration.Hours())
	if hours == 0 {
		hours = 1
	}
	return fc.strategy.Calculate(t, vSize, hours)
}

// ==========================================
// 4. Logic Components (Manager)
// ==========================================

// ParkingManager manages the allocation and release of spots.
type ParkingManager struct {
	spots []*ParkingSpot
	mu    sync.RWMutex // Ensures thread safety for concurrent access
}

func NewParkingManager(numCompact, numRegular, numOversized int) *ParkingManager {
	manager := &ParkingManager{
		spots: []*ParkingSpot{},
	}
	idCounter := 1
	addSpots := func(count int, sType SpotType) {
		for i := 0; i < count; i++ {
			manager.spots = append(manager.spots, &ParkingSpot{
				ID: idCounter, Type: sType,
			})
			idCounter++
		}
	}
	addSpots(numCompact, Compact)
	addSpots(numRegular, Regular)
	addSpots(numOversized, Oversized)
	return manager
}

func (pm *ParkingManager) FindAndAssignSpot(v *Vehicle) (*ParkingSpot, error) {
	pm.mu.Lock()
	defer pm.mu.Unlock()

	for _, spot := range pm.spots {
		if !spot.IsOccupied && spot.CanFit(v) {
			spot.Occupy(v)
			return spot, nil
		}
	}
	return nil, errors.New("no available spot found for this vehicle size")
}

func (pm *ParkingManager) ReleaseSpot(spotID int) error {
	pm.mu.Lock()
	defer pm.mu.Unlock()

	for _, spot := range pm.spots {
		if spot.ID == spotID {
			if !spot.IsOccupied {
				return errors.New("spot is already empty")
			}
			spot.Vacate()
			return nil
		}
	}
	return errors.New("spot not found")
}

// ==========================================
// 5. Facade (ParkingLot)
// ==========================================

type ParkingLot struct {
	manager    *ParkingManager
	calculator *FareCalculator
	tickets    map[string]*Ticket
	mu         sync.Mutex
}

func NewParkingLot(manager *ParkingManager) *ParkingLot {

	return &ParkingLot{
		manager:    manager,
		calculator: &FareCalculator{strategy: &PeakHoursFareStrategy{}},
		tickets:    make(map[string]*Ticket),
	}
}


func (pl *ParkingLot) ParkVehicle(v *Vehicle) (*Ticket, error) {
	spot, err := pl.manager.FindAndAssignSpot(v)
	if err != nil {
		return nil, fmt.Errorf("parking failed: %v", err)
	}

	ticketID := fmt.Sprintf("TKT-%d-%s", time.Now().UnixNano()/1000, v.LicensePlate)
	ticket := &Ticket{
		ID:           ticketID,
		VehiclePlate: v.LicensePlate,
		SpotID:       spot.ID,
		EntryTime:    time.Now(),
	}

	// 3. Persist Ticket
	pl.mu.Lock()
	pl.tickets[ticketID] = ticket
	pl.mu.Unlock()

	fmt.Printf("[Entry] %s (%s) parked at Spot %d (%s). Ticket: %s\n", 
        v.LicensePlate, v.Size, spot.ID, spot.Type, ticketID)
	return ticket, nil
}

// ExitVehicle handles the exit workflow.
func (pl *ParkingLot) ExitVehicle(ticketID string, vSize VehicleSize) (float64, error) {
	pl.mu.Lock()
	ticket, exists := pl.tickets[ticketID]
	if !exists {
		pl.mu.Unlock()
		return 0, errors.New("invalid ticket")
	}
	delete(pl.tickets, ticketID)
	pl.mu.Unlock()

	// 1. Calculate Fee via Strategy
	fee := pl.calculator.CalculateFee(*ticket, vSize)

	// 2. Free the Spot via Manager
	err := pl.manager.ReleaseSpot(ticket.SpotID)
	if err != nil {
		return fee, fmt.Errorf("fee %.2f calculated, but error releasing spot: %v", fee, err)
	}

	fmt.Printf("[Exit] Ticket %s paid $%.2f and left Spot %d.\n", ticketID, fee, ticket.SpotID)
	return fee, nil
}

func main() {
	manager := NewParkingManager(2, 2, 1)
	parkingLot := NewParkingLot(manager)

	fmt.Println("--- Parking Lot System Started ---")

	moto := &Vehicle{LicensePlate: "MOTO-01", Size: Small}
	ticket1, _ := parkingLot.ParkVehicle(moto)

	car := &Vehicle{LicensePlate: "CAR-01", Size: Medium}
	ticket2, _ := parkingLot.ParkVehicle(car)

	truck := &Vehicle{LicensePlate: "TRUCK-01", Size: Large}
	ticket3, _ := parkingLot.ParkVehicle(truck)

	truck2 := &Vehicle{LicensePlate: "TRUCK-02", Size: Large}
	_, err := parkingLot.ParkVehicle(truck2)
	if err != nil {
		fmt.Printf("[Entry Denied] %v\n", err)
	}

	time.Sleep(500 * time.Millisecond)
	fmt.Println("\n--- Processing Exits ---")

	parkingLot.ExitVehicle(ticket2.ID, Medium)

	car2 := &Vehicle{LicensePlate: "CAR-02", Size: Medium}
	parkingLot.ParkVehicle(car2)
    
	parkingLot.ExitVehicle(ticket1.ID, Small)
    
 	parkingLot.ExitVehicle(ticket3.ID, Large)
}