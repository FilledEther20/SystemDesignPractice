package models

type VehicleType string

const (
	TwoWheeler  VehicleType = "2W"
	FourWheeler VehicleType = "4W"
)

type Vehicle struct{
	Number string
	Type VehicleType
}

