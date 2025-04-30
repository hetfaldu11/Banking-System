#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>
#include <conio.h>

#define MAX_CUSTOMERS 50
#define MAX_TRANSACTIONS 100
#define MAX_OFFICERS 5
#define MAX_STRING_LENGTH 50
#define CUSTOMER_FILE "customers.dat"
#define OFFICER_FILE "officers.dat"
#define ACCOUNT_FILE "next_account_number.dat"


typedef struct {
    char type[MAX_STRING_LENGTH];
    float amount;
    char timestamp[MAX_STRING_LENGTH];
    int accountNumber;
    char description[MAX_STRING_LENGTH];
} Transaction;

typedef struct {
    char name[MAX_STRING_LENGTH];
    int age;
    float balance;
    int accountNumber;
    char phone[MAX_STRING_LENGTH];
    char email[MAX_STRING_LENGTH];
    char address[MAX_STRING_LENGTH];
    char aadhar[MAX_STRING_LENGTH];
    Transaction transactions[MAX_TRANSACTIONS];
    int transactionCount;
} Customer;

typedef struct {
    char id[MAX_STRING_LENGTH];
    char password[MAX_STRING_LENGTH];
} BankOfficer;

typedef struct {
    Customer customers[MAX_CUSTOMERS];
    int customerCount;
    BankOfficer officers[MAX_OFFICERS];
    int officerCount;
    int nextAccountNumber;
} BankSystem;

void initializeBankSystem(BankSystem* bank);
void runBankSystem(BankSystem* bank);
int officerLogin(BankSystem* bank);
void mainMenu(BankSystem* bank);
void customerAccountMenu(BankSystem* bank);
void accountOperationsMenu(BankSystem* bank);
void reportsMenu(BankSystem* bank);
void addCustomer(BankSystem* bank);
void viewCustomerDetails(BankSystem* bank);
void updateCustomerInfo(BankSystem* bank);
void deleteCustomerAccount(BankSystem* bank);
void depositAmount(BankSystem* bank);
void withdrawAmount(BankSystem* bank);
void viewTransactionStatement(BankSystem* bank);
void viewAllCustomersList(BankSystem* bank);
void viewAccountsByBalanceThreshold(BankSystem* bank);
Customer* findCustomerByAccountNumber(BankSystem* bank, int accountNumber);
void loadOfficerData(BankSystem* bank);
void loadCustomerData(BankSystem* bank);
void saveOfficerData(BankSystem* bank);
void saveCustomerData(BankSystem* bank);
void initializeCustomer(Customer* c, const char* name, int age, const char* phone, 
                       const char* email, const char* address, const char* aadhar, 
                       float amount, int account_number);
void createTransaction(const char* type, float amount, int accountNum, const char* desc, Transaction* t);
void deposit(Customer* c, float amount);
int withdraw(Customer* c, float amount);
void showCustomerDetails(const Customer* c);
void printTransactionStatement(const Customer* c);
void initializeBankOfficer(BankOfficer* o, const char* id, const char* password);
int authenticateOfficer(const BankOfficer* o, const char* inputPassword);
void clearScreen();
void clearInputBuffer();
void waitForKey();

