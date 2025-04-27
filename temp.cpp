#include <iostream>
#include <string>
#include <conio.h>
#include <cmath>
#include <iomanip>
#include <vector>
#include <fstream>
#include <sstream>
#include <ctime>
#include <stdexcept>

using namespace std;

const int startingAccountNumber = 326012200;

class Transaction {
private:
    string type;
    float amount;
    string timestamp;
    int accountNumber;
    string description;

public:
    Transaction(string t, float a, int accNum, string desc = "") 
        : type(t), amount(a), accountNumber(accNum), description(desc) {
        time_t now = time(0);
        timestamp = ctime(&now);
        timestamp.pop_back(); // remove newline
    }

    string getType() const { return type; }
    float getAmount() const { return amount; }
    string getTimestamp() const { return timestamp; }
    int getAccountNumber() const { return accountNumber; }
    string getDescription() const { return description; }

    string toString() const {
        ostringstream oss;
        oss << timestamp << " - " << type << " - $" << fixed << setprecision(2) << amount;
        if (!description.empty()) {
            oss << " - " << description;
        }
        return oss.str();
    }
};

class Customer {
private:
    string name;
    string username;
    float balance;
    int personal_account_no;
    string phone_no;
    string email;
    string address;
    string password;
    vector<Transaction> transactions;

public:
    Customer(string name, string username, string email, string phone_no, 
             string address, float amount, string password, int account_number) {
        this->name = name;
        this->username = username;
        this->email = email;
        this->phone_no = phone_no;
        this->address = address;
        this->balance = amount;
        this->password = password;
        this->personal_account_no = account_number;
        recordTransaction("Account Creation", amount);
    }

    Customer() : personal_account_no(-1), balance(0) {}

    string get_username() const { return username; }
    string get_phoneno() const { return phone_no; }
    string get_email() const { return email; }
    string get_address() const { return address; }
    float get_balance() const { return balance; }
    string get_name() const { return name; }
    int get_personnalaccountnumber() const { return personal_account_no; }

    void deposit(float amount) {
        if (amount <= 0) {
            throw invalid_argument("Deposit amount must be positive");
        }
        balance += amount;
        recordTransaction("Deposit", amount);
    }

    float withdraw(float amount) {
        if (amount <= 0) {
            throw invalid_argument("Withdrawal amount must be positive");
        }
        if (amount > balance) {
            throw runtime_error("Insufficient balance");
        }
        if (balance - amount < 5000) {
            throw runtime_error("Minimum balance requirement not met");
        }
        balance -= amount;
        recordTransaction("Withdrawal", amount);
        return amount;
    }

    void update_username(string username) { this->username = username; }
    void update_phoneno(string phone_no) { this->phone_no = phone_no; }
    void update_email(string email) { this->email = email; }
    void update_address(string address) { this->address = address; }
    void update_password(string password) { this->password = password; }
    void update_name(string name) { this->name = name; }

    void show_balance() const {
        cout << "Name: " << name << endl;
        cout << "Balance: $" << fixed << setprecision(2) << balance << endl;
    }

    bool check_passwordclass(string password) const {
        return password == this->password;
    }

    void deleteaccount() {
        name = "";
        username = "";
        email = "";
        phone_no = "";
        address = "";
        balance = 0;
        password = "";
        personal_account_no = -1;
        recordTransaction("Account Deletion", 0);
    }

    void recordTransaction(string type, float amount, string description = "") {
        Transaction t(type, amount, personal_account_no, description);
        transactions.push_back(t);
        saveTransactionToFile(t);
    }

    void printTransactionStatement() const {
        cout << "Transaction Statement for Account: " << personal_account_no << endl;
        cout << "Customer: " << name << endl << endl;
        cout << "Date/Time                Type            Amount      Description" << endl;
        cout << "---------------------------------------------------------------" << endl;

        for (const auto& t : transactions) {
            cout << left << setw(24) << t.getTimestamp() 
                 << setw(16) << t.getType() 
                 << setw(12) << fixed << setprecision(2) << t.getAmount()
                 << t.getDescription() << endl;
        }
    }

private:
    void saveTransactionToFile(const Transaction& t) const {
        ofstream outfile("transactions_" + to_string(personal_account_no) + ".txt", ios::app);
        if (outfile.is_open()) {
            outfile << t.toString() << endl;
            outfile.close();
        }
    }
};

