#include<iostream>
#include<vector>

using namespace std;

class Product{
public:
    string name;
    double price;   

    Product(string txt,double pr){
        this->name=txt;
        this->price=pr;
    }
};

class ShoppingCart{
private:
    vector<Product*> products;

public:
    void addProduct(Product *p){
        products.push_back(p);
        cout<<"Product added"<<endl;
    }

    const vector<Product*>&getProducts(){
        return products;
    }

    double calculateTotal(){
        double total=0;
        for(auto &x:products){
            total+=x->price;
        }
        return total;
    }
    void printInvoice(){
        cout << "Shopping Cart Invoice:\n";
        for (auto p : products) {
            cout << p->name << " - Rs " << p->price << endl;
        }
        cout << "Total: Rs " << calculateTotal() << endl;
    }

    void saveToDatabase(){
        cout<<"Saving to databases"<<endl;
    }
};

int main(){
    ShoppingCart *cart=new ShoppingCart();
    cart->addProduct(new Product("Laptop",1000));
    cart->addProduct(new Product("Maximus",2000));

    cart->printInvoice();
    cart->saveToDatabase();

    return 0;
}


