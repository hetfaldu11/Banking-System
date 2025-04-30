#include "customer.h"

Customer::Customer(string name, int age, string phone, string email, 
         string address, string aadhar, float amount, int account_number) {
    this->name = name;
    this->age = age;
    this->phone = phone;
    this->email = email;
    this->address = address;
    this->aadhar = aadhar;
    this->balance = amount;
    this->accountNumber = account_number;
    recordTransaction("Account Creation", amount);
}

Customer::Customer() : accountNumber(-1), balance(0), age(0) {}

string Customer::getPhone() const { return phone; }
string Customer::getEmail() const { return email; }
string Customer::getAddress() const { return address; }
string Customer::getAadhar() const { return aadhar; }
float Customer::getBalance() const { return balance; }
string Customer::getName() const { return name; }
int Customer::getAge() const { return age; }
int Customer::getAccountNumber() const { return accountNumber; }

void Customer::deposit(float amount) {
    if (amount <= 0) {
        throw invalid_argument("Deposit amount must be positive");
    }
    balance += amount;
    recordTransaction("Deposit", amount);
}

float Customer::withdraw(float amount) {
    if (amount <= 0) {
        throw invalid_argument("Withdrawal amount must be positive");
    }
    if (amount > balance) {
        throw runtime_error("Insufficient balance");
    }
    balance -= amount;
    recordTransaction("Withdrawal", amount);
    return amount;
}

void Customer::updatePhone(string phone) { this->phone = phone; }
void Customer::updateEmail(string email) { this->email = email; }
void Customer::updateAddress(string address) { this->address = address; }
void Customer::updateName(string name) { this->name = name; }
void Customer::updateAge(int age) { this->age = age; }
void Customer::updateAadhar(string aadhar) { this->aadhar = aadhar; }

void Customer::showDetails() const {
    cout << "Account Number: " << accountNumber << endl;
    cout << "Name: " << name << endl;
    cout << "Age: " << age << endl;
    cout << "Address: " << address << endl;
    cout << "Phone: " << phone << endl;
    cout << "Email: " << email << endl;
    cout << "Aadhar: " << aadhar << endl;
    cout << "Balance: Rs." << fixed << setprecision(2) << balance << endl;
}

void Customer::recordTransaction(string type, float amount, string description) {
    Transaction t(type, amount, accountNumber, description);
    transactions.push_back(t);
    saveTransactionToFile(t);
}

void Customer::printTransactionStatement() const {
    cout << "Transaction Statement for Account: " << accountNumber << endl;
    cout << "Customer: " << name << endl;
    cout << "Current Balance: Rs." << fixed << setprecision(2) << balance << endl <<endl;
    
    cout << left << setw(30) << "Date & Time" 
             << setw(22) << "Type" 
             << setw(17) << "Amount" << endl
             << string(69, '-') <<endl;

    for (const auto& t : transactions) {
        cout << left << setw(30) << t.getTimestamp() 
             << setw(22) << t.getType() 
             << setw(17) << fixed << setprecision(2) << t.getAmount() << endl;
    }
}

void Customer::saveTransactionToFile(const Transaction& t) const {
    ofstream outfile("transactions_" + to_string(accountNumber) + ".txt", ios::app);
    if (outfile.is_open()) {
        outfile << t.toString() << endl;
        outfile.close();
    }
}

bool Customer::transfer(Customer& recipient, float amount) {
    if (amount <= 0) {
        throw invalid_argument("Transfer amount must be positive");
    }
    if (amount > balance) {
        throw runtime_error("Insufficient balance for transfer");
    }
    
    balance -= amount;
    recipient.balance += amount;
    
    recordTransaction("Transfer Out", amount, "To Acc# " + to_string(recipient.getAccountNumber()));
    recipient.recordTransaction("Transfer In", amount, "From Acc# " + to_string(accountNumber));
    
    return true;
}