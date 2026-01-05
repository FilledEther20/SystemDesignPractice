package models

import "time"

type CostStrategy interface {
	CalculateCost(entryTime, exitTime time.Time, vehicleType VehicleType) float64
}
