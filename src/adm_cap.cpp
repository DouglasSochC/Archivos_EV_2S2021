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

void adm_cap::mkdir(map<string, string> param_got, disco::Mount partitionMount, disco::User userLoggedIn){
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
    //Se verifica que sea usuario root
    bool isRoot = (userLoggedIn.usuario == "root" && userLoggedIn.contrasenia == "123");    

    cout << csnt_cap.GREEN << "RESPUESTA:" << csnt_cap.NC << " La carpeta ha sido creada correctamente " << csnt_cap.BLUE << comentario << csnt_cap.NC << endl;
}

//Estrategias:
//1. Obtener la posicion en donde se encuentra el archivo
//2. Crear un metodo que directamente lea un inodo de tipo archivo
//Notas:
//1. Se debe verificar los permisos del inodo
//2. 
void adm_cap::cat(map<string, string> param_got, disco::Mount partitionMount, disco::User userLoggedIn){
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
    //Se verifica que sea usuario root
    bool isRoot = (userLoggedIn.usuario == "root" && userLoggedIn.contrasenia == "123");    

    cout << csnt_cap.GREEN << "RESPUESTA:" << csnt_cap.NC << " La carpeta ha sido creada correctamente " << csnt_cap.BLUE << comentario << csnt_cap.NC << endl;
}
//CREAR FUNCION CAT