class BankSystem {
private:
    vector<Customer> customers;
    int nextAccountNumber;

    string passwordtexter() {
        string password;
        char ch;
        while ((ch = _getch()) != '\r') {
            if (ch == '\b') {
                if (!password.empty()) {
                    password.pop_back();
                    cout << "\b \b";
                }
            } else {
                password.push_back(ch);
                cout << '*';
            }
        }
        return password;
    }

    bool checkaccountnumberisexistornot(int account_number) {
        for (const auto& cust : customers) {
            if (cust.get_personnalaccountnumber() == account_number) {
                return true;
            }
        }
        return false;
    }

    string checkforuniqueUsername(const string& username) {
        string newUsername = username;
        while (true) {
            bool uniqueusername = true;
            for (const auto& cust : customers) {
                if (newUsername == cust.get_username()) {
                    uniqueusername = false;
                    cout << endl << "This username is already taken..." << endl << endl;
                    break;
                }
            }

            if (uniqueusername) {
                return newUsername;
            } else {
                cout << "Enter other username: ";
                getline(cin, newUsername);
            }
        }
    }

    string validphonenumberchecker(string phoneNumber) {
        while (true) {
            bool isValid = true;
            if (phoneNumber.size() != 10 || phoneNumber.empty()) {
                isValid = false;
            } else {
                for (int i = 0; i < 10; i++) {
                    if (phoneNumber[i] < '0' || phoneNumber[i] > '9') {
                        isValid = false;
                        break;
                    }
                }
            }

            if (isValid) break;

            cout << endl << "You entered an invalid number..." << endl << endl;
            cout << "Enter a valid number: ";
            cin >> phoneNumber;
            cin.ignore();
        }
        return phoneNumber;
    }

    string validemailchecker(string email) {
        while (true) {
            int dot = 0, atherate = 0;
            for (int i = 0; i < email.size(); i++) {
                if (email[i] == '.') dot++;
                if (email[i] == '@' && email[i+1] != '.' && email[i-1] != '.') atherate++;
            }
            if (atherate == 1 && dot >= 1 && 
                email[email.size()-1] != '.' && email[email.size()-1] != '@' &&
                email[0] != '@' && email[0] != '.') {
                break;
            } else {
                cout << endl << "You entered an invalid email address..." << endl << endl;
                cout << "Enter a valid email address: ";
                cin >> email;
                cin.ignore();
            }
        }
        return email;
    }

    float validdepositchecker(float amount) {
        while (true) {
            if (amount < 5000) {
                cout << endl << "Amount must be greater than or equal to 5000" << endl << endl;
                cout << "Enter amount you want to deposit: ";
                cin >> amount;
                cin.ignore();
            } else {
                break;
            }
        }
        return amount;
    }

    bool passwordchecker(const Customer& client) {
        string password;
        cout << "Enter password: ";
        password = passwordtexter();

        while (true) {
            if (client.check_passwordclass(password)) {
                system("CLS");
                return true;
            } else {
                system("CLS");
                cout << endl << "Password is incorrect..." << endl << endl;
                cout << "Enter password or enter 0 to exit: ";
                password = passwordtexter();
                if (password == "0") return false;
            }
        }
    }

