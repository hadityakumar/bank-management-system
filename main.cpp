// Created by: H Aditya Kumar

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cctype>

using namespace std;

class Bank_Account{
    private:
        char name[50];
        int accountNumber;
        double balance;
        char type;
    public:
        void create_account();
        void print_account();
        void displayAllAccounts();
        void setBalance(double bal){
            balance = bal;
            return;
        }
        char* getName(){
            return name;
        }
        int getAccountNumber(){
            return accountNumber;
        }
        double getBalance(){
            return balance;
        }
        char getType(){
            return type;
        }
        void deposit(double amount){
            balance += amount;
        }
        void withdraw(double);
        
};

char inputType(){

    char type;
    bool flag = false;
    while(flag == false){
        cout << "Enter your type of account (C/S): ";
        cin >> type;
        type = static_cast<char>(toupper(type));
        if(type != 'C' && type != 'S'){
            cout << "Invalid type of account!!" << endl;
        }
        else{
            flag = true;
        }
    }
    return type;
}

bool checkAccount(int accnum){
    Bank_Account acc;
    ifstream inFile;
    inFile.open("account.txt", ios::binary);
    if(!inFile){
        cout<<"No Accounts created!"<<endl;
        return false;
    }
    while(inFile.read(reinterpret_cast<char *> (&acc), sizeof(Bank_Account))){
        if(acc.getAccountNumber() == accnum){
            return true;
        }
    }
    return false;
}

int inputAccountNumber(){
    int accnum;
    bool flag = false;
    while(flag == false){
        cout << "Enter your account number: ";
        cin >> accnum;
        if(checkAccount(accnum)){
            cout << "Account number already exists!!" << endl;
        }
        else if(accnum <= 0){
            cout << "Negative numbers or Zero numbers not accepted!!" << endl;
        }
        else{
            flag = true;
        }
    }
    return accnum;
}

void Bank_Account::create_account(){
    system("clear");
    cout << "Enter your name: ";
    cin.ignore();
    cin.getline(name, 50);
    accountNumber = inputAccountNumber();
    type = inputType();
    cout << "Enter initial balance: ";
    cin >> balance;
}

void Bank_Account::print_account(){
    cout << "Name: " << name<< endl;
    cout << "Account Number: " << accountNumber << endl;
    cout << "Balance: " << balance << endl;
    cout << "Type: " << type << endl;
}

void writeAccount(){
    ofstream outFile;
    outFile.open("account.txt", ios::binary | ios::app);
    Bank_Account account;
    account.create_account();
    outFile.write(reinterpret_cast<char *> (&account), sizeof(Bank_Account));
    outFile.close();
}

void deleteAccount(int accnum){
    Bank_Account account;
    ifstream inFile;
    ofstream outFile;
    inFile.open("account.txt", ios::binary);
    if(!inFile){
        cout << "File was not found!!";
        return;
    }
    outFile.open("temp.txt", ios::binary);
    inFile.seekg(0, ios::beg);
    while(inFile.read(reinterpret_cast<char *> (&account), sizeof(Bank_Account))){
        if(account.getAccountNumber() != accnum){
            outFile.write(reinterpret_cast<char *> (&account), sizeof(Bank_Account));
        }
    }
    inFile.close();
    outFile.close();
    remove("account.txt");
    rename("temp.txt", "account.txt");
    cout << "Account deleted successfully!!" << endl;
}

void Show_Details(int accnum){
    Bank_Account acc;
    bool flag = false;
    ifstream inFile;
    inFile.open("account.txt", ios::binary);
    if(!inFile){
        cout<<"No Accounts created!"<<endl;
        return;
    }
    cout<<"\nBALANCE DETAILS\n";
    while(inFile.read(reinterpret_cast<char *> (&acc), sizeof(Bank_Account))){
        if(acc.getAccountNumber() == accnum){
            flag=true;
            acc.print_account();
            break;
        }
    }
    inFile.close();
    if(flag==false){
        cout<<"Account does not exist"<<endl;
    }
}

void addBalance(int accno, double deposit){
    Bank_Account acc;
    bool flag = false;
    fstream File;
    File.open("account.txt", ios::binary | ios::in | ios::out);
    if(!File){
        cout<<"No Accounts created!"<<endl;
        return;
    }

    while(!File.eof() && !flag){
        File.read(reinterpret_cast<char *> (&acc), sizeof(Bank_Account));
        if(acc.getAccountNumber() == accno){
            acc.deposit(deposit);
            int pos = (-1)*static_cast<int>(sizeof(Bank_Account));
            File.seekp(pos, ios::cur);
            File.write(reinterpret_cast<char *> (&acc), sizeof(Bank_Account));
            cout<<"Amount deposited successfully!!"<<endl;
            flag = true;
        }
    }

    File.close();
    if(!flag)
        cout<<"Account does not exist"<<endl;
}

