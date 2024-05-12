// these are all the header files
#include<iostream>
#include<vector>
#include<string>
#include<sstream>
#include<fstream>
#include<queue>
#include<cstdlib>
#include<algorithm>
using namespace std;

// Base class for accounts
class Account {
protected:
    string username; 
    string password; 
public:
    static bool compareByExpenditure(const Account* a, const Account* b) {
        return a->expenditure < b->expenditure; // Use < for descending order
    } //compares account by expenditure for the purpose of trend analysis to understand who spends most

    double expenditure =0;
    string getUsername() const {
        return username;
    }
    string getPassword() const {
        return password;
    }
    // Constructor to initialize username, password, and expenditure
    Account(const string& username, const string& password, int expenditure = 0) : username(username), password(password), expenditure(expenditure) {}

    // Virtual method to authenticate login credentials by comparing stored username 
    //and password to the entered credentials
    virtual bool authenticate(const string& inputUsername, const string& inputPassword) const 
    {
        return username == inputUsername && password == inputPassword;
    }
    virtual double getBalance() const {
        return 0;
    }
    virtual string getAccountType() const = 0;
};

// Derived class for customer accounts
class CustomerAccount : public Account {
    double balance;
public:

// Constructor to initialize username, password, expenditure, and balance
    CustomerAccount(const string& username, const string& password, int expenditure = 0, int balance =0) : Account(username, password, expenditure) {
        this->balance = balance;
    }
    // Overrides the getAccountType() method from the base class Account
    string getAccountType() const override {
        return "User";
    }
    double getBalance() const {
        return balance;
    }
    void updateBalance(double amount) {
        balance += amount;
    }
};

// Derived class for merchant accounts
class MerchantAccount : public Account {

public:
    // Constructor to initialize username and password
    MerchantAccount(const string& username, const string& password) : Account(username, password) {}

    // Overrides the getAccountType() method from the base class Account
    string getAccountType() const override {
        return "Seller";
    }

    // Iterates through the provided vector of accounts, checks if each account is a CustomerAccount,
    // and prints the username & balance (non-zero)
    void viewUsersWithBalances(const vector<Account*>& accounts) {
        cout << "Users with balances:" << endl;
        for (const auto& acc : accounts) {
            if (dynamic_cast<CustomerAccount*>(acc)) {
                cout << "Username: " << username << ", Balance: " << static_cast<CustomerAccount*>(acc)->getBalance() << endl;
            }
        }
    }
};

class Product
{
    private:
        int id;
        string name;
        string category;
        double price;
        int quantity;
        int sales;

    public:
    // Operator overloading to compare products
        bool operator<(const Product& other) const {
            return sales * price < other.sales * other.price;
        } //ANCHOR - for priority queue
        int getTotalSales() const{
            return sales;
        }

        // Increases the total sales of the product by the specified quantity
        void addSales(int quantity)
        {
            this->sales+=quantity;
        }

        // Constructor to initialize product details
        Product(int id, string name, string category, double price, int quantity, int sales=0)
        {
            this->id=id;
            this->name=name;
            this->category=category;
            this->price=price;
            this->quantity=quantity;
            this->sales=sales;
        }

        // Accesors for product details
        int getId() const{
            return id;
        }

        void setId(int id){
            this->id=id;
        }

        string getName() const{
            return name;
        }

        void setName(string name){
            this->name=name;
        }

        string getCategory() const{
            return category;
        }

        void setCategory(string category){
            this->category=category;
        }

        double getPrice() const{
            return price;
        }

        void setPrice(double price){
            this->price=price;
        }

        int getQuantity() const{
            return quantity;
        }

        void setQuantity(int quantity){
            this->quantity=quantity;
        }
};

class Inventory
{
    private:
        vector<Product> products; // Vector to store products

    public:
        

        void printAllProducts() const{
            for(auto i=products.begin();i!=products.end();i++)
            {
                cout<<"ID : "<<i->getId()<<endl;
                cout<<"Name : "<<i->getName()<<endl;
                cout<<"Category : "<<i->getCategory()<<endl;
                cout<<"Price : ₹"<<i->getPrice()<<endl;
                cout<<"Quantity : "<<i->getQuantity()<<endl << endl;
            }
        }

