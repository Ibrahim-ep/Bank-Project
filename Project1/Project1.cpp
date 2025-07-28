#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <limits>

using namespace std;

struct stUsers
{
    string Name;
    string PassWord;
    short Permissions;
    bool MarkUser = false;
};

const string FileName = "Clients.txt";

enum enMainMenueOptions {ClientList = 1, AddNewClient = 2, DeleteClientsFromFile = 3, UpdateClient = 4, FindClient = 5, eTransactions = 6,eManageUsers = 7, eLogOut = 8};

enum enTransactionsMenueOptions {eDeposit = 1, eWithdraw = 2, eTotalBalances = 3, eMainMenue = 4};

enum enManageUserMenueOptions {eUsersList = 1, eAddNewUser = 2, eDeleteUser = 3, eUpdateUser = 4, eFindUser = 5, Mange2MainMenue = 6};

enum enMainMenuePermissions {
eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4,
pUpdateClients = 8, pFindClient = 16, pTranactions = 32, ManageUsers = 64
} ;

void ManageUsersScreen();
void ShowTransactionsMenue();
void ShowMainMenue();
void ShowAccessDeniedScreen();
bool CheckUserAccessPermissions(enMainMenuePermissions Permissions);


stUsers CurrentUser;

struct sClient
{
    string AcountNumber;
    string PINcode;
    string Name;
    string PhoneNumber;
    double Balance;
    bool MarkClient = false;
};

string ReadAcountNumber()
{
    string AcountNmber;

    cout << "Pleas enter the acount number\n";
    cin >> AcountNmber;

    return AcountNmber;
}

string ReadUsername()
{
    string Username;

    cout << "Pleas enter the Username\n";
    cin >> Username;

    return Username;
}

string ReadPassword()
{
    string Password;

    cout << "Pleas enter the Password\n";
    cin >> Password;

    return Password;
}

sClient ChangeClientRecord(string AcountNumber)
{
    sClient Client;

    Client.AcountNumber = AcountNumber;

    cout << "Enter PinCode? ";
    getline(cin >> ws, Client.PINcode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);    
 
    cout << "Enter Phone? ";     
    getline(cin, Client.PhoneNumber);     

    cout << "Enter AccountBalance? ";
    cin >> Client.Balance;

    return Client;
}

short ReadPermissionsToSet()
{
    char Answer = ' ';
    short PermissionNumber = 0;
    
    cout << "Do you want to give full acces ? Y/N ?\n";
    cin >> Answer;
    
    if (toupper(Answer) == 'Y')
    {
        return -1;
    }
    
    cout << "Show client list ? Y/N ?\n";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        PermissionNumber += enMainMenuePermissions::pListClients;
    }

    cout << "Add new client ? Y/N ?\n";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        PermissionNumber += enMainMenuePermissions::pAddNewClient;
    }

    cout << "Delete client ? Y/N ?\n";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        PermissionNumber += enMainMenuePermissions::pDeleteClient;
    }

    cout << "Update client ? Y/N ?\n";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        PermissionNumber += enMainMenuePermissions::pUpdateClients;
    }

    cout << "Find client ? Y/N ?\n";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        PermissionNumber += enMainMenuePermissions::pFindClient;
    }

    cout << "Transactions ? Y/N ?\n";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        PermissionNumber += enMainMenuePermissions::pTranactions;
    }

    cout << "Manage Users ? Y/N ?\n";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        PermissionNumber += enMainMenuePermissions::ManageUsers;
    }

    return PermissionNumber;
}

stUsers ChangeUserInfo(string Username)
{
    stUsers User;

    User.Name = Username;

    cout << "Enter Password? ";
    getline(cin >> ws, User.PassWord);

    User.Permissions = ReadPermissionsToSet();

    return User;
}

string ConvertRecordToLine(sClient Client, string Seporator = "#//#")
{
    string sClient;
    
    sClient += Client.AcountNumber + Seporator;
    sClient += Client.PINcode + Seporator;
    sClient += Client.Name + Seporator;
    sClient += Client.PhoneNumber + Seporator;
    sClient += to_string(Client.Balance);

    return sClient;
}