int main() {
    BankSystem bank;
    initializeBankSystem(&bank);
    runBankSystem(&bank);
    return 0;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void waitForKey() {
    printf("\nPress Enter to continue...");
    clearInputBuffer();
}

void createTransaction(const char* type, float amount, int accountNum, const char* desc, Transaction* t) {
    strncpy(t->type, type, MAX_STRING_LENGTH-1);
    t->type[MAX_STRING_LENGTH-1] = '\0';
    t->amount = amount;
    t->accountNumber = accountNum;
    
    if (desc != NULL) {
        strncpy(t->description, desc, MAX_STRING_LENGTH-1);
        t->description[MAX_STRING_LENGTH-1] = '\0';
    } else {
        strcpy(t->description, "");
    }
    
    time_t now = time(NULL);
    struct tm* tm_info = localtime(&now);
    strftime(t->timestamp, sizeof(t->timestamp), "%Y-%m-%d %H:%M:%S", tm_info);
}

void initializeCustomer(Customer* c, const char* name, int age, const char* phone, 
                       const char* email, const char* address, const char* aadhar, 
                       float amount, int account_number) {
    strncpy(c->name, name, MAX_STRING_LENGTH-1);
    c->name[MAX_STRING_LENGTH-1] = '\0';
    c->age = age;
    strncpy(c->phone, phone, MAX_STRING_LENGTH-1);
    c->phone[MAX_STRING_LENGTH-1] = '\0';
    strncpy(c->email, email, MAX_STRING_LENGTH-1);
    c->email[MAX_STRING_LENGTH-1] = '\0';
    strncpy(c->address, address, MAX_STRING_LENGTH-1);
    c->address[MAX_STRING_LENGTH-1] = '\0';
    strncpy(c->aadhar, aadhar, MAX_STRING_LENGTH-1);
    c->aadhar[MAX_STRING_LENGTH-1] = '\0';
    c->balance = amount;
    c->accountNumber = account_number;
    c->transactionCount = 0;
    
    createTransaction("Account Creation", amount, account_number, NULL, &c->transactions[c->transactionCount++]);
}

void deposit(Customer* c, float amount) {
    if (amount <= 0) {
        printf("Deposit amount must be positive\n");
        return;
    }
    c->balance += amount;
    createTransaction("Deposit", amount, c->accountNumber, NULL, &c->transactions[c->transactionCount++]);
}

int withdraw(Customer* c, float amount) {
    if (amount <= 0) {
        printf("Withdrawal amount must be positive\n");
        return 0;
    }
    if (amount > c->balance) {
        printf("Insufficient balance\n");
        return 0;
    }
    c->balance -= amount;
    createTransaction("Withdrawal", amount, c->accountNumber, NULL, &c->transactions[c->transactionCount++]);
    return 1;
}

void showCustomerDetails(const Customer* c) {
    printf("\nAccount Number: %d\n", c->accountNumber);
    printf("Name: %s\n", c->name);
    printf("Age: %d\n", c->age);
    printf("Address: %s\n", c->address);
    printf("Phone: %s\n", c->phone);
    printf("Email: %s\n", c->email);
    printf("Aadhar: %s\n", c->aadhar);
    printf("Balance: $%.2f\n", c->balance);
}

void printTransactionStatement(const Customer* c) {
    printf("\nTransaction Statement for Account: %d\n", c->accountNumber);
    printf("Customer: %s\n\n", c->name);
    printf("Date/Time                Type            Amount      Description\n");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < c->transactionCount; i++) {
        const Transaction* t = &c->transactions[i];
        printf("%-24s%-16s%-12.2f%s\n", t->timestamp, t->type, t->amount, t->description);
    }
}

void initializeBankOfficer(BankOfficer* o, const char* id, const char* password) {
    strncpy(o->id, id, MAX_STRING_LENGTH-1);
    o->id[MAX_STRING_LENGTH-1] = '\0';
    strncpy(o->password, password, MAX_STRING_LENGTH-1);
    o->password[MAX_STRING_LENGTH-1] = '\0';
}

int authenticateOfficer(const BankOfficer* o, const char* inputPassword) {
    return strcmp(o->password, inputPassword) == 0;
}

void loadOfficerData(BankSystem* bank) {
    FILE* officerFile = fopen(OFFICER_FILE, "r");
    if (officerFile != NULL) {
        char id[MAX_STRING_LENGTH];
        char password[MAX_STRING_LENGTH];
        bank->officerCount = 0;
        
        while (fscanf(officerFile, "%49s %49s", id, password) == 2 && bank->officerCount < MAX_OFFICERS) {
            initializeBankOfficer(&bank->officers[bank->officerCount], id, password);
            bank->officerCount++;
        }
        fclose(officerFile);
    } else {
        initializeBankOfficer(&bank->officers[0], "admin", "admin123");
        bank->officerCount = 1;
        saveOfficerData(bank);
    }
}

void loadCustomerData(BankSystem* bank) {
    FILE* customerFile = fopen(CUSTOMER_FILE, "r");
    if (customerFile != NULL) {
        char line[512];
        bank->customerCount = 0;
        
        while (fgets(line, sizeof(line), customerFile) && bank->customerCount < MAX_CUSTOMERS) {
            if (strlen(line) <= 1) continue;
            
            char name[MAX_STRING_LENGTH] = {0};
            int age = 0;
            char phone[MAX_STRING_LENGTH] = {0};
            char email[MAX_STRING_LENGTH] = {0};
            char address[MAX_STRING_LENGTH] = {0};
            char aadhar[MAX_STRING_LENGTH] = {0};
            int accountNumber = 0;
            float balance = 0.0f;
            
            char* token = strtok(line, "|");
            if (token) strncpy(name, token, MAX_STRING_LENGTH-1);
            
            token = strtok(NULL, "|");
            if (token) age = atoi(token);
            
            token = strtok(NULL, "|");
            if (token) strncpy(phone, token, MAX_STRING_LENGTH-1);
            
            token = strtok(NULL, "|");
            if (token) strncpy(email, token, MAX_STRING_LENGTH-1);
            
            token = strtok(NULL, "|");
            if (token) strncpy(address, token, MAX_STRING_LENGTH-1);
            
            token = strtok(NULL, "|");
            if (token) strncpy(aadhar, token, MAX_STRING_LENGTH-1);
            
            token = strtok(NULL, "|");
            if (token) accountNumber = atoi(token);
            
            token = strtok(NULL, "|");
            if (token) balance = atof(token);
            
            initializeCustomer(&bank->customers[bank->customerCount], name, age, phone, 
                             email, address, aadhar, balance, accountNumber);
            bank->customerCount++;
        }
        fclose(customerFile);
    }
}