        void addProduct(Product product)
        {
            // Check if the product ID already exists using binary search
            Product* found = findProduct(product.getId());
            if (found != nullptr)
            {
                cout << "Id already exists." << endl;
                cout << "-----------------------------------------------------------" <<endl;
            }
            else
            {
                // Insert the new product at the back of the products vector
                products.push_back(product);
                cout << "Product added successfully." << endl;
                cout << "-----------------------------------------------------------" <<endl;
            }
        }

         // Method to retrieve a copy of the products and return a vector containing all the products
        vector<Product> getProducts() const 
        {
            return products;
        }

        // Method to remove a product from inventory
        void removeProduct(int id)
        {
            Product* found = findProduct(id);
            if (found != nullptr)
            {
                auto index = found - &products[0];  // Calculate the index of the found product 
                                                    //Auto deduces type from expression, over here it's assigning int
                                                    
                products.erase(products.begin() + index); // Erases the product from 'products' vector at calculated index
                                                          //Uses products.begin() to get the iterator to the first element
                cout << "Product removed successfully." << endl;        
                cout << "-----------------------------------------------------------" <<endl;
            }
            else
            {
                cout << "Id does not exist" << endl;
            }
        }

        Product* findProduct(int id)  // Function to find a product by its ID
        {
            int low = 0;
            int high = products.size() - 1;
            while (low <= high)  // Perform binary search until the low index is less than or equal to the high index
            {
                int mid = low + (high - low) / 2;
                if (products[mid].getId() == id)
                {
                    return &products[mid]; // Return a pointer to the product found
                }
                else if (products[mid].getId() < id)
                {
                    low = mid + 1;  // Update the low index to search the right half of the current range
                }
                else
                {
                    high = mid - 1; // Update the high index to search the left half of the current range
                }
            }
            return nullptr; // If the loop exits without finding the target ID, 
                            //return nullptr indicating that the product was not found
        }

        // Updates product details if found by ID
        void updateProduct(int id, string name, string category, double price, int quantity)
        {
            bool found = false;
            for (auto i = products.begin(); i != products.end(); i++)
            {
                if (i->getId() == id)
                {
                    i->setName(name);
                    i->setCategory(category);
                    i->setPrice(price);
                    i->setQuantity(quantity);
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                cout << "ID does not exist." << endl;
            }
        }        

        // Saves inventory data to a file
        void saveInventoryToFile(string filename) 
        {
            ofstream file;
            file.open(filename, ios::out | ios::trunc);
            for (int i = 0; i < products.size(); i++) 
            {
                Product p = products[i];
                file << p.getId() << "," << p.getName() << "," << p.getCategory() << "," << p.getPrice() << "," << p.getQuantity()<< ","<< p.getTotalSales() << endl;
            }
            file.close();
        }

        // Loads inventory data from a file
        void loadInventoryFromFile(string filename) 
        {
            ifstream file; // Declare an input file stream object
            file.open(filename);

            if (file.is_open()) 
            {
                string line;
                while (getline(file, line)) 
                {
                    stringstream ss(line); // Create a stringstream object to parse the line
                    string idStr, name, category, priceStr, quantityStr, salesStr; // Declare string variables to store data fields
                    // Extract data fields from the stringstream separated by commas 
                    getline(ss, idStr, ',');
                    getline(ss, name, ',');
                    getline(ss, category, ',');
                    getline(ss, priceStr, ',');
                    getline(ss, quantityStr, ',');
                    getline(ss, salesStr, ',');

                    // Convert string data fields to their respective data types
                    int id = stoi(idStr); //stoi = string to int
                    double price = stod(priceStr); //stod = string to double
                    int quantity = stoi(quantityStr);
                    int sales = stoi(salesStr);

                    // Create a Product object using the extracted data
                    Product p(id, name, category, price, quantity, sales);
                    // Add the Product object to the products vector
                    products.push_back(p);
                }

                file.close();
            } else {
                cout << "Error: Could not open file " << filename << endl;
            }
        }
};

// Function to compare accounts based on expenditure
bool compareAccounts(const Account* a1, const Account* a2) {
    return a1->expenditure < a2->expenditure;
}

class Store {
private:
    Inventory inventory;
    vector<Account*> accounts;
    Account* loggedInAccount = nullptr;
    // caesar cipher
    void encrypt(string& str) {
        int key = 3; // the key
        for (char& c : str) {
            if (isalpha(c)) {
                c = ((c - 'a' + key) % 26) + 'a'; 
            } else if (isupper(c)) {
                c = ((c - 'A' + key) % 26) + 'A'; 
            }
        }
    }
    void decrypt(string& str) {
        int key = 3; // You should use the same key that you used for encryption
        for (char& c : str) {
            if (isalpha(c)) {
                if (islower(c)) {
                    c = (((c - 'a' - key) % 26 + 26) % 26) + 'a'; // Decrypt lowercase letters
                } else {
                    c = (((c - 'A' - key) % 26 + 26) % 26) + 'A'; // Decrypt uppercase letters
                }
            }
        }
    }

