#ifndef BANK_SYSTEM_H
#define BANK_SYSTEM_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <limits>
#include <conio.h>
#include "customer.h"
#include "bankofficer.h"
using namespace std;

class BankSystem {
        vector<Customer> customers;
        vector<BankOfficer> officers;
        int nextAccountNumber;
        const string CUSTOMER_FILE = "customers.dat";
        const string OFFICER_FILE = "officers.dat";
        const string ACCOUNT_FILE = "next_account_number.dat";
    
        string passwordInput() ;
    
        void loadData() ;
        bool officerLogin() ;
    
        void addCustomer() ;
    
        Customer* findCustomerByAccountNumber(int) ;
    
        void viewCustomerDetails() ;
    
        void updateCustomerInfo() ;
    
        void deleteCustomerAccount() ;
    
        void depositAmount() ;
    
        void withdrawAmount() ;
    
        void viewTransactionStatement() ;
    
        void viewAllCustomersList() ;
    
        void viewAccountsByBalanceThreshold() ;
    
        void customerAccountMenu() ;
    
        void accountOperationsMenu() ;
    
        void reportsMenu() ;

        void saveCustomerData();

        void saveOfficerData();

        void saveNextAccountNumber() ;
        
        void loadNextAccountNumber() ;

        void mainMenu() ;

        string validphonenumberchecker(string) ;

        string validemailchecker(string);

        string validaadharchecker(string);

        void transferFunds();
    
    public:
        BankSystem() ;
    
        void run() ;
};
#endif 