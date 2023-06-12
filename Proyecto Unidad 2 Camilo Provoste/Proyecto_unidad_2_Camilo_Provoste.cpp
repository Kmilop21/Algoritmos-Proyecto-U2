#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

struct Guardian
{   
    string name;
    int PowerLevel;
    string MainMaster;
    string Village;
    vector<Guardian*> apprentices;
};

class GuardianTree
{
    public:

        GuardianTree() : root(nullptr){}

        void InsertGuardian(const string n, const string PW, const string MM, const string V)
        {
            Guardian* guardian = new Guardian;
            guardian->name = n;
            guardian->PowerLevel = stoi(PW);
            guardian->MainMaster = MM;
            guardian->Village = V;
            guardians.push_back(guardian); 

            if(root == nullptr)
            {
                 root = guardian;
            }


        }   
        void LoadGuardianFile(const string& FileName)
        {
            ifstream file(FileName); 
            if(!file)
            {
                cerr << "Failed to open file " << FileName << endl;
                return; 
            }  
            string line;
            getline(file,line);
            while(getline(file,line))
            {
                istringstream iss(line);
                string name, MainMaster, Village;
                string PowerLevel;
                getline(iss, name, ',');
                getline(iss, PowerLevel, ',');
                getline(iss, MainMaster, ',');
                getline(iss, Village);
                
                try
                {
                    InsertGuardian(name,PowerLevel,MainMaster,Village);
                } catch(const std::invalid_argument& e)
                {
                    cerr << "Invalid Power level: " << PowerLevel << " for guardian: " << name << endl;
                }
            }
            file.close();
        }
        void connectMaster()
        {
            for (Guardian* guardian : guardians) 
            {
                Guardian* master = findGuardian(guardian->MainMaster);
                if(master != nullptr)
                {
                    master->apprentices.push_back(guardian);
                }
            }

            for(Guardian* guardian : guardians)
            {
                if(guardian != Player)
                {
                    Moldable.push_back(guardian);
                }
            }

        }
        void CreateGuardian()
        {
            Guardian* guardian = new Guardian;

            string n, V, MM;
            cout << "Choose a name for your guardian: " << endl;
            cin >> n;
            cout << "\n Your guardian's power level will start at 50" << endl;
            cin.ignore();
            cout << "Choose the starting village for your Guardian: " << endl;
            getline(cin,V);
            
            while(!findVillage(V))
            {
                cout << "Village does not exist, please try again: ";
                getline(cin,V);
            }
            cout << "\n Choose the main master for your guardian: " << endl;
            cin >> MM;

            Guardian* master = findGuardian(MM);
            while(master == nullptr)
            {
                cout << "Master does not exist, please try again: ";
                cin >> MM;
                master = findGuardian(MM);
            }
            master->apprentices.push_back(guardian);


            guardian->name = n;
            guardian->PowerLevel = 50;
            guardian->Village = V;
            guardian->MainMaster = MM;
            guardians.push_back(guardian);

            Player = guardian;
        }
        void Info()
        {
            info(root,0);
        }
        void PlayerInfo()
        {
            cout << "- " << Player->name << ", power level: " << Player->PowerLevel << ",  village: " << Player->Village << ", main master: " << Player->MainMaster << endl;
        }
        void SelectGuardian()
        {
            string name;
            cout << "\n Escriba el nombre del guardian que desea: ";
            cin >> name;
            Player = findGuardian(name);
            while(Player == nullptr)
            {
                cout << "Guardian doesn't exist, please try again: ";
                cin >> name;
                Player = findGuardian(name);
            }
            Player->PowerLevel = 50;
        }

        Guardian* Player;

    private:
        vector<Guardian*> guardians;
        vector<Guardian*> Moldable;
        Guardian* root;

        Guardian* findGuardian(const string& name) {
            for (Guardian* guardian : guardians) {
                if (guardian->name == name) {
                    return guardian;
                }
            }
            return nullptr;
        }

