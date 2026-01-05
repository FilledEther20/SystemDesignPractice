package models

import "errors"

type Slot struct {
	ID         int
	Vehicle    *Vehicle
	isOccupied bool
	Distance   int
}

func (s *Slot) Park(vehicle *Vehicle) error {
	if s.isOccupied {
		return errors.New("The slot is already occupied")
	}
	s.isOccupied = true
	s.Vehicle = vehicle
	return nil
}

func (s *Slot) Unpark() {
	s.Vehicle = nil
	s.isOccupied = false
}
