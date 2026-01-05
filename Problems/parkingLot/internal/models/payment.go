package models

type PaymentMethod interface {
	Pay(amount float64) bool
}
