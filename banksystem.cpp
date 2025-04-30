#include "banksystem.h"
string BankSystem::passwordInput() {
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
    
void BankSystem::loadData() {
    ifstream officerFile(OFFICER_FILE);
    if (officerFile.is_open()) {
        string id, password;
        while (officerFile >> id >> password) {
            officers.emplace_back(id, password);
        }
        officerFile.close();
    } else {
        officers.emplace_back("admin", "admin123");
        saveOfficerData();
    }
    ifstream customerFile(CUSTOMER_FILE);
    if (customerFile.is_open()) {
        string line;
        while (getline(customerFile, line)) {
            if (line.empty()) continue;
            
            istringstream iss(line);
            string name, phone, email, address, aadhar;
            int age, accountNumber;
            float balance;
            
            getline(iss, name, '|');
            iss >> age;
            iss.ignore();
            getline(iss, phone, '|');
            getline(iss, email, '|');
            getline(iss, address, '|');
            getline(iss, aadhar, '|');
            iss >> accountNumber;
            iss.ignore();
            iss >> balance;
            
            customers.emplace_back(name, age, phone, email, address, aadhar, balance, accountNumber);
        }
        customerFile.close();
    }
}
    
void BankSystem::saveOfficerData() {
    ofstream officerFile(OFFICER_FILE);
    if (officerFile.is_open()) {
        for (const auto& officer : officers) {
            officerFile << officer.getId() << " " << officer.getPassword() << endl;
        }
        officerFile.close();
    }
}
    
void BankSystem::saveCustomerData() {
    ofstream customerFile(CUSTOMER_FILE);
    if (customerFile.is_open()) {
        for (const auto& customer : customers) {
            customerFile << customer.getName() << "|" 
                         << customer.getAge() << "|"
                         << customer.getPhone() << "|"
                         << customer.getEmail() << "|"
                         << customer.getAddress() << "|"
                         << customer.getAadhar() << "|"
                         << customer.getAccountNumber() << "|"
                         << customer.getBalance() << endl;
        }
        customerFile.close();
    }
}

void BankSystem::saveNextAccountNumber() {
    ofstream out(ACCOUNT_FILE);
    if (out.is_open()) {
        out << nextAccountNumber;
        out.close();
    }
}

void BankSystem::loadNextAccountNumber() {
    ifstream in(ACCOUNT_FILE);
    if (in.is_open()) {
        in >> nextAccountNumber;
        in.close();
    } else {
        nextAccountNumber = 326012200;
    }
}

bool BankSystem::officerLogin() {
    system("CLS");
    cout << "================ BANK OFFICER LOGIN ================" << endl;
    string id, password;
    
    cout << "Enter Officer ID: ";
    cin >> id;
    cout << "Enter Password: ";
    password = passwordInput();
    cout << endl;
    
    for (const auto& officer : officers) {
        if (officer.getId() == id && officer.authenticate(password)) {
            cout << "Login successful! Welcome, " << id << "!" << endl;
            cout << "Press any key to continue...";
            _getch();
            return true;
        }
    }
    
    cout << "Invalid credentials. Please try again." << endl;
    cout << "Press any key to continue...";
    _getch();
    return false;
}

string BankSystem::validphonenumberchecker(string phoneNumber) {
    while (true) {
        bool isValid = true;

        if (phoneNumber.size() != 10||phoneNumber.empty()) {
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

        cout <<endl<< "You entered an invalid number..."<<endl<<endl;
        cout << "Enter a valid number: ";
        cin >> phoneNumber;
    }
    return phoneNumber;
}

string BankSystem::validaadharchecker(string phoneNumber) {
    while (true) {
        bool isValid = true;

        if (phoneNumber.size() != 12||phoneNumber.empty()) {
            isValid = false;
        } else {
            for (int i = 0; i < 12; i++) {
                if (phoneNumber[i] < '0' || phoneNumber[i] > '9') {
                    isValid = false;
                    break;
                }
            }
        }

        if (isValid) break;

        cout <<endl<< "You entered an invalid aadhar number..."<<endl<<endl;
        cout << "Enter a valid aadhar number: ";
        cin >> phoneNumber;
    }
    return phoneNumber;
}

string BankSystem::validemailchecker(string email){
    while(true)
    {
        int dot=0,atherate=0;
        for(int i=0;i<email.size();i++)
        {
            if(email[i]=='.')   dot++;
            if(email[i]=='@'&&email[i+1]!='.'&&email[i-1]!='.') atherate++;
        }
        if(atherate==1&&dot>=1&&email[email.size()-1]!='.'&&email[email.size()-1]!='@'&&email[0]!='@'&&email[0]!='.') break;
        else{
            cout<<endl<<"You entered an invalid email addreess..."<<endl<<endl;
            cout<<"Enter an valid email address : ";
            cin>>email;
        }
    }
    return email;
}
    
void BankSystem::addCustomer() {
    system("CLS");
    cout << "======== ADD NEW CUSTOMER ========" << endl;
    
    string name, phone, email, address, aadhar;
    int age;
    float initialDeposit;
    
    cout << "Enter Name: ";
    cin.ignore();
    getline(cin, name);
    
    cout << "Enter Age: ";
    cin >> age;
    
    cout << "Enter Phone: ";
    cin >> phone;
    phone=validphonenumberchecker(phone);
    
    cout << "Enter Email: ";
    cin>>email;
    email=validemailchecker(email);
    
    cout << "Enter Address: ";
    cin.ignore();
    getline(cin, address);
    
    cout << "Enter Aadhar Number: ";
    cin >> aadhar;
    aadhar=validaadharchecker(aadhar);
    
    cout << "Enter Initial Deposit: ";
    cin >> initialDeposit;
    
    int accountNumber = nextAccountNumber++;
    customers.emplace_back(name, age, phone, email, address, aadhar, initialDeposit, accountNumber);
    saveCustomerData();
    saveNextAccountNumber();
    
    cout << "Customer added successfully!" << endl;
    cout << "Account Number: " << accountNumber << endl;
    cout << "Press any key to continue...";
    _getch();
}
    
Customer* BankSystem::findCustomerByAccountNumber(int accountNumber) {
    for (auto& customer : customers) {
        if (customer.getAccountNumber() == accountNumber) {
            return &customer;
        }
    }
    return nullptr;
}
    
void BankSystem::viewCustomerDetails() {
    system("CLS");
    int accountNumber;
    cout << "Enter Account Number: ";
    cin >> accountNumber;
    
    Customer* customer = findCustomerByAccountNumber(accountNumber);
    if (customer) {
        system("CLS");
        customer->showDetails();
    } else {
        cout << "Customer not found!" << endl;
    }
    cout << "Press any key to continue...";
    _getch();
}
    
void BankSystem::updateCustomerInfo() {
    system("CLS");
    cout << "======== UPDATE CUSTOMER INFO ========" << endl;
    int accountNumber;
    cout << "Enter Account Number: ";
    cin >> accountNumber;
    
    Customer* customer = findCustomerByAccountNumber(accountNumber);
    if (!customer) {
        cout << "Customer not found!" << endl;
        cout << "Press any key to continue...";
        _getch();
        return;
    }
    
    system("CLS");
    cout << "Current Details:" << endl;
    customer->showDetails();
    cout << endl;
    
    cout << "Select field to update:" << endl;
    cout << "1. Name" << endl;
    cout << "2. Age" << endl;
    cout << "3. Phone" << endl;
    cout << "4. Email" << endl;
    cout << "5. Address" << endl;
    cout << "6. Aadhar" << endl;
    cout << "7. Cancel" << endl;
    cout << "Enter choice: ";
    
    int choice;
    cin >> choice;
    cin.ignore();
    
    string newValue;
    int newAge;
    
    switch (choice) {
        case 1:
            cout << "Enter new Name: ";
            getline(cin, newValue);
            customer->updateName(newValue);
            break;
        case 2:
            cout << "Enter new Age: ";
            cin >> newAge;
            customer->updateAge(newAge);
            break;
        case 3:
            cout << "Enter new Phone: ";
            cin >> newValue;
            newValue=validphonenumberchecker(newValue);
            customer->updatePhone(newValue);
            break;
        case 4:
            cout << "Enter new Email: ";
            cin >> newValue;
            newValue=validemailchecker(newValue);
            customer->updateEmail(newValue);
            break;
        case 5:
            cout << "Enter new Address: ";
            getline(cin, newValue);
            customer->updateAddress(newValue);
            break;
        case 6:
            cout << "Enter new Aadhar: ";
            cin >> newValue;
            newValue=validaadharchecker(newValue);
            customer->updateAadhar(newValue);
            break;
        case 7:
            return;
        default:
            cout << "Invalid choice!" << endl;
            cout << "Press any key to continue...";
            _getch();
            return;
    }
    
    saveCustomerData();
    customer->recordTransaction("Info Update", 0);
    cout << "Customer information updated successfully!" << endl;
    cout << "Press any key to continue...";
    _getch();
}
    
void BankSystem::deleteCustomerAccount() {
    system("CLS");
    cout << "======== DELETE CUSTOMER ACCOUNT ========" << endl;
    int accountNumber;
    cout << "Enter Account Number: ";
    cin >> accountNumber;
    
    for (auto it = customers.begin(); it != customers.end(); ++it) {
        if (it->getAccountNumber() == accountNumber) {
            system("CLS");
            cout << "Customer Details:" << endl;
            it->showDetails();
            
            cout << endl << "Are you sure you want to delete this account? (y/n): ";
            char confirm;
            cin >> confirm;
            
            if (tolower(confirm) == 'y') {
                it->recordTransaction("Account Deleted", 0);
                customers.erase(it);
                saveCustomerData();
                cout << "Account deleted successfully!" << endl;
            } else {
                cout << "Deletion cancelled." << endl;
            }
            
            cout << "Press any key to continue...";
            _getch();
            return;
        }
    }
    
    cout << "Customer not found!" << endl;
    cout << "Press any key to continue...";
    _getch();
}
    
void BankSystem::depositAmount() {
    system("CLS");
    cout << "======== DEPOSIT AMOUNT ========" << endl;
    int accountNumber;
    float amount;
    
    cout << "Enter Account Number: ";
    cin >> accountNumber;
    
    Customer* customer = findCustomerByAccountNumber(accountNumber);
    if (!customer) {
        cout << "Customer not found!" << endl;
        cout << "Press any key to continue...";
        _getch();
        return;
    }
    
    cout << "Current Balance: $" << customer->getBalance() << endl;
    cout << "Enter Amount to Deposit: ";
    cin >> amount;
    
    try {
        customer->deposit(amount);
        saveCustomerData();
        cout << "Deposit successful! New Balance: $" << customer->getBalance() << endl;
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    
    cout << "Press any key to continue...";
    _getch();
}
    
void BankSystem::withdrawAmount() {
    system("CLS");
    cout << "======== WITHDRAW AMOUNT ========" << endl;
    int accountNumber;
    float amount;
    
    cout << "Enter Account Number: ";
    cin >> accountNumber;
    
    Customer* customer = findCustomerByAccountNumber(accountNumber);
    if (!customer) {
        cout << "Customer not found!" << endl;
        cout << "Press any key to continue...";
        _getch();
        return;
    }
    
    cout << "Current Balance: $" << customer->getBalance() << endl;
    cout << "Enter Amount to Withdraw: ";
    cin >> amount;
    
    try {
        customer->withdraw(amount);
        saveCustomerData();
        cout << "Withdrawal successful! New Balance: $" << customer->getBalance() << endl;
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
    
    cout << "Press any key to continue...";
    _getch();
}
    
void BankSystem::viewTransactionStatement() {
    system("CLS");
    cout << "======== VIEW TRANSACTION STATEMENT ========" << endl;
    int accountNumber;
    cout << "Enter Account Number: ";
    cin >> accountNumber;
    
    Customer* customer = findCustomerByAccountNumber(accountNumber);
    if (!customer) {
        cout << "Customer not found!" << endl;
        cout << "Press any key to continue...";
        _getch();
        return;
    }
    
    system("CLS");
    customer->printTransactionStatement();
    cout << "Press any key to continue...";
    _getch();
}
    
void BankSystem::viewAllCustomersList() {
    system("CLS");
    cout << "======== ALL CUSTOMERS ========" << endl;
    
    if (customers.empty()) {
        cout << "No customers found." << endl;
    } else {
        cout << left << setw(15) << "Account No." 
             << setw(25) << "Name" 
             << setw(10) << "Age" 
             << setw(15) << "Phone" 
             << setw(15) << "Balance" << endl;
        cout << string(80, '-') << endl;
        
        for (const auto& customer : customers) {
            cout << left << setw(15) << customer.getAccountNumber() 
                 << setw(25) << customer.getName() 
                 << setw(10) << customer.getAge() 
                 << setw(15) << customer.getPhone() 
                 << fixed << setprecision(2) << setw(15) << customer.getBalance() << endl;
        }
    }
    
    cout << "Press any key to continue...";
    _getch();
}
    
void BankSystem::viewAccountsByBalanceThreshold() {
    system("CLS");
    cout << "======== ACCOUNTS BY BALANCE ========" << endl;
    cout << "1. View accounts with balance above threshold" << endl;
    cout << "2. View accounts with balance below threshold" << endl;
    cout << "Enter choice: ";
    
    int choice;
    cin >> choice;
    
    if (choice != 1 && choice != 2) {
        cout << "Invalid choice!" << endl;
        cout << "Press any key to continue...";
        _getch();
        return;
    }
    
    float threshold;
    cout << "Enter threshold amount: ";
    cin >> threshold;
    
    system("CLS");
    cout << "Accounts ";
    cout << (choice == 1 ? "above " : "below ");
    cout << threshold << ":" << endl;
    
    cout << left << setw(15) << "Account No." 
         << setw(25) << "Name" 
         << setw(15) << "Balance" << endl;
    cout << string(55, '-') << endl;
    
    bool found = false;
    for (const auto& customer : customers) {
        if ((choice == 1 && customer.getBalance() >= threshold) || 
            (choice == 2 && customer.getBalance() <= threshold)) {
            found = true;
            cout << left << setw(15) << customer.getAccountNumber() 
                 << setw(25) << customer.getName() 
                 << fixed << setprecision(2) << setw(15) << customer.getBalance() << endl;
        }
    }
    
    if (!found) {
        cout << "No accounts found matching the criteria." << endl;
    }
    
    cout << "Press any key to continue...";
    _getch();
}
    
void BankSystem::mainMenu() {
    while (true) {
        system("CLS");
        cout << "=============== BANK MANAGEMENT SYSTEM ===============" << endl;
        cout << "1. Customer Account Management" << endl;
        cout << "2. Bank Account Operations" << endl;
        cout << "3. Reports & Lists" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1: customerAccountMenu(); break;
            case 2: accountOperationsMenu(); break;
            case 3: reportsMenu(); break;
            case 4: return;
            default:
                cout << "Invalid choice! Please try again." << endl;
                cout << "Press any key to continue...";
                _getch();
        }
    }
}
    
void BankSystem::customerAccountMenu() {
    while (true) {
        system("CLS");
        cout << "======== CUSTOMER ACCOUNT MANAGEMENT ========" << endl;
        cout << "1. Add New Customer" << endl;
        cout << "2. View Customer Details" << endl;
        cout << "3. Update Customer Info" << endl;
        cout << "4. Delete Customer Account" << endl;
        cout << "5. Back to Main Menu" << endl;
        cout << "Enter your choice: ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1: addCustomer(); break;
            case 2: viewCustomerDetails(); break;
            case 3: updateCustomerInfo(); break;
            case 4: deleteCustomerAccount(); break;
            case 5: return;
            default:
                cout << "Invalid choice! Please try again." << endl;
                cout << "Press any key to continue...";
                _getch();
        }
    }
}
    
void BankSystem::accountOperationsMenu() {
    while (true) {
        system("CLS");
        cout << "======== BANK ACCOUNT OPERATIONS ========" << endl;
        cout << "1. Deposit Amount" << endl;
        cout << "2. Withdraw Amount" << endl;
        cout << "3. View Transaction Statement" << endl;
        cout << "4. Back to Main Menu" << endl;
        cout << "Enter your choice: ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1: depositAmount(); break;
            case 2: withdrawAmount(); break;
            case 3: viewTransactionStatement(); break;
            case 4: return;
            default:
                cout << "Invalid choice! Please try again." << endl;
                cout << "Press any key to continue...";
                _getch();
        }
    }
}
    
void BankSystem::reportsMenu() {
    while (true) {
        system("CLS");
        cout << "======== REPORTS & LISTS ========" << endl;
        cout << "1. View All Customers" << endl;
        cout << "2. View Accounts by Balance" << endl;
        cout << "3. Back to Main Menu" << endl;
        cout << "Enter your choice: ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1: viewAllCustomersList(); break;
            case 2: viewAccountsByBalanceThreshold(); break;
            case 3: return;
            default:
                cout << "Invalid choice! Please try again." << endl;
                cout << "Press any key to continue...";
                _getch();
        }
    }
}
BankSystem::BankSystem() {
    loadNextAccountNumber();
    loadData();
}
    
void BankSystem::run() {
    while (true) {
        if (officerLogin()) {
            mainMenu();
        }
    }
}