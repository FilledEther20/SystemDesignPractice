#include<iostream>

using namespace std;

// class NoSingleton{
//     public:
//         NoSingleton(){
//             cout<<"No singleton"<<endl;
//         }
// };

class Singleton{
private:
    static Singleton* instance;
    Singleton() {
        cout << "Singleton Constructor called" << endl;
    }
public:
    static Singleton* getInstance(){
        if(instance==nullptr){
            instance = new Singleton();
        }
        return instance;
    }
};

Singleton* Singleton::instance=nullptr;
int main() {
    Singleton* s1 = Singleton::getInstance();
    Singleton* s2 = Singleton::getInstance();

    cout << (s1 == s2) << endl;
}

