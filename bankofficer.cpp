#include "bankofficer.h"

BankOfficer::BankOfficer(string id, string password) : id(id), password(password) {}
    
string BankOfficer::getId() const { return id; }
string BankOfficer::getPassword() const { return password; }
    
bool BankOfficer::authenticate(string inputPassword) const {
    return password == inputPassword;
}