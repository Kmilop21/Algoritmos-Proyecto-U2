#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
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

        GuardianTree() : Player(nullptr), root(nullptr){}

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
                    cerr << "Nivel de poder invalido: " << PowerLevel << " para guardian: " << name << endl;
                }
            }
            file.close();
        }
        void connectMaster()
        {
            for (Guardian* guardian : guardians) 
            {
                if(guardian != Player)
                {
                    Guardian* master = findGuardian(guardian->MainMaster);
                    if(master != nullptr)
                    {
                        master->apprentices.push_back(guardian);
                    }
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
            cout << "Elija el nombre para su guardian: " << endl;
            cin >> n;
            cout << "\n El nivel de poder de su guardian empezara en 50" << endl;
            cin.ignore();
            cout << "Elija la villa inicial para su guardian: " << endl;
            getline(cin,V);
            
            while(!findVillage(V))
            {
                cout << "Villa no existe, por favor intente nuevamente: ";
                getline(cin,V);
            }
            cout << "\n Elija el maestro para su guardian: " << endl;
            cin >> MM;

            Guardian* master = findGuardian(MM);
            while(master == nullptr)
            {
                cout << "El maestro no existe, por favor intente nuevamente: ";
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
        void MoldableInfo()
        {
            for(Guardian* guardian : guardians)
            {
                cout << "- " << guardian->name << " (Power level: " << guardian->PowerLevel << ", Village: " << guardian->Village << ")" << endl;
            }
        }
        void PlayerInfo()
        {
            cout << "- " << Player->name << ", nivel de poder: " << Player->PowerLevel << ",  villa: " << Player->Village << ", maestro principal: " << Player->MainMaster << endl;
        }
        void SelectGuardian()
        {
            string name;
            cout << "\n Escriba el nombre del guardian que desea: ";
            cin >> name;
            Player = findGuardian(name);
            while(Player == nullptr || Player->name == "Stormheart")
            {
                cout << "El guardian no existe o es Stormheart, por favor intente nuevamente: ";
                cin >> name;
                Player = findGuardian(name);
            }
            Player->PowerLevel = 50;
        }
        void FindInVillage(string& name)
        {
            enVilla.clear();
            cout << "\n\nGuardianes en la villa: ";
            for(Guardian* guardian : Moldable)
            {
                if(guardian->Village == name && guardian!=Player)
                {
                    enVilla.push_back(guardian);
                }    
            }
            for(Guardian* in : enVilla)
            {
                cout << in->name << ", ";
            }
        }
        Guardian* FindLowestPW()
        {
            Guardian* LowestPWGuardian = nullptr;
            int LowestPW = std::numeric_limits<int>::max();

            for(Guardian* guardian : enVilla)
            {
                if(guardian->PowerLevel < LowestPW)
                {
                    LowestPW = guardian->PowerLevel;
                    LowestPWGuardian = guardian;
                }
            }
            return LowestPWGuardian;
        }
        Guardian* findGuardian(const string& name) {
            for (Guardian* guardian : guardians) {
                if (guardian->name == name) {
                    return guardian;
                }
            }
            return nullptr;
        }
        bool Fight(Guardian* Rival)
        {
            cout << "\nNivel del poder de: " << Rival->name << " -> " << Rival->PowerLevel;

            int randNum = rand()%2+1;
            int PWLV = Player->PowerLevel;

            cout << "\nResultado de numero generado: " << randNum;

            switch (randNum)
            {
            case 1:
                PWLV = static_cast<int>(PWLV * 1.6);
                break;
            case 2:
                PWLV = static_cast<int>(PWLV * 1.7);
                break;
            default:
                break;
            }

            cout << "\nNivdel de poder aumentado temporalmente a: " << PWLV;

            if(PWLV > Rival->PowerLevel)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        Guardian* FindMaster()
        {
            Guardian* Master = nullptr;
            bool isMaster = true;

            for(const auto& guardian : enVilla)
            {
                for(const auto& otherguardian : enVilla)
                {
                    if(guardian != otherguardian && guardian->MainMaster < otherguardian->MainMaster)
                    {
                        isMaster = false;
                        break;
                    }
                }

                if(isMaster)
                {
                    Master = guardian;
                    break;
                }
                isMaster = true;
            }
            return Master;
        }
        void SaveTravelHistory(const string& name)
        {
            TravelHistory.push_back(name);
        }
        void PrintTravelHistory()
        {
            cout << "Lugares visitados: " << endl;
            for(const auto& visited : TravelHistory)
            {
                cout << " -" << visited << endl;
            }
        }
        void SaveTrainingHistory(const string& text)
        {
            TrainingHistory.push_back(text);
        }
        void PrintTrainingHistory()
        {
            cout << "Resultado de los entrenamientos: " << endl;
            for(const auto& result : TrainingHistory)
            {
                cout << " -" << result << endl;
            }
        }
        
        Guardian* Player;
        bool Victoria = false;

    private:
        vector<Guardian*> guardians;
        vector<Guardian*> Moldable;
        vector<Guardian*> enVilla;
        vector<string> TravelHistory;
        vector<string> TrainingHistory;
        Guardian* root;


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
                cout << string(static_cast<std::string::size_type>(indent),' ') << "- " << guardian->name << "(Nivel de poder: " << guardian->PowerLevel << ", Villa: " << guardian->Village << ")" << endl; 
                
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
                cout << "- " << village->Name << " vecinos: ";
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
                    cout << "Vecinos: " << currentVillage->neighbours[i]->Name << ", "; 
                }
                cout << currentVillage->neighbours.back()->Name;
            }
        }
        bool ExistNeighbour(string& dest, string& currt)
        {
            Village* current = FindVillage(currt);
            for(Village* neighbour : current->neighbours)
            {
                if(neighbour->Name == dest)
                {
                    return true;
                }
            }
            return false;
        }
        void RemoveXP(int n, const string& name)
        {
            Village* village = FindVillage(name);
            village->XP -= n;
        }
        bool HasXP(const string& name, int n)
        {
            Village* village = FindVillage(name);
            if(village->XP >= n)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        void PrintXP(const string& name)
        {
            Village* village = FindVillage(name);
            cout << "\nExperiencia disponible: " << village->XP;
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
    std::srand(static_cast<unsigned int>(std::time(nullptr)));


    int o = -1, action = -1;

    while(o!=0)
    {
        GuardianTree tree;
        tree.LoadGuardianFile("Guardianes.txt");  
        tree.connectMaster();
        //tree.Info();

        Villages map;
        map.LoadVillageFile("Aldeas.txt");

        cout << "Bienvenido, presione 0 para finalizar, 1 para elegir su personaje y 2 para crear a un personaje: ";
        cin >> o;
        while(o < -1 || o > 2)
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
        else if(o==0)
        {
            cout << "\nFinalizando..." << endl;
        }

        if(o!=0)
        {

            tree.SaveTravelHistory(tree.Player->Village);

            while(action!=0 && tree.Victoria == false)
            {
                cout << "\n";
                tree.PlayerInfo();
                cout << "\n";

                cout << "\nPresione 1 para viajar, 2 para entrenar, 3 para realizar alquimia o 0 para finalizar: ";
                cin >> action;

                while(action<0 || action >3)
                {
                    cout << "Ingreso incorrecto, intentelo nuevamente: ";
                    cin >> action;
                }

                if(action == 1)//viajar
                {
                    map.CurrentInfo(tree.Player->Village);
                    string destino;
                    cout << "\nIngrese la villa de destino: ";
                    cin.ignore();
                    getline(cin,destino);
                    while(!map.ExistNeighbour(destino,tree.Player->Village))
                    {
                        cout << "\nLa villa ingresada no existe o no es vecina, ingrese nuevamente: ";
                        getline(cin,destino);
                    }
                    tree.Player->Village = destino;
                    tree.SaveTravelHistory(tree.Player->Village);
                }
                else if(action == 2)//Entrenar
                {
                    if(tree.Player->Village == "Tesla")
                    {
                        cout << "\n\nSe encuentra en Tesla, por lo que solo puede enfrentar a Stormhear" << endl;
                        cout << "¿Desea enfrentar a Stormhear? Ingrese S para aceptar y N para declinar" << endl;

                        string respuesta;

                        cin.ignore();
                        getline(cin,respuesta);

                        if(respuesta == "S")
                        {
                            Guardian* Rival = tree.findGuardian("Stormheart");

                            if(tree.Fight(Rival))
                            {
                                cout << "\n******El jugador ha superado el entrenamiento contra: " << Rival->name << " ******" << endl;
                                string text = "******El jugador ha superado el entrenamiento contra: " + Rival->name + " ******";
                                tree.SaveTrainingHistory(text);
                                tree.Victoria = true;
                            }
                            else{
                                cout << "\nEl jugador no ha superado el entrenamiento contra: " << Rival->name << endl;
                                string text = "El jugador no ha superado el entrenamiento contra: " + Rival->name;
                                tree.SaveTrainingHistory(text);
                                action = 0;
                            }
                        }
                        else if(respuesta == "N")
                        {
                            cout << "\nVuelva cuando se sienta listo/lista";
                        }
                    }
                    else
                    {
                        if(map.HasXP(tree.Player->Village, 1) == true)
                        {
                            map.PrintXP(tree.Player->Village);
                            tree.FindInVillage(tree.Player->Village);
                            Guardian* Lowest = tree.FindLowestPW();
                            cout << "\nGuardian de nivel minimo (recomendado): " << Lowest->name;
                            Guardian* Master = tree.FindMaster();
                            cout << "\nMaestro de la villa: " << Master->name;

                            cout << "\nEscriba el nombre del guardian con el que le gustaria entrenar: ";
                            string target;
                            cin.ignore();
                            getline(cin,target);
                            Guardian* Rival = tree.findGuardian(target);
                            if(Rival == Master)
                            {
                                while(map.HasXP(tree.Player->Village, 2) == false)
                                {
                                    cout << "\nNo hay suficiente experiencia para enfrentarlo, intentelo nuevamente: ";
                                    getline(cin,target);
                                    Rival = tree.findGuardian(target);
                                }
                            }
                            while(Rival == nullptr)
                            {
                                cout << "\nEl guardian ingresado no existe, intentelo nuevamente: ";
                                getline(cin,target);
                                Rival = tree.findGuardian(target);
                            }

                            if(tree.Fight(Rival))
                            {
                                cout << "\nEl jugador ha superado el entrenamiento contra: " << Rival->name << endl;
                                string text = "El jugador ha superado el entrenamiento contra: " + Rival->name;
                                tree.SaveTrainingHistory(text);
                                if(Rival == Master)
                                {
                                    tree.Player->PowerLevel+=2;
                                    map.RemoveXP(2,tree.Player->Village);
                                }
                                else
                                {
                                    tree.Player->PowerLevel+=1;
                                    map.RemoveXP(1,tree.Player->Village);
                                }
                            }
                            else{
                                cout << "\nEl jugador no ha superado el entrenamiento contra: " << Rival->name << endl;
                                string text = "El jugador no ha superado el entrenamiento contra: " + Rival->name;
                                tree.SaveTrainingHistory(text);
                                action = 0;
                            }
                        }
                        else
                        {
                            cout << "\nNo hay mas puntos por obtener en esta villa" << endl;
                        }
                    }

                }
                else if(action == 3)//Alquimia
                {
                    int r = rand()&3+2;
                    cout << "Elija la villa a la que quiera moverse: ";
                    string name;
                    cin.ignore();
                    getline(cin,name);
                    tree.Player->Village = name;
                    tree.Player->PowerLevel -= r;
                    cout << "Moviendose a la villa: " << tree.Player->Village << " por el costo de: " << r << " puntos de poder";
                }

            }
            
            tree.PrintTravelHistory();
            cout << "\n\n";
            tree.PrintTrainingHistory();
            
        }
    }


    return 0;
}