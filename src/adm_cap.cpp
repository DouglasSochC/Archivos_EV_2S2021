#include <iostream>
#include <fstream>
#include <map>
#include <cstring>
#include "../util/util_p.h"
#include "../util/constant.h"
#include "adm_cap.h"

//Administracion de Carpetas, Archivos y Permisos
using namespace std;

util_p util_cap;
constant csnt_cap;

void adm_cap::mkdir(map<string, string> param_got, vector<disco::Mount> listMount){
    if (param_got.size() == 0){return;}

    /*Obteniendo datos*/
    string comentario = param_got["-comentario"];
    string id = param_got["-id"];
    string path = param_got["-path"];
    string var_p;
    if (param_got.find("-p") == param_got.end()) {
        //NO SE ENCONTRO
    } else {
        var_p = "p";
    }

    /*Formateo de datos*/
    path = (path.substr(0,1) == "\"") ? path.substr(1, path.size()-2): path;

    /*Flujo del void*/

    cout << csnt_cap.GREEN << "RESPUESTA:" << csnt_cap.NC << " La carpeta ha sido creada correctamente " << csnt_cap.BLUE << comentario << csnt_cap.NC << endl;
}