void withdrawBalance(int accno, double withdraw){
    Bank_Account acc;
    bool flag = false;
    fstream File;
    File.open("account.txt", ios::binary | ios::in | ios::out);
    if(!File){
        cout<<"No Accounts created!"<<endl;
        return;
    }

    while(!File.eof() && !flag){
        File.read(reinterpret_cast<char *> (&acc), sizeof(Bank_Account));
        if(acc.getAccountNumber() == accno){
            if(acc.getBalance() < withdraw){
                cout<<"Insufficient Balance"<<endl;
                return;
            }
            acc.deposit(-1*withdraw);
            int pos = (-1)*static_cast<int>(sizeof(Bank_Account));
            File.seekp(pos, ios::cur);
            File.write(reinterpret_cast<char *> (&acc), sizeof(Bank_Account));
            cout<<"Amount withdrawn successfully!!"<<endl;
            flag = true;
        }
    }

    File.close();
    if(!flag)
        cout<<"Account does not exist"<<endl;
}


void displayAllAccounts(){
    Bank_Account acc;
    ifstream inFile;
    inFile.open("account.txt", ios::binary);
    if(!inFile){
        cout<<"No Accounts created!"<<endl;
        return;
    }
    cout<<"\n\n\t\tACCOUNT HOLDER LIST\n\n";
    cout<<"==========================================================================\n"
        <<"A/c no.              NAME                       Type              Balance\n"
        <<"==========================================================================\n";
        while(inFile.read(reinterpret_cast<char *> (&acc), sizeof(Bank_Account))){
            cout<<acc.getAccountNumber()<<setw(25)<<acc.getName()<<setw(25)<<acc.getType()<<setw(15)<<acc.getBalance()<<endl;
        }
        inFile.close();
}

void modifyAccount(int accno){
    Bank_Account acc;
    bool flag = false;
    fstream File;
    File.open("account.txt", ios::binary | ios::in | ios::out);
    if(!File){
        cout<<"No Accounts created!"<<endl;
        return;
    }

    while(!File.eof() && !flag){
        File.read(reinterpret_cast<char *> (&acc), sizeof(Bank_Account));
        if(acc.getAccountNumber() == accno){
            acc.print_account();
            cout<<"Enter the new details of the account"<<endl;
            acc.create_account();
            int pos = (-1)*static_cast<int>(sizeof(Bank_Account));
            File.seekp(pos, ios::cur);
            File.write(reinterpret_cast<char *> (&acc), sizeof(Bank_Account));
            cout<<"Account modified successfully!!"<<endl;
            flag = true;
        }
    }

    File.close();
    if(!flag)
        cout<<"Account does not exist"<<endl;
}

int main(){
    char option;
    int num;
    do{
        system("clear");
        cout<<"\t-----------------------------------"<<endl;
        cout<<"\t\tBank Management System"<<endl;
        cout<<"\t-----------------------------------"<<endl;
        cout<<"\t\t\tMENU"<<endl;
        cout<<"\t1.Create Account"<<endl;   //done
        cout<<"\t2.Deposit Amount"<<endl;   //done
        cout<<"\t3.Withdraw Amount"<<endl;  //done
        cout<<"\t4.Balance Enquiry"<<endl;  //done
        cout<<"\t5.All Account Holder List"<<endl;  //done 
        cout<<"\t6.Close An Account"<<endl;     //done
        cout<<"\t7.Modify An Account"<<endl;    //done
        cout<<"\t8.Exit"<<endl;     //done

        cout<<"\tSelect Your Option (1-8): ";
        cin>>option;

        switch (option)
        {
        case '1':
            writeAccount();
            cout << "Account created successfully!" << endl;
            break;
            
        
        case '2':
            system("clear");
            cout<<"Enter your account number: ";
            cin>>num;
            int deposit;
            cout<<"\nEnter the amount you want to deposit: ";
            cin>>deposit;
            addBalance(num, deposit);
            break;

        case '3':
            system("clear");
            cout<<"Enter your account number: ";
            cin>>num;
            int withdraw;
            cout<<"\nEnter the amount you want to withdraw: ";
            cin>>withdraw;
            withdrawBalance(num, withdraw);
            break;

        case '4':
            system("clear");
            cout<<"Enter your account number: ";
            cin>>num;
            Show_Details(num);
            break;

        case '5':
            system("clear");
            displayAllAccounts();
            break;

        case '6':
            system("clear");
            cout<<"Enter your account number: ";
            cin>>num;
            deleteAccount(num);
            break;

        case '7':
            system("clear");
            cout<<"Enter your account number: ";
            cin>>num;
            modifyAccount(num);
            break;

        case '8':
            //exit
            break;

        default:
            cout<<"Invalid Option"<<endl;
        }
        cin.ignore();
        cin.get();
    }while(option != '8');

    return 0;
}




