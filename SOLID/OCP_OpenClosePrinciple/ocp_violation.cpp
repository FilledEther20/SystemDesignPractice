// OPEN CLOSE PRINCIPLE 
// Theoretical: A code should be close for modification but open for extension
// What I could understand: 
#include <iostream>
#include <vector>

using namespace std;

class Product
{
public:
    string name;
    double price;

    Product(string name, double price)
    {
        this->name = name;
        this->price = price;
    }
};

class ShoppingCart
{
private:
    vector<Product *> products;

public:
    void addProduct(Product *p)
    {
        products.push_back(p);
    }
    double calculateTotal()
    {
        double total = 0;
        for (auto p : products)
        {
            total += p->price;
        }
        return total;
    }

    const vector<Product *> &getProducts()
    {
        return products;
    }
};
class ShoppingCartPrinter
{
private:
    ShoppingCart *cart;

public:
    ShoppingCartPrinter(ShoppingCart *c)
    {
        this->cart = c;
    }
    void printInvoice()
    {
        cout << "Shopping Cart Invoice:\n";
        for (auto p : cart->getProducts())
        {
            cout << p->name << " - Rs " << p->price << endl;
        }
        cout << "Total: Rs " << cart->calculateTotal() << endl;
    }
};

// ShoppingCartStorage violates ocp because we have designed this class in such a way wherein if we have to add another dbSaving logic like Cassandra we would have to modify this only
// and even if we try to perform some logic based on the type of DB then it would make the loop in main function tightly coupled to the db which would increase coupling
class ShoppingCartStorage
{
private:
    ShoppingCart *cart;

public:
    ShoppingCartStorage(ShoppingCart *cart)
    {
        this->cart = cart;
    }

    void saveToSQLDatabase()
    {
        cout << "Saving shopping cart to SQL DB..." << endl;
    }

    void saveToMongoDatabase()
    {
        cout << "Saving shopping cart to Mongo DB..." << endl;
    }

    void saveToFile()
    {
        cout << "Saving shopping cart to File..." << endl;
    }
};
int main()
{
    ShoppingCart *cart = new ShoppingCart();

    cart->addProduct(new Product("Laptop", 50000));
    cart->addProduct(new Product("Mouse", 2000));

    ShoppingCartPrinter *printer = new ShoppingCartPrinter(cart);
    printer->printInvoice();

    ShoppingCartStorage *db = new ShoppingCartStorage(cart);
    db->saveToSQLDatabase();

    return 0;
}