    Account* login() {
        string username, password;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        for (Account* acc : accounts) {
            if (acc->authenticate(username, password)) {
                cout << "Login successful. Welcome, " << username << " (" << acc->getAccountType() << ")" << endl;
                return acc;
            }
        }
        cout << "Login failed. Invalid username or password." << endl;
        return nullptr;
    }

    void createUser() {
        string username, password;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        accounts.push_back(new CustomerAccount(username, password));
        cout << "User account created successfully." << endl;
    }

    void stockAlert() // TODO
    {
        for(auto i=inventory.getProducts().begin();i!=inventory.getProducts().end();i++)
        {
            if(i->getQuantity() <= 4)
            {
                cout<<"Stock Empty Alert! for product : "<< i->getName() << "With product quantity" << i->getQuantity() <<endl;
            }
        }
        cout << "Stock Empty Alert!" << endl;
    }
    void createSeller() {
        string username, password;
        cout << "Enter username: ";
        cin >> username;
        cout << "Enter password: ";
        cin >> password;

        accounts.push_back(new MerchantAccount(username, password));
        cout << "Seller account created successfully." << endl;
    }

    void saveToFile() {
        ofstream file;
        file.open("users.csv");

        for (const auto& acc : accounts) {
            string a = acc->getAccountType();
            string b = acc->getUsername();
            string c = acc->getPassword();
            double d = acc->expenditure;
            double e = acc->getBalance();

            encrypt(a);
            encrypt(b);
            encrypt(c);

            d = d + 1029;
            e = e + 1029;

            file << a << "," << b << "," << c << "," << d << "," << e << endl;
        }
        file.close();
    }

    void loadFromFile() {
        ifstream file;
        file.open("users.csv");

        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                string type, username, password, exp, bal;
                getline(ss, type, ',');
                getline(ss, username, ',');
                getline(ss, password, ',');
                getline(ss, exp, ',');
                getline(ss, bal, ',');
                double expenditure = stoi(exp);
                double balance = stoi(bal);

                decrypt(type);
                decrypt(username);
                decrypt(password);
                expenditure = expenditure - 1029;
                balance = balance - 1029;

                if (type == "User") {
                    accounts.push_back(new CustomerAccount(username, password, expenditure, balance));
                } else if (type == "Seller") {
                    accounts.push_back(new MerchantAccount(username, password));
                }
            }

            file.close();
        } else {
            cout << "Error: Could not open file accounts.txt" << endl;
        }
    }

