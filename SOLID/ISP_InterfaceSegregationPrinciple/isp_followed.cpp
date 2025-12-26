#include <iostream>
using namespace std;


class AreaShape {
public:
    virtual double area() const = 0;
    virtual ~AreaShape() = default;
};

class VolumeShape {
public:
    virtual double volume() const = 0;
    virtual ~VolumeShape() = default;
};


class Square : public AreaShape {
private:
    double side;

public:
    Square(double s) : side(s) {}

    double area() const override {
        return side * side;
    }
};

class Rectangle : public AreaShape {
private:
    double length, breadth;

public:
    Rectangle(double l, double b) : length(l), breadth(b) {}

    double area() const override {
        return length * breadth;
    }
};

class Cube : public AreaShape, public VolumeShape {
private:
    double side;

public:
    Cube(double s) : side(s) {}

    double area() const override {
        return 6 * side * side;
    }

    double volume() const override {
        return side * side * side;
    }
};


int main() {
    AreaShape* square = new Square(5);
    AreaShape* rectangle = new Rectangle(4, 6);
    Cube* cube = new Cube(3);

    cout << "Square Area: " << square->area() << endl;
    cout << "Rectangle Area: " << rectangle->area() << endl;
    cout << "Cube Area: " << cube->area() << endl;
    cout << "Cube Volume: " << cube->volume() << endl;

    delete square;
    delete rectangle;
    delete cube;

    return 0;
}
