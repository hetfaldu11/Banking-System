#ifndef BANK_OFFICER_H
#define BANK_OFFICER_H

#include <iostream>
#include <string>

using namespace std;

class BankOfficer {
        string id;
        string password;
    
    public:
        BankOfficer(string , string ) ;
    
        string getId() const;
        string getPassword() const ;
    
        bool authenticate(string ) const ;
};

#endif 