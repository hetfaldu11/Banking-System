#include<iostream>
#include<string>
#include<conio.h>
#include<windows.h>
using namespace std;

static int  number=32600;
int const startingAccountNumber=32600;

class Customer{
        string username;
        float balance;
        int personal_account_no;
        string phone_no;
        string email;
        string address;
        string password;
        static int account_number;

    public:

        Customer(string username,string email,string phone_no,string address,float amount,string password){
            this->username=username;
            this->email=email;
            this->phone_no=phone_no;
            this->address=address;
            this->balance=amount;
            this->password=password;
            personal_account_no=account_number;
            account_number++;
        }

        Customer(){}
        
        string get_username(){
            return username;
        }

        string get_phoneno(){
            return phone_no;
        }

        string get_email(){
            return email;
        }

        string get_address(){
            return address;
        }

        float get_balance(){
            return balance;
        }

        void deposit(float amount){
            this->balance += amount;
        }

        void withdraw(float amount) {
            if (amount <= balance) {
                balance -= amount;
            } else {
                cout <<endl<< "Insufficient balance!" << endl;
            }
        }

        void update_username(string username){
            this->username=username;
        }

        void update_phoneno(string phone_no){
            this->phone_no=phone_no;
        }

        void update_email(string email){
            this->email=email;
        }

        void update_address(string address){
            this->address=address;
        }
        
        void update_password(string password){
            this->password=password;
        }

        void show_balance(){
            cout<<"Name : "<<username<<endl;
            cout<<"Balance : "<<balance<<endl;
        }

        void show_accountnumber(){
            cout<<endl<<"Account number : "<<personal_account_no<<endl;
        }

        static int get_accountnumber(){
            return account_number;
        }

        bool check_password(string pass) {
            return password == pass;
        }
}cust[10];

bool checkforlogin()
{
    system("CLS");
    cout<<"Welcome to the banking system"<<endl<<endl;
    cout<<"1 : Create an account"<<endl;
    cout<<"2 : Login an account"<<endl<<endl;

    cout<<"Choose an option : ";
    while(true){
        int choice;
        cin>>choice;
        system("CLS");
        if(choice == 1)
            return false;
        else if(choice == 2)
            return true;
        else
            cout<<"Please choose appropriate option : ";
    }
    system("CLS");
}

string passwordtexter(){
    string password;
    while(1){
        char s;
        s=getch();
        if (s == '\b') {
            cout << "\b \b";
            password.pop_back();
        }
        if(s=='\r') break;
        else    password+=s;
        cout<<"*";
    }
    return password;
}


Customer &login(){

    system("CLS");
    cout<<"Login Section : " << endl<<endl; 

    int account_number;
    string password;
    cout<<"Enter your account number : ";
    cin.ignore();
    cin>>account_number;
    cout<<"Enter password : ";
    
    while(true){
        password=passwordtexter();
        if(cust[account_number - startingAccountNumber].check_password(password)){
            
            system("CLS");
            break;
        }
        else{
            cout<<endl<<"Accountnumber or Password incorrect !"<<endl;
            cout<<"Enter valid password : ";
        }
    }

    cout<<"Logged in successfully !"<<endl;
    cout<<"press any key to continue...";
    getch();
    system("CLS");
    return cust[account_number-startingAccountNumber];
}

void create_account(){

    cout<<" Create an account"<<endl<<endl;

    string username,email,phone_no,address,password,confirm_password;
    float balance;
    cout<<"Enter your name : ";
    cin.ignore();
    getline(cin,username);
    cout<<"Enter email address : ";
    cin>>email;
    cout<<"Enter phone no : ";
    cin>>phone_no;
    cin.ignore();
    cout<<"Enter your address : ";
    getline(cin,address);
    cout<<"Enter the amount you want to deposit : ";
    cin>>balance;
    cout<<"Enter password : ";
    password=passwordtexter();

    while(true){
        cout<<endl<<"Confirm password : ";
        confirm_password=passwordtexter();
        if(password!=confirm_password){
            system("CLS");
            cout<<endl<<"Password does not match !"<<endl;
            cout<<"Enter password again : ";   
            continue;
        }
        else break;

    }

    cust[number-startingAccountNumber]=Customer(username,email,phone_no,address,balance,password);
    cust[number-startingAccountNumber].show_accountnumber();
    cout<<endl<<"Account created successfully !"<<endl;
    cout<<"press any key to continue...";
    getch();
    system("CLS");
    number++;
}

