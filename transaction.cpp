#include "transaction.h"

Transaction::Transaction(string t, float a, int accNum, string desc = "") 
    : type(t), amount(a), accountNumber(accNum), description(desc) {
    time_t now = time(0);
    timestamp = ctime(&now);
    timestamp.pop_back();
}
    
string Transaction::getType() const { return type; }
float Transaction::getAmount() const { return amount; }
string Transaction::getTimestamp() const { return timestamp; }
int Transaction::getAccountNumber() const { return accountNumber; }
string Transaction::getDescription() const { return description; }
    
string Transaction::toString() const {
    ostringstream oss;
    oss << timestamp << " - " << type << " - $" << fixed << setprecision(2) << amount;
    if (!description.empty()) {
        oss << " - " << description;
    }
    return oss.str();
}