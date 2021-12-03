#include <iostream>
#include <fstream>
#include <map>
#include <cstring>
#include "../util/util_p.h"
#include "../util/constant.h"
#include "script_adm.h"
#include "../analizador/scanner.h"

using namespace std;

util_p util_scrpt;
constant csnt_scrpt;
scanner scnr_scrpt;

void script_adm::exec(map<string, string> param_got){
    if (param_got.size() == 0){
        return;
    }
    
    /*Obteniendo Datos*/
    string path = param_got["-path"].c_str();
    
    /*Formateo de Datos*/
    path = (path.substr(0,1) == "\"") ? path.substr(1, path.size()-2): path;
    
    /*Flujo del void*/
    FILE *file;
    file = fopen(path.c_str(), "r");
    fclose(file);
    string command;
    ifstream datos(path.c_str());
    while(!datos.eof()){
        getline(datos, command); //con esta funcion se obtiene la linea(limitada por \n)
        if (!command.empty())
        {
            if (util_scrpt.toLowerString(command) == "pause")
            {
                cout << "Presione el boton enter para continuar..." << endl;
                cin.ignore();
            }else{
                vector<string> spliting = scnr_scrpt.tokens(command);
                scnr_scrpt.exec_command(spliting);
            }
        }
    }    
}