    void fundtransfer(Customer& client) {
        int account_number;
        float amount;
        cout << "Enter account number you want to transfer money to: ";
        cin >> account_number;
        cin.ignore();

        while (true) {
            if (!checkaccountnumberisexistornot(account_number)) {
                system("CLS");
                cout << endl << "Account number does not exist..." << endl << endl;
                cout << "Enter valid account number or enter 0 to exit: ";
                cin >> account_number;
                cin.ignore();
                if (account_number == 0) return;
            } else {
                break;
            }
        }

        while (true) {
            cout << "Enter amount you want to transfer: ";
            cin >> amount;
            cin.ignore();

            try {
                if (client.get_balance() - amount < 5000) {
                    throw runtime_error("Insufficient balance to maintain minimum requirement");
                }

                string password;
                cout << "Enter password or press 0 to cancel: ";
                password = passwordtexter();
                cout << endl;

                if (password == "0") {
                    break;
                }

                if (client.check_passwordclass(password)) {
                    client.withdraw(amount);
                    getCustomerByAccountNumber(account_number).deposit(amount);
                    
                    client.recordTransaction("Transfer Out", amount, "To Acct: " + to_string(account_number));
                    getCustomerByAccountNumber(account_number).recordTransaction("Transfer In", amount, "From Acct: " + to_string(client.get_personnalaccountnumber()));
                    
                    system("CLS");
                    cout << endl << "Fund Transfer Successful..." << endl << endl;
                    cout << "Press any key to continue...";
                    _getch();
                    system("CLS");
                    return;
                } else {
                    system("CLS");
                    cout << "Incorrect password..." << endl << endl;
                }
            } catch (const exception& e) {
                system("CLS");
                cout << endl << "Error: " << e.what() << endl << endl;
                cout << "Enter 0 to exit or amount to transfer: ";
                cin >> amount;
                cin.ignore();
                if (amount == 0) break;
            }
        }
        system("CLS");
    }

    Customer& getCustomerByAccountNumber(int account_number) {
        for (auto& cust : customers) {
            if (cust.get_personnalaccountnumber() == account_number) {
                return cust;
            }
        }
        throw runtime_error("Account not found");
    }

    double EMIcalculatorformonths() {
        double principalamount, annualrate, monthlyrate;
        int months;

        cout << "Enter Loan amount: ";
        cin >> principalamount;
        cout << "Enter Interest rate (Annual %): ";
        cin >> annualrate;
        cout << "Loan Tenure in months: ";
        cin >> months;
        cin.ignore();

        monthlyrate = (annualrate / 100) / 12;

        double EMI = (principalamount * monthlyrate * pow(1 + monthlyrate, months)) / 
                     (pow(1 + monthlyrate, months) - 1);

        system("CLS");

        cout << "Loan EMI: $" << fixed << setprecision(2) << EMI << endl;
        cout << "Total interest payable: $" << (EMI * months) - principalamount << endl;
        cout << "Total payment: $" << EMI * months << endl << endl;

        return EMI;
    }

    void create_account() {
        system("CLS");
        cout << "Create an account" << endl << endl;
        string name, username, email, phone_no, address, password, confirm_password;
        float balance;

        cout << "Enter your name: ";
        getline(cin, name);

        cout << "Enter your Username: ";
        getline(cin, username);
        username = checkforuniqueUsername(username);

        cout << "Enter email address: ";
        cin >> email;
        email = validemailchecker(email);

        cout << "Enter phone no: ";
        cin >> phone_no;
        phone_no = validphonenumberchecker(phone_no);
        cin.ignore();

        cout << "Enter your address: ";
        getline(cin, address);

        cout << "Enter amount you want to deposit: ";
        cin >> balance;
        balance = validdepositchecker(balance);
        cin.ignore();

        while (true) {
            cout << "Enter password: ";
            password = passwordtexter();
            cout << endl << "Confirm password: ";
            confirm_password = passwordtexter();
            if (password != confirm_password) {
                system("CLS");
                cout << endl << "Password does not match! Try again..." << endl;
                continue;
            } else {
                break;
            }
        }

        int account_number = nextAccountNumber++;
        customers.emplace_back(name, username, email, phone_no, address, balance, password, account_number);
        
        system("CLS");
        cout << "Your account is successfully created!" << endl << endl;
        cout << "Your AccountNumber: " << account_number << endl;
        cout << "Press any key to continue...";
        _getch();
    }

