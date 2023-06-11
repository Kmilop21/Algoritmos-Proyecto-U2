#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include <sstream>
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
            else
            {
                Guardian* master = findGuardian(MM);
                if(master != nullptr)
                {
                    master->apprentices.push_back(guardian);
                }
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

        }
        void Info()
        {
            info(root,0);
        }

    private:
        vector<Guardian*> guardians;
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
    string Village;
    string ConnectedVillage;
};

class Villages
{
    public:
        void addVillage(string V, string CV)
        {
            Village* village = new Village;
            village->Village = V;
            village->ConnectedVillage = CV;
            Neighbors.push_back(village);
        }
    private:
        vector<Village*> Neighbors;

};

int main()
{
    GuardianTree tree;
    tree.LoadGuardianFile("Guardianes.txt");  
    tree.CreateGuardian(); 
    tree.Info();

    return 0;
}