#include<iostream>
#include<conio.h>
#include<windows.h>
using namespace std;

static int number = 32600;
int const startingAccountNumber = 32600;

class Customer {
    string username;
    float balance;
    int personal_account_no;
    string phone_no;
    string email;
    string address;
    string password;
    static int account_number;

public:
    Customer(string uname, string mail, string phone, string addr, float amt, string pass) {
        username = uname;
        email = mail;
        phone_no = phone;
        address = addr;
        balance = amt;
        password = pass;
        personal_account_no = account_number;
        account_number++;
    }

    Customer() {}

    string get_username() {
        return username;
    }

    string get_phoneno() {
        return phone_no;
    }

    string get_email() {
        return email;
    }

    string get_address() {
        return address;
    }

    float get_balance() {
        return balance;
    }

    void deposit(float amount) {
        balance += amount;
    }

    void withdraw(float amount) {
        if (amount <= balance) {
            balance -= amount;
        } else {
            cout << "Insufficient balance!" << endl;
        }
    }

    void update_username(string uname) {
        username = uname;
    }

    void update_phoneno(string phone) {
        phone_no = phone;
    }

    void update_email(string mail) {
        email = mail;
    }

    void update_address(string addr) {
        address = addr;
    }

    void update_password(string pass) {
        password = pass;
    }

    void show_balance() {
        cout << "Name: " << username << endl;
        cout << "Balance: " << balance << endl;
    }

    void show_accountnumber() {
        cout << "Account number: " << personal_account_no << endl;
    }

    static int get_accountnumber() {
        return account_number;
    }

    bool check_password(string pass) {
        return password == pass;
    }
} cust[10];

bool checkforlogin() {
    system("CLS");
    cout << "Welcome to the banking system" << endl << endl;
    cout << "1: Create an account" << endl;
    cout << "2: Login to an account" << endl << endl;

    cout << "Choose an option: ";
    int choice;
    while (true) {
        cin >> choice;
        system("CLS");
        if (choice == 1)
            return false;
        else if (choice == 2)
            return true;
        else
            cout << "Please choose an appropriate option: ";
    }
}

string passwordtexter() {
    string password;
    while (true) {
        char s = getch();
        if (s == '\r') break;
        if (s == '\b' && !password.empty()) {
            cout << "\b \b";
            password.pop_back();
        } else {
            password += s;
            cout << '*';
        }
    }
    return password;
}

Customer& login() {
    int account_number;
    string password;

    cout << "Enter your account number: ";
    cin >> account_number;
    cout << "Enter password: ";

    while (true) {
        password = passwordtexter();
        if (cust[account_number - startingAccountNumber].check_password(password)) {
            system("CLS");
            break;
        } else {
            cout << endl << "Account number or Password incorrect!" << endl;
            cout << "Enter valid password: ";
        }
    }

    cout << "Logged in successfully!" << endl;
    cout << "Press any key to continue...";
    getch();
    system("CLS");
    return cust[account_number - startingAccountNumber];
}

void create_account() {
    string username, email, phone_no, address, password, confirm_password;
    float balance;

    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, username);
    cout << "Enter email address: ";
    cin >> email;
    cout << "Enter phone number: ";
    cin >> phone_no;
    cin.ignore();
    cout << "Enter your address: ";
    getline(cin, address);
    cout << "Enter the amount you want to deposit: ";
    cin >> balance;

    cout << "Enter password: ";
    password = passwordtexter();

    while (true) {
        cout << "\nConfirm password: ";
        confirm_password = passwordtexter();
        if (password != confirm_password) {
            system("CLS");
            cout << "\nPasswords do not match!" << endl;
            cout << "Enter password again: ";
        } else {
            break;
        }
    }

    cust[number - startingAccountNumber] = Customer(username, email, phone_no, address, balance, password);
    cust[number - startingAccountNumber].show_accountnumber();

    cout << "\nAccount created successfully!" << endl;
    cout << "Press any key to continue...";
    getch();
    system("CLS");
    number++;
}

int Customer::account_number = 32600;

int main() {
    while (true) {
        if (checkforlogin() != true) {
            create_account();
        }

        Customer& customer = login();
        customer.show_accountnumber();
    }
    return 0;
}