    Customer& login(bool& loginflag) {
        system("CLS");
        cout << "Login Section: " << endl << endl;

        string AccountnumberOrUsername, password;
        cout << "Enter AccountNumber or Username: ";
        cin >> AccountnumberOrUsername;
        cin.ignore();

        bool AccountNumber = true;
        for (char val : AccountnumberOrUsername) {
            if (!(val >= '0' && val <= '9')) {
                AccountNumber = false;
                break;
            }
        }

        if (AccountNumber) {
            int account_number = stoi(AccountnumberOrUsername);
            while (true) {
                cout << "Enter password: ";
                password = passwordtexter();
                cout << endl;

                try {
                    Customer& customer = getCustomerByAccountNumber(account_number);
                    if (customer.check_passwordclass(password) && !password.empty()) {
                        system("CLS");
                        cout << "Logged in successfully!" << endl;
                        cout << "Press any key to continue...";
                        _getch();
                        system("CLS");
                        loginflag = true;
                        return customer;
                    } else {
                        throw runtime_error("Accountnumber or Password incorrect!");
                    }
                } catch (const exception& e) {
                    cout << endl << e.what() << endl;
                    cout << "Enter your account number or enter 0 for exit: ";
                    cin >> account_number;
                    cin.ignore();
                    if (account_number == 0) {
                        loginflag = false;
                        throw;
                    }
                }
            }
        } else {
            string Username = AccountnumberOrUsername;
            while (true) {
                cout << "Enter password: ";
                password = passwordtexter();
                cout << endl;

                try {
                    for (auto& cust : customers) {
                        if (cust.get_username() == Username && cust.check_passwordclass(password)) {
                            system("CLS");
                            cout << "Logged in successfully!" << endl;
                            cout << "Press any key to continue...";
                            _getch();
                            system("CLS");
                            loginflag = true;
                            return cust;
                        }
                    }
                    throw runtime_error("Invalid Username or Password!");
                } catch (const exception& e) {
                    system("CLS");
                    cout << endl << e.what() << endl << endl;
                    cout << "Enter valid Username or enter 0 for exit: ";
                    cin >> Username;
                    cin.ignore();
                    if (Username == "0") {
                        loginflag = false;
                        throw;
                    }
                }
            }
        }
    }

    bool checkforcreateaccountorlogin() {
        system("CLS");
        cout << "Welcome to the banking system" << endl << endl;
        cout << "1: Login an account" << endl;
        cout << "2: Create an account" << endl;
        cout << "3: Exit" << endl << endl;

        cout << "Choose an option: ";
        while (true) {
            int choice;
            cin >> choice;
            cin.ignore();
            system("CLS");
            if (choice == 1) return true;
            else if (choice == 2) return false;
            else if (choice == 3) exit(0);
            else cout << "Please choose appropriate option: ";
        }
    }

