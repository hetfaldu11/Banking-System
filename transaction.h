#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <fstream>
#include <string>
#include <ctime>
#include <iomanip>
#include <sstream>
using namespace std;

class Transaction {
        string type;
        float amount;
        string timestamp;
        int accountNumber;
        string description;
    
    public:
        Transaction(string , float , int , string) ;
    
        string getType() const ;
        float getAmount() const;
        string getTimestamp() const;
        int getAccountNumber() const;
        string getDescription() const;

        string toString() const ;
};

#endif 