void saveOfficerData(BankSystem* bank) {
    FILE* officerFile = fopen(OFFICER_FILE, "w");
    if (officerFile != NULL) {
        for (int i = 0; i < bank->officerCount; i++) {
            fprintf(officerFile, "%s %s\n", bank->officers[i].id, bank->officers[i].password);
        }
        fclose(officerFile);
    } else {
        printf("Error saving officer data!\n");
    }
}

void saveCustomerData(BankSystem* bank) {
    FILE* customerFile = fopen(CUSTOMER_FILE, "w");
    if (customerFile != NULL) {
        for (int i = 0; i < bank->customerCount; i++) {
            const Customer* c = &bank->customers[i];
            fprintf(customerFile, "%s|%d|%s|%s|%s|%s|%d|%.2f\n", 
                   c->name, c->age, c->phone, c->email, c->address, c->aadhar, 
                   c->accountNumber, c->balance);
        }
        fclose(customerFile);
    } else {
        printf("Error saving customer data!\n");
    }
}

int officerLogin(BankSystem* bank) {
    clearScreen();
    printf("================ BANK OFFICER LOGIN ================\n");
    char id[MAX_STRING_LENGTH] = {0};
    char password[MAX_STRING_LENGTH] = {0};
    
    printf("Enter Officer ID: ");
    if (scanf("%49s", id) != 1) {
        clearInputBuffer();
        printf("Invalid input!\n");
        waitForKey();
        return 0;
    }
    
    printf("Enter Password: ");
    if (scanf("%49s", password) != 1) {
        clearInputBuffer();
        printf("Invalid input!\n");
        waitForKey();
        return 0;
    }
    
    for (int i = 0; i < bank->officerCount; i++) {
        if (strcmp(bank->officers[i].id, id) == 0 && 
            authenticateOfficer(&bank->officers[i], password)) {
            printf("\nLogin successful! Welcome, %s!\n", id);
            waitForKey();
            return 1;
        }
    }

    printf("\nInvalid credentials. Please try again.\n");
    waitForKey();
    return 0;
}

Customer* findCustomerByAccountNumber(BankSystem* bank, int accountNumber) {
    for (int i = 0; i < bank->customerCount; i++) {
        if (bank->customers[i].accountNumber == accountNumber) {
            return &bank->customers[i];
        }
    }
    return NULL;
}

void addCustomer(BankSystem* bank) {
    clearScreen();
    printf("======== ADD NEW CUSTOMER ========\n");
    
    char name[MAX_STRING_LENGTH] = {0};
    int age = 0;
    char phone[MAX_STRING_LENGTH] = {0};
    char email[MAX_STRING_LENGTH] = {0};
    char address[MAX_STRING_LENGTH] = {0};
    char aadhar[MAX_STRING_LENGTH] = {0};
    float initialDeposit = 0.0f;
    
    printf("Enter Name: ");
    clearInputBuffer();
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;
    
    printf("Enter Age: ");
    scanf("%d", &age);
    
    printf("Enter Phone: ");
    scanf("%49s", phone);
    
    printf("Enter Email: ");
    scanf("%49s", email);
    
    printf("Enter Address: ");
    clearInputBuffer();
    fgets(address, sizeof(address), stdin);
    address[strcspn(address, "\n")] = 0;
    
    printf("Enter Aadhar Number: ");
    scanf("%49s", aadhar);
    
    printf("Enter Initial Deposit: ");
    scanf("%f", &initialDeposit);
    
    int accountNumber = bank->nextAccountNumber++;
    initializeCustomer(&bank->customers[bank->customerCount], name, age, phone, 
                      email, address, aadhar, initialDeposit, accountNumber);
    bank->customerCount++;
    saveCustomerData(bank);
    
    printf("\nCustomer added successfully!\n");
    printf("Account Number: %d\n", accountNumber);
    waitForKey();
}