        bool findVillage(const string& village)
        {
            for(Guardian* guardian : guardians)
            {
                if(guardian->Village == village)
                {
                    return true;
                }    
            }
            return false;
        }
        void info(Guardian* guardian, int indent)
        {
            if(guardian != nullptr)
            {
                cout << string(static_cast<std::string::size_type>(indent),' ') << "- " << guardian->name << "(Power level: " << guardian->PowerLevel << ", Village: " << guardian->Village << ")" << endl; 
                
                for(Guardian* apprentice : guardian->apprentices)
                {
                    info(apprentice, indent + 4);
                }
            }
        }
};

struct Village
{
    string Name;
    string ConnectedVillage;
    int XP = 4;
    vector<Village*> neighbours;
};

class Villages
{
    public:
        void addVillage(const string& V, const string& CV)
        {
            Village* village = FindVillage(V);
            Village* connectedVillage = FindVillage(CV);
            
            if (village == nullptr)
            {
                village = new Village;
                village->Name = V;
                Villages.push_back(village);
            }

            if (connectedVillage == nullptr)
            {
                connectedVillage = new Village;
                connectedVillage->Name = CV;
                Villages.push_back(connectedVillage);
            }

            village->neighbours.push_back(connectedVillage);
            connectedVillage->neighbours.push_back(village);

            if(root == nullptr)
            {
                root = village;
            }
        }
        void LoadVillageFile(const string& FileName)
        {
            ifstream file(FileName); 
            if(!file)
            {
                cerr << "Failed to open file " << FileName << endl;
                return; 
            }  
            string line;
            getline(file,line);
            while(getline(file,line))
            {
                istringstream iss(line);
                string village, ConnectedVillage;
                getline(iss, village, ',');
                getline(iss, ConnectedVillage);
                addVillage(village,ConnectedVillage);
            }
            file.close();
        }
        void AllInfo()
        {
            for(Village* village : Villages)
            {
                cout << "- " << village->Name << " neighbours: ";
                if(!village->neighbours.empty())
                {
                    for(size_t i = 0; i < village->neighbours.size()-1 ; i++)
                    {
                        cout << village->neighbours[i]->Name << ", "; 
                    }
                    cout << village->neighbours.back()->Name;
                }
                cout << endl;
            }
        }
        void CurrentInfo(const string& name)
        {
            Village* currentVillage = FindVillage(name);

            cout << "Villa: " << currentVillage->Name << endl;
            if(!currentVillage->neighbours.empty())
            {
                for(size_t i = 0; i < currentVillage->neighbours.size()-1 ; i++)
                {
                    cout << "Neighbours: " << currentVillage->neighbours[i]->Name << ", "; 
                }
                cout << currentVillage->neighbours.back()->Name;
            }
        }
    private:
        vector<Village*> Villages;
        Village* root;

        Village* FindVillage(const string& name) {
            for (Village* village : Villages) {
                if (village->Name == name) {
                    return village;
                }
            }
            return nullptr;
        }


};

int main()
{
    //cout << "\n\nGuardian: \n\n";

    GuardianTree tree;
    tree.LoadGuardianFile("Guardianes.txt");  
    //tree.CreateGuardian(); 
    tree.connectMaster();
    //tree.Info();

    //cout << "\n\nVillages: \n\n";

    Villages map;
    map.LoadVillageFile("Aldeas.txt");
    //map.AllInfo();

    int o = -1;

    while(o!=0)
    {
        cout << "Bienvenido, presione 0 para finalizar, 1 para elegir su personaje y 2 para crear a un personaje: ";
        cin >> o;
        while(o < -1 && o > 3)
        {
            cout << "Ingreso incorrecto, debe igresar un numero de 0 a 3: ";
            cin >> o;
        }
        if(o==1)
        {
            tree.Info();
            tree.SelectGuardian();
        }
        else if(o==2)
        {
            tree.CreateGuardian();
        }
        
        tree.PlayerInfo();
        map.CurrentInfo(tree.Player->Village);

    }


    return 0;
}