#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <map>
#include <vector>
#include <ctime>
#include <chrono>
using namespace std;
// -------------------- Notification system --------------------
enum class NotificationType { ORDER_CONFIRMED, ORDER_PREPARING, ORDER_READY, PROMOTION, NEW_COMBO };
class Notification {
    private:
    string notification_id;
    NotificationType type;
    string title;
    string message;
    string timestamp;
    bool is_read;
    inline static int notification_cnt = 0;
    public:
    Notification(NotificationType _type, string _title, string _message)
        : type(_type), title(_title), message(_message), is_read(false) {
            notification_cnt++;
            stringstream ss;
            ss << "N" << setw(3) << setfill('0') << notification_cnt;
            notification_id = ss.str();

            auto now = chrono::system_clock::now();
            auto time_t = chrono::system_clock::to_time_t(now);
            stringstream ts;
            ts << put_time(localtime(&time_t), "%Y-%m-%d %H:%M:%S");
            timestamp = ts.str();
        }
        void display() const{
            cout<< "[" << timestamp << "] " << title <<endl;
            cout<< " " << message <<endl;
            cout << " ID: " << notification_id << " | Status: " << (is_read ? "Read" : "Unread") << endl;
            cout << "------------------------" << endl;
        }
        void markAsRead() { is_read = true; }
        string getId() const { return notification_id;}
        NotificationType getType() const { return type; }
        bool isRead() const { return is_read; }
};
class NotificationManager {
    private:
    vector<Notification> notifications;
    bool permission_requested;
    bool push_enabled;
    public:
    NotificationManager() : push_enabled(false), permission_requested(false) {}

    bool requestPermission() {
        if (permission_requested) {
            cout << "Permission already requested. Current status: " << (push_enabled ? "Granted" : "Denied") << endl;
            return push_enabled;
        }
        
        cout << "\n NOTIFICATION PERMISSION REQUEST " << endl;
        cout << "This application would like to send you notifications about:" << endl;
        cout << "- Order status updates (confirmed, preparing, ready)" << endl;
        cout << "- New menu items and combo deals" << endl;
        cout << "=========================================" << endl;
        
        char choice;
        cout << "Allow notifications? (y/n): ";
        cin >> choice;
        
        permission_requested = true;
        
        if (choice == 'y' || choice == 'Y') {
            push_enabled = true;
            cout << "Notifications enabled. You will receive updates about your orders." << endl;
            return true;
        } else {
            push_enabled = false;
            cout << "Notifications disabled. You can enable them later in settings." << endl;
            return false;
        }
    }

    void enablePushNotifications() { 
        if (!permission_requested) {
            cout << "Please request permission first." << endl;
            return;
        }
        push_enabled = true; 
        cout << "Push notifications enabled." << endl;
    }
    
    void disablePushNotifications() { 
        push_enabled = false; 
        cout << "Push notifications disabled." << endl;
    }

    bool isPermissionGranted() const {
        return push_enabled && permission_requested;
    }

    bool isPermissionRequested() const {
        return permission_requested;
    }

    void sendNotification(NotificationType type, string title, string message) {
        if (!push_enabled) return;
        
        Notification notification(type, title, message);
        notifications.push_back(notification);
        
        cout << "\n[NEW NOTIFICATION]" << endl;
        notification.display();
    }

    void sendOrderUpdate(string order_id, string status) {
        string title, message;
        NotificationType type;
        
        if (status == "Confirmed") {
            type = NotificationType::ORDER_CONFIRMED;
            title = "Order Confirmed.";
            message = "Order " + order_id + " has been confirmed and is being prepared.";
        } else if (status == "Preparing") {
            type = NotificationType::ORDER_PREPARING;
            title = "Order in Kitchen";
            message = "Order " + order_id + " is now being prepared.";
        } else if (status == "Ready") {
            type = NotificationType::ORDER_READY;
            title = "Order Ready.";
            message = "Order " + order_id + " is ready for pickup.";
        } else {
            return;
        }
        
        sendNotification(type, title, message);
    }