void viewCustomerDetails(BankSystem* bank) {
    clearScreen();
    printf("======== VIEW CUSTOMER DETAILS ========\n");
    printf("1. Search by Account Number\n");
    printf("2. Search by Name\n");
    printf("Enter choice: ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        clearInputBuffer();
        printf("Invalid input!\n");
        waitForKey();
        return;
    }
    
    if (choice == 1) {
        int accountNumber;
        printf("Enter Account Number: ");
        scanf("%d", &accountNumber);
        
        Customer* customer = findCustomerByAccountNumber(bank, accountNumber);
        if (customer) {
            clearScreen();
            showCustomerDetails(customer);
        } else {
            printf("Customer not found!\n");
        }
    } else if (choice == 2) {
        char name[MAX_STRING_LENGTH] = {0};
        printf("Enter Name (or part of name): ");
        clearInputBuffer();
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;
        
        clearScreen();
        int found = 0;
        for (int i = 0; i < bank->customerCount; i++) {
            if (strstr(bank->customers[i].name, name) != NULL) {
                showCustomerDetails(&bank->customers[i]);
                printf("-------------------------\n");
                found = 1;
            }
        }
        
        if (!found) {
            printf("No customers found with that name.\n");
        }
    } else {
        printf("Invalid choice!\n");
    }
    
    waitForKey();
}

void updateCustomerInfo(BankSystem* bank) {
    clearScreen();
    printf("======== UPDATE CUSTOMER INFO ========\n");
    int accountNumber;
    printf("Enter Account Number: ");
    scanf("%d", &accountNumber);
    
    Customer* customer = findCustomerByAccountNumber(bank, accountNumber);
    if (!customer) {
        printf("Customer not found!\n");
        waitForKey();
        return;
    }
    
    clearScreen();
    printf("Current Details:\n");
    showCustomerDetails(customer);
    printf("\n");
    
    printf("Select field to update:\n");
    printf("1. Name\n");
    printf("2. Age\n");
    printf("3. Phone\n");
    printf("4. Email\n");
    printf("5. Address\n");
    printf("6. Aadhar\n");
    printf("7. Cancel\n");
    printf("Enter choice: ");
    
    int choice;
    if (scanf("%d", &choice) != 1) {
        clearInputBuffer();
        printf("Invalid input!\n");
        waitForKey();
        return;
    }
    clearInputBuffer();
    
    char newValue[MAX_STRING_LENGTH] = {0};
    int newAge = 0;
    
    switch (choice) {
        case 1:
            printf("Enter new Name: ");
            fgets(newValue, sizeof(newValue), stdin);
            newValue[strcspn(newValue, "\n")] = 0;
            strncpy(customer->name, newValue, MAX_STRING_LENGTH-1);
            break;
        case 2:
            printf("Enter new Age: ");
            scanf("%d", &newAge);
            customer->age = newAge;
            break;
        case 3:
            printf("Enter new Phone: ");
            scanf("%49s", newValue);
            strncpy(customer->phone, newValue, MAX_STRING_LENGTH-1);
            break;
        case 4:
            printf("Enter new Email: ");
            scanf("%49s", newValue);
            strncpy(customer->email, newValue, MAX_STRING_LENGTH-1);
            break;
        case 5:
            printf("Enter new Address: ");
            clearInputBuffer();
            fgets(newValue, sizeof(newValue), stdin);
            newValue[strcspn(newValue, "\n")] = 0;
            strncpy(customer->address, newValue, MAX_STRING_LENGTH-1);
            break;
        case 6:
            printf("Enter new Aadhar: ");
            scanf("%49s", newValue);
            strncpy(customer->aadhar, newValue, MAX_STRING_LENGTH-1);
            break;
        case 7:
            return;
        default:
            printf("Invalid choice!\n");
            waitForKey();
            return;
    }
    
    saveCustomerData(bank);
    createTransaction("Info Update", 0, customer->accountNumber, NULL, 
                     &customer->transactions[customer->transactionCount++]);
    printf("\nCustomer information updated successfully!\n");
    waitForKey();
}

