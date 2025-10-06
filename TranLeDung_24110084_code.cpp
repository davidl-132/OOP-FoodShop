#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
using namespace std;

// ================= Food ================= I do 
class Food {
protected:
    string id;
    string name;
    double price;

public:
    inline static int cnt = 0;
    Food(string _name, double _price) : name(_name), price(_price) {
        cnt++;
        stringstream ss;
        ss << "F" << setw(3) << setfill('0') << cnt;
        id = ss.str();
    }

    virtual void display() {
        cout << "ID: " << id << ", Name: " << name << ", Price: $" << price << endl;
    }

    string getId() { return id; }
    string getName() { return name; }
    double getPrice() { return price; }

    virtual ~Food() {}
};

class rice_don : public Food {
private:
    string rice_type;
    string protein;

public:
    rice_don(string _name, double _price, string _rice_type = "White Rice", string _protein = "Chicken")
        : Food(_name, _price), rice_type(_rice_type), protein(_protein) {}

    void display() override {
        cout << "ID: " << id << ", Rice Don: " << name
             << ", Rice: " << rice_type << ", Protein: " << protein
             << ", Price: $" << price << endl;
    }
};

class ramen : public Food {
private:
    string broth_type;
    string noodle_type;

public:
    ramen(string _name, double _price, string _broth = "Tonkotsu", string _noodle = "Thin")
        : Food(_name, _price), broth_type(_broth), noodle_type(_noodle) {}

    void display() override {
        cout << "ID: " << id << ", Ramen: " << name
             << ", Broth: " << broth_type << ", Noodles: " << noodle_type
             << ", Price: $" << price << endl;
    }
};

class topping : public Food {
private:
    string category;

public:
    topping(string _name, double _price, string _category = "Vegetable")
        : Food(_name, _price), category(_category) {}

    void display() override {
        cout << "ID: " << id << ", Topping: " << name
             << ", Category: " << category
             << ", Price: $" << price << endl;
    }
};

class SideDish : public Food {
private:
    string dish_type;
    bool is_vegetarian;

public:
    SideDish(string _name, double _price, string _type = "Appetizer", bool _veg = false)
        : Food(_name, _price), dish_type(_type), is_vegetarian(_veg) {}

    void display() override {
        cout << "ID: " << id << ", Side Dish: " << name
             << ", Type: " << dish_type
             << ", Vegetarian: " << (is_vegetarian ? "Yes" : "No")
             << ", Price: $" << price << endl;
    }
};

class Drink : public Food {
private:
    string oz;
public:
    Drink(string _name, double _price, string _oz) : Food(_name, _price), oz(_oz) {}
    void display() override {
        cout << "ID: " << id << ", Drink: " << name
             << ", Ounces: " << oz
             << ", Price: $" << price << endl;
    }
};

// -------------------- Manage Food -------------------- I do 
map<string, Food*> manageFood;

void addToManageFood(Food* food) {
    if (food != nullptr) {
        manageFood[food->getId()] = food;
    }
}

void displayAllFood() {
    cout << "=== All Available Food Items ===" << endl;
    for (auto& pair : manageFood) {
        pair.second->display();
    }
    cout << "===============================" << endl;
}

Food* findFoodById(string id) {
    auto it = manageFood.find(id);
    return (it != manageFood.end()) ? it->second : nullptr;
}

// -------------------- Combo -------------------- I do
class Combo {
private:
    vector<Food*> FoodHavecombo;
    string combo_id;
    string combo_name;
    double price;
    double discount;
    inline static int combo_cnt = 0;

public:
    Combo(string _combo_name, double _discount = 0.1)
        : combo_name(_combo_name), discount(_discount) {
        combo_cnt++;
        stringstream ss;
        ss << "C" << setw(3) << setfill('0') << combo_cnt;
        combo_id = ss.str();
        price = 0.0;
    }

