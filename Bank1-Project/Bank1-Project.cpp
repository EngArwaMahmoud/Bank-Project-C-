#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <iomanip>
#include <fstream>

using namespace std;
const string ClientsFile = "Clients.txt";

enum enMainMenue{show = 1, Add = 2, Delete = 3, Update = 4, Find = 5, Transaction = 6 , Exit = 7};

enum enTransactionMenue { DepositBalance = 1, Withdraw = 2, TotalBalance = 3, MainMenue = 4 };

struct stData {
    string AccountNumber;
    string PinCode;
    string Name;
    string PhoneNumber;
    double AccountBalance;
    bool MarkDelete = false;
};

void MainMenuScreen();

void TransactionMenue();

// Show Vlient List

vector <string> SplitString(string Name, string delim)
{
    vector<string> vString;

    string Word;
    short pos = 0;

    while ((pos = Name.find(delim)) != std::string::npos)
    {
        Word = Name.substr(0, pos);
        if (Word != "")
        {
            vString.push_back(Word);
        }
        Name.erase(0, pos + delim.length());
    }

    if (Name != " ")
    {
        vString.push_back(Name);
    }
    return vString;
}

stData ConvertLineDataToRecord(string Line, string Seperator = "#//#")
{
    stData Data;
    vector<string> vClientData = SplitString(Line, Seperator);


    Data.AccountNumber = vClientData[0];
    Data.PinCode = vClientData[1];
    Data.Name = vClientData[2];
    Data.PhoneNumber = vClientData[3];
    Data.AccountBalance = stod(vClientData[4]);

    return Data;
}

vector <stData> LoadRecordFromFile(string File)
{
    vector<stData> vClient;
    fstream MyFile;

    MyFile.open(File, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        stData Data;

        while (getline(MyFile, Line))
        {
            Data = ConvertLineDataToRecord(Line);
            vClient.push_back(Data);
        }
        MyFile.close();
    }
    return vClient;
}


void PrintData(stData Data)
{

    cout << "| " << setw(15) << left << Data.AccountNumber;
    cout << "| " << setw(10) << left << Data.PinCode;
    cout << "| " << setw(40) << left << Data.Name;
    cout << "| " << setw(12) << left << Data.PhoneNumber;
    cout << "| " << setw(12) << left << Data.AccountBalance;
}