void deleteCustomerAccount(BankSystem* bank) {
    clearScreen();
    printf("======== DELETE CUSTOMER ACCOUNT ========\n");
    int accountNumber;
    printf("Enter Account Number: ");
    scanf("%d", &accountNumber);
    
    for (int i = 0; i < bank->customerCount; i++) {
        if (bank->customers[i].accountNumber == accountNumber) {
            clearScreen();
            printf("Customer Details:\n");
            showCustomerDetails(&bank->customers[i]);
            
            printf("\nAre you sure you want to delete this account? (y/n): ");
            char confirm;
            scanf(" %c", &confirm);
            
            if (tolower(confirm) == 'y') {
                createTransaction("Account Deleted", 0, bank->customers[i].accountNumber, NULL, 
                                &bank->customers[i].transactions[bank->customers[i].transactionCount++]);
                
                for (int j = i; j < bank->customerCount - 1; j++) {
                    bank->customers[j] = bank->customers[j + 1];
                }
                bank->customerCount--;
                saveCustomerData(bank);
                printf("\nAccount deleted successfully!\n");
            } else {
                printf("\nDeletion cancelled.\n");
            }
            
            waitForKey();
            return;
        }
    }
    
    printf("\nCustomer not found!\n");
    waitForKey();
}

void depositAmount(BankSystem* bank) {
    clearScreen();
    printf("======== DEPOSIT AMOUNT ========\n");
    int accountNumber;
    float amount;
    
    printf("Enter Account Number: ");
    scanf("%d", &accountNumber);
    
    Customer* customer = findCustomerByAccountNumber(bank, accountNumber);
    if (!customer) {
        printf("Customer not found!\n");
        waitForKey();
        return;
    }
    
    printf("Current Balance: $%.2f\n", customer->balance);
    printf("Enter Amount to Deposit: ");
    scanf("%f", &amount);
    
    if (amount <= 0) {
        printf("Deposit amount must be positive\n");
    } else {
        deposit(customer, amount);
        saveCustomerData(bank);
        printf("\nDeposit successful! New Balance: $%.2f\n", customer->balance);
    }
    
    waitForKey();
}

void withdrawAmount(BankSystem* bank) {
    clearScreen();
    printf("======== WITHDRAW AMOUNT ========\n");
    int accountNumber;
    float amount;
    
    printf("Enter Account Number: ");
    scanf("%d", &accountNumber);
    
    Customer* customer = findCustomerByAccountNumber(bank, accountNumber);
    if (!customer) {
        printf("Customer not found!\n");
        waitForKey();
        return;
    }
    
    printf("Current Balance: $%.2f\n", customer->balance);
    printf("Enter Amount to Withdraw: ");
    scanf("%f", &amount);
    
    if (amount <= 0) {
        printf("Withdrawal amount must be positive\n");
    } else if (amount > customer->balance) {
        printf("Insufficient balance\n");
    } else {
        withdraw(customer, amount);
        saveCustomerData(bank);
        printf("\nWithdrawal successful! New Balance: $%.2f\n", customer->balance);
    }
    
    waitForKey();
}

void viewTransactionStatement(BankSystem* bank) {
    clearScreen();
    printf("======== VIEW TRANSACTION STATEMENT ========\n");
    int accountNumber;
    printf("Enter Account Number: ");
    scanf("%d", &accountNumber);
    
    Customer* customer = findCustomerByAccountNumber(bank, accountNumber);
    if (!customer) {
        printf("Customer not found!\n");
        waitForKey();
        return;
    }
    
    clearScreen();
    printTransactionStatement(customer);
    waitForKey();
}

void viewAllCustomersList(BankSystem* bank) {
    clearScreen();
    printf("======== ALL CUSTOMERS ========\n");
    
    if (bank->customerCount == 0) {
        printf("No customers found.\n");
    } else {
        printf("%-15s%-25s%-10s%-15s%-15s\n", "Account No.", "Name", "Age", "Phone", "Balance");
        printf("-----------------------------------------------------------------\n");
        
        for (int i = 0; i < bank->customerCount; i++) {
            const Customer* c = &bank->customers[i];
            printf("%-15d%-25s%-10d%-15s%-15.2f\n", 
                  c->accountNumber, c->name, c->age, c->phone, c->balance);
        }
    }
    
    waitForKey();
}

