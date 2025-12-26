// Principle Clients should not be forced to depend on interfaces they donot use
// Its better to have many small,client-specific interfaces than one large, general purpose interface.


#include<iostream>
#include<vector>
using namespace std;
class Shape{
    int length;
    int breadth;

public:
    virtual double area()=0;
    virtual double vol()=0;
};

class Square:public Shape{
private:
    double side;
public:    
    Square(double s){
        this->side=s;
    }
    double area(){
        cout<<"Area Square ==> "<<side*side<<endl;
    }
    double vol(){
        cout<<"Volume Rectangle ==> "<<4*side<<endl;
    }
};

class Rectangle:public Shape{

private:
    double len,breadth;
public:
    Rectangle(double l,double b){
        this->len=l;
        this->breadth=b;
    }
    double area(){
        cout<<"Area Rectangle ==> "<<len*breadth<<endl;
    }

    double vol(){
        throw logic_error("Volume not applicable");
    }
};
class Cube : public Shape {
private:
    double side;

public:
    Cube(double s) : side(s) {}

    double area() override {
        return 6 * side * side;
    }

    double vol() override {
        return side * side * side;
    }
};

int main() {
    Shape* square = new Square(5);
    Shape* rectangle = new Rectangle(4, 6);
    Shape* cube = new Cube(3);

    cout << "Square Area: " << square->area() << endl;
    cout << "Rectangle Area: " << rectangle->area() << endl;
    cout << "Cube Area: " << cube->area() << endl;
    cout << "Cube Volume: " << cube->vol() << endl;

    try {
        cout << "Square Volume: " << square->vol() << endl;
    } catch (logic_error& e) {
        cout << "Exception: " << e.what() << endl;
    }
    
    return 0;
}