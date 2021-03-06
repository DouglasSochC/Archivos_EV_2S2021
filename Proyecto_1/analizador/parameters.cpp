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
    for (int i = 1; i < tokens.size(); i++){
        vector<string> return_params = util_prmts.separateString(tokens[i]);
        if (return_params.size() == 2){
            string llave = util_prmts.toLowerString(return_params[0]);
            string valor = return_params[1];
            if (!(llave == "-comentario" || llave == "-size" || llave == "-fit" || llave == "-unit" || llave == "-path")){
                cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << llave << " no es valido para el comando ejecutado" << endl;
                map_u.clear();
                return map_u;
            }else{
                if (!map_u[llave].empty()){
                    cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El parametro " << llave << " esta siendo ingresado 2 veces por lo cual se tomara como valor el primer " << llave << " encontrado" << endl;
                }else{
                    map_u[llave] = valor;
                }
            }
        }else{
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << tokens[i] << " no es valido para el comando ejecutado" << endl;
            map_u.clear();
            return map_u;
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
    for (int i = 1; i < tokens.size(); i++){
        vector<string> return_params = util_prmts.separateString(tokens[i]);
        if (return_params.size() == 2){
            string llave = util_prmts.toLowerString(return_params[0]);
            string valor = return_params[1];
            if (!(llave == "-comentario" || llave == "-path")){
                cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << llave << " no es valido para el comando ejecutado" << endl;
                map_u.clear();
                return map_u;
            }else{
                if (!map_u[llave].empty()){
                    cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El parametro " << llave << " esta siendo ingresado 2 veces por lo cual se tomara como valor el primer " << llave << " encontrado" << endl;
                }else{
                    map_u[llave] = valor;
                }
            }
        }else{
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << tokens[i] << " no es valido para el comando ejecutado" << endl;
            map_u.clear();
            return map_u;
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

map<string, string> parameters::param_fdisk(vector<string> tokens){
    
    map<string, string> map_u;
    for (int i = 1; i < tokens.size(); i++)
    {
        vector<string> return_params = util_prmts.separateString(tokens[i]);
        if (return_params.size() == 2){
            string llave = util_prmts.toLowerString(return_params[0]);
            string valor = return_params[1];
            if (!(llave == "-comentario" || llave == "-size" || llave == "-unit" || llave == "-path" || llave == "-type" || llave == "-fit" || llave == "-delete" || llave == "-name" || llave == "-add")){
                cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << llave << " no es valido para el comando ejecutado" << endl;
                map_u.clear();
                return map_u;
            }else{
                if (!map_u[llave].empty())
                {
                    cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El parametro " << llave << " esta siendo ingresado 2 veces por lo cual se tomara como valor el primer " << llave << " encontrado" << endl;
                }else{
                    map_u[llave] = valor;
                }
            }
        }else{
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << tokens[i] << " no es valido para el comando ejecutado" << endl;
            map_u.clear();
            return map_u;
        }
    }

    string comentario = map_u["-comentario"];
    string size = map_u["-size"];
    string unit = map_u["-unit"].empty() ? map_u["-unit"] = "K": util_prmts.toUpperString(map_u["-unit"]);
    string path = map_u["-path"];
    string type = map_u["-type"].empty() ? map_u["-type"] = "P": util_prmts.toUpperString(map_u["-type"]);
    string fit = map_u["-fit"].empty() ? map_u["-fit"] = "WF": util_prmts.toUpperString(map_u["-fit"]);    
    string delete_p = map_u["-delete"] = util_prmts.toUpperString(map_u["-delete"]);
    string name = map_u["-name"];
    string add_p = map_u["-add"];
    
    if (!path.empty() && !name.empty()){
        if (delete_p.empty() && add_p.empty() && size.empty()){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro SIZE es obligatorio para crear una particion " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if (!size.empty() && !util_prmts.isNumber(size)){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro SIZE debe de ser numerico " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if (!size.empty() && delete_p.empty() && add_p.empty() && atoi(size.c_str()) <= 0){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro SIZE debe de ser mayor a 0 " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(name.size() > 16){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro NAME tiene mas de 16 caracteres " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if (!(unit == "B" || unit == "K" || unit == "M")){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El valor ingresado en el parametro UNIT es incorrecto " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(!util_prmts.check_correctExtFile(path)){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " La extension del archivo es incorrecto " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(!util_prmts.check_existFile(path)){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El disco no existe en la ruta: " << path << " " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(!(type == "P" || type == "E" || type == "L")){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El valor ingresado en el parametro TYPE es incorrecto" << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(!(fit == "BF" || fit == "FF" || fit == "WF")){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El valor ingresado en el parametro FIT es incorrecto" << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(!delete_p.empty() && !(delete_p == "FAST" || delete_p == "FULL")){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El valor ingresado en el parametro DELETE es incorrecto" << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(!delete_p.empty() && !add_p.empty()){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " No se puede hacer un DELETE y un ADD al mismo tiempo" << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }
    }else{
        cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " No ha ingresado algunos de los campos obligatorios (-name, -path)" << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
        map_u.clear();
    }
    return map_u;
}

map<string, string> parameters::param_mount(vector<string> tokens){
    
    map<string, string> map_u;
    for (int i = 1; i < tokens.size(); i++)
    {
        vector<string> return_params = util_prmts.separateString(tokens[i]);
        if (return_params.size() == 2)
        {
            string llave = util_prmts.toLowerString(return_params[0]);
            string valor = return_params[1];
            if (!(llave == "-comentario" || llave == "-path" || llave == "-name")){
                cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << llave << " no es valido para el comando ejecutado" << endl;
                map_u.clear();
                return map_u;
            }else{
                if (!map_u[llave].empty()){
                    cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El parametro " << llave << " esta siendo ingresado 2 veces por lo cual se tomara como valor el primer " << llave << " encontrado" << endl;
                }else{
                    map_u[llave] = valor;
                }
            }
        }else{
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << tokens[i] << " no es valido para el comando ejecutado" << endl;
            map_u.clear();
            return map_u;
        }
    }
    string comentario = map_u["-comentario"];
    string path = map_u["-path"];
    string name = map_u["-name"];

    if (!name.empty() && !path.empty())
    {
        if(!util_prmts.check_correctExtFile(path)){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " La extension del archivo es incorrecto " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(!util_prmts.check_existFile(path)){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El disco no existe en la ruta: " << path << " " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }
    }else{
        cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " No ha ingresado algunos de los campos obligatorios (-name, -path) " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
        map_u.clear();
    }
    return map_u;
}

map<string, string> parameters::param_unmount(vector<string> tokens){
    
    map<string, string> map_u;
    for (int i = 1; i < tokens.size(); i++)
    {
        vector<string> return_params = util_prmts.separateString(tokens[i]);
        if (return_params.size() == 2)
        {
            string llave = util_prmts.toLowerString(return_params[0]);
            string valor = return_params[1];
            if (!(llave == "-comentario" || llave == "-id")){
                cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << llave << " no es valido para el comando ejecutado" << endl;
                map_u.clear();
                return map_u;
            }else{
                if (!map_u[llave].empty())
                {
                    cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El parametro " << llave << " esta siendo ingresado 2 veces por lo cual se tomara como valor el primer " << llave << " encontrado" << endl;
                }else{
                    map_u[llave] = valor;
                }
            }
        }else{
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << tokens[i] << " no es valido para el comando ejecutado" << endl;
            map_u.clear();
            return map_u;
        }
    }

    string comentario = map_u["-comentario"];
    string id = map_u["-id"];
    if (id.empty()){
        cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " No ha ingresado algunos de los campos obligatorios (-id) " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
        map_u.clear();
    }
    return map_u;
}

map<string, string> parameters::param_mkfs(vector<string> tokens){
    
    map<string, string> map_u;
    for (int i = 1; i < tokens.size(); i++){
        vector<string> return_params = util_prmts.separateString(tokens[i]);
        if (return_params.size() == 2){
            string llave = util_prmts.toLowerString(return_params[0]);
            string valor = return_params[1];
            if (!(llave == "-comentario" || llave == "-id" || llave == "-type" || llave == "-fs")){
                cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << llave << " no es valido para el comando ejecutado" << endl;
                map_u.clear();
                return map_u;
            }else{
                if (!map_u[llave].empty()){
                    cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El parametro " << llave << " esta siendo ingresado 2 veces por lo cual se tomara como valor el primer " << llave << " encontrado" << endl;
                }else{
                    map_u[llave] = valor;
                }
            }
        }else{
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << tokens[i] << " no es valido para el comando ejecutado" << endl;
            map_u.clear();
            return map_u;
        }
    }
    string comentario = map_u["-comentario"];
    string id = map_u["-id"];
    string type = (map_u["-type"].empty()) ? map_u["-type"] = "full": util_prmts.toLowerString(map_u["-type"]);
    string fs = (map_u["-fs"].empty()) ? map_u["-fs"] = "2fs": util_prmts.toLowerString(map_u["-fs"]);

    if (!id.empty()){
        if(!(type == "full" || type == "fast")){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El valor ingresado en el parametro TYPE es incorrecto " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(!(fs == "2fs" || fs == "3fs")){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El valor ingresado en el parametro FS es incorrecto " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }
    }else{
        cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " No ha ingresado algunos de los campos obligatorios (-id) " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
        map_u.clear();
    }
    return map_u;
}
/*FIN DISCOS*/

/*INI CAP*/

map<string, string> parameters::param_ren(vector<string> tokens){
    
    map<string, string> map_u;
    for (int i = 1; i < tokens.size(); i++){
        vector<string> return_params = util_prmts.separateString(tokens[i]);
        if (return_params.size() == 2){
            string llave = util_prmts.toLowerString(return_params[0]);
            string valor = return_params[1];
            if (!(llave == "-comentario" || llave == "-path" || llave == "-name")){
                cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << llave << " no es valido para el comando ejecutado" << endl;
                map_u.clear();
                return map_u;
            }else{
                if (!map_u[llave].empty()){
                    cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El parametro " << llave << " esta siendo ingresado 2 veces por lo cual se tomara como valor el primer " << llave << " encontrado" << endl;
                }else{
                    map_u[llave] = valor;
                }
            }
        }else{
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << tokens[i] << " no es valido para el comando ejecutado" << endl;
            map_u.clear();
            return map_u;
        }
    }
    
    string comentario = map_u["-comentario"];
    string path = map_u["-path"];
    string name = map_u["-name"];
    //Formateo del name
    name = (name.substr(0,1) == "\"") ? name.substr(1, name.size()-2): name;

    if (!path.empty() && !name.empty()){
        if (name.length() > 10){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " No se puede ingresar el grupo debido a que excede la cantidad de 10 caracteres " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }
    }else{
        cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " No ha ingresado algunos de los campos obligatorios (-path, -name) " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
        map_u.clear();
    }
    return map_u;
}

map<string, string> parameters::param_mkdir(vector<string> tokens){
    
    map<string, string> map_u;
    for (int i = 1; i < tokens.size(); i++){
        vector<string> return_params = util_prmts.separateString(tokens[i]);
        
        //Se verifica que el parametro -p no tenga una asignacion
        if(return_params.size() == 0 && util_prmts.toLowerString(tokens[i]) == "-p"){
            return_params.push_back("-p");
            return_params.push_back("");
        }

        if (return_params.size() == 2){
            string llave = util_prmts.toLowerString(return_params[0]);
            string valor = return_params[1];
            if (!(llave == "-comentario" || llave == "-path" || llave == "-p")){
                cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << llave << " no es valido para el comando ejecutado" << endl;
                map_u.clear();
                return map_u;
            }else{
                if (!map_u[llave].empty()){
                    cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El parametro " << llave << " esta siendo ingresado 2 veces por lo cual se tomara como valor el primer " << llave << " encontrado" << endl;
                }else{
                    map_u[llave] = valor;
                }
            }
        }else{
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << tokens[i] << " no es valido para el comando ejecutado" << endl;
            map_u.clear();
            return map_u;
        }
    }

    string comentario = map_u["-comentario"];
    string path = map_u["-path"];
    string var_p;
    if (map_u.find("-p") == map_u.end()) {
        //NO EXISTE LA LLAVE
    } else {
        var_p = map_u["-p"];        
    }
    if (!path.empty()){
        if(!var_p.empty()){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " Al parametro -P no se le debe asignar un valor " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }
    }else{
        cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " No ha ingresado algunos de los campos obligatorios (-path) " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
        map_u.clear();
    }
    return map_u;
}

map<string, string> parameters::param_cat(vector<string> tokens){
    
     map<string, string> map_u;
    for (int i = 1; i < tokens.size(); i++){
        vector<string> return_params = util_prmts.separateString(tokens[i]);
        if (return_params.size() == 2){
            string llave = util_prmts.toLowerString(return_params[0]);
            string valor = return_params[1];
            if (!(llave == "-comentario" || llave == "-file1")){
                cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << llave << " no es valido para el comando ejecutado" << endl;
                map_u.clear();
                return map_u;
            }else{
                if (!map_u[llave].empty()){
                    cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El parametro " << llave << " esta siendo ingresado 2 veces por lo cual se tomara como valor el primer " << llave << " encontrado" << endl;
                }else{
                    map_u[llave] = valor;
                }
            }
        }else{
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << tokens[i] << " no es valido para el comando ejecutado" << endl;
            map_u.clear();
            return map_u;
        }
    }
    
    string comentario = map_u["-comentario"];
    string path = map_u["-file1"];

    if (!path.empty()){
        //NO SE VERIFICA NADA
    }else{
        cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " No ha ingresado algunos de los campos obligatorios (-path) " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
        map_u.clear();
    }
    return map_u;
}

map<string, string> parameters::param_mkfile(vector<string> tokens){
    
    map<string, string> map_u;
    for (int i = 1; i < tokens.size(); i++){
        vector<string> return_params = util_prmts.separateString(tokens[i]);
        
        //Se verifica que el parametro -p no tenga una asignacion
        if(return_params.size() == 0 && util_prmts.toLowerString(tokens[i]) == "-p"){
            return_params.push_back("-p");
            return_params.push_back("");
        }

        if (return_params.size() == 2){
            string llave = util_prmts.toLowerString(return_params[0]);
            string valor = return_params[1];
            if (!(llave == "-comentario" || llave == "-path" || llave == "-p" || llave == "-size" || llave == "-cont")){
                cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << llave << " no es valido para el comando ejecutado" << endl;
                map_u.clear();
                return map_u;
            }else{
                if (!map_u[llave].empty()){
                    cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El parametro " << llave << " esta siendo ingresado 2 veces por lo cual se tomara como valor el primer " << llave << " encontrado" << endl;
                }else{
                    map_u[llave] = valor;
                }
            }
        }else{
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << tokens[i] << " no es valido para el comando ejecutado" << endl;
            map_u.clear();
            return map_u;
        }
    }

    string comentario = map_u["-comentario"];
    string path = map_u["-path"];
    string size = map_u["-size"];
    string cont = map_u["-cont"];
    string var_p;
    if (map_u.find("-p") == map_u.end()) {
        //NO EXISTE LA LLAVE
    } else {
        var_p = map_u["-p"];        
    }
    if (!path.empty()){
        if(!var_p.empty()){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " Al parametro -P no se le debe asignar un valor " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(!util_prmts.isNumber(size)){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro -SIZE debe de ser un valor numerico " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(atoi(size.c_str()) < 0 && cont.empty()){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro -SIZE debe de ser mayor o igual a 0 " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(!cont.empty() && !util_prmts.check_existFile(cont)){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El path ingresado en el parametro -CONT no existe " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }
    }else{
        cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " No ha ingresado algunos de los campos obligatorios (-path) " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
        map_u.clear();
    }
    return map_u;
}

/*FIN CAP*/

/*INI UG*/

map<string, string> parameters::param_login(vector<string> tokens){
    
    map<string, string> map_u;
    for (int i = 1; i < tokens.size(); i++){
        vector<string> return_params = util_prmts.separateString(tokens[i]);
        if (return_params.size() == 2){
            string llave = util_prmts.toLowerString(return_params[0]);
            string valor = return_params[1];
            if (!(llave == "-comentario" || llave == "-usr" || llave == "-pwd" || llave == "-id")){
                cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << llave << " no es valido para el comando ejecutado" << endl;
                map_u.clear();
                return map_u;
            }else{
                if (!map_u[llave].empty()){
                    cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El parametro " << llave << " esta siendo ingresado 2 veces por lo cual se tomara como valor el primer " << llave << " encontrado" << endl;
                }else{
                    map_u[llave] = valor;
                }
            }
        }else{
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << tokens[i] << " no es valido para el comando ejecutado" << endl;
            map_u.clear();
            return map_u;
        }
    }
    string comentario = map_u["-comentario"];
    string usr = map_u["-usr"];
    string pwd = map_u["-pwd"];
    string id = map_u["-id"];
    //Formateo
    usr = (usr.substr(0,1) == "\"") ? usr.substr(1, usr.size()-2): usr;
    pwd = (pwd.substr(0,1) == "\"") ? pwd.substr(1, pwd.size()-2): pwd;

    if (!usr.empty() && !pwd.empty() && !id.empty()){
        if (usr.length() > 10){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El nombre del usuario excede la cantidad de 10 caracteres " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(pwd.length() > 10){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " La contrase??a excede la cantidad de 10 caracteres " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(util_prmts.hasSpecialCharacter(usr)){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El nombre del usuario posee caracteres no soportados por el sistema de archivos " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }
    }else{
        cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " No ha ingresado algunos de los campos obligatorios (-usr, -pwd, -id) " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
        map_u.clear();
    }
    return map_u;
}

map<string, string> parameters::param_mkgrp(vector<string> tokens){
    
    map<string, string> map_u;
    for (int i = 1; i < tokens.size(); i++){
        vector<string> return_params = util_prmts.separateString(tokens[i]);
        if (return_params.size() == 2){
            string llave = util_prmts.toLowerString(return_params[0]);
            string valor = return_params[1];
            if (!(llave == "-comentario" || llave == "-name")){
                cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << llave << " no es valido para el comando ejecutado" << endl;
                map_u.clear();
                return map_u;
            }else{
                if (!map_u[llave].empty()){
                    cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El parametro " << llave << " esta siendo ingresado 2 veces por lo cual se tomara como valor el primer " << llave << " encontrado" << endl;
                }else{
                    map_u[llave] = valor;
                }
            }
        }else{
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << tokens[i] << " no es valido para el comando ejecutado" << endl;
            map_u.clear();
            return map_u;
        }
    }
    string comentario = map_u["-comentario"];
    string name = map_u["-name"];
    //Formateo
    name = (name.substr(0,1) == "\"") ? name.substr(1, name.size()-2): name;

    if (!name.empty()){
        if (name.length() > 10){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " No se puede ingresar el grupo debido a que excede la cantidad de 10 caracteres " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(util_prmts.hasSpecialCharacter(name)){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El nombre asignado para el grupo posee caracteres no soportados por el sistema de archivos " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }
    }else{
        cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " No ha ingresado algunos de los campos obligatorios (-name) " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
        map_u.clear();
    }
    return map_u;
}

map<string, string> parameters::param_rmgrp(vector<string> tokens){
    
    map<string, string> map_u;
    for (int i = 1; i < tokens.size(); i++){
        vector<string> return_params = util_prmts.separateString(tokens[i]);
        if (return_params.size() == 2){
            string llave = util_prmts.toLowerString(return_params[0]);
            string valor = return_params[1];
            if (!(llave == "-comentario" || llave == "-name")){
                cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << llave << " no es valido para el comando ejecutado" << endl;
                map_u.clear();
                return map_u;
            }else{
                if (!map_u[llave].empty()){
                    cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El parametro " << llave << " esta siendo ingresado 2 veces por lo cual se tomara como valor el primer " << llave << " encontrado" << endl;
                }else{
                    map_u[llave] = valor;
                }
            }
        }else{
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << tokens[i] << " no es valido para el comando ejecutado" << endl;
            map_u.clear();
            return map_u;
        }
    }
    string comentario = map_u["-comentario"];
    string name = map_u["-name"];
    //Formateo
    name = (name.substr(0,1) == "\"") ? name.substr(1, name.size()-2): name;

    if (!name.empty()){
        if (name.length() > 10){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " No se puede ingresar el grupo debido a que excede la cantidad de 10 caracteres " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(util_prmts.hasSpecialCharacter(name)){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El nombre asignado para el grupo posee caracteres no soportados por el sistema de archivos " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }
    }else{
        cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " No ha ingresado algunos de los campos obligatorios (-name) " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
        map_u.clear();
    }
    return map_u;
}

map<string, string> parameters::param_mkusr(vector<string> tokens){
    
    map<string, string> map_u;
    for (int i = 1; i < tokens.size(); i++){
        vector<string> return_params = util_prmts.separateString(tokens[i]);
        if (return_params.size() == 2){
            string llave = util_prmts.toLowerString(return_params[0]);
            string valor = return_params[1];
            if (!(llave == "-comentario" || llave == "-usr" || llave == "-pwd" || llave == "-grp")){
                cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << llave << " no es valido para el comando ejecutado" << endl;
                map_u.clear();
                return map_u;
            }else{
                if (!map_u[llave].empty()){
                    cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El parametro " << llave << " esta siendo ingresado 2 veces por lo cual se tomara como valor el primer " << llave << " encontrado" << endl;
                }else{
                    map_u[llave] = valor;
                }
            }
        }else{
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << tokens[i] << " no es valido para el comando ejecutado" << endl;
            map_u.clear();
            return map_u;
        }
    }
    string comentario = map_u["-comentario"];
    string usr = map_u["-usr"];
    string pwd = map_u["-pwd"];
    string grp = map_u["-grp"];
    //Formateo
    usr = (usr.substr(0,1) == "\"") ? usr.substr(1, usr.size()-2): usr;
    pwd = (pwd.substr(0,1) == "\"") ? pwd.substr(1, pwd.size()-2): pwd;
    grp = (grp.substr(0,1) == "\"") ? grp.substr(1, grp.size()-2): grp;

    if (!usr.empty() && !pwd.empty() && !grp.empty()){
        if (usr.length() > 10){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El nombre del usuario excede la cantidad de 10 caracteres " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(pwd.length() > 10){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " La contrase??a excede la cantidad de 10 caracteres " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(grp.length() > 10){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El nombre del grupo excede la cantidad de 10 caracteres " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(util_prmts.hasSpecialCharacter(grp)){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El nombre del grupo posee caracteres no soportados por el sistema de archivos " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(util_prmts.hasSpecialCharacter(usr)){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El nombre del usuario posee caracteres no soportados por el sistema de archivos " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }
    }else{
        cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " No ha ingresado algunos de los campos obligatorios (-usr, -pwd, -grp) " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
        map_u.clear();
    }
    return map_u;
}

map<string, string> parameters::param_rmusr(vector<string> tokens){
    
    map<string, string> map_u;
    for (int i = 1; i < tokens.size(); i++){
        vector<string> return_params = util_prmts.separateString(tokens[i]);
        if (return_params.size() == 2){
            string llave = util_prmts.toLowerString(return_params[0]);
            string valor = return_params[1];
            if (!(llave == "-comentario" || llave == "-usr")){
                cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << llave << " no es valido para el comando ejecutado" << endl;
                map_u.clear();
                return map_u;
            }else{
                if (!map_u[llave].empty()){
                    cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El parametro " << llave << " esta siendo ingresado 2 veces por lo cual se tomara como valor el primer " << llave << " encontrado" << endl;
                }else{
                    map_u[llave] = valor;
                }
            }
        }else{
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << tokens[i] << " no es valido para el comando ejecutado" << endl;
            map_u.clear();
            return map_u;
        }
    }
    string comentario = map_u["-comentario"];
    string usr = map_u["-usr"];
    //Formateo
    usr = (usr.substr(0,1) == "\"") ? usr.substr(1, usr.size()-2): usr;
    
    if (!usr.empty()){
        if (usr.length() > 10){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El nombre del usuario excede la cantidad de 10 caracteres " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(util_prmts.hasSpecialCharacter(usr)){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El nombre del usuario posee caracteres no soportados por el sistema de archivos " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }
    }else{
        cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " No ha ingresado algunos de los campos obligatorios (-name) " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
        map_u.clear();
    }
    return map_u;
}

/*FIN UG*/

/*INI REPORTES*/
map<string, string> parameters::param_rep(vector<string> tokens){
    
    map<string, string> map_u;
    for (int i = 1; i < tokens.size(); i++)
    {
        vector<string> return_params = util_prmts.separateString(tokens[i]);
        if (return_params.size() == 2){
            string llave = util_prmts.toLowerString(return_params[0]);
            string valor = return_params[1];
            if (!(llave == "-comentario" || llave == "-name" || llave == "-path" || llave == "-id" || llave == "-ruta")){
                cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << llave << " no es valido para el comando ejecutado" << endl;
                map_u.clear();
                return map_u;
            }else{
                if (!map_u[llave].empty()){
                    cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El parametro " << llave << " esta siendo ingresado 2 veces por lo cual se tomara como valor el primer " << llave << " encontrado" << endl;
                }else{
                    map_u[llave] = valor;
                }
            }
        }else{
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << tokens[i] << " no es valido para el comando ejecutado" << endl;
            map_u.clear();
            return map_u;
        }
    }
    
    string comentario = map_u["-comentario"];
    string name = util_prmts.toLowerString(map_u["-name"]);
    string path = map_u["-path"];
    string id = util_prmts.toUpperString(map_u["-id"]);
    string ruta = map_u["-ruta"];

    if (!name.empty() && !path.empty() && !id.empty()){
        if (!(name == "mbr" || name == "disk" || name == "sb" || name == "journaling" || name == "bm_inode" || name == "bm_block" || name == "tree" || name == "file")){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El valor ingresado en el parametro NAME es incorrecto " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }else if(util_prmts.check_existFile(path)){
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " Ya existe un reporte con el mismo nombre en la ruta: " << path << " " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
            map_u.clear();
        }
    }else{
        cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " No ha ingresado algunos de los campos obligatorios (-name, -path, -id) " << cnst_prmts.BLUE << comentario << cnst_prmts.NC << endl;
        map_u.clear();
    }
    return map_u;
}
/*FIN REPORTES*/

/*INI SCRIPT*/
map<string, string> parameters::param_exec(vector<string> tokens){
    
    map<string, string> map_u;
    for (int i = 1; i < tokens.size(); i++)
    {
        vector<string> return_params = util_prmts.separateString(tokens[i]);
        if (return_params.size() == 2){
            string llave = util_prmts.toLowerString(return_params[0]);
            string valor = return_params[1];
            if (!(llave == "-comentario" || llave == "-path")){
                cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << llave << " no es valido para el comando ejecutado" << endl;
                map_u.clear();
                return map_u;
            }else{
                if (!map_u[llave].empty())
                {
                    cout << cnst_prmts.YELLOW << "AVISO:" << cnst_prmts.NC << " El parametro " << llave << " esta siendo ingresado 2 veces por lo cual se tomara como valor el primer " << llave << " encontrado" << endl;
                }else{
                    map_u[llave] = valor;
                }
            }
        }else{
            cout << cnst_prmts.RED << "ERROR:" << cnst_prmts.NC << " El parametro " << tokens[i] << " no es valido para el comando ejecutado" << endl;
            map_u.clear();
            return map_u;
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