    void addFood(Food* food) {
        if (food != nullptr) {
            FoodHavecombo.push_back(food);
            calculatePrice();
        }
    }

    void removeFood(string food_id) {
        for (auto it = FoodHavecombo.begin(); it != FoodHavecombo.end(); ++it) {
            if ((*it)->getId() == food_id) {
                FoodHavecombo.erase(it);
                calculatePrice();
                break;
            }
        }
    }

    void calculatePrice() {
        double total = 0.0;
        for (Food* food : FoodHavecombo) {
            total += food->getPrice();
        }
        price = total * (1.0 - discount);
    }

    void display() {
        cout << "=== Combo Details ===" << endl;
        cout << "Combo ID: " << combo_id << endl;
        cout << "Combo Name: " << combo_name << endl;
        cout << "Discount: " << (discount * 100) << "%" << endl;
        cout << "Items in combo:" << endl;

        double original_total = 0.0;
        for (Food* food : FoodHavecombo) {
            cout << "  - ";
            food->display();
            original_total += food->getPrice();
        }

        cout << "Original Total: $" << fixed << setprecision(2) << original_total << endl;
        cout << "Discounted Price: $" << fixed << setprecision(2) << price << endl;
        cout << "You Save: $" << fixed << setprecision(2) << (original_total - price) << endl;
        cout << "========================" << endl;
    }

    string getComboId() { return combo_id; }
    string getComboName() { return combo_name; }
    double getPrice() { return price; }
    vector<Food*> getFoodItems() { return FoodHavecombo; }
};

// -------------------- User -------------------- My partner and I do 
class User {
protected:
    string username;
    string password;
    string id;
public:
    User(string _username, string _password){
        username = _username;
        password = _password;
    }

    string getId(){ return id; }
    string getUsername(){ return username; }
    string getPassword(){ return password; }

    void setId(string _id){ id = _id; }
    void setUsername(string _username){ username = _username; }
    void setPassword(string _password){ password = _password; }

    virtual bool login(string _username, string _password) = 0;
    virtual string getRole() = 0;
    virtual ~User() {}
};

class Guest : public User {
private:
    string role;
    inline static int cnt = 0;
public:
    Guest(string _username, string _password) : User(_username, _password){
        role = "Guest";
        cnt++;
        stringstream ss;
        ss << "G" << setw(3) << setfill('0') << cnt;
        id = ss.str();
    }

    string getRole() override { return role; }

    bool login(string _username, string _password) override {
        return (username == _username && password == _password);
    }
};

class Staff : public User {
private:
    string role;
    inline static int cnt = 0;
public:
    Staff(string _username, string _password) : User(_username, _password){
        role = "Staff";
        cnt++;
        stringstream ss;
        ss << "S" << setw(3) << setfill('0') << cnt;
        id = ss.str();
    }

    string getRole() override { return role; }

    bool login(string _username, string _password) override {
        return (username == _username && password == _password);
    }
};

// -------------------- Account Manager --------------------
class AccountManager {
private:
    map<string, User*> guests;   // chỉ quản lý Guest
    Staff* staffAccount;         // 1 staff duy nhất
public:
    AccountManager(){
        // tạo staff mặc định
        staffAccount = new Staff("admin", "123");
    }

    ~AccountManager(){
        for(auto &p : guests){
            delete p.second;
        }
        delete staffAccount;
    }

    // Đăng ký Guest
    bool registerGuest(string username, string password){
        if(guests.find(username) != guests.end()) return false; // đã tồn tại
        guests[username] = new Guest(username, password);
        return true;
    }

    // Đăng nhập Guest hoặc Staff
    bool login(string username, string password){
        if(username == staffAccount->getUsername()){ 
            return staffAccount->login(username, password);
        }
        auto it = guests.find(username);
        if(it != guests.end()){
            return it->second->login(username, password);
        }
        return false;
    }

