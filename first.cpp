#include<iostream>
#include<string>
#include<conio.h>
#include<windows.h>
using namespace std;

int const startingAccountNumber=326012200;
static int  number=startingAccountNumber;

class Customer{
        string name;
        string username;
        float balance;
        int personal_account_no;
        string phone_no;
        string email;
        string address;
        string password;
        static int account_number;

    public:

        Customer(string name,string username,string email,string phone_no,string address,float amount,string password){
            this->name=name;
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

        string get_name(){
            return name;
        }

        void deposit(float amount){
            this->balance += amount;
        }

        float withdraw(float amount) {
            while(true){
                if (amount <= balance&&balance-amount<5000) {
                    balance -= amount;
                    break;
                } else {
                    cout <<endl<< "Insufficient balance!" << endl<<endl;
                    cout<<"Enter valid amount you want to withdraw : ";
                    cin>>amount;
                }
            }
            return amount;
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

        void update_name(string name){
            this->name=name;
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
    cout<<"2 : Login an account"<<endl;
    cout<<"3 : Exit"<<endl<<endl;

    cout<<"Choose an option : ";
    while(true){
        int choice;
        cin>>choice;
        system("CLS");
        if(choice == 1)
            return false;
        else if(choice == 2)
            return true;
        else if(choice == 3)
            exit(0);
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
    
    while(true){
        cout<<"Enter your account number : ";
        cin>>account_number;

        if(!(account_number>=startingAccountNumber&&account_number<=startingAccountNumber+10))
        {
            cout<<endl<<"Accountnumber is not exist !"<<endl;
            continue;
        }

        cout<<"Enter password : ";
        cin.ignore();
        password=passwordtexter();

        if(cust[account_number - startingAccountNumber].check_password(password)&&!password.empty()){
            system("CLS");
            break;
        }
        else{
            cout<<endl<<"Accountnumber or Password incorrect !"<<endl;
        }
    }

    cout<<"Logged in successfully !"<<endl;
    cout<<"press any key to continue...";
    getch();
    system("CLS");
    return cust[account_number-startingAccountNumber];
}

float validdepositchecker(float amount){
    while(true){
        if(amount<5000)
        {
            cout<<endl<<"Amount is must greter than or equal 5000"<<endl<<endl;
            cout<<"Enter amount you want to deposit : ";
            cin>>amount;    
        }
        else break;
    }
    return amount;
}

string validphonenumberchecker(string phoneNumber) {
    while (true) {
        bool isValid = true;

        // Check length
        if (phoneNumber.size() != 10) {
            isValid = false;
        } else {
            // Check each character is a digit
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

string validemailchecker(string email){
    while(true)
    {
        int dot=0,atherate=0;
        for(int i=0;i<email.size();i++)
        {
            if(email[i]=='.')   dot++;
            if(email[i]=='@')   atherate++; 
        }
        if(atherate==1&&dot>=1) break;
        else{
            cout<<endl<<"You entered an invalid email addreess..."<<endl<<endl;
            cout<<"Enter an valid email address : ";
            cin>>email;
        }
    }
    return email;
}

void create_account(){
    cin.ignore();
    cout<<"Create an account"<<endl<<endl;
    string name,username,email,phone_no,address,password,confirm_password;
    float balance;
    cout<<"Enter your name : ";
    getline(cin,name);
    cout<<"Enter your Username : ";
    getline(cin,username);
    cout<<"Enter email address : ";
    cin>>email;
    email=validemailchecker(email);
    cout<<"Enter phone no : ";
    cin>>phone_no;
    phone_no=validphonenumberchecker(phone_no);
    cin.ignore();
    cout<<"Enter your address : ";
    getline(cin,address);
    cout<<"Enter amount you want to deposit : ";
    cin>>balance;
    balance=validdepositchecker(balance);
    cin.ignore();
    while(true){
        cout<<"Enter password : ";
        password=passwordtexter();
        cout<<endl<<"Confirm password : ";
        confirm_password=passwordtexter();
        if(password!=confirm_password){
            system("CLS");
            cout<<endl<<"Password does not match ! Try again... "<<endl; 
            continue;
        }
        else break;
    }

    cust[number-startingAccountNumber]=Customer(name,username,email,phone_no,address,balance,password);
    cust[number-startingAccountNumber].show_accountnumber();
    cout<<endl<<"Account created successfully !"<<endl;
    cout<<"press any key to continue...";
    getch();
    system("CLS");
    number++;
}

bool checkingyesorno(string yesorno){
    while(true)
    {
        if(tolower(yesorno[0])=='y'&&tolower(yesorno[1]=='e')&&tolower(yesorno[2]=='s')&&yesorno.size()==3)
            return true;
        else if(tolower(yesorno[0]=='n'&&tolower(yesorno[1])=='o'&&yesorno.size()==2))
            return false;
        else{
            cout<<"Please type an valid input : ";
            cin>>yesorno;
        }
    }
}

int Customer :: account_number = startingAccountNumber;

int main(){
    while(true){
        if(checkforlogin()!=true)
            create_account();    

        Customer &client=login();
        client.show_accountnumber();

        bool flag=true;

        while(flag){
            cout<<"Welcome "<<client.get_name()<<endl;
            cout<<"1 : Deposit"<<endl;
            cout<<"2 : Withdraw"<<endl;
            cout<<"3 : Update account information"<<endl;
            cout<<"4 : Show balance"<<endl;
            cout<<"5 : View account details"<<endl;
            cout<<"6 : Exit"<<endl;
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
                    amount=client.withdraw(amount);
                    system("CLS");
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
                    cout<<"Name : "<<client.get_name()<<endl;
                    cout<<"Username : "<<client.get_username()<<endl;
                    cout<<"Email : "<<client.get_email()<<endl;
                    cout<<"Phone : "<<client.get_phoneno()<<endl;
                    cout<<"Address : "<<client.get_address()<<endl;
                    cout<<"Press any key to continue...";
                    getch();
                    system("CLS");
                    break;
                
                case 6:
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
                    cout<<"1 : Update name"<<endl;
                    cout<<"2 : Update username"<<endl;
                    cout<<"3 : Update email"<<endl;
                    cout<<"4 : Update phone no"<<endl;
                    cout<<"5 : Update address"<<endl;
                    cout<<"6 : Update password"<<endl;
                    cout<<endl<<"Choose an option : ";
                    int choice;
                    cin>>choice;
                    string new_data;
                    string old_password;
                    string yesorno;
                    switch(choice){
                        case 1:
                            system("CLS");
                            cout<<"Enter new name : ";
                            cin.ignore();
                            getline(cin,new_data);

                            cout<<endl<<"Are you sure you want to change name , type Yes otherwise No: ";
                            cin>>yesorno;

                            if(!checkingyesorno(yesorno))   break;

                            while(true){
                                cout<<"Enter password : ";
                                old_password=passwordtexter();
                                if(client.check_password(old_password)){
                                    system("CLS");
                                    break;
                                }
                                else {
                                    cout<<endl<<"You enter incorrect password !"<<endl<<endl;
                                }
                            }

                            client.update_name(new_data);
                            cout<<"Name updated successfully !"<<endl;
                            cout<<"Press any key to continue...";
                            getch();
                            system("CLS");
                            break;
                            
                        case 2:
                            system("CLS");
                            cout<<"Enter new username : ";
                            cin.ignore();
                            getline(cin,new_data);

                            cout<<endl<<"Are you sure you want to change username , type Yes otherwise No: ";
                            cin>>yesorno;

                            if(!checkingyesorno(yesorno))   break;

                            while(true){
                                cout<<"Enter password : ";
                                old_password=passwordtexter();
                                if(client.check_password(old_password)){
                                    system("CLS");
                                    break;
                                }
                                else {
                                    cout<<endl<<"You enter incorrect password !"<<endl<<endl;
                                }
                            }

                            client.update_username(new_data);
                            cout<<"Username updated successfully !"<<endl;
                            cout<<"Press any key to continue...";
                            getch();
                            system("CLS");
                            break;

                        case 3:
                            system("CLS");
                            cout<<"Enter new email : ";
                            cin>>new_data;

                            new_data=validemailchecker(new_data);

                            cout<<endl<<"Are you sure you want to change email , type Yes otherwise No: ";
                            cin>>yesorno;

                            if(!checkingyesorno(yesorno))   break;

                            while(true){
                                cout<<"Enter password : ";
                                old_password=passwordtexter();
                                if(client.check_password(old_password)){
                                    system("CLS");
                                    break;
                                }
                                else {
                                    cout<<endl<<"You enter incorrect password !"<<endl<<endl;
                                }
                            }

                            client.update_email(new_data);
                            cout<<"Email updated successfully !"<<endl;
                            cout<<"Press any key to continue...";
                            getch();
                            system("CLS");
                            break;

                        case 4:
                            system("CLS");
                            cout<<"Enter new phone no : ";
                            cin>>new_data;

                            new_data=validphonenumberchecker(new_data);

                            cout<<endl<<"Are you sure you want to change phone number , type Yes otherwise No: ";
                            cin>>yesorno;

                            if(!checkingyesorno(yesorno))   break;

                            while(true){
                                cout<<"Enter password : ";
                                old_password=passwordtexter();
                                if(client.check_password(old_password)){
                                    system("CLS");
                                    break;
                                }
                                else {
                                    cout<<endl<<"You enter incorrect password !"<<endl<<endl;
                                }
                            }

                            client.update_phoneno(new_data);
                            cout<<"Phone no updated successfully !"<<endl;
                            cout<<"Press any key to continue...";
                            getch();
                            system("CLS");
                            break;

                        case 5:
                            system("CLS");
                            cout<<"Enter new address : ";
                            cin.ignore();

                            cout<<endl<<"Are you sure you want to change  address, type Yes otherwise No: ";
                            cin>>yesorno;

                            if(!checkingyesorno(yesorno))   break;

                            while(true){
                                cout<<"Enter old password : ";
                                old_password=passwordtexter();
                                if(client.check_password(old_password)){
                                    system("CLS");
                                    break;
                                }
                                else {
                                    cout<<endl<<"You enter incorrect password !"<<endl<<endl;
                                }
                            }

                            getline(cin,new_data);
                            client.update_address(new_data);
                            cout<<"Address updated successfully !"<<endl;
                            cout<<"Press any key to continue...";
                            getch();
                            system("CLS");
                            break;

                        case 6:
                            system("CLS");
                            cout<<"Enter new password : ";
                            new_data=passwordtexter();

                            cout<<endl<<"Are you sure you want to change password , type Yes otherwise No: ";
                            cin>>yesorno;

                            if(!checkingyesorno(yesorno))   break;

                            while(true){
                                cout<<"Enter old password : ";
                                old_password=passwordtexter();
                                if(client.check_password(old_password)){
                                    system("CLS");
                                    break;
                                }
                                else {
                                    cout<<endl<<"You enter incorrect password !"<<endl<<endl;
                                }
                            }

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