public:

    void printAccounts(){
        for(Account* each : accounts){
            cout << each->getUsername() << endl;
            cout << each->getPassword() << endl;
        }
    }
    Inventory getInventory() const {
        return inventory;
    }
    Store() {
        // create some default accounts for testing, removed from here
        inventory.loadInventoryFromFile("inventory.csv");
    }
    void Load()
    {
        loadFromFile();
    }
    void makeAPurchase(){
        cout<< "_____________________________________________________" << endl;
        inventory.printAllProducts();
        if (loggedInAccount && loggedInAccount->getAccountType() == "User") {
            double total = 0;
            char choice;
            do {
                int id;
                cout << "Enter product id: ";
                cin >> id;
                Product* product = inventory.findProduct(id);
                int quantity;
                if (!product) {
                    cout << "Product not found." << endl;
                    cout << "-----------------------------------------------------------" <<endl;
                    continue;
                }
                cout << "Enter quantity: ";
                cin >> quantity;
                while(quantity<=0 || quantity > product->getQuantity())
                {
                    cout << "Enter quantity in the range of 1 to " << product->getQuantity() << ": (_Enter Q to quit_)\n";
                    cin >> quantity;
                }
                
                total += product->getPrice() * quantity;

                if(loggedInAccount->getBalance() < total)  // When user has insufficient balance
                {
                    cout << "Insufficient Balance! Retry" << endl;
                    cout << "-----------------------------------------------------------" <<endl;
                    this->run();
                }
                cout << "Do you want to add more products to cart? (Y/N): ";
                product->setQuantity(product->getQuantity()-quantity);  //updating the inventory product quantity
                product->addSales(quantity); //updating the sales of the product
                cin >> choice;
            } while (choice == 'Y' || choice == 'y');
            cout << "Total amount: ₹" << total << endl<< "Transaction Completed!" << endl;
            if (total > 0) {
                static_cast<CustomerAccount*>(loggedInAccount)->updateBalance(-total);
                loggedInAccount->expenditure += total;
            }
            inventory.saveInventoryToFile("inventory.csv");
        } else {
            cout << "You need to be logged in as a user to make a purchase." << endl;
            cout << "-----------------------------------------------------------" <<endl;
        }
    }
    void printHighestSpending(){
        // CAN't do variable declaration in switch case
        printHighestSpending();
        cout << "-----------------------------------------------------------" <<endl;
        cout << "\nList of highest spending users in descending order:" << endl;

        // priority_queue<Account*, vector<Account*>, comparator> pq; //ANCHOR - This uses class or functor
        priority_queue<Account*, vector<Account*>, decltype(compareAccounts)*> pq(compareAccounts);  // this uses function pointer
        for (const auto& acc : accounts) {
            pq.push(acc);
        }
        int i = 9;
        cout << "Top 10 highest spending users:" << endl;

        while (i && !pq.empty()) {
            Account* acc = pq.top();
            pq.pop();
            cout << "Username: " << acc->getUsername() << ", Expenditure: " << acc->expenditure << endl;
            i--;
        }
        cout << "-----------------------------------------------------------" <<endl;
    }

    void run() {
        char choice;
        do {
            cout << "-----------------------------------------------------------" <<endl;
            cout << "Please choose an option:" << endl;
            cout << "1. Login" << endl;
            cout << "2. Create User Account" << endl;
            cout << "3. Create Seller Account" << endl;
            cout << "4. Update Balance (for users)" << endl;
            cout << "5. View Users with Balances (for sellers)" << endl;
            cout << "6. Make a purchase (Customers Only)" << endl;
            cout << "7. Stock Alert" << endl;
            cout << "8. List of highest spending users" << endl;
            cout << "Q. Quit" << endl;
            cin >> choice;

            switch (choice) {
            case '1':
                loggedInAccount = login();
                break;
            case '2':
                createUser();
                break;
            case '3':
                createSeller();
                break;
            case '4':
                if (loggedInAccount && loggedInAccount->getAccountType() == "User") {
                    double amount;
                    cout << "-----------------------------------------------------------" <<endl;
                    cout << "Enter amount to update balance: ";
                    cin >> amount;
                    static_cast<CustomerAccount*>(loggedInAccount)->updateBalance(amount);
                    cout << "Balance updated successfully." << endl;
                } else {
                    cout << "You need to be logged in as a user to update balance." << endl;
                    cout << "-----------------------------------------------------------" <<endl;
                }
                break;
            case '5':
                if (loggedInAccount && loggedInAccount->getAccountType() == "Seller") {
                    static_cast<MerchantAccount*>(loggedInAccount)->viewUsersWithBalances(accounts);
                } else {
                    cout << "You need to be logged in as a seller to view users with balances." << endl;
                    cout << "-----------------------------------------------------------" <<endl;

                }
                break;
            case '6':{
                makeAPurchase();
                break;}

            case '7':
                stockAlert();
                break;

            case '8': { // CAN't do variable declaration in switch case
                printHighestSpending();
                break;
            }
            
            case 'Q':
                cout << "Goodbye!" << endl;
                saveToFile();
                return;
            default:
                cout << "Invalid Choice. Please Try again" << endl;
                break;
            }
        } while (true);
    }

    void runInv(){

        cout << "-----------------------------------------------------------" <<endl;
        cout << "---------------Inventory Management System ----------------" <<endl;  
        cout << "------------------------- Welcome! ------------------------" <<endl;
        cout << "-----------------------------------------------------------" <<endl;
        char choice;
        do {
            cout << "Please choose an option:" << endl;
            cout << "1. Add a product" << endl;
            cout << "2. Remove a product" << endl;
            cout << "3. Find a product" << endl;
            cout << "4. Update a product" << endl;
            cout << "5. View all products" << endl;
            cout << "6. Save inventory to file" << endl;
            cout << "7. Load Inventory from file" << endl;
            cout << "8. Get most selling products" << endl;
            cout << "Q. Quit, and terminate" << endl;
            cout << "q. Quit, And go to Accounts Screen" << endl;
            cin >> choice;

            switch (choice) {
            case '1': {
                int id;
                string name, category;
                double price;
                int quantity;
                cout << "Enter ID: ";
                cin >> id;
                cout << "Enter product name: ";
                cin >> name;
                cout << "Enter product category: ";
                cin >> category;
                cout << "Enter product price: ₹ ";
                cin >> price;
                cout << "Enter product quantity: ";
                cin >> quantity;
                Product product(id, name, category, price, quantity);
                inventory.addProduct(product);
                break;
            }

            case '2': {
                int id;
                cout << "Enter product id: ";
                cin >> id;
                inventory.removeProduct(id);
                break;
            }

            case '3': {
                int id;
                cout << "Enter product id: ";
                cin >> id;
                Product* product = inventory.findProduct(id);
                if (product) {
                    cout << "Name: " << product->getName() << endl;
                    cout << "Category: " << product->getCategory() << endl;
                    cout << "Price: ₹ " << product->getPrice() << endl;
                    cout << "Quantity: " << product->getQuantity() << endl;
                    cout << "-----------------------------------------------------------" <<endl;
                }
                else {
                    cout << "Product not found." << endl;
                    cout << "-----------------------------------------------------------" <<endl;
                }
                break;
            }

            case '4': {
                int id;
                string name, category;
                double price;
                int quantity;
                cout << "Enter the product id: ";
                cin >> id;
                cout << "Enter new product name: ";
                cin >> name;
                cout << "Enter new product category: ";
                cin >> category;
                cout << "Enter new product price: ₹ ";
                cin >> price;
                cout << "Enter new product quantity: ";
                cin >> quantity;
                inventory.updateProduct(id, name, category, price, quantity);
                cout << "Product updated successfully." << endl;
                cout << "-----------------------------------------------------------" <<endl;
                break;
            }
            case '5': {
                inventory.printAllProducts();
                break;
            }
            case '6': {
                inventory.saveInventoryToFile("inventory.csv");
                cout << "Inventory saved to file." << endl;
                cout << "-----------------------------------------------------------" <<endl;
                break;
            }
            case '7': {
                inventory.loadInventoryFromFile("inventory.csv");
                cout << "Inventory loaded from file." << endl;
                cout << "-----------------------------------------------------------" <<endl;
                break;
            }
            case '8': { // CAN't do variable declaration in switch case
                    
                    priority_queue<Product> pq;
                    for (const auto& p : inventory.getProducts()) {
                        pq.push(p);
                    }
                    int i = 9;
                    cout << "Top 10 highest selling products:" << endl;
                    while (i) {
                        Product p = pq.top();
                        pq.pop();
                        cout << "Product Name: " << p.getName() << ", Sales: " << p.getTotalSales() << endl;
                        i--;
                    }
                    break;
                }

            case 'Q':
                cout << "Goodbye!" << endl;
                inventory.saveInventoryToFile("inventory.csv");
                cout << "-----------------------------------------------------------" <<endl;
            case 'q':
                cout << "Welcome to Accounts!" << endl;
                inventory.saveInventoryToFile("inventory.csv");
                run();
                cout << "-----------------------------------------------------------" <<endl;
            default:
                cout << "Invalid Choice. Please Try again" << endl;
                cout << "-----------------------------------------------------------" <<endl;
                break;
            }
        } while (true);
    }
};

int main() {
    system("\x1b[32m");

    int option;
    Store store; // solved the instantiation problem
    store.Load();
    
    Inventory inventory = store.getInventory();
    cout << "Enter 1 if you want to enter accounts and 0 if you want to enter inventory: ";
    cin >> option;
    if(option==1)
    {
        store.run();
    }
    else
    {
        store.runInv();
    }
    return 0;
}