    void sendPromotion(string promo_message) {
        sendNotification(NotificationType::PROMOTION, "Special Promotion!", promo_message);
    }

    void sendNewCombo(string combo_name, double discount) {
        string message = "New combo '" + combo_name + "' with " + 
                        to_string(int(discount * 100)) + "% discount is now available!";
        sendNotification(NotificationType::NEW_COMBO, "New Combo Added!", message);
    }

    void showAllNotifications() {
        if (notifications.empty()) {
            cout << "No notifications available." << endl;
            return;
        }
        
        cout << "\n == ALL NOTIFICATIONS == " << endl;
        for (auto& notification : notifications) {
            notification.display();
        }
    }

    void showUnreadNotifications() {
        bool hasUnread = false;
        cout << "\n == UNREAD NOTIFICATIONS == " << endl;
        for (auto& notification : notifications) {
            if (!notification.isRead()) {
                notification.display();
                hasUnread = true;
            }
        }
        if (!hasUnread) {
            cout << "No unread notifications." << endl;
        }
    }

    int getUnreadCount() {
        int count = 0;
        for (const auto& notification : notifications) {
            if (!notification.isRead()) count++;
        }
        return count;
    }
};

NotificationManager notificationManager;
// ================= Food =================
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

// -------------------- Manage Food --------------------
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

// -------------------- Combo --------------------
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
        notificationManager.sendNewCombo(_combo_name, _discount); //new combo notification
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

// -------------------- User --------------------
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

class CashPayment : public PaymentMethod{
private:
    string currency;    //VND, Euro,...
public:
    CashPayment(double _amount, string _cur) : PaymentMethod("Cash", _amount){
        currency = _cur;
    }
    string getCurrency(){return currency;}
    void setCurrency(string _cur){currency = _cur;}

    void display() override{
        cout << "Payment via Cash" << endl;
        cout << "Amount: " << fixed << setprecision(2) << getAmount() << endl;
        cout << "Currency: " << getCurrency() << endl;
        cout << "====================" << endl;
    }
};

class CreditPayment : public PaymentMethod{
private:
    string card_number;
public:
    CreditPayment(double _amount, string _card_number) : PaymentMethod("Credit", _amount){
        card_number = _card_number;
    }
    string getCardNumber(){return card_number;}
    void setCardNumber(string _card){card_number = _card;}
    void display() override {
    cout << "Payment via Credit Card" << endl;
    cout << "Amount: " << fixed << setprecision(2) << getAmount() << endl;
    if (card_number.size() >= 4)
        cout << "Card Number: ****" << card_number.substr(card_number.size() - 4) << endl;
    else
        cout << "Card Number: (invalid)" << endl;
    cout << "====================" << endl;
    }
};

class eWalletPayment : public PaymentMethod{
private:
    string wallet_name;
public:
    eWalletPayment(double _amount, string _wallet_name) : PaymentMethod("e-Wallet", _amount){
        wallet_name = _wallet_name;
    }
    string getWalletName(){return wallet_name;}
    void setWalletName(string _wallet){wallet_name = _wallet;}
    void display() override{
        cout << "Payment via e-Wallet" << endl;
        cout << "Amount: " << fixed << setprecision(2) << getAmount() << endl;
        cout << "Wallet Name: " << getWalletName() << endl;
        cout << "====================" << endl;
    }
};

class PaymentManager {
private:
    vector<PaymentMethod*> payments;
public:
    void addPayment(PaymentMethod* payment) {
        if (payment != nullptr) {
            payments.push_back(payment);
        }
    }

    void displayAllPayments() {
        cout << "=== All Payments ===" << endl;
        for (PaymentMethod* payment : payments) {
            payment->display();
        }
    }
};
PaymentManager paymentManager;

