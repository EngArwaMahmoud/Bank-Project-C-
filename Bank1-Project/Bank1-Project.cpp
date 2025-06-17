#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <iomanip>
#include <fstream>

using namespace std;
const string ClientsFile = "Clients.txt";
const string UserFile = "Users.txt";

enum enMainMenue { show = 1, Add = 2, Delete = 3, Update = 4, Find = 5, Transaction = 6, ManageUsers = 7, LogOut = 8 };

enum enTransactionMenue { DepositBalance = 1, Withdraw = 2, TotalBalance = 3, MainMenue = 4 };

enum enUserMenue { ShowUsers = 1, AddUser = 2, DeleteUser = 3, UpdateUser = 4, FindUser = 5, MainMenu = 6 };

enum enMainMenuePermissions{ eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4, pUpdateClients = 8, pFindClient = 16, pTranactions = 32, pManageUsers = 64 };


struct stData {
    string AccountNumber;
    string PinCode;
    string Name;
    string PhoneNumber;
    double AccountBalance;
    bool MarkDelete = false;
};

// User Structure
struct stUser {
    string UserName;
    string Password;
    int permission; // Admin, User
    bool MarkDelete = false;
};

stUser CurrentUser;

void MainMenuScreen();

void TransactionMenue();

void ManageUsersMainMenue();

void LoginScreen();

void GoBackToMainMenue();

//permsion
bool CheckPermission(enMainMenuePermissions Permission)
{
    if (CurrentUser.permission == enMainMenuePermissions::eAll)
        return true;

    if ((CurrentUser.permission & Permission) == Permission)
        return true;
    else
        return false;
}

void ShowAccessDeniedMessage()
{
	cout << "\n\n====================================\n";
	cout << "\n\nAccess Denied! You do not have permission to perform this action.\n";
	cout << "Please contact the administrator if you believe this is an error.\n";
	cout << "\n====================================\n";
}
// Show Client List

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
	return false;
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
    if (!CheckPermission(enMainMenuePermissions::pFindClient))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
        return;
    }

    cout << "---------------------------\n\n";
    cout << "Find Client Screen \n\n";
    cout << "---------------------------\n\n";

    PrintAcoutNumber();
}

void ShowUpdateNewClientsScreen()
{
    if (!CheckPermission(enMainMenuePermissions::pUpdateClients))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
        //return;
    }

    vector<stData>vClient = LoadRecordFromFile(ClientsFile);

    cout << "---------------------------\n\n";
    cout << "Updated Client Screen \n\n";
    cout << "---------------------------\n\n";
    UpdateAccountNumber(SearchAccountNumber(), vClient);

}

void ShowDeleteNewClientsScreen()
{
    if (!CheckPermission(enMainMenuePermissions::pDeleteClient))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
        return;
    }

    vector<stData>vClient = LoadRecordFromFile(ClientsFile);

    cout << "---------------------------\n\n";
    cout << "Deleted Client Screen \n\n";
    cout << "---------------------------\n\n";
    DeleteAccountNumber(SearchAccountNumber(), vClient);

}

void ShowAddNewClientsScreen()
{
    if (!CheckPermission(enMainMenuePermissions::pAddNewClient))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
        return;
    }


    cout << "---------------------------\n\n";
    cout << "Add Client Screen \n\n";
    cout << "---------------------------\n\n";
    AddClients();
}