void viewAccountsByBalanceThreshold(BankSystem* bank) {
    clearScreen();
    printf("======== ACCOUNTS BY BALANCE ========\n");
    printf("1. View accounts with balance above threshold\n");
    printf("2. View accounts with balance below threshold\n");
    printf("Enter choice: ");
    
    int choice;
    scanf("%d", &choice);
    
    if (choice != 1 && choice != 2) {
        printf("Invalid choice!\n");
        waitForKey();
        return;
    }
    
    float threshold;
    printf("Enter threshold amount: ");
    scanf("%f", &threshold);
    
    clearScreen();
    printf("Accounts %s %.2f:\n", (choice == 1 ? "above" : "below"), threshold);
    printf("%-15s%-25s%-15s\n", "Account No.", "Name", "Balance");
    printf("-----------------------------------------\n");
    
    int found = 0;
    for (int i = 0; i < bank->customerCount; i++) {
        const Customer* c = &bank->customers[i];
        if ((choice == 1 && c->balance >= threshold) || 
            (choice == 2 && c->balance <= threshold)) {
            found = 1;
            printf("%-15d%-25s%-15.2f\n", c->accountNumber, c->name, c->balance);
        }
    }
    
    if (!found) {
        printf("No accounts found matching the criteria.\n");
    }
    
    waitForKey();
}

void customerAccountMenu(BankSystem* bank) {
    while (1) {
        clearScreen();
        printf("======== CUSTOMER ACCOUNT MANAGEMENT ========\n");
        printf("1. Add New Customer\n");
        printf("2. View Customer Details\n");
        printf("3. Update Customer Info\n");
        printf("4. Delete Customer Account\n");
        printf("5. Back to Main Menu\n");
        printf("Enter your choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input!\n");
            waitForKey();
            continue;
        }
        
        switch (choice) {
            case 1: addCustomer(bank); break;
            case 2: viewCustomerDetails(bank); break;
            case 3: updateCustomerInfo(bank); break;
            case 4: deleteCustomerAccount(bank); break;
            case 5: return;
            default:
                printf("Invalid choice! Please try again.\n");
                waitForKey();
        }
    }
}

void accountOperationsMenu(BankSystem* bank) {
    while (1) {
        clearScreen();
        printf("======== BANK ACCOUNT OPERATIONS ========\n");
        printf("1. Deposit Amount\n");
        printf("2. Withdraw Amount\n");
        printf("3. View Transaction Statement\n");
        printf("4. Back to Main Menu\n");
        printf("Enter your choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input!\n");
            waitForKey();
            continue;
        }
        
        switch (choice) {
            case 1: depositAmount(bank); break;
            case 2: withdrawAmount(bank); break;
            case 3: viewTransactionStatement(bank); break;
            case 4: return;
            default:
                printf("Invalid choice! Please try again.\n");
                waitForKey();
        }
    }
}

void reportsMenu(BankSystem* bank) {
    while (1) {
        clearScreen();
        printf("======== REPORTS & LISTS ========\n");
        printf("1. View All Customers\n");
        printf("2. View Accounts by Balance\n");
        printf("3. Back to Main Menu\n");
        printf("Enter your choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input!\n");
            waitForKey();
            continue;
        }
        
        switch (choice) {
            case 1: viewAllCustomersList(bank); break;
            case 2: viewAccountsByBalanceThreshold(bank); break;
            case 3: return;
            default:
                printf("Invalid choice! Please try again.\n");
                waitForKey();
        }
    }
}

void mainMenu(BankSystem* bank) {
    while (1) {
        clearScreen();
        printf("=============== BANK MANAGEMENT SYSTEM ===============\n");
        printf("1. Customer Account Management\n");
        printf("2. Bank Account Operations\n");
        printf("3. Reports & Lists\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid input!\n");
            waitForKey();
            continue;
        }
        
        switch (choice) {
            case 1: customerAccountMenu(bank); break;
            case 2: accountOperationsMenu(bank); break;
            case 3: reportsMenu(bank); break;
            case 4: return;
            default:
                printf("Invalid choice! Please try again.\n");
                waitForKey();
        }
    }
}

void initializeBankSystem(BankSystem* bank) {
    FILE* f = fopen(ACCOUNT_FILE, "r");
    if (f != NULL) {
        fscanf(f, "%d", &bank->nextAccountNumber);
        fclose(f);
    } else {
        bank->nextAccountNumber = 326012200;
    }    
    bank->customerCount = 0;
    bank->officerCount = 0;
    
    loadOfficerData(bank);
    loadCustomerData(bank);
    
    for (int i = 0; i < bank->customerCount; i++) {
        if (bank->customers[i].accountNumber >= bank->nextAccountNumber) {
            bank->nextAccountNumber = bank->customers[i].accountNumber + 1;
        }
    }
}

void runBankSystem(BankSystem* bank) {
    while (1) {
        if (officerLogin(bank)) {
            mainMenu(bank);
        }
    }
}