enum class OrderStatus { Pending, Preparing, Completed, Cancelled };
// -------------------- Reservation --------------------
class Reservation {
    private:
    string reservation_id;
    User* customer;
    string date;
    string time;
    int party_size;
    string status;
    inline static int reservation_cnt = 0;
public:
    Reservation(User* _customer, string _date, string _time, int _party_size) : customer(_customer), date(_date), time(_time), party_size(_party_size),status("Pending") {
        reservation_cnt++;
        stringstream ss;
        ss << "R" << setw(3) << setfill('0') << reservation_cnt;
        reservation_id = ss.str();
        notificationManager.sendNotification(NotificationType::ORDER_CONFIRMED, "Reservation Confirmed", "Reservation " + reservation_id + " for " + to_string(party_size) + " people on " + date + " at " + _time + " is pending confirmation.");
    }
    void setStatus(string s){
        status = s;
        if (s == "Confirmed") {
            notificationManager.sendNotification(NotificationType::ORDER_CONFIRMED, "Reservation Confirmed", "Reservation " + reservation_id + " is confirmed.");
        } else if (s == "Cancelled") {
            notificationManager.sendNotification(NotificationType::ORDER_CONFIRMED, "Reservation Cancelled", "Reservation " + reservation_id + " is cancelled.");
        }
    }
    string getReservationID(){
        return reservation_id;
    }
    string getStatus(){
        return status;
    }
    string getDate(){
        return date;
    }
    string getTime(){
        return time;
    }
    User* getCustomer(){
        return customer;
    }
    void displayInfo(){
        cout << "=== Reservation Details ===" << endl;
        cout << "Reservation ID: " << reservation_id << endl;
        if (customer) {
            cout<< "Customer: " << customer->getUsername() << " ( ID: " << customer->getId() << " )" << endl;
        }
        cout<<"Date: "<<date<<endl;
        cout<<"Time: "<<time<<endl;
        cout<<"Party Size: "<<party_size<<endl;
        cout<<"Status: "<<status<<endl;
        cout<< "==========================" <<endl;
    }
};
// -------------------- Order --------------------
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
        notificationManager.sendOrderUpdate(order_id, "Confirmed"); //confirmation notification
    }

    void setStatus(OrderStatus s) { //updated status based on status change
         status = s;
         switch(s) { 
            case OrderStatus::Preparing:
                notificationManager.sendOrderUpdate(order_id, "Preparing");
                break;
            case OrderStatus::Completed:
                notificationManager.sendOrderUpdate(order_id, "Ready");
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

void Guest_option(User* guest, Order& order,vector<Reservation*>& reservations) {
    /*updated menu
    implemented reservation (choice =8 -> 10)*/
    int choice;
    do {
        cout << "\n--- Guest Menu ---\n";
int unread = notificationManager.getUnreadCount();
        if (unread > 0) {
            cout << " [" << unread << " unread notifications]";
        }
        cout << "\n";
        cout << "1. Show Menu\n";
        cout << "2. Show Order\n";
        cout << "3. Cancel Order (Pending only)\n";
        cout << "4. View Notifications\n";
        cout << "5. View Unread Notifications\n";
        cout << "6. Notification Settings\n";
        cout << "7. Payment Menu\n";
        cout << "8. Make a Reservation\n";
        cout << "9. View Reservations\n";
        cout << "10. Cancel Reservation\n";
        cout << "0. Exit\n";
        cout << "Choose: ";
        cin >> choice;

        if (choice == 1) {
            displayAllFood();
        } else if (choice == 2) {
            order.display();
        } else if (choice == 3) {
            if (order.getStatus() == OrderStatus::Pending) {
                order.setStatus(OrderStatus::Cancelled);
                cout << "Order cancelled!\n";
            } else {
                cout << "Cannot cancel order (not Pending).\n";
            }
        } else if (choice == 4) {
            notificationManager.showAllNotifications();
        } else if (choice == 5) {
            notificationManager.showUnreadNotifications();
        } else if (choice == 6) {
            cout << "1. Enable notifications\n2. Disable notifications\nChoose: ";
            int notif_choice;
            cin >> notif_choice;
            if (notif_choice == 1) {
                notificationManager.enablePushNotifications();
            } else if (notif_choice == 2) {
                notificationManager.disablePushNotifications();
            }
        } else if (choice == 7) {
            cout << "\n--- Payment Menu ---\n";
            cout << "1. Cash\n";
            cout << "2. Credit Card" << endl;
            cout << "3. e-Wallet" << endl;
            cout << "Choose payment method: ";
            int pChoice; cin >> pChoice;; cin.ignore();
            PaymentMethod *payment = nullptr;

            if(pChoice == 1){
                double cash;
                string currency;
                cout << "Enter currency (e.g., USD, VND): "; getline(cin, currency);
                cout << "Enter cash amount: $"; cin >> cash;
                if(cash < order.getTotalPrice()) cout << "Not enough cash!" << endl;
                else {
                    payment = new CashPayment(cash,currency);
                    cout << "Payment successful!" << endl;
                    cout << "Change: $" << cash - order.getTotalPrice() << endl;
                    order.setPaymentMethod(payment);
                    paymentManager.addPayment(payment);
                }
            } 
            else if (pChoice == 2){
                string card;
                cout << "Enter 16-digit card number: "; cin >> card;
                if(card.size() != 16) cout << "Invalid card number!" << endl;
                else {
                    payment = new CreditPayment(order.getTotalPrice(), card);
                    cout << "Payment successful via Credit Card!" << endl;
                    order.setPaymentMethod(payment);
                    paymentManager.addPayment(payment);
                }
            }
            else if (pChoice == 3){
                string wallet;
                cout << "Enter e-Wallet name (e.g., PayPal, Momo): "; getline(cin, wallet);
                payment = new eWalletPayment(order.getTotalPrice(), wallet);
                cout << "Payment successful via e-Wallet! (" << wallet << ")"<< endl;
                order.setPaymentMethod(payment);
                paymentManager.addPayment(payment);
            } 
            if (payment != nullptr) {
                order.setPaymentMethod(payment);
            }
        } else if (choice == 8) {
            cin.ignore();
            string date,time;
            int party_size;
            cout <<"Enter date (YYYY-MM-DD): "; getline(cin, date);
            cout <<"Enter time (HH:MM): "; getline(cin, time);
            cout <<"Enter party size: "; cin >> party_size;
            Reservation* newRes = new Reservation(guest, date, time, party_size);
            reservations.push_back(newRes);
            cout<<"Reservation created, waiting to confirm"<<endl;
            newRes->displayInfo();
        } else if (choice == 9) {
            cout << "\n=== Your Reservations ===\n";
            bool hasReservations = false;
            for (Reservation* res : reservations) {
                if (res->getCustomer() == guest) {
                    res->displayInfo();
                    hasReservations = true;
                }
            }
            if (!hasReservations) {
                cout << "No reservations found." << endl;
            }
        } else if (choice == 10){
            string res_id;
            cout << "Enter Reservation ID to cancel: ";
            cin>>res_id;
            for(Reservation* res : reservations){
                if(res->getReservationID() == res_id && res->getCustomer() ==guest){
                    if(res->getStatus() == "Pending" || res->getStatus() == "Confirmed"){
                        res->setStatus("Cancelled");
                        cout << "Reservation cancelled." << endl;
                    } else {
                        cout << "Cannot cancel this reservation." << endl;
                    }
                    break;
                }
            }
        }
    } while (choice != 0);
}

void Admin_option(vector<Order*>& orders, vector<Reservation*>& reservations) {
    int choice;
    do {
        cout << "\n--- Admin Menu ---\n";
        cout << "1. Show all food\n";
        cout << "2. Show all orders\n";
        cout << "3. Update order status\n";
        cout << "4. View all reservations\n";
        cout << "5. Confirm/Update reservation\n";
        cout << "6. Send promotion\n";
        cout << "7. Show Payment History\n";
        cout << "0. Exit\n";
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
            cout << "Enter Order ID: ";
            cin >> oid;
            for (Order* o : orders) {
                if (o->getOrderId() == oid) {
                    int s;
                    cout << "Choose status (0=Pending,1=Preparing,2=Completed,3=Cancelled): ";
                    cin >> s;
                    o->setStatus(static_cast<OrderStatus>(s));
                    cout << "Order " << oid << " status updated!\n";
                }
            }
        } else if (choice == 4) {
            cout << "\n=== All Reservations ===\n";
            if (reservations.empty()) {
                cout << "No reservations found.\n";
            } else {
                for (Reservation* res : reservations) {
                    res->displayInfo();
                }
            }
        } else if (choice == 5) {
            string res_id;
            cout << "Enter Reservation ID: ";
            cin >> res_id;
            for (Reservation* res : reservations) {
                if (res->getReservationID() == res_id) {
                    cout << "Choose status:\n";
                    cout << "1. Pending\n2. Confirmed\n3. Cancelled\n4. Completed\n";
                    cout << "Choose: ";
                    int s;
                    cin >> s;
                    string status;
                    if (s == 1) status = "Pending";
                    else if (s == 2) status = "Confirmed";
                    else if (s == 3) status = "Cancelled";
                    else if (s == 4) status = "Completed";
                    
                    res->setStatus(status);
                    cout << "Reservation " << res_id << " status updated to " << status << "!\n";
                    break;
                }
            }
        } else if (choice == 6) {
            cin.ignore();
            string promo;
            cout << "Enter promotion message: ";
            getline(cin, promo);
            notificationManager.sendPromotion(promo);
        } else if (choice == 7) {
            paymentManager.displayAllPayments();
        }
    } while (choice != 0);
}


// -------------------- main --------------------
int main() {
    cout << "===== Restaurant Ordering System Demo =====\n\n";

    // Initialize account manager and create sample accounts
    AccountManager accManager;
    accManager.registerGuest("Alice", "pass123");
    accManager.registerGuest("Bob", "abc123");

    // Display all accounts
    accManager.displayAllAccounts();
    cout << endl;

    // ===== Guest logs in =====
    cout << "--- Guest Login (Alice) ---\n";
    User* alice = new Guest("Alice", "pass123");
    if (alice->login("Alice", "pass123")) {
        cout << "Login successful! Welcome, " << alice->getUsername() << " (" << alice->getRole() << ")\n\n";
    }

    // ===== Create some food items and combos =====
    Food* ramen1 = new ramen("Tonkotsu Ramen", 12.50);
    Food* don1 = new rice_don("Chicken Katsu Don", 10.00);
    Food* drink1 = new Drink("Coca-Cola", 2.50, "12 oz");

    addToManageFood(ramen1);
    addToManageFood(don1);
    addToManageFood(drink1);
    displayAllFood();
    cout << endl;

    Combo lunchCombo("Lunch Combo", 0.1);
    lunchCombo.addFood(ramen1);
    lunchCombo.addFood(drink1);
    lunchCombo.display();
    cout << endl;

    // ===== Alice creates a new order =====
    Order order1(alice);
    order1.addFood(don1);
    order1.addCombo(lunchCombo);
    cout << "Order created successfully!\n";
    order1.display();
    cout << endl;

    // ===== Payment Process =====
    cout << "--- Payment Menu ---\n";
    PaymentMethod* pay1 = new eWalletPayment(order1.getTotalPrice(), "Momo");
    order1.setPaymentMethod(pay1);
    paymentManager.addPayment(pay1);
    cout << "Payment successful using e-Wallet (Momo)!\n\n";

    // ===== Display updated order with payment =====
    order1.display();
    cout << endl;

    // ===== Order Cancellation (Refund) =====
    cout << "--- Cancelling Order ---\n";
    order1.setStatus(OrderStatus::Cancelled);
    cout << "Order cancelled successfully. Refund processed.\n\n";

    // ===== Staff Login =====
    cout << "--- Staff Login ---\n";
    User* staff = new Staff("admin", "123");
    if (staff->login("admin", "123")) {
        cout << "Staff login successful. Viewing payment history...\n\n";
    }

    // ===== Staff views payment history =====
    paymentManager.displayAllPayments();

    cout << "\n===== Demo Complete =====\n";

    // Cleanup
    delete alice;
    delete staff;
    delete ramen1;
    delete don1;
    delete drink1;
    delete pay1;

    return 0;
}