void ShowAllNewClientsScreen()
{
    if (!CheckPermission(enMainMenuePermissions::pListClients))
    {
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
        return;
    }

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
    cout << "Choose What do you want to do? [1 to 8] ? ";
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

    case enMainMenue::ManageUsers:
        system("cls");
        ManageUsersMainMenue();
        break;

    case enMainMenue::LogOut:
        system("cls");
        //ExitProgramm();
        LoginScreen();
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
    system("cls");

    cout << "====================================\n";
    cout << "\tMain Menu Screen\n";
    cout << "====================================\n\n";

    cout << "[1] Show Client List\n";
    cout << "[2] Add New Client\n";
    cout << "[3] Delete Client\n";
    cout << "[4] Update Client\n";
    cout << "[5] Find Client\n";
    cout << "[6] Transaction\n";
    cout << "[7] Manage Users\n";
    cout << "[8] Log Out\n\n";

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

bool DepositBalanceToClientByAccountNumber(double amount, vector <stData>& vClient, string AccountNumber)
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

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
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

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
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
    DepositBalanceToClientByAccountNumber(amount * -1, vClients, AccountNumber);
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

void TransactionMenue()
{
	if (!CheckPermission(enMainMenuePermissions::pTranactions))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}

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


// Manage Users

stUser ConvertLineDataToRecordUser(string Line, string Seperator = "#//#")
{
    stUser User;
    vector<string> vUserData = SplitString(Line, Seperator);

    User.UserName = vUserData[0];
    User.Password = vUserData[1];
    User.permission =stoi (vUserData[2]);

    return User;
}

string ConvertRecordToLineUser(stUser User, string Seperator = "#//#")
{
    string S = "";

    S += (User.UserName) + Seperator;
    S += (User.Password) + Seperator;
    S += to_string(User.permission);

    return S;
}

void AddDataUserToFile(string FileName, string stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;
    }
    MyFile.close();
}


void PrintUserList(stUser User)
{

    cout << "| " << setw(15) << left << User.UserName;
    cout << "| " << setw(40) << left << User.Password;
    cout << "| " << setw(12) << left << User.permission;
}

void PrintAllUserList(vector <stUser> vUsers)
{

    cout << "\n\t\t\t\t\tUsers List (" << vUsers.size() << ")User(s).";
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    cout << "| " << left << setw(15) << "UserName";
    cout << "| " << left << setw(40) << "Password";
    cout << "| " << left << setw(12) << "Permission";
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
    for (stUser& Users : vUsers)
    {
        PrintUserList(Users);

        cout << endl;
    }
    cout <<
        "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;
}

vector <stUser> LoadDataFromFileUser(string File)
{
    vector<stUser> vUsers;
    fstream MyFile;

    MyFile.open(File, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        stUser User;

        while (getline(MyFile, Line))
        {
            User = ConvertLineDataToRecordUser(Line);
            vUsers.push_back(User);
        }
        MyFile.close();
    }
    return vUsers;
}


bool UserExistsByName(string Name, string FileName)
{
    vector <stUser> vUser;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode
    if (MyFile.is_open())
    {
        string Line;
        stUser User;
        while (getline(MyFile, Line))
        {
            User = ConvertLineDataToRecordUser(Line);
            if (User.UserName == Name)
            {
                MyFile.close();
                return true;
            }
            vUser.push_back(User);
        }
        MyFile.close();
    }
    return false;
}

int ReadPermissionToSet()
{
    char Answer = 'n';
    int permission = 0;

	cout << "\nDo you want to give access to all permissions? y/n?";
	cin >> Answer;
	if (Answer == 'y' || Answer == 'Y')
	{
		return enMainMenuePermissions::eAll; // All permissions
	}

    cout << "\n\nDo you want to give access to: \n";

    cout << "\nShow client list? y/n?";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        permission += enMainMenuePermissions::pListClients;
    }

    cout << "\nAdd new client? y/n?";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        permission += enMainMenuePermissions::pAddNewClient;
    }

    cout << "\nDelete client? y/n?";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        permission += enMainMenuePermissions::pDeleteClient;
    }

    cout << "\nUpdate client? y/n?";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        permission += enMainMenuePermissions::pUpdateClients;
    }

    cout << "\nFind client? y/n?";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        permission += enMainMenuePermissions::pFindClient;
    }

    cout << "\nTransaction? y/n?";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        permission += enMainMenuePermissions::pTranactions;
    }

    cout << "\nmanage Users? y/n?";
    cin >> Answer;
    if (Answer == 'y' || Answer == 'Y')
    {
        permission += enMainMenuePermissions::pManageUsers;
    }

    return permission;
}

stUser AddOneUser()
{
    stUser User;
    char answer;

    cout << "\nEnter User Name? ";
    getline(cin >> ws, User.UserName);

    while (UserExistsByName(User.UserName, UserFile))
    {
        cout << "User With [ " << User.UserName << " ] already exists, Enter a User Name ? ";
        getline(cin >> ws, User.UserName);
    }

    cout << "Enter Password? ";
    getline(cin, User.Password);

	User.permission = ReadPermissionToSet();

	return User;
}

