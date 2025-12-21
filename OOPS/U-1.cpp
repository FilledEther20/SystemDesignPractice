#include <iostream>
using namespace std;

class Vector{
    int size;
    int capacity;
    int *arr;
public:
    Vector(){
        size=0;
        capacity=1;
        arr=new int[capacity];
    }

    void add(int ele){
        if(size==capacity){
            capacity=capacity*2;
            int *arr2=new int[capacity];
            for(int i=0;i<size;i++){
                arr2[i]=arr[i];
            }
        }
        arr[size++]=ele;
    }
};

int main(){
    Vector v1;
    v1.add(10);

}