// MOST IMPORTANT THING “Never optimize away a function call unless you have measured a problem.”

#include<iostream>
#include<vector>
using namespace std;

class Product{
public:
    string name;
    double price;
    Product(string n,double d){
        this->name=n;
        this->price=d;
    }
};

class ShoppingCart{
private:
    vector<Product*> products;

public:
    void addProduct(Product *p){
        cout<<"Product added"<<endl;
        products.push_back(p);
    }

    const vector<Product*> &getProducts(){
        // If we do the printing here only then again the SRP violation occurs.
        // cout<<"Products"<<endl;
        // for(auto &x:products){
        //     cout<<"Name: "<<x->name<<endl;
        //     cout<<"Price: "<<x->price<<endl;
        // }
        return products;
    }

    double calculateTotal() {
        double total = 0;
        for(auto &p:products){
            total+=p->price;
        }
        return total;
    }  
};

// Responsible for printing invoice
class ShoppingCartPrinter{
private:
    ShoppingCart* cart;

public:
    ShoppingCartPrinter(ShoppingCart*cart){
        this->cart=cart;
    }

    void printInvoice(){
        cout<<endl;
        cout<<"Shopping cart Invoice"<<endl;
        for(auto &x:cart->getProducts()){
            cout<<"Name: "<<x->name<<endl;
            cout<<"Price: "<<x->price<<endl;
        }
        cout<<cart->calculateTotal()<<endl;
    }
};

// Responsible for storing data in DB
class ShoppingCartStorage{
private:
    ShoppingCart *cart;

public:
    ShoppingCartStorage(ShoppingCart *cart){
        this->cart=cart;
    }

    void storeToDB(){
        cout<<"Save to DB"<<endl;
    }
};

int main(){
    ShoppingCart *cart=new ShoppingCart();
    cart->addProduct(new Product("Toyota",10000));
    cart->addProduct(new Product("Mitsubishi",5000));

    // cart->getProducts();
    // To print
    ShoppingCartPrinter*printer=new ShoppingCartPrinter(cart);
    printer->printInvoice();

    // Save to DB
    ShoppingCartStorage *storage=new ShoppingCartStorage(cart);
    storage->storeToDB();

}