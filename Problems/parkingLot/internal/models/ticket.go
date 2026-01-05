package models

import "time"

type Ticket struct {
	ID        int
	Vehicle   *Vehicle
	Slot      *Slot
	EntryTime time.Time
	Cost      float64
	Paid      bool
	Payment   PaymentMethod
}