    void updateAccountInfo(Customer& client) {
        system("CLS");
        cout << "Update account information: " << endl << endl;
        cout << "1: Update name" << endl;
        cout << "2: Update username" << endl;
        cout << "3: Update email" << endl;
        cout << "4: Update phone no" << endl;
        cout << "5: Update address" << endl;
        cout << "6: Update password" << endl;
        cout << "7: Back" << endl;
        cout << endl << "Choose an option: ";
        
        int choice;
        cin >> choice;
        cin.ignore();
        
        string new_data;
        string password;
        bool back = false;

        try {
            switch (choice) {
                case 1:
                    system("CLS");
                    cout << "Enter new name: ";
                    getline(cin, new_data);

                    cout << "Enter password or enter 0 to exit: ";
                    password = passwordtexter();
                    cout << endl;

                    if (password == "0") break;
                    if (!client.check_passwordclass(password)) {
                        throw runtime_error("Incorrect password!");
                    }

                    client.update_name(new_data);
                    client.recordTransaction("Name Update", 0, "Changed to: " + new_data);
                    cout << "Name updated successfully!" << endl;
                    cout << "Press any key to continue...";
                    _getch();
                    break;

                case 2:
                    system("CLS");
                    cout << "Enter new username: ";
                    getline(cin, new_data);
                    new_data = checkforuniqueUsername(new_data);

                    cout << "Enter password or enter 0 to exit: ";
                    password = passwordtexter();
                    cout << endl;

                    if (password == "0") break;
                    if (!client.check_passwordclass(password)) {
                        throw runtime_error("Incorrect password!");
                    }

                    client.update_username(new_data);
                    client.recordTransaction("Username Update", 0, "Changed to: " + new_data);
                    cout << "Username updated successfully!" << endl;
                    cout << "Press any key to continue...";
                    _getch();
                    break;

                case 3:
                    system("CLS");
                    cout << "Enter new email: ";
                    cin >> new_data;
                    new_data = validemailchecker(new_data);

                    cout << "Enter password or enter 0 to exit: ";
                    password = passwordtexter();
                    cout << endl;

                    if (password == "0") break;
                    if (!client.check_passwordclass(password)) {
                        throw runtime_error("Incorrect password!");
                    }

                    client.update_email(new_data);
                    client.recordTransaction("Email Update", 0, "Changed to: " + new_data);
                    cout << "Email updated successfully!" << endl;
                    cout << "Press any key to continue...";
                    _getch();
                    break;

                case 4:
                    system("CLS");
                    cout << "Enter new phone no: ";
                    cin >> new_data;
                    new_data = validphonenumberchecker(new_data);

                    cout << "Enter password or enter 0 to exit: ";
                    password = passwordtexter();
                    cout << endl;

                    if (password == "0") break;
                    if (!client.check_passwordclass(password)) {
                        throw runtime_error("Incorrect password!");
                    }

                    client.update_phoneno(new_data);
                    client.recordTransaction("Phone Update", 0, "Changed to: " + new_data);
                    cout << "Phone no updated successfully!" << endl;
                    cout << "Press any key to continue...";
                    _getch();
                    break;

                case 5:
                    system("CLS");
                    cout << "Enter new address: ";
                    getline(cin, new_data);

                    cout << "Enter password or enter 0 to exit: ";
                    password = passwordtexter();
                    cout << endl;

                    if (password == "0") break;
                    if (!client.check_passwordclass(password)) {
                        throw runtime_error("Incorrect password!");
                    }

                    client.update_address(new_data);
                    client.recordTransaction("Address Update", 0, "Changed to: " + new_data);
                    cout << "Address updated successfully!" << endl;
                    cout << "Press any key to continue...";
                    _getch();
                    break;

                case 6:
                    system("CLS");
                    cout << "Enter new password: ";
                    new_data = passwordtexter();
                    cout << endl;

                    cout << "Enter current password to confirm or enter 0 to exit: ";
                    password = passwordtexter();
                    cout << endl;

                    if (password == "0") break;
                    if (!client.check_passwordclass(password)) {
                        throw runtime_error("Incorrect current password!");
                    }

                    client.update_password(new_data);
                    client.recordTransaction("Password Update", 0);
                    cout << "Password updated successfully!" << endl;
                    cout << "Press any key to continue...";
                    _getch();
                    break;

                case 7:
                    break;

                default:
                    throw runtime_error("Invalid option!");
            }
        } catch (const exception& e) {
            system("CLS");
            cout << "Error: " << e.what() << endl;
            cout << "Press any key to continue...";
            _getch();
        }
        system("CLS");
    }

public:
    BankSystem() : nextAccountNumber(startingAccountNumber) {}