void AddNewUser()
{
	stUser User;
	User = AddOneUser();
	AddDataUserToFile(UserFile, ConvertRecordToLineUser(User));
}

void AddUsers()
{
    char AddMore = 'Y';

    do {
        system("cls");

        cout << "Add New User : \n\n";
        AddNewUser();
        cout << "Client added successfully, Do you want to add more clients? y/n?";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}

void showlistscreen()
{
    vector<stUser>vuser = LoadDataFromFileUser(UserFile);
    PrintAllUserList(vuser);
}

void AddNewUserScreen()
{

    cout << "-------------------------\n";
    cout << "\tAdd New User Screen\n";
    cout << "-------------------------\n";

    AddUsers();
}

// Delete User 

void PrintUserCard(stUser User)
{
    cout << "\nThe following are the User details:\n";
    cout << "--------------------------------\n";
    cout << "\nUser Name: " << User.UserName;
    cout << "\nPassword : " << User.Password;
    cout << "\nPirmission : " << User.permission;
    cout << "\n--------------------------------\n";
}

bool FindUserBuyUserName(string UserName, vector <stUser> vUser, stUser& User)
{
	for (stUser U : vUser)
	{
		if (U.UserName == UserName)
		{
			User = U;
			return true;
		}
	}
	return false;
}

bool FindUserByUserNameandPassword(string UserName, string Password, stUser& User)
{
	vector <stUser> vUser = LoadDataFromFileUser(UserFile);

    for (stUser U : vUser)
    {
        if (U.UserName == UserName && U.Password == Password)
        {
            User = U;
            return true;
        }
    }
    return false;
}

string SearchUserName()
{
    string Username;

    cout << "Please enter User Name? ";
    cin >> Username;

    //getline(cin, AccountNumber);
    return Username;
}

bool MarkDeleteAccount(vector <stUser>& vUser, string Username)
{
    for (stUser& U : vUser)
    {
        if (U.UserName == Username)
        {
            U.MarkDelete = true;
            return true;
        }
    }
    return false;
}

vector <stUser> SaveUsers(string File, vector<stUser> vUser)
{

    fstream MyFile;

    string DataLine;
    MyFile.open(File, ios::out);

    if (MyFile.is_open())
    {
        for (stUser& U : vUser)
        {
            if (U.MarkDelete == false)
            {
                DataLine = ConvertRecordToLineUser(U);
                MyFile << DataLine << endl;
            }
        }

        MyFile.close();
    }
    return vUser;
}

bool DeleteUserName(string UserName, vector <stUser>& vUser)
{
    stUser User;
    char answer = 'y';

    if (UserName == "Admin")
    {
        cout << "\nYou can not delete Admin User!";
        return false;
    }

    if (FindUserBuyUserName(UserName, vUser, User))
    {

        PrintUserCard(User);

        cout << "\n\nAre you sure thet you want delete this account? y/n?";
        cin >> answer;

        if (answer == 'y' || answer == 'Y')
        {
            MarkDeleteAccount(vUser, UserName);
            SaveUsers(UserFile, vUser);

            vUser = LoadDataFromFileUser(UserFile);
            cout << "\n\nClient Deleted Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with User Name (" << UserName << ") is Not Found!";
        return false;
    }
}

void ShowDeleteScreen()
{
    vector<stUser>vUser = LoadDataFromFileUser(UserFile);
    stUser  user;

    cout << "---------------------------\n\n";
    cout << "Deleted User Screen \n\n";
    cout << "---------------------------\n\n";
    DeleteUserName(SearchUserName(),vUser);
}

// Update User

stUser UpdatePassword(string UserName)
{
    stUser User;
    User.UserName = UserName;

    cout << "\nEnter Password? ";
    getline(cin >> ws, User.Password);

    return User;
}

bool UpdatePass(string UserName, vector<stUser> vUser)
{
    stUser User;
    char answer = 'y';

    if (FindUserBuyUserName(UserName, vUser, User))
    {

        PrintUserCard(User);

        cout << "\n\nAre you sure you want Update this account? y/n?";
        cin >> answer;

        if (answer == 'y' || answer == 'Y')
        {
            for (stUser& U : vUser)
            {
                if (U.UserName == UserName)
                {
                    U = UpdatePassword(UserName);
                    break;
                }
            }

            SaveUsers(UserFile, vUser);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }
    }
    else
    {
        cout << "\nClient with User Name (" << UserName << ") is Not Found!";
        return false;
    }
}

void ShowUpdateUserScreen() {

    vector<stUser>vUser = LoadDataFromFileUser(UserFile);
    stUser user;

    cout << "---------------------------\n\n";
    cout << "Update User Screen \n\n";
    cout << "---------------------------\n\n";

    UpdatePass(SearchUserName(),vUser);
}

// Find user
void FindUsername()
{
    vector <stUser> vuser = LoadDataFromFileUser(UserFile);
    string UserName = SearchUserName();
    stUser User;

    if (FindUserBuyUserName(UserName,vuser, User))
    {
        PrintUserCard(User);
    }
    else {
        cout << "\nUser with User Name (" << UserName << ") is Not Found!";
    }
}

void  ShowFindUserScreen()
{

    cout << "---------------------------\n";
    cout << "Find User Screen \n";
    cout << "---------------------------\n\n";

    FindUsername();
}

void GoBackToUserMenue()
{
    cout << "\n\nPress any key to go back to User menue...";
    cin.ignore();
    cin.get();
    system("cls");

    //system("pause>0");
    ManageUsersMainMenue();
}

short ReadUserMenue()
{
    int Choose;
    cout << "Choose What do you want to do? [1 to 6] ? ";
    cin >> Choose;

    return Choose;
}

void ShowUserAnswer(enUserMenue USChoose)
{
    switch ((USChoose))
    {
    case enUserMenue::ShowUsers:
        system("cls");
        showlistscreen();
        GoBackToUserMenue();
        break;

    case enUserMenue::AddUser:
        system("cls");
        AddNewUserScreen();
        GoBackToUserMenue();
        break;

    case enUserMenue::DeleteUser:
        system("cls");
        ShowDeleteScreen();
        GoBackToUserMenue();
        break;

    case enUserMenue::UpdateUser:
        system("cls");
        ShowUpdateUserScreen();
        GoBackToUserMenue();
        break;

    case enUserMenue::FindUser:
        system("cls");
        ShowFindUserScreen();
        GoBackToUserMenue();
        break;

    case enUserMenue::MainMenu:
        system("cls");
        MainMenuScreen();
        break;
    }
}

void ManageUsersMainMenue()
{
	if (!CheckPermission(enMainMenuePermissions::pManageUsers))
	{
		ShowAccessDeniedMessage();
		GoBackToMainMenue();
		return;
	}

    cout << "====================================\n";
    cout << "\tManage Users Screen\n";
    cout << "====================================\n\n";
    cout << "[1] Show All Users\n";
    cout << "[2] Add New User\n";
    cout << "[3] Delete User\n";
    cout << "[4] Update User\n";
    cout << "[5] Find User\n";
    cout << "[6] Main Menu\n\n";
    cout << "====================================\n";
    ShowUserAnswer(enUserMenue(ReadUserMenue()));
}

bool LoadUserInfo(string Username, string Password)
{
	if (FindUserByUserNameandPassword(Username , Password , CurrentUser))
		return true;
    else
		return false;
}

void LoginScreen()
{

    bool LoginFailed = false;
	string Username, Password;

    do
    {
        system("cls");

        cout << "====================================\n";
        cout << "\tLogin Screen\n";
        cout << "====================================\n\n";

		if (LoginFailed)
		{
			cout << "\nInvalid username/password. Please try again.\n";
		}

        cout << "\nEnter User Name? ";
        cin >> Username;

        cout << "Enter Password? ";
        cin >> Password;

		LoginFailed = !LoadUserInfo( Username,  Password);

    } while (LoginFailed);

    MainMenuScreen();
}

int main()
{
    LoginScreen();
    system("pause>0");
    return 0;
}