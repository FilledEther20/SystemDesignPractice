#include <iostream>
using namespace std;
class Car
{
protected:
    string brand;
    string model;
    bool isEngineOn;
    int currentSpeed;

public:
    Car(string b, string model)
    {
        this->brand = b;
        this->model = model;
        this->isEngineOn = false;
        this->currentSpeed = 0;
    }

    void startCar()
    {
        isEngineOn = true;
        cout<<"Car started"<<endl;
    }

    int getSpeed()
    {
        return currentSpeed;
    }

    void stopEngine(){
        cout<<"Engine stopping"<<endl;
        isEngineOn=false;
        currentSpeed=0;
        cout<<"Engine Stopped"<<endl;
    }

    void accelerate(){
        if(!isEngineOn){
            cout<<"The engine is off so car cannot be accelerated"<<endl;
            return;
        }

        currentSpeed=currentSpeed+5;
        cout<<"Accelerated by 5 units"<<endl;
    }

    void brake(){
        if(!isEngineOn){
            cout<<"The engine is OFF"<<endl;
            return;
        }
        currentSpeed-=5;
        cout<<"Brakes applied"<<endl;
    }

};

class ManualCar:public Car{
    int currentGear;

public:
    ManualCar(string b,string m):Car(b,m){
        currentGear=0;
    }

    int getCurrentGear(){
        return currentGear;
    }

    void shiftGear(int g){
        currentGear=g;
        cout<<"Gear Shifted to "<<g<<endl;
    }
};

class ElectricCar:public Car{
    int batteryLevel;
public:
    ElectricCar(string b,string m):Car(b,m){
        batteryLevel=0;
    }
    void chargeBattery(){
        batteryLevel=100;
        cout<<brand<<" "<<model<<" Battery Charged"<<endl;
    }
};

int main()
{
    ManualCar *myManualCar=new ManualCar("maruti","Wagonr");
    myManualCar->startCar();
    myManualCar->shiftGear(3);
    myManualCar->accelerate();
    myManualCar->brake();
    myManualCar->stopEngine();
    delete myManualCar;

    ElectricCar *myElectricCar=new ElectricCar("Tesla","Model S");
    myElectricCar->chargeBattery();
    myElectricCar->startCar();
    myElectricCar->accelerate();
    delete myElectricCar;

    return 0;
}

/*
    Basically child joh hoga he would inherit few things
    aur jab inn hi few things ko change karte h toh we use the concept of polymorphism
*/