    void run() {
        while (true) {
            bool loginflag = true;
            try {
                while (checkforcreateaccountorlogin() != true) {
                    create_account();
                }

                Customer& client = login(loginflag);
                if (loginflag == false) continue;

                bool flag = true;
                while (flag) {
                    cout << "Welcome " << client.get_name() << endl << endl;
                    cout << "1: Deposit" << endl;
                    cout << "2: Withdraw" << endl;
                    cout << "3: Fund Transfer" << endl;
                    cout << "4: Show balance" << endl;
                    cout << "5: View account details" << endl;
                    cout << "6: Update account information" << endl;
                    cout << "7: EMI calculator" << endl;
                    cout << "8: Transaction Statement" << endl;
                    cout << "9: Delete Account" << endl;
                    cout << "10: Logout" << endl;
                    cout << endl << "Choose an option: ";

                    int choice;
                    cin >> choice;
                    cin.ignore();

                    try {
                        float amount;
                        string password;

                        switch (choice) {
                            case 1:
                                system("CLS");
                                cout << "Enter the amount you want to deposit: ";
                                cin >> amount;
                                cin.ignore();
                                client.deposit(amount);
                                cout << "Amount deposited successfully!" << endl;
                                cout << "Press any key to continue...";
                                _getch();
                                system("CLS");
                                break;

                            case 2:
                                system("CLS");
                                cout << "Enter the amount you want to withdraw: ";
                                cin >> amount;
                                cin.ignore();

                                cout << "Enter password or exit with 0: ";
                                password = passwordtexter();
                                cout << endl;

                                if (password == "0") break;
                                if (client.check_passwordclass(password)) {
                                    client.withdraw(amount);
                                    system("CLS");
                                    cout << endl << "Withdrawal Successful..." << endl;
                                    cout << endl << "Remaining Balance: $" << client.get_balance() << endl << endl;
                                    cout << "Press any key to continue...";
                                    _getch();
                                } else {
                                    system("CLS");
                                    cout << "Incorrect password..." << endl;
                                    cout << "Press any key to continue...";
                                    _getch();
                                }
                                system("CLS");
                                break;

                            case 4:
                                system("CLS");
                                client.show_balance();
                                cout << "Press any key to continue...";
                                _getch();
                                system("CLS");
                                break;

                            case 5:
                                system("CLS");
                                cout << "Name: " << client.get_name() << endl;
                                cout << "Username: " << client.get_username() << endl;
                                cout << "Accountnumber: " << client.get_personnalaccountnumber() << endl;
                                cout << "Email: " << client.get_email() << endl;
                                cout << "Phone: " << client.get_phoneno() << endl;
                                cout << "Address: " << client.get_address() << endl;
                                cout << "Press any key to continue...";
                                _getch();
                                system("CLS");
                                break;

                            case 3:
                                fundtransfer(client);
                                break;

                            case 8:
                                system("CLS");
                                client.printTransactionStatement();
                                cout << "Press any key to continue...";
                                _getch();
                                system("CLS");
                                break;

                            case 9:
                                system("CLS");
                                if (passwordchecker(client)) {
                                    client.deleteaccount();
                                    cout << "Your account is successfully deleted..." << endl << endl;
                                    cout << "Press any key to continue.";
                                    _getch();
                                    flag = false;
                                }
                                system("CLS");
                                break;

                            case 7:
                                system("CLS");
                                EMIcalculatorformonths();
                                cout << "Press any key to continue...";
                                _getch();
                                system("CLS");
                                break;

                            case 10:
                                system("CLS");
                                cout << "Logging out..." << endl;
                                cout << "Press any key to continue...";
                                _getch();
                                system("CLS");
                                flag = false;
                                break;

                            case 6:
                                updateAccountInfo(client);
                                break;

                            default:
                                system("CLS");
                                cout << "Please choose appropriate option from list..." << endl << endl;
                                cout << "Press any key to continue...";
                                _getch();
                                system("CLS");
                                break;
                        }
                    } catch (const exception& e) {
                        system("CLS");
                        cout << "Error: " << e.what() << endl;
                        cout << "Press any key to continue...";
                        _getch();
                        system("CLS");
                    }
                }
            } catch (...) {
                // Login failed, continue to main menu
            }
        }
    }
};

int main() {
    BankSystem bank;
    bank.run();
    return 0;
}