string ConvertUserInfoToLine(stUsers User, string Seporator = "#//#")
{
    string sUser;
    
    sUser += User.Name + Seporator;
    sUser += User.PassWord + Seporator;
    sUser += to_string(User.Permissions);

    return sUser;
}

vector <string> SplitEachWord(string S1,string delim)
{
    short pos = 0;

    string sWord;
    vector <string> vWord;
    // use find() function to find position of the delimiters

    while ((pos = S1.find(delim)) != std::string::npos)
    {
        sWord = S1.substr(0,pos); // store thr word

        if (S1 != " ")
        {
            vWord.push_back(sWord);
        }

        (S1.erase(0,pos + delim.length())); //erase() until position and move to next word
    }
    if (S1 != "") // it print last word of the string
    {
        vWord.push_back(S1);
    }
    return vWord;
}

sClient ConvertLineToRecord(string Line, string Seporator = "#//#")
{
    sClient Client;
    vector <string> vClient = SplitEachWord(Line,Seporator);

    Client.AcountNumber = vClient[0];     
    Client.PINcode = vClient[1];    
    Client.Name = vClient[2];    
    Client.PhoneNumber = vClient[3];    
    Client.Balance = stod(vClient[4]);

    return Client;
}

stUsers ConvertLineToUserInfo(string Line, string Seporator = "#//#")
{
    stUsers User;
    vector <string> vUser = SplitEachWord(Line,Seporator);

    User.Name = vUser[0];
    User.PassWord = vUser[1];
    User.Permissions = stoi(vUser[2]);

    return User;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName) 
{ 
    vector <sClient> vClients; 
    fstream MyFile;     
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())    
    { 
        string Line; 
        sClient Client; 
        while (getline(MyFile, Line))         
        {             
            Client = ConvertLineToRecord(Line); 
            if (Client.AcountNumber == AccountNumber)       
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

bool UserExistsByUsername(string Username, string FileName) 
{ 
    vector <stUsers> vUser; 
    fstream MyFile;     
    MyFile.open(FileName, ios::in);
    if (MyFile.is_open())    
    { 
        string Line; 
        stUsers User; 
        while (getline(MyFile, Line))         
        {             
            User = ConvertLineToUserInfo(Line);
            if (User.Name == Username)       
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

sClient ReadClientRecord()
{
    sClient Client;

    cout << "\nEnter Account Number? ";
    getline(cin >> ws, Client.AcountNumber);

    while (ClientExistsByAccountNumber(Client.AcountNumber,"Clients.txt"))
    {
        cout << "\nClient with ["<< Client.AcountNumber <<"] already exists, Enter another Account Number? ";         
        getline(cin >> ws, Client.AcountNumber);
    }

    cout << "Enter PinCode? ";
    getline(cin, Client.PINcode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);    
 
    cout << "Enter Phone? ";     
    getline(cin, Client.PhoneNumber);     

    cout << "Enter AccountBalance? ";
    cin >> Client.Balance;

    return Client;
}

bool FindUsertByUserName(stUsers &User, vector <stUsers> vUser, string Username)
{
    for (stUsers &u : vUser)
    {
        if (u.Name == Username)
        {
            User = u;
            return true;
        }
    }
    return false;
}

stUsers ReadUserInfo()
{
    stUsers User;

    cout << "\nEnter Username? ";
    getline(cin >> ws, User.Name);

    while (UserExistsByUsername(User.Name,"Users.txt"))
    {
        cout << "\nClient with ["<< User.Name <<"] already exists, Enter another Account Number? ";         
        getline(cin >> ws, User.Name);
    }

    cout << "Enter Password? ";
    getline(cin, User.PassWord);

    User.Permissions = ReadPermissionsToSet();

    return User;
}

vector <sClient> LoadClientsDataFromFile(string FileName)
{
    vector <sClient> vClient;
    fstream MyFile;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile,Line))
        {
            Client = ConvertLineToRecord(Line);

            vClient.push_back(Client);
        }
        MyFile.close();
    }
    return vClient;
}

vector <stUsers> LoadUsersDataFromFile(string FileName)
{
    vector <stUsers> vUser;
    fstream MyFile;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        stUsers User;

        while (getline(MyFile,Line))
        {
            User = ConvertLineToUserInfo(Line);

            vUser.push_back(User);
        }
        MyFile.close();
    }
    return vUser;
}

vector <stUsers> SaveUserDataToFile(string FileName, vector <stUsers> vUser)
{
    fstream MyFile;

    MyFile.open(FileName, ios::out);

    string DataLine;

    if (MyFile.is_open())
    {
        for (stUsers &u : vUser)
        {
            if (u.MarkUser == false)
            {
                DataLine = ConvertUserInfoToLine(u);

                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vUser;
}

vector <sClient> SaveClientDataToFile(string FileName, vector <sClient> vClient)
{
    fstream MyFile;

    MyFile.open(FileName, ios::out);

    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient &c : vClient)
        {
            if (c.MarkClient == false)
            {
                DataLine = ConvertRecordToLine(c);

                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vClient;
}

void SaveClientRecordToFile(string FileName,string stDataLine)
{
    fstream MyFile;

    MyFile.open("Clients.txt", ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;

        MyFile.close();
    }
}

void SaveUserInfoToFile(string FileName,string stDataLine)
{
    fstream MyFile;

    MyFile.open("Users.txt", ios::out | ios::app);

    if (MyFile.is_open())
    {
        MyFile << stDataLine << endl;

        MyFile.close();
    }
}

void AddClient()
{
    sClient Client = ReadClientRecord();

    SaveClientRecordToFile("Clients.txt",ConvertRecordToLine(Client));
}

void AddUser()
{
    stUsers User = ReadUserInfo();

    SaveUserInfoToFile("Users.txt",ConvertUserInfoToLine(User));
}

void AddClientsScreen()
{
    char AddMore = ' ';

    if (!CheckUserAccessPermissions(enMainMenuePermissions::pAddNewClient))
    {
        ShowAccessDeniedScreen();
        return;
    }

    cout << "\n\n-------------------------------\n";
    cout << "     Add New Clients Screen\n";
    cout << "-------------------------------\n" << endl;

    do
    {
        cout << "\nAdding new client\n\n";

        AddClient();

       cout << "Client Added Successfully, Do you want to add more clients? Y/N\n";
       cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}

void AddUsersScreen()
{
    char AddMore = ' ';

    cout << "\n\n-------------------------------\n";
    cout << "     Add New Users Screen\n";
    cout << "-------------------------------\n" << endl;

    do
    {
        cout << "\nAdding new User\n";

        AddUser();

       cout << "User Added Successfully, Do you want to add more users? Y/N\n";
       cin >> AddMore;

    } while (toupper(AddMore) == 'Y');
}

void PrintClientRecord(sClient Client) 
{     
    cout << "| " << setw(15) << left << Client.AcountNumber;     
    cout << "| " << setw(10) << left << Client.PINcode;     
    cout << "| " << setw(40) << left << Client.Name;     
    cout << "| " << setw(12) << left << Client.PhoneNumber;     
    cout << "| " << setw(12) << left << Client.Balance; 
}

void PrintUserInfo(stUsers User) 
{     
    cout << "| " << setw(15) << left << User.Name;     
    cout << "| " << setw(10) << left << User.PassWord;     
    cout << "| " << setw(40) << left << User.Permissions;
}

void PrintClientBalanceRecord(sClient Client) 
{     
    cout << "| " << setw(15) << left << Client.AcountNumber;      
    cout << "| " << setw(40) << left << Client.Name;     
    cout << "| " << setw(12) << left << Client.Balance; 
}

void PrintClientsDetails(sClient Client)
{
    cout << "The following are the client details\n";
    cout << "---------------------------\n";
    cout << "Acount Number : " << Client.AcountNumber << endl;
    cout << "PIN code      : " << Client.PINcode << endl;
    cout << "Acount Name   : " << Client.Name << endl;
    cout << "Phone Number  : " << Client.PhoneNumber << endl;
    cout << "Acount Balance : " << Client.Balance << endl;
    cout << "---------------------------\n";
}

void PrintUsersDetails(stUsers User)
{
    cout << "The following are the User details\n";
    cout << "---------------------------\n";
    cout << "Username    : " << User.Name << endl;
    cout << "Password    : " << User.PassWord << endl;
    cout << "Permissions : " << User.Permissions << endl;
    cout << "---------------------------\n";
}

void ShowAccessDeniedScreen()
{
    cout << "\n\n==================================\n";
    cout << "       Access Denied\n";
    cout << "    You Don't Have Permissions\n";
    cout << "==================================\n";
}

void PrintAllClientsData() 
{     
    if (!CheckUserAccessPermissions(enMainMenuePermissions::pListClients))
    {
        ShowAccessDeniedScreen();
        return;
    }

    vector <sClient> vClient = LoadClientsDataFromFile(FileName);

    cout << "\n\t\t\t\t\tClient List (" << vClient.size() << ") Client(s).";     
    cout << "\n_______________________________________________________";     
    cout << "_________________________________________\n" << endl;     
    cout << "| " << left << setw(15) << "Accout Number";     
    cout << "| " << left << setw(10) << "Pin Code";     
    cout << "| " << left << setw(40) << "Client Name";     
    cout << "| " << left << setw(12) << "Phone";     
    cout << "| " << left << setw(12) << "Balance";     
    cout << "\n_______________________________________________________";     
    cout << "_________________________________________\n" << endl; 

    for (sClient &Client : vClient)     
    {         
        PrintClientRecord(Client);         
        cout << endl;    
    }     

    cout << "\n_______________________________________________________";     
    cout << "_________________________________________\n" << endl; 
}

void PrintAllUsersData() 
{     
    vector <stUsers> vUser = LoadUsersDataFromFile("Users.txt");

    cout << "\n\t\t\t\t\tClient List (" << vUser.size() << ") User(s).";     
    cout << "\n_______________________________________________________";     
    cout << "_________________________________________\n" << endl;     
    cout << "| " << left << setw(15) << "Username";     
    cout << "| " << left << setw(10) << "Password";     
    cout << "| " << left << setw(40) << "Permissions";      
    cout << "\n_______________________________________________________";     
    cout << "_________________________________________\n" << endl; 

    for (stUsers &User : vUser)     
    {         
        PrintUserInfo(User);  
        cout << endl;    
    }     

    cout << "\n_______________________________________________________";     
    cout << "_________________________________________\n" << endl; 
}

bool FindClientByAcountNumber(sClient &Client, vector <sClient> vClient, string AcountNumber)
{
    for (sClient &c : vClient)
    {
        if (c.AcountNumber == AcountNumber)
        {
            Client = c;
            return true;
        }
    }
    return false;
}

bool YouWantToDeleteClient()
{
    char Delete = ' ';

    cout << "Are you sure you want to delete this client ? Y/N";
    cin >> Delete;

    if (toupper(Delete) == 'Y')
    {
        return true;
    }
    return false;
}

bool YouWantToDeleteUser()
{
    char Delete = ' ';

    cout << "Are you sure you want to delete this user ? Y/N";
    cin >> Delete;

    if (toupper(Delete) == 'Y')
    {
        return true;
    }
    return false;
}

bool YouWantToUpdateClient()
{
    char Answer = ' ';

    cout << "\nAre you sure you want to update this client ? Y/N ? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        return true;
    }
    return false;
}

bool YouWantToUpdateUser()
{
    char Answer = ' ';

    cout << "\nAre you sure you want to update this User ? Y/N ? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        return true;
    }
    return false;
}

bool YouWantToPerfromTransaction()
{
    char Answer = ' ';

    cout << "\nAre you sure you want to Perfrom this transaction ? Y/N ? ";
    cin >> Answer;

    if (toupper(Answer) == 'Y')
    {
        return true;
    }
    return false;
}

bool MarkClientForDeleteByAcountNumber(vector <sClient> &vClient, string AcountNumber)
{
    for (sClient &c : vClient)
    {
        if (c.AcountNumber == AcountNumber)
        {
            c.MarkClient = true;
            return true;
        }
    }
    return false;
}

bool MarkUserByUsername(vector <stUsers> &vUser, string UserName)
{
    for (stUsers &u : vUser)
    {
        if (u.Name == UserName)
        {
            u.MarkUser = true;
            return true;
        }
    }
    return false;
}

bool DeleteClientsF(vector <sClient> &vClient, string AcountNumber)
{
    sClient Client;

    if (FindClientByAcountNumber(Client,vClient,AcountNumber))
    {
        PrintClientsDetails(Client);
        {
            if (YouWantToDeleteClient())
            {
                MarkClientForDeleteByAcountNumber(vClient,AcountNumber);

                SaveClientDataToFile(FileName,vClient);

                vClient = LoadClientsDataFromFile(FileName);

                cout << "\n\nClient Deleted Successfully\n";
                return true;
            }
        }
    }
    else
    {
        cout << "Client with " << AcountNumber << " Not found" << endl;
    }
   
    return false;
}

bool DeleteUsers(vector <stUsers> &vUser, string UserName)
{
    stUsers User;

    if (UserName == "Admin")
    {
        cout << "You can't delete this user\n";
        return false;
    }
    else if (FindUsertByUserName(User,vUser,UserName))
    {
        PrintUsersDetails(User);
        {
            if (YouWantToDeleteUser())
            {
                MarkUserByUsername(vUser,UserName);

                SaveUserDataToFile("Users.txt",vUser);

                vUser = LoadUsersDataFromFile("Users.txt");

                cout << "\n\nUser Deleted Successfully\n";
                return true;
            }
        }
    }
    else
    {
        cout << "User with " << UserName << " Not found" << endl;
    }
   
    return false;
}

void DeleteClientScreen()
{
    if (!CheckUserAccessPermissions(enMainMenuePermissions::pDeleteClient))
    {
        ShowAccessDeniedScreen();
        return;
    }

    cout << "\n\n-------------------------------\n";
    cout << "     Delete Client Screen\n";
    cout << "-------------------------------\n" << endl;

    vector <sClient> vClient = LoadClientsDataFromFile("Clients.txt");
    string AcountNumber = ReadAcountNumber();

    DeleteClientsF(vClient,AcountNumber);
}

void DeleteUserScreen()
{
    cout << "\n\n-------------------------------\n";
    cout << "     Delete User Screen\n";
    cout << "-------------------------------\n" << endl;

    vector <stUsers> vUser = LoadUsersDataFromFile("Users.txt");
    string Username = ReadUsername();

    DeleteUsers(vUser,Username);
}

bool UpdateClientInformation(vector <sClient> &vClient, string AcountNumber)
{
    sClient Client;

    if(FindClientByAcountNumber(Client,vClient,AcountNumber))
    {
        PrintClientRecord(Client);

        if (YouWantToUpdateClient())
        {
            for (sClient &c : vClient)
            {
                if (c.AcountNumber == AcountNumber)
                {
                    c = ChangeClientRecord(AcountNumber);
                    break;
                }
            }

            SaveClientDataToFile(FileName,vClient);

            cout << "\n\nClient Updated Successfully" << endl;

            return true;
        }
    }
    else
    {
        cout << "Client with Acount Number " << (AcountNumber) <<  " not found" << endl;
    }
    return false;
}

bool UpdateUserInformation(vector <stUsers> &vUser, string Username)
{
    stUsers User;

    if(FindUsertByUserName(User,vUser,Username))
    {
        PrintUserInfo(User);

        if (YouWantToUpdateUser())
        {
            for (stUsers &u : vUser)
            {
                if (u.Name == Username)
                {
                    u = ChangeUserInfo(Username);
                    break;
                }
            }

            SaveUserDataToFile("Users.txt",vUser);

            cout << "\n\nUser Updated Successfully" << endl;

            return true;
        }
    }
    else
    {
        cout << "User with Username " << (Username) <<  " not found" << endl;
    }
    return false;
}

void UpdateClientInfoScreen()
{
    if (!CheckUserAccessPermissions(enMainMenuePermissions::pUpdateClients))
    {
        ShowAccessDeniedScreen();
        return;
    }

    cout << "\n\n-----------------------------------\n";
    cout << "     Update Client Info Screen\n";
    cout << "-----------------------------------\n" << endl;

    vector <sClient> vClient = LoadClientsDataFromFile("Clients.txt");
    string AcountNumber = ReadAcountNumber();

    UpdateClientInformation(vClient,AcountNumber);
}

void UpdateUserInfoScreen()
{
    cout << "\n\n-----------------------------------\n";
    cout << "     Update User Info Screen\n";
    cout << "-----------------------------------\n" << endl;

    vector <stUsers> vUser = LoadUsersDataFromFile("Users.txt");
    string Username = ReadUsername();

    UpdateUserInformation(vUser,Username);
}

void FindClientScreen()
{
    if (!CheckUserAccessPermissions(enMainMenuePermissions::pFindClient))
    {
        ShowAccessDeniedScreen();
        return;
    }

    sClient Client;
    vector <sClient> vClient = LoadClientsDataFromFile(FileName);

    cout << "\n\n-----------------------------------\n";
    cout << "     Find Client Screen\n";
    cout << "-----------------------------------\n" << endl;

    string AcountNumber = ReadAcountNumber();

    if (FindClientByAcountNumber(Client,vClient,AcountNumber))
    {
        PrintClientsDetails(Client);
    }
    else
    {
        cout << "Client with Acount Number " << (AcountNumber) <<  " not found" << endl;
    }
}

void FindUserScreen()
{
    stUsers User;
    vector <stUsers> vUser = LoadUsersDataFromFile("Users.txt");

    cout << "\n\n-----------------------------------\n";
    cout << "     Find User Screen\n";
    cout << "-----------------------------------\n" << endl;

    string Username = ReadUsername();

    if (FindUsertByUserName(User,vUser,Username))
    {
        PrintUsersDetails(User);
    }
    else
    {
        cout << "User with Username " << (Username) <<  " not found" << endl;
    }
}

bool FindUserByUserNameAndPassword(string Username,string Password,stUsers &CurrentUser)
{
    vector <stUsers> vUser = LoadUsersDataFromFile("Users.txt");

    for (stUsers &u : vUser)
    {
        if (u.Name == Username && u.PassWord == Password)
        {
            CurrentUser = u;
            return true;
        }
    }
    return false;
}

bool LoadUserInfo(string Username, string Password)
{
    if (FindUserByUserNameAndPassword(Username,Password,CurrentUser))
       return true;
    else
       return false;   
}

bool CheckUserAccessPermissions(enMainMenuePermissions Permisiions)
{
    if (CurrentUser.Permissions == enMainMenuePermissions::eAll)
    {
        return true;
    }
    if ((Permisiions & CurrentUser.Permissions) == Permisiions)
    {
        return true;
    }
    else
    return false;
}

void ProgramEndScreen()
{
    cout << "\n\n-----------------------------------\n";
    cout << "     Program Ends :-)\n";
    cout << "-----------------------------------\n" << endl;
}

short ReadMainMenueOption()
{
    short Choice;

    cout << "Choose what do you want to do ? [1 to 8]? ";
    cin >> Choice;

    return Choice;
}

short ReadTransactionMenueOption()
{
    short Choice;

    cout << "Choose what do you want to do ? [1 to 4]? ";
    cin >> Choice;

    return Choice;
}

short ReadUsersMenueOptions()
{
    short Choice;

    cout << "Choose what do you want to do ? [1 to 6]? ";
    cin >> Choice;

    return Choice;
}

bool Deposit(vector <sClient> &vClient, string AcountNumber)
{
    sClient Client;
    double DepositAmount;

    if (FindClientByAcountNumber(Client,vClient,AcountNumber))
    {
        PrintClientsDetails(Client);

        cout << "Enter deposit amount ?";
        cin >> DepositAmount;

        if (YouWantToPerfromTransaction())
        {
            for (sClient &c : vClient)
            {
                if (c.AcountNumber == AcountNumber)
                {
                    c.Balance += DepositAmount;
                    break;
                }
            }

            SaveClientDataToFile(FileName,vClient);

            cout << "\n\nTransaction done Successfully New Balance is : " << Client.Balance + DepositAmount << endl;

            return true;
        }
    }
    else
    {
        cout << "Client with Acount Number " << (AcountNumber) <<  " not found" << endl;
    }
    return false;
}

bool Withdraw(vector <sClient> &vClient, string AcountNumber)
{
    sClient Client;
    double Withdraw;

    if (FindClientByAcountNumber(Client,vClient,AcountNumber))
    {
        PrintClientsDetails(Client);

        cout << "Enter Withdraw amount ?";
        cin >> Withdraw;
        
        while (Withdraw > Client.Balance)
        {
        
            cout << "Amount Exceeds the balance, you can withraw up to : " << Client.Balance << endl;
            cout << "Pleas enter withdraw amount ? "; 
            cin >> Withdraw;
        } 
        

        if (YouWantToPerfromTransaction())
        {
            for (sClient &c : vClient)
            {
                if (c.AcountNumber == AcountNumber)
                {
                    c.Balance -= Withdraw;
                    break;
                }
            }

            SaveClientDataToFile(FileName,vClient);

            cout << "\n\nTransaction done Successfully New Balance is : " << Client.Balance - Withdraw << endl;

            return true;
        }
    }
    else
    {
        cout << "Client with Acount Number " << (AcountNumber) <<  " not found" << endl;
    }
    return false;
}

void PrintAllBalancesData() 
{    
    sClient bClient; 
    vector <sClient> vClient = LoadClientsDataFromFile(FileName);

    cout << "\n\t\t\t\t\tBalance List (" << vClient.size() << ") Client(s).";     
    cout << "\n_______________________________________________________";     
    cout << "_________________________________________\n" << endl;     
    cout << "| " << left << setw(15) << "Accout Number";     
    cout << "| " << left << setw(40) << "Client Name";     
    cout << "| " << left << setw(12) << "Balance";     
    cout << "\n_______________________________________________________";     
    cout << "_________________________________________\n" << endl; 

    for (sClient &Client : vClient)     
    {         
        PrintClientBalanceRecord(Client);
        bClient.Balance = bClient.Balance + Client.Balance;
        cout << endl;    
    } 

    cout << "\n_______________________________________________________";     
    cout << "_________________________________________\n" << endl; 

    cout << "\n\n\t\t\t    Total Balances = "  << bClient.Balance << "\n"; 
}

void ShowDepositScreen()
{
    cout << "\n\n-----------------------------------\n";
    cout << "          Deposit Screen\n";
    cout << "-----------------------------------\n" << endl;

    vector <sClient> vClient = LoadClientsDataFromFile("Clients.txt");
    string AcountNumber = ReadAcountNumber();

    Deposit(vClient,AcountNumber);
}

void ShowWithdrawScreen()
{
    cout << "\n\n-----------------------------------\n";
    cout << "          Withdraw Screen\n";
    cout << "-----------------------------------\n" << endl;

    vector <sClient> vClient = LoadClientsDataFromFile("Clients.txt");
    string AcountNumber = ReadAcountNumber();

    Withdraw(vClient,AcountNumber);
}

void PerfromMainMenue(enMainMenueOptions MainMenueOption)
{

    switch (MainMenueOption)
    {
    case enMainMenueOptions::ClientList:
    {
        PrintAllClientsData();
        break;
    }    
    case enMainMenueOptions::AddNewClient:
    {
        AddClientsScreen();
        break;
    }
    case enMainMenueOptions::DeleteClientsFromFile:
    {
        DeleteClientScreen();
        break;
    }
    case enMainMenueOptions::UpdateClient:
    {
        UpdateClientInfoScreen();
    }
    case enMainMenueOptions::FindClient:
    {
        FindClientScreen();
        break;
    }
    case enMainMenueOptions::eTransactions:
    {
        ShowTransactionsMenue();
        break;
    }
    case enMainMenueOptions::eManageUsers:
    {
        ManageUsersScreen();
        break;
    }
    case enMainMenueOptions::eLogOut:
    {
       ProgramEndScreen();
       break;
    }
    }
}    

void PerfromMangeUsersMenue(enManageUserMenueOptions UserMenueOption)
{

    switch (UserMenueOption)
    {
    case enManageUserMenueOptions::eUsersList:
        PrintAllUsersData();
        break;
    case enManageUserMenueOptions::eAddNewUser:
    {
        AddUsersScreen();
        break;
    }
    case enManageUserMenueOptions::eDeleteUser:
    {
        DeleteUserScreen();
        break;
    }
    case enManageUserMenueOptions::eUpdateUser:
    {
        UpdateUserInfoScreen();
        break;
    }
    case enManageUserMenueOptions::eFindUser:
    {
        FindUserScreen();
        break;
    }
    case enManageUserMenueOptions::Mange2MainMenue:
    {
        ShowMainMenue();
        break;
    }
    }
}

void PerfromTransactionsMenue(enTransactionsMenueOptions TransactionMenueOption)
{
    switch (TransactionMenueOption)
    {
    case enTransactionsMenueOptions::eDeposit:
    {
        ShowDepositScreen();
        break;
    }    
    case enTransactionsMenueOptions::eWithdraw:
    {
        ShowWithdrawScreen();
        break;
    }
    case enTransactionsMenueOptions::eTotalBalances:
    {
        PrintAllBalancesData();
        break;
    }
    case enTransactionsMenueOptions::eMainMenue:
    {
        ShowMainMenue();
        break;
    }
    }
}

void ShowTransactionsMenue()
{
    if (!CheckUserAccessPermissions(enMainMenuePermissions::pTranactions))
    {
        ShowAccessDeniedScreen();
        return;
    }

    cout << "\n\n==================================\n";
    cout << "       Transactions Menue Screen\n";
    cout << "==================================\n";
    cout << "[1] Deposit." << endl;
    cout << "[2] Withdraw." << endl;
    cout << "[3] Total Balances." << endl;
    cout << "[4] Main Menue." << endl;

    PerfromTransactionsMenue((enTransactionsMenueOptions)ReadTransactionMenueOption());

}

void ManageUsersScreen()
{
    if (!CheckUserAccessPermissions(enMainMenuePermissions::ManageUsers))
    {
        ShowAccessDeniedScreen();
        return;
    }

    cout << "\n\n==================================\n";
    cout << "       Manage Users Screen\n";
    cout << "==================================\n";
    cout << "[1] Show Users List." << endl;
    cout << "[2] Add New User." << endl;
    cout << "[3] Delete User." << endl;
    cout << "[4] Update User Info." << endl;
    cout << "[5] Find User." << endl;
    cout << "[6] Main Menue." << endl;
    cout << "==================================\n";
    PerfromMangeUsersMenue((enManageUserMenueOptions)ReadUsersMenueOptions());
}

void ShowMainMenue()
{
   
    cout << "\n\n==================================\n";
    cout << "       Main Menue Screen\n";
    cout << "==================================\n";
    cout << "[1] Show Client List." << endl;
    cout << "[2] Add New Client." << endl;
    cout << "[3] Delete Client." << endl;
    cout << "[4] Update Client Info." << endl;
    cout << "[5] Find Client." << endl;
    cout << "[6] Transactions." << endl;
    cout << "[7] Manage Users." << endl;
    cout << "[8] Log Out." << endl;
    cout << "==================================\n";

    PerfromMainMenue((enMainMenueOptions)ReadMainMenueOption());
}

void Login()
{
    bool LoginFailed = false;
    string Username,Password;

    cout << "\n\n==================================\n";
    cout << "         Login Screen\n";
    cout << "==================================\n";

    do 
    {
        if (LoginFailed)
        {
            cout << "Invalide Username/Password\n";
        }    

        cout << "Enter Uesrname : ";
        cin >> Username;

        cout << "Enter Password : ";
        cin >> Password;

        LoginFailed = !LoadUserInfo(Username,Password);

    } while (LoginFailed);

    ShowMainMenue();
}

int main()
{
  
    Login();

    return 0;
}