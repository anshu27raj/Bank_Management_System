#include<bits/stdc++.h>
using namespace std;

struct Transaction{
    string type;
    double amount;
    Transaction* next;

    Transaction(string t,double a){
        type=t;
        amount=a;
        next=NULL;
    }
};

struct Account{
    int accNo;
    string name;
    double balance;
    Transaction* history;

    Account(int no,string n,double bal){
        accNo=no;
        name=n;
        balance=bal;
        history=NULL;
    }
};

unordered_map<int,Account*> bankAccounts;

struct CompareBalance{
    bool operator()(Account* a,Account* b){
        return a->balance<b->balance;
    }
};

queue<pair<int,pair<string,double>>> pendingTransactions;

void addTransaction(Account* acc,string type,double amount){
    Transaction* newT=new Transaction(type,amount);
    newT->next=acc->history;
    acc->history=newT;
}

void openAccount(int accNo,string name,double balance){
    if(bankAccounts.find(accNo)!=bankAccounts.end()){
        cout<<"Account already exists!\n";
        return;
    }

    bankAccounts[accNo]=new Account(accNo,name,balance);
    addTransaction(bankAccounts[accNo],"OPEN",balance);
    cout<<"Account created successfully!\n";
}

void deposit(int accNo,double amount){
    if(bankAccounts.find(accNo)==bankAccounts.end()){
        cout<<"Account not found!\n";
        return;
    }

    bankAccounts[accNo]->balance+=amount;
    addTransaction(bankAccounts[accNo],"DEPOSIT",amount);
    cout<<"Amount deposited successfully!\n";
}

void withdraw(int accNo,double amount){
    if(bankAccounts.find(accNo)==bankAccounts.end()){
        cout<<"Account not found!\n";
        return;
    }

    if(bankAccounts[accNo]->balance<amount){
        cout<<"Insufficient balance!\n";
        return;
    }

    bankAccounts[accNo]->balance-=amount;
    addTransaction(bankAccounts[accNo],"WITHDRAW",amount);
    cout<<"Withdrawal successful!\n";
}

void showAccount(int accNo){
    if(bankAccounts.find(accNo)==bankAccounts.end()){
        cout<<"Account not found!\n";
        return;
    }

    Account* acc=bankAccounts[accNo];

    cout<<"\nAccount No : "<<acc->accNo;
    cout<<"\nName       : "<<acc->name;
    cout<<"\nBalance    : "<<acc->balance<<endl;
}

void showTopAccounts(int k){
    priority_queue<Account*,vector<Account*>,CompareBalance> pq;

    for(auto &it:bankAccounts)
        pq.push(it.second);

    cout<<"\nTop "<<k<<" Account Holders:\n";

    while(!pq.empty() && k--){
        Account* acc=pq.top();
        pq.pop();

        cout<<acc->name<<" (Acc No: "<<acc->accNo
            <<") Balance: "<<acc->balance<<endl;
    }
}

void addPendingTransaction(int accNo,string type,double amount){
    pendingTransactions.push({accNo,{type,amount}});
    cout<<"Transaction added to queue.\n";
}

void processPendingTransactions(){
    while(!pendingTransactions.empty()){
        auto t=pendingTransactions.front();
        pendingTransactions.pop();

        if(t.second.first=="DEPOSIT")
            deposit(t.first,t.second.second);
        else if(t.second.first=="WITHDRAW")
            withdraw(t.first,t.second.second);
    }
    cout<<"All pending transactions processed.\n";
}

void showTransactionHistory(int accNo){
    if(bankAccounts.find(accNo)==bankAccounts.end()){
        cout<<"Account not found!\n";
        return;
    }
    Transaction* temp=bankAccounts[accNo]->history;
    cout<<"\nTransaction History:\n";
    while(temp){
        cout<<temp->type<<" : "<<temp->amount<<endl;
        temp=temp->next;
    }
}

int main(){
    int choice;

    while(true){
        cout<<"\n===== Bank Management System =====\n";
        cout<<"1.Open Account\n";
        cout<<"2.Deposit\n";
        cout<<"3.Withdraw\n";
        cout<<"4.Show Account\n";
        cout<<"5.Show Top Accounts\n";
        cout<<"6.Add Pending Transaction\n";
        cout<<"7.Process Pending Transactions\n";
        cout<<"8.Transaction History\n";
        cout<<"9.Exit\n";

        cout<<"Enter choice: ";
        cin>>choice;

        if(choice==1){
            int no;
            string name;
            double bal;

            cout<<"Enter Account Number: ";
            cin>>no;
            cout<<"Enter Name: ";
            cin.ignore();
            getline(cin,name);
            cout<<"Enter Initial Balance: ";
            cin>>bal;
            openAccount(no,name,bal);
        }
        else if(choice==2){
            int no;
            double amt;
            cout<<"Enter Account Number: ";
            cin>>no;

            cout<<"Enter Amount: ";
            cin>>amt;

            deposit(no,amt);
        }
        else if(choice==3){
            int no;
            double amt;

            cout<<"Enter Account Number: ";
            cin>>no;
            cout<<"Enter Amount: ";
            cin>>amt;
            withdraw(no,amt);
        }

        else if(choice==4){
            int no;
            cout<<"Enter Account Number: ";
            cin>>no;
            showAccount(no);
        }
        else if(choice==5){
            int k;
            cout<<"Enter value of K: ";
            cin>>k;
            showTopAccounts(k);
        }
        else if(choice==6){
            int no;
            string type;
            double amt;

            cout<<"Enter Account Number: ";
            cin>>no;
            cout<<"Enter Type(DEPOSIT/WITHDRAW): ";
            cin>>type;
            cout<<"Enter Amount: ";
            cin>>amt;
            addPendingTransaction(no,type,amt);
        }
        else if(choice==7){
            processPendingTransactions();
        }
        else if(choice==8){
            int no;
            cout<<"Enter Account Number: ";
            cin>>no;

            showTransactionHistory(no);
        }
        else if(choice==9){
            break;
        }
        else{
            cout<<"Invalid Choice!\n";
        }
    }
    return 0;
}
