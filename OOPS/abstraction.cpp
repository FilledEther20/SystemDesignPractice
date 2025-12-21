#include<iostream>
#include<string>
using namespace std;

class Car{
public:
    virtual void startEngine()=0;
    virtual void stopEngine()=0;
    virtual void shiftGear(int gear)=0;
    virtual void accelerate()=0;
    virtual void reverse()=0;
    virtual void brake()=0;
    virtual ~Car(){};
};

class SportsCar:public Car{
public:    
    string brand;
    string model;
    bool isEngineOn;
    int currentSpeed;
    int currentGear;

    SportsCar(string brand,string model){
        this->brand=brand;
        this->model=model;
        isEngineOn=false;
        currentSpeed=0;
        currentGear=0;
    }

    void startEngine(){
        isEngineOn=true;
        cout<<brand<<" "<<model<<" "<<"Engine started"<<endl;
    }

    void shiftGear(int gear){
        if(!isEngineOn){
            cout<<brand<<" "<<model<<" "<<"Engine Is Not Started! Cannot shift gear"<<endl;
            return;
        }
        currentGear=gear;
        cout<<brand<<" "<<model<<" "<<"Gear shifted to "<<gear<<endl;
    }

    void accelerate(){
        if(!isEngineOn){
            cout<<brand<<" "<<model<<" "<<" Engine is off! Cannot accelerate"<<endl;
            return;
        }
        currentSpeed+=20;
        cout<<brand<<" "<<model<<" "<<" Accelerated by 20km/h"<<endl;
    }

    void brake(){
        if(currentSpeed==0){
            cout<<brand<<" "<<model<<" "<<" Speed is already 0km/h "<<endl;
            return;
        }
        currentSpeed=((currentSpeed-20)<0)?0:currentSpeed-20;
        cout<<brand<<" "<<model<<" "<<"Brakes applied"<<endl;
    }

    void stopEngine(){
        isEngineOn=false;
        currentGear=0;
        currentSpeed=0;
    }
};