int Customer :: account_number = 32600;

int main(){
    while(true){
        if(checkforlogin()!=true)
            create_account();    

        Customer &client=login();
        client.show_accountnumber();

        bool flag=true;

        while(flag){
            cout<<"Welcome "<<client.get_username()<<endl;
            cout<<"1 : Deposit"<<endl;
            cout<<"2 : Withdraw"<<endl;
            cout<<"3 : Update account information"<<endl;
            cout<<"4 : Show balance"<<endl;
            cout<<"5 : Exit"<<endl;
            cout<<endl<<"Choose an option : ";
            int choice;
            cin>>choice;

            switch(choice) {
                case 1:
                    system("CLS");
                    float amount;
                    cout<<"Enter the amount you want to deposit : ";
                    cin>>amount;
                    client.deposit(amount);
                    cout<<"Amount deposited successfully !"<<endl;
                    cout<<"Press any key to continue...";
                    getch();
                    system("CLS");
                    break;
                

                case 2:
                    system("CLS");
                    cout<<"Enter the amount you want to withdraw : ";
                    cin>>amount;
                    if(amount>client.get_balance()){
                        cout<<"Insufficient balance !"<<endl;
                        cout<<"Press any key to continue...";
                        getch();
                        system("CLS");
                    }
                    else{
                        client.withdraw(amount);
                        cout<<"Amount withdrawn successfully !"<<endl;
                        cout<<"Press any key to continue...";
                        getch();
                        system("CLS");
                    }
                    break;


                case 4:
                    system("CLS");
                    client.show_balance();
                    cout<<"Press any key to continue...";
                    getch();
                    system("CLS");
                    break;

                
                case 5:
                    system("CLS");
                    cout<<"Logging out..."<<endl;
                    cout<<"Press any key to continue...";
                    getch();
                    system("CLS");
                    flag=false;
                    break;

                case 3:
                    system("CLS");
                    cout<<"Update account information : "<<endl<<endl;
                    cout<<"1 : Update username"<<endl;
                    cout<<"2 : Update email"<<endl;
                    cout<<"3 : Update phone no"<<endl;
                    cout<<"4 : Update address"<<endl;
                    cout<<"5 : Update password"<<endl;
                    cout<<endl<<"Choose an option : ";
                    int choice;
                    cin>>choice;
                    string new_data;
                    switch(choice){
                        case 1:
                            system("CLS");
                            cout<<"Enter new username : ";
                            cin.ignore();
                            getline(cin,new_data);
                            client.update_username(new_data);
                            cout<<"Username updated successfully !"<<endl;
                            cout<<"Press any key to continue...";
                            getch();
                            system("CLS");
                            break;

                        case 2:
                            system("CLS");
                            cout<<"Enter new email : ";
                            cin>>new_data;
                            client.update_email(new_data);
                            cout<<"Email updated successfully !"<<endl;
                            cout<<"Press any key to continue...";
                            getch();
                            system("CLS");
                            break;

                        case 3:
                            system("CLS");
                            cout<<"Enter new phone no : ";
                            cin>>new_data;
                            client.update_phoneno(new_data);
                            cout<<"Phone no updated successfully !"<<endl;
                            cout<<"Press any key to continue...";
                            getch();
                            system("CLS");
                            break;

                        case 4:
                            system("CLS");
                            cout<<"Enter new address : ";
                            cin.ignore();
                            getline(cin,new_data);
                            client.update_address(new_data);
                            cout<<"Address updated successfully !"<<endl;
                            cout<<"Press any key to continue...";
                            getch();
                            system("CLS");
                            break;

                        case 5:
                            system("CLS");
                            cout<<"Enter new password : ";
                            new_data=passwordtexter();
                            client.update_password(new_data);
                            cout<<"Password updated successfully !"<<endl;
                            cout<<"Press any key to continue...";
                            getch();
                            system("CLS");
                            break;

                        default:
                            system("CLS");
                            cout<<"Invalid option !"<<endl;
                            cout<<"Press any key to continue...";
                            getch();
                            system("CLS");
                            break;
                    }
                    break;
            }
        
        }
    }
    return 0;
}