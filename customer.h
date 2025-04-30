#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include "transaction.h"
using namespace std;
class Customer {
        string name;
        int age;
        float balance;
        int accountNumber;
        string phone;
        string email;
        string address;
        string aadhar;
        vector<Transaction> transactions;
    
    public:
        Customer(string , int , string , string , 
                 string , string , float , int ) ;
    
        Customer();
    
        string getPhone() const;
        string getEmail() const;
        string getAddress() const;
        string getAadhar() const ;
        float getBalance() const;
        string getName() const;
        int getAge() const;
        int getAccountNumber() const;
    
        void deposit(float ) ;
    
        float withdraw(float ) ;
    
        void updatePhone(string ) ;
        void updateEmail(string ) ;
        void updateAddress(string ) ;
        void updateName(string ) ;
        void updateAge(int ) ;
        void updateAadhar(string ) ;
    
        void showDetails() const ;
    
        void recordTransaction(string , float , string description = "") ;
    
        void printTransactionStatement() const ;

        bool transfer(Customer& recipient, float amount);
    
    private:
        void saveTransactionToFile(const Transaction& ) const ;
};

#endif