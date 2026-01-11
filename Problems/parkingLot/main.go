package main

import (
	"math"
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
	ID             string
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
	
*/