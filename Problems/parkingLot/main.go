package main

import "time"

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

type FareStrategy interface {
	Calculate(t Ticket, vsize VehicleSize, spotType SpotType, duration time.Time)
}
