#include <iostream>
#include <map>
#include "../util/util_p.h"
#include "../util/constant.h"
#include "parameters.h"

using namespace std;

util_p util_prmts;
constant cnst_prmts;

/*INI DISCOS*/
map<string, string> parameters::param_mkdisk(vector<string> tokens){
    
    map<string, string> map_u;
    for (int i = 1; i < tokens.size(); i++)
    {
        vector<string> return_params = util_prmts.separateString(tokens[i]);
        if (return_params.size() == 2){
            string llave = util_prmts.toLowerString(return_params[0]);
            string valor = return_params[1];
            if (!(llave == "-comentario" || llave == "-size" || llave == "-fit" || llave == "-unit" || llave == "-path")){
                cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El parametro " << llave << " no es valido por lo tanto no se tomara en cuenta" << endl;
            }else{
                if (!map_u[llave].empty()){
                    cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El parametro " << llave << " esta siendo ingresado 2 veces por lo cual se tomara como valor el primer " << llave << " encontrado" << endl;
                }else{
                    map_u[llave] = valor;
                }
            }
        }else{
            cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El dato " << tokens[i] << " es incorrecto por lo tanto no se toma en cuenta" << endl;
        }
    }

    string comentario = map_u["-comentario"];
    int size = util_prmts.isNumber(map_u["-size"])? atoi(map_u["-size"].c_str()): -1;
    string fit = map_u["-fit"].empty() ? map_u["-fit"] = "FF": util_prmts.toUpperString(map_u["-fit"]);
    string unit = map_u["-unit"].empty() ? map_u["-unit"] = "m": util_prmts.toLowerString(map_u["-unit"]);
    string path = map_u["-path"];

    if (size != -1 && !path.empty()){   

        if (size <= 0){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El valor ingresado en el parametro SIZE debe de ser mayor a 0 " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if (!(fit == "BF" || fit == "FF" || fit == "WF")){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El valor ingresado en el parametro FIT es incorrecto " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(!(unit == "k" || unit == "m")){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El valor ingresado en el parametro UNIT es incorrecto " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(!util_prmts.check_correctExtFile(path)){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " La extension del archivo es incorrecto " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(util_prmts.check_existFile(path)){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El disco ya existe en la ruta: " << path << " " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }
    }else{
        cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " No ha ingresado algunos de los campos obligatorios (-size, -path) " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
        map_u.clear();
    }
    return map_u;
}

map<string, string> parameters::param_rmdisk(vector<string> tokens){
    
    map<string, string> map_u;
    for (int i = 1; i < tokens.size(); i++)
    {
        vector<string> return_params = util_prmts.separateString(tokens[i]);
        if (return_params.size() == 2)
        {
            string llave = util_prmts.toLowerString(return_params[0]);
            string valor = return_params[1];
            if (!(llave == "-comentario" || llave == "-path")){
                cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El parametro " << llave << " no es valido por lo tanto no se tomara en cuenta" << endl;
            }else{
                if (!map_u[llave].empty()){
                    cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El parametro " << llave << " esta siendo ingresado 2 veces por lo cual se tomara como valor el primer " << llave << " encontrado" << endl;
                }else{
                    map_u[llave] = valor;
                }
            }
        }else{
            cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El dato " << tokens[i] << " es incorrecto por lo tanto no se toma en cuenta" << endl;
        }
    }
    
    string comentario = map_u["-comentario"];
    string path = map_u["-path"];

    if (!path.empty()){
        if(!util_prmts.check_correctExtFile(path)){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " La extension del archivo es incorrecto " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(!util_prmts.check_existFile(path)){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El disco no existe en la ruta: " << path << " " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }
    }else{
        cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " No ha ingresado algunos de los campos obligatorios (-path) " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
        map_u.clear();
    }
    return map_u;
}

/*FIN DISCOS*/

/*INI SCRIPT*/
map<string, string> parameters::param_exec(vector<string> tokens){
    
    map<string, string> map_u;
    for (int i = 1; i < tokens.size(); i++)
    {
        vector<string> return_params = util_prmts.separateString(tokens[i]);
        if (return_params.size() == 2){
            string llave = util_prmts.toLowerString(return_params[0]);
            string valor = return_params[1];
            if (!(llave == "-comentario" || llave == "-path"))
            {
                cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El parametro " << llave << " no es valido por lo tanto no se tomara en cuenta" << endl;
            }else{
                if (!map_u[llave].empty())
                {
                    cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El parametro " << llave << " esta siendo ingresado 2 veces por lo cual se tomara como valor el primer " << llave << " encontrado" << endl;
                }else{
                    map_u[llave] = valor;
                }
            }
        }else{
            cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El dato " << tokens[i] << " es incorrecto por lo tanto no se toma en cuenta" << endl;
        }
    }
    
    string comentario = map_u["-comentario"];
    string path = map_u["-path"];

    if (!path.empty()){
        if(!util_prmts.check_correctExtEXEC(path)){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " La extension del archivo es incorrecto " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(!util_prmts.check_existFile(path)){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El archivo no existe en la ruta: " << path << " " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }
    }else{
        cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " No ha ingresado algunos de los campos obligatorios (-path) " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
        map_u.clear();
    }
    return map_u;
}
/*FIN SCRIPT*/