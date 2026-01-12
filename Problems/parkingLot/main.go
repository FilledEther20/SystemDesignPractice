package main

import (
	"errors"
	"fmt"
	"math"
	"strconv"
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
	var arr = [...]string{"Small", "Medium", "Large"}
	return arr[v]
}

func (s SpotType) String() string {
	return [...]string{"Compact", "Regular", "Oversized"}[s]
}

/*
	Objects
*/

type Vehicle struct {
	LicensePlate string
	Size         VehicleSize
}

type ParkingSpot struct {
	SpotID         string
	Type           SpotType
	IsOccupied     bool
	CurrentVehicle *Vehicle
}

func (p *ParkingSpot) CanFit(v *Vehicle) bool {
	if p.IsOccupied {
		return false
	}
	return int(p.Type) >= int(v.Size)
}

func (p *ParkingSpot) Occupy(v *Vehicle) {
	p.IsOccupied = true
	p.CurrentVehicle = v
}

func (p *ParkingSpot) Vacate() {
	p.IsOccupied = false
	p.CurrentVehicle = nil
}

type Ticket struct {
	ID           string
	VehiclePlate string
	SpotID       string
	EntryTime    time.Time
}

/*
Strategy Pattern for fees calculation
*/

type FareStrategy interface {
	Calculate(t Ticket, vsize VehicleSize, duration float64) float64
}

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

/*
	ParkingManager
*/

type ParkingManager struct {
	spots []*ParkingSpot
	mu    sync.RWMutex
}

func NewParkingManager(numCompact, numRegular, numOversized int) *ParkingManager {
	manager := &ParkingManager{
		spots: []*ParkingSpot{},
	}
	idCounter := 1
	addSpots := func(count int, sType SpotType) {
		for i := 0; i < count; i++ {
			manager.spots = append(manager.spots, &ParkingSpot{
				SpotID: strconv.Itoa(idCounter), Type: sType,
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
	return nil, errors.New("no available spot found for this vehicle size.")
}

func (pm *ParkingManager) ReleaseSpot(spotID string) error {
	pm.mu.Lock()
	defer pm.mu.Unlock()

	for _, spot := range pm.spots {
		if spot.SpotID == spotID {
			if !spot.IsOccupied {
				return errors.New("spot is already empty")
			}
			spot.Vacate()
			return nil
		}
	}
	return errors.New("spot not found")
}

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
		SpotID:       spot.SpotID,
		EntryTime:    time.Now(),
	}

	pl.mu.Lock()
	pl.tickets[ticketID] = ticket
	pl.mu.Unlock()
	fmt.Printf("[Entry] %s (%s) parked at Spot %d (%s). Ticket: %s\n",
		v.LicensePlate, v.Size, spot.SpotID, spot.Type, ticketID)
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

	fee := pl.calculator.CalculateFee(*ticket, vSize)

	err := pl.manager.ReleaseSpot((ticket.SpotID))
	if err != nil {
		return fee, fmt.Errorf("fee %.2f calculated, but error releasing spot: %v", fee, err)
	}

	fmt.Printf("[Exit] Ticket %s paid $%.2f and left Spot %d.\n", ticketID, fee, ticket.SpotID)
	return fee, nil
}

func main() {
	manager := NewParkingManager(2, 2, 1)
	parkingLot := NewParkingLot(manager)

	fmt.Println(" Parking Lot System Started ")

	vehicles := []*Vehicle{
		{LicensePlate: "MOTO-01", Size: Small},
		{LicensePlate: "CAR-01", Size: Medium},
		{LicensePlate: "TRUCK-01", Size: Large},
	}

	var tickets []*Ticket

	for _, v := range vehicles {
		ticket, err := parkingLot.ParkVehicle(v)
		if err != nil {
			fmt.Println("[Entry Failed]", err)
			continue
		}
		tickets = append(tickets, ticket)
	}

	_, err := parkingLot.ParkVehicle(&Vehicle{
		LicensePlate: "TRUCK-02",
		Size:         Large,
	})
	if err != nil {
		fmt.Println("[Entry Denied]", err)
	}

	time.Sleep(2 * time.Second)

	fmt.Println("\n--- Processing Exits ---")

	for i, ticket := range tickets {
		fee, err := parkingLot.ExitVehicle(ticket.ID, vehicles[i].Size)
		if err != nil {
			fmt.Println("[Exit Error]", err)
			continue
		}
		fmt.Printf("Vehicle %s paid ₹%.2f\n", vehicles[i].LicensePlate, fee)
	}
}