void PrintAllClientsData(vector <stData> vClients)
{
    vector<stData>vClient = LoadRecordFromFile(ClientsFile);


    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ")Client(s).";
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    for (stData& Client : vClients)
    {
        PrintData(Client);
        cout << endl;
    }
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

// Add New Client

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{
    vector <stData> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode
    if (MyFile.is_open())
    {
        string Line;
        stData Client;
        while (getline(MyFile, Line))
        {
            Client = ConvertLineDataToRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return false;
}

stData EnterAccountData()
{
    stData Data;

    cout << "Enter Account Number? ";
    getline(cin >> ws, Data.AccountNumber);

	while (ClientExistsByAccountNumber(Data.AccountNumber, ClientsFile))
	{
		cout << "Account Number already exists, please enter a different Account Number: ";
		getline(cin >> ws, Data.AccountNumber);
	}

    cout << "Enter PinCode? ";
    getline(cin, Data.PinCode);

    cout << "Enter YourName? ";
    getline(cin, Data.Name);

    cout << "Enter Phone Number? ";
    getline(cin, Data.PhoneNumber);

    cout << "Enter Account Balance? ";
    cin >> Data.AccountBalance;

    return Data;
}

string ConvertRecordToLine(stData Client, string Seperator = "#//#")
{
    string S = "";

    S += (Client.AccountNumber) + Seperator;
    S += (Client.PinCode) + Seperator;
    S += Client.Name + Seperator;
    S += (Client.PhoneNumber) + Seperator;
    S += to_string(Client.AccountBalance);

    return S;
}

void AddDataLineToFile(string FileName, string stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
    }
    MyFile.close();
}

void AddNewClient()
{
    stData Client;
    Client = EnterAccountData();
    AddDataLineToFile(ClientsFile, ConvertRecordToLine(Client));
}

void AddClients()
{
    char AddMore = 'Y';

    do {
        system("cls");

        cout << "Add New Client : \n\n";
        AddNewClient();
        cout << "Client added successfully, Do you want to add more clients?";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}

// Delete Client

void PrintClientCard(stData Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "--------------------------------\n";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code : " << Client.PinCode;
    cout << "\nName : " << Client.Name;
    cout << "\nPhone : " << Client.PhoneNumber;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n--------------------------------\n";
}

bool FindClientByAccountNumber(string AccountNumber, vector <stData>& vClient, stData& Client)
{

    for (stData C : vClient)
    {
        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }
    }
    return false;
}

string SearchAccountNumber()
{
    string AccountNumber;
    
    cout << "Please enter Account Number? ";
    cin >> AccountNumber;

    //getline(cin, AccountNumber);
    return AccountNumber;
}

bool MarkDeleteAccount(vector <stData>& vClient, string AccountNumber)
{
    for (stData& C : vClient)
    {
        if (C.AccountNumber == AccountNumber)
        {
            C.MarkDelete = true;
            return true;
        }
    }
    return false;
}

vector <stData> SaveAccounts(string File, vector<stData> vClient)
{

    fstream MyFile;

    string DataLine;
    MyFile.open(File, ios::out);

    if (MyFile.is_open())
    {
        for (stData& C : vClient)
        {
            if (C.MarkDelete == false)
            {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }

        MyFile.close();
    }
    return vClient;
}

bool DeleteAccountNumber(string AccountNumber, vector <stData>& vClient)
{
    stData Client;
    char answer = 'y';

    if (FindClientByAccountNumber(AccountNumber, vClient, Client))
    {

        PrintClientCard(Client);

        cout << "\n\nAre you sure thet you want delete this account? y/n?";
        cin >> answer;

        if (answer == 'y' || answer == 'Y')
        {
            MarkDeleteAccount(vClient, AccountNumber);
            SaveAccounts(ClientsFile, vClient);

            vClient = LoadRecordFromFile(ClientsFile);
            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber
            << ") is Not Found!";
        return false;
    }
}

// Update Client

stData UpdateAccountData(string AccountNumber)
{
    stData Data;
    Data.AccountNumber = AccountNumber;

    cout << "\nEnter PinCode? ";
    getline(cin >> ws, Data.PinCode);

    cout << "Enter YourName? ";
    getline(cin, Data.Name);

    cout << "Enter Phone Number? ";
    getline(cin, Data.PhoneNumber);

    cout << "Enter Account Balance? ";
    cin >> Data.AccountBalance;

    return Data;
}


bool UpdateAccountNumber(string AccountNumber, vector <stData>& vClient)
{
    stData Client;
    char answer = 'y';

    if (FindClientByAccountNumber(AccountNumber, vClient, Client))
    {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want Update this account? y/n?";
        cin >> answer;

        if (answer == 'y' || answer == 'Y')
        {
            for (stData& C : vClient)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = UpdateAccountData(AccountNumber);
                    break;
                }
            }
            SaveAccounts(ClientsFile, vClient);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

// Find Client

void PrintAcoutNumber()
{
    vector <stData> vClient = LoadRecordFromFile(ClientsFile);
    string AccountNumber = SearchAccountNumber();
    stData Client;

    if (FindClientByAccountNumber(AccountNumber, vClient, Client))
    {
        PrintClientCard(Client);
    }
    else {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
    }
}

void ShowFindClientsScreen()
{
    cout << "---------------------------\n\n";
    cout << "Find Client Screen \n\n";
    cout << "---------------------------\n\n";

    PrintAcoutNumber();
} 

void ShowUpdateNewClientsScreen()
{
    vector<stData>vClient = LoadRecordFromFile(ClientsFile);

    cout << "---------------------------\n\n";
    cout << "Updated Client Screen \n\n";
    cout << "---------------------------\n\n";
    UpdateAccountNumber(SearchAccountNumber(), vClient);

}

void ShowDeleteNewClientsScreen()
{
    vector<stData>vClient = LoadRecordFromFile(ClientsFile);

    cout << "---------------------------\n\n";
    cout << "Deleted Client Screen \n\n";
    cout << "---------------------------\n\n";
    DeleteAccountNumber(SearchAccountNumber(), vClient);

}

void ShowAddNewClientsScreen()
{
	cout << "---------------------------\n\n";
	cout << "Add Client Screen \n\n";
	cout << "---------------------------\n\n";
	AddClients();
}

void ShowAllNewClientsScreen()
{
    vector<stData>vClient = LoadRecordFromFile(ClientsFile);

    cout << "---------------------------\n\n";
    cout << "Show All Client Screen \n\n";
    cout << "---------------------------\n\n";
    PrintAllClientsData(vClient);

}

// End Programm

void ExitProgramm()
{
    cout << "-----------------------\n";
	cout << "\tProramm Ended\n";
    cout << "-----------------------\n";
}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to main menue...";
    cin.ignore();
    cin.get();
    system("cls");

	//system("pause>0");
	MainMenuScreen();
}

short ReadMainMenue()
{
    int Choose;
    cout << "Choose What do you want to do? [1 to 7]?";
    cin >> Choose;

    return Choose;
}

void ShowAnswer(enMainMenue Choose)
{

    
    switch ((Choose))
    {
    case enMainMenue::show:
        system("cls");
        ShowAllNewClientsScreen();
		GoBackToMainMenue();
        break;

    case enMainMenue::Add:
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;

    case enMainMenue::Delete:
        system("cls");
        ShowDeleteNewClientsScreen();
        GoBackToMainMenue();
        break;

    case enMainMenue::Update:
        system("cls");
        ShowUpdateNewClientsScreen();
        GoBackToMainMenue();
        break;

    case enMainMenue::Find:
        system("cls");
		ShowFindClientsScreen();
        GoBackToMainMenue();
        break;

    case enMainMenue::Transaction:
        system("cls");
        TransactionMenue();
        break;

    case enMainMenue::Exit:
		system("cls");
        ExitProgramm();
        break;

	default:
		//cout << "Invalid choice, please try again.\n";
		//GoBackToMainMenue();
		//Choose = enMainMenue::show; // Reset to show menu
        break;
    }
}

// Main Menue Screen
void MainMenuScreen()
{
    cout << "====================================\n";
    cout << "\tMain Menu Screen\n";
    cout << "====================================\n\n";

    cout << "[1] Show Client List\n";
    cout << "[2] Add New Client\n";
    cout << "[3] Delete Client\n";
    cout << "[4] Update Client\n";
    cout << "[5] Find Client\n";
    cout << "[6] Transaction\n";
    cout << "[7] Exit\n\n";

    cout << "====================================\n";

    ShowAnswer((enMainMenue)ReadMainMenue());
}

// New Feature

void GoBackToTransactionMenue()
{
    cout << "\n\nPress any key to go back to Transaction menue...";
    cin.ignore();
    cin.get();
    system("cls");

    //system("pause>0");
    TransactionMenue();
}

void PrintDataBalance(stData Data)
{

    cout << "| " << setw(15) << left << Data.AccountNumber;
    cout << "| " << setw(40) << left << Data.Name;
    cout << "| " << setw(12) << left << Data.AccountBalance;
}

void PrintAllClientsDataBalance(vector <stData> vClients)
{
    vector<stData>vClient = LoadRecordFromFile(ClientsFile);


    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ")Client(s).";
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    for (stData& Client : vClients)
    {
        PrintDataBalance(Client);
        cout << endl;
    }
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

void TotalBalances()
{
    vector <stData> vClients = LoadRecordFromFile(ClientsFile);
    PrintAllClientsDataBalance(vClients);
    int sum = 0;

    for (stData C : vClients)
    {
        sum += C.AccountBalance;
    }
    cout << "\n\n\t\t\t\tTotal Balances = " << sum;
}

bool DepositBalanceToClientByAccountNumber(double amount, vector <stData>& vClient,string AccountNumber)
{
    int NewBalance = 0;
    char Answer = 'n';

    cout << "\n\nAre you sure you want Perform this Transactino ? Y/n?";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y')
    {
        for (stData& C : vClient)
        {
            if (C.AccountNumber == AccountNumber)
            {
                NewBalance = C.AccountBalance += amount;
                SaveAccounts(ClientsFile, vClient);
                cout << "Done successfully :), New Balance is " << NewBalance;

                return true;
            }
          }
        return false;
    }
}

void ShowDepositScreen()
{

    cout << "---------------------------\n\n";
    cout << "Deposit Screen \n\n";
    cout << "---------------------------\n\n";

    stData Client;
    vector <stData> vClients = LoadRecordFromFile(ClientsFile);
    string AccountNumber = SearchAccountNumber();

    while (!FindClientByAccountNumber(AccountNumber, vClients,Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = SearchAccountNumber();
    }
    PrintClientCard(Client);

    double amount;
    cout << "\n\nEnter the amount of deposit?";
    cin >> amount;

    DepositBalanceToClientByAccountNumber(amount, vClients, AccountNumber);
}

void ShowWithdrawScreen()
{

    cout << "---------------------------\n\n";
    cout << "Withdraw Screen \n\n";
    cout << "---------------------------\n\n";

    stData Client;
    vector <stData> vClients = LoadRecordFromFile(ClientsFile);
    string AccountNumber = SearchAccountNumber();

    while (!FindClientByAccountNumber(AccountNumber, vClients,Client))
    {
        cout << "\nClient with [" << AccountNumber << "] does not exist.\n";
        AccountNumber = SearchAccountNumber();
    }
    PrintClientCard(Client);

    double amount;
    cout << "\n\nEnter the amount of Withdraw?";
    cin >> amount;

    while (amount > Client.AccountBalance)
    {
        cout << "\nAmount Exceeds the balance, you can withdraw upto : " << Client.AccountBalance << endl;
        cout << "Please enter another amount? ";
        cin >> amount;
    }
    DepositBalanceToClientByAccountNumber(amount * -1, vClients, AccountNumber );
}

void ShowTotalBalancesScreen()
{
    TotalBalances();
}


short ReadTransactionMenue()
{
    int Choose;
    cout << "Choose What do you want to do? [1 to 4]?";
    cin >> Choose;

    return Choose;
}

void ShowTransactionAnswer(enTransactionMenue TrChoose)
{
    switch ((TrChoose))
    {
    case enTransactionMenue::DepositBalance:
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionMenue();
        break;

    case enTransactionMenue::Withdraw:
        system("cls");
        ShowWithdrawScreen();
        GoBackToTransactionMenue();
        break;

    case enTransactionMenue::TotalBalance:
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionMenue();
        break;

    case enTransactionMenue::MainMenue:
        system("cls");
        MainMenuScreen();
        break;
    }
}

// TransactionMenueScreen

void TransactionMenue()
{

    cout << "====================================\n";
    cout << "\nTransaction Menu Screen\n";
    cout << "====================================\n\n";

    cout << "[1] Deposit\n";
    cout << "[2] Withdraw\n";
    cout << "[3] Total Balances\n";
    cout << "[4] MainMenue\n\n";

    cout << "====================================\n";

    ShowTransactionAnswer((enTransactionMenue)ReadTransactionMenue());
}

int main()
{
    MainMenuScreen();

	system("pause>0");
}