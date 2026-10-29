// Лабораторная работа №2. Предметная область: "Минимаркет"
// Тема: наследование в C++ (private, protected, public, virtual,
// using, множественное и виртуальное наследование)

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;


class Product {
protected:
    string name;   
    double price; 
public:
    Product(const string& n, double p) : name(n), price(p) {}
    virtual ~Product() {}  

    
    virtual double getPrice() const { return price; }
    virtual void describe() const {
        cout << "Товар: " << name << ", цена " << getPrice() << " руб." << endl;
    }

    string getName() const { return name; }
};


class Food : public Product {
    int calories;
public:
    Food(const string& n, double p, int c) : Product(n, p), calories(c) {}
    void describe() const override {
        cout << "Продукт: " << name << ", цена " << getPrice()
             << " руб., " << calories << " ккал" << endl;
    }
};

class Drink : public Product {
    double volume;  
public:
    Drink(const string& n, double p, double v) : Product(n, p), volume(v) {}
    void describe() const override {
        cout << "Напиток: " << name << ", цена " << getPrice()
             << " руб., " << volume << " л" << endl;
    }
};


class ColdStorageItem : virtual public Product {
protected:
    int temperature;  
public:
    ColdStorageItem(const string& n, double p, int t)
        : Product(n, p), temperature(t) {}
    void printStatus() const {
        cout << "  Хранить при +" << temperature << " C" << endl;
    }
};

class Perishable : virtual public Product {
protected:
    int daysLeft;  
public:
    Perishable(const string& n, double p, int d)
        : Product(n, p), daysLeft(d) {}
    void printStatus() const {  
        cout << "  Осталось дней годности: " << daysLeft << endl;
    }
};


class Dairy : public ColdStorageItem, public Perishable {
public:
   
    Dairy(const string& n, double p, int t, int d)
        : Product(n, p), ColdStorageItem(n, p, t), Perishable(n, p, d) {}

   
    using Perishable::printStatus;

    
    void printColdStatus() const { ColdStorageItem::printStatus(); }

    
    double getPrice() const override {
        if (daysLeft <= 2) return price * 0.8;
        return price;
    }
    void describe() const override {
        cout << "Молочный продукт: " << name << ", цена " << getPrice() << " руб." << endl;
    }
};



class Employee {
protected:
    string name;
    double salary;
public:
    Employee(const string& n, double s) : name(n), salary(s) {}
    virtual ~Employee() {}
    virtual void introduce() const {
        cout << "Сотрудник " << name << endl;
    }
    double getSalary() const { return salary; }
};

class Seller : protected Employee {
public:
    Seller(const string& n, double s) : Employee(n, s) {}

    using Employee::introduce;

    void sell(const Product& p) const {
        cout << name << " продал(а): " << p.getName()
             << " за " << p.getPrice() << " руб." << endl;
    }
};


class Cashier : public Seller {
public:
    Cashier(const string& n, double s) : Seller(n, s) {}

    void introduce() const override {  
        cout << "Кассир " << name << endl;
    }
    void showSalary() const {
        cout << "Зарплата кассира " << name << ": " << getSalary() << " руб." << endl;
    }
};


class Storage {
    vector<Product*> items;
public:
    void add(Product* p) { items.push_back(p); }
    int count() const { return (int)items.size(); }
    Product* get(int i) const { return items[i]; }
};


class Shelf : private Storage {
    string title;
    static const int CAPACITY = 3;  
public:
    Shelf(const string& t) : title(t) {}

    using Storage::count; 

    void place(Product* p) {
        if (count() >= CAPACITY) {
            cout << "Полка \"" << title << "\" заполнена, не поместилось: "
                 << p->getName() << endl;
            return;
        }
        add(p);  
        cout << "На полку \"" << title << "\" выложено: " << p->getName() << endl;
    }
    void show() const {
        cout << "Полка \"" << title << "\":" << endl;
        for (int i = 0; i < count(); i++) {
            cout << "  ";
            get(i)->describe();
        }
    }
};


int main() {
    cout << fixed << setprecision(2);

    Food bread("Хлеб", 45, 250);
    Drink water("Вода", 60, 1.5);
    Dairy milk("Молоко", 90, 4, 2);   
    Dairy kefir("Кефир", 80, 4, 7);

    
    cout << "1. Товары (виртуальные методы)" << endl;
    Product* products[] = { &bread, &water, &milk, &kefir };
    double total = 0;
    for (int i = 0; i < 4; i++) {
        products[i]->describe();
        total += products[i]->getPrice();
    }
    cout << "Итого за все товары: " << total << " руб." << endl;

   
    cout << endl << "2. Молочные продукты " << endl;
    cout << milk.getName() << ":" << endl;  
    milk.printStatus();       
    milk.printColdStatus();   

  
    cout << endl << " 3. Сотрудники " << endl;
    Seller seller("Анна", 35000);
    Cashier cashier("Иван", 40000);
    seller.introduce();       
    seller.sell(bread);
    cashier.introduce();
    cashier.showSalary();
    

    Seller& ref = cashier;    
    ref.introduce();         

  
    cout << endl << "4. Полка " << endl;
    Shelf shelf("Хлеб и молочка");
    shelf.place(&bread);
    shelf.place(&milk);
    shelf.place(&kefir);
    shelf.place(&water);      
    shelf.show();
    cout << "Товаров на полке: " << shelf.count() << endl;
    

    return 0;
}