    void displayAllAccounts(){
        cout << "Staff Account (dev cấp): " 
             << staffAccount->getUsername() << endl;

        cout << "Guest Accounts:\n";
        int i = 1;
        for(auto &p : guests){
            cout << "\t#" << i++ << ". " 
                 << p.second->getUsername() 
                 << " (" << p.second->getRole() << ")"
                 << " [ID: " << p.second->getId() << "]"
                 << endl;
        }
    }
};
class PaymentMethod {
protected:
    string method_name;
    double amount;

public:
    PaymentMethod(string _method_name, double _amount)
        : method_name(_method_name), amount(_amount) {
        stringstream ss;
    }

    virtual void display() {}

    string getMethodName() { return method_name; }
    double getAmount() { return amount; }

    virtual ~PaymentMethod() {}
};

enum class OrderStatus { Pending, Preparing, Completed, Cancelled };


// -------------------- Order -------------------- I do 
class Order {
private:
    string order_id;
    User* customer;
    vector<Food*> food_items;
    vector<Combo> combos;
    double total_price;
    OrderStatus status;
    PaymentMethod* payment;
    inline static int order_cnt = 0;

    void calculateTotal() {
        double total = 0.0;
        for (Food* food : food_items) {
            total += food->getPrice();
        }
        for (Combo& combo : combos) {
            total += combo.getPrice();
        }
        total_price = total;
    }

public:
    Order(User* _customer) : customer(_customer), payment(nullptr) {
        order_cnt++;
        stringstream ss;
        ss << "O" << setw(3) << setfill('0') << order_cnt;
        order_id = ss.str();
        total_price = 0.0;
        status = OrderStatus::Pending; // mặc định
    }

    void setStatus(OrderStatus s) { //updated status based on status change
         status = s;
         switch(s) { 
            case OrderStatus::Preparing:
                break;
            case OrderStatus::Completed:
                break;
            case OrderStatus::Cancelled:
                if(payment != nullptr) {
                    cout << "Refunding $" << fixed << setprecision(2) << payment->getAmount() << " to customer." << endl;
                }
            default:
                break;
        }
    }
    void setPaymentMethod(PaymentMethod* pm){payment = pm;}
    double getTotalPrice() { return total_price; }
    OrderStatus getStatus() { return status; }
    string getOrderId() { return order_id; }
    User* getCustomer() { return customer; }
    PaymentMethod* getPaymentMethod() { return payment; }

    void addFood(Food* food) {
        if (food != nullptr) {
            food_items.push_back(food);
            calculateTotal();
        }
    }

    void addCombo(Combo combo) {
        combos.push_back(combo);
        calculateTotal();
    }

    void display() {
        cout << "=== Order Details ===" << endl;
        cout << "Order ID: " << order_id << endl;
        if (customer) {
            cout << "Customer: " << customer->getUsername() 
                 << " (" << customer->getId() << ")" << endl;
        }
        cout << "Status: ";
        switch (status) {
            case OrderStatus::Pending: cout << "Pending"; break;
            case OrderStatus::Preparing: cout << "Preparing"; break;
            case OrderStatus::Completed: cout << "Completed"; break;
            case OrderStatus::Cancelled: cout << "Cancelled"; break;
        }
        cout << endl;

        cout << "Items in order:" << endl;
        for (Food* food : food_items) {
            cout << "  - ";
            food->display();
        }
        for (Combo& combo : combos) {
            cout << "  - Combo: " << combo.getComboName() << endl;
            combo.display();
        }
        cout << "Total Price: $" << fixed << setprecision(2) << total_price << endl;

        if(payment){
            cout << "Payment Details: " << endl;
            payment->display();
        } else {
            cout << "Payment Method: Not set" << endl;
        }
        cout << "=====================" << endl;
    }
};

