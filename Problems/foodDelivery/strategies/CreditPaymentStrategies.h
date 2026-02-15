#ifndef CREDIT_PAYMENT_STRATEGIES
#define CREDIT_PAYMENT_STRATEGIES

#include "PaymentStrategy.h"
#include<iostream>
#include<string>
#include<iomanip>

using namespace std;

class CreditPaymentStrategy:public PaymentStrategy{
private:
    string cardNumber;

public:
    CreditPaymentStrategy(const string &card){
        cardNumber=card;
    }

    void pay(double amount) override{
        cout << "Paid ₹" << amount << " using Credit Card (" << cardNumber << ")" << endl;
    }
};

#endif