// -------------TERMINAL CLIENT DEMO -------------- I do 
void Guest_option(User* guest, Order& order) {
    int choice;
    do {
        cout << "\n--- Guest Menu ---\n";
        cout << "1. show Menu\n";
        cout << "2. show Order\n";
        cout << "3. delete Order (Pending)\n";
        cout << "0. exit\n";
        cout << "choose: ";
        cin >> choice;

        if (choice == 1) {
            displayAllFood();
        } else if (choice == 2) {
            order.display();
        } else if (choice == 3) {
            if (order.getStatus() == OrderStatus::Pending) {
                order.setStatus(OrderStatus::Cancelled);
                cout << "Order deleted!\n";
            } else {
                cout << "Can't order (not Pending).\n";
            }
        }
    } while (choice != 0);
}

void Admin_option(vector<Order*>& orders) {
    int choice;
    do {
        cout << "\n--- Admin Menu ---\n";
        cout << "1. show all food\n";
        cout << "2. show all order\n";
        cout << "3. show order status\n";
        cout << "0. exist\n";
        cout << "Choose: ";
        cin >> choice;

        if (choice == 1) {
            displayAllFood();
        } else if (choice == 2) {
            for (Order* o : orders) {
                o->display();
            }
        } else if (choice == 3) {
            string oid;
            cout << "enter Order ID: ";
            cin >> oid;
            for (Order* o : orders) {
                if (o->getOrderId() == oid) {
                    int s;
                    cout << "choose status (0=Pending,1=Preparing,2=Completed,3=Cancelled): ";
                    cin >> s;
                    o->setStatus(static_cast<OrderStatus>(s));
                    cout << " Order " << oid << " completely change";
                }
            }
        }
    } while (choice != 0);
}


int main() {
    cout << "\n=============================================\n";
    cout << "         FOOD ORDERING SYSTEM TEST CASE     \n";
    cout << "=============================================\n";

    // ---------- Create Food ----------
    cout << "\n STEP 1: Create Food Items\n";
    Food* ramen1 = new ramen("Shoyu Ramen", 14.50, "Shoyu", "Medium");
    Food* rice1 = new rice_don("Beef Don", 13.00, "White Rice", "Beef");
    Food* drink1 = new Drink("Iced Tea", 2.50, "16 oz");
    Food* side1 = new SideDish("Gyoza", 5.00, "Appetizer", false);

    // Add to food manager
    addToManageFood(ramen1);
    addToManageFood(rice1);
    addToManageFood(drink1);
    addToManageFood(side1);

    cout << "\n Displaying All Food Items:\n";
    displayAllFood();

    // ---------- Create Combo ----------
    cout << "\n STEP 2: Create Combo\n";
    Combo lunchCombo("Lunch Combo", 0.10);
    lunchCombo.addFood(ramen1);
    lunchCombo.addFood(side1);
    lunchCombo.addFood(drink1);

    cout << "\n Combo Details:\n";
    lunchCombo.display();

    // ---------- Create User ----------
    cout << "\n STEP 3: Create Guest User\n";
    User* guest = new Guest("Alice", "1234");
    cout << "Guest Created -> Username: " << guest->getUsername()
         << " | ID: " << guest->getId() << endl;

    // ---------- Create Order ----------
    cout << "\n STEP 4: Create Order for Guest\n";
    Order order1(guest);
    order1.addFood(rice1);
    order1.addCombo(lunchCombo);

    cout << "\n Display Order Details:\n";
    order1.display();

    // ---------- Change Order Status ----------
    cout << "\n STEP 5: Change Order Status\n";
    cout << "Setting status to 'Preparing'...\n";
    order1.setStatus(OrderStatus::Preparing);
    order1.display();

    cout << "Setting status to 'Completed'...\n";
    order1.setStatus(OrderStatus::Completed);
    order1.display();

    // ---------- Cleanup ----------
    cout << "\n STEP 6: Cleanup Memory\n";
    for (auto& pair : manageFood)
        delete pair.second;
    manageFood.clear();
    delete guest;

    cout << "\n TEST CASE COMPLETED SUCCESSFULLY \n";
    cout << "=============================================\n\n";
    return 0;
}

