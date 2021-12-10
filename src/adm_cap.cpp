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

//NOTA: UNA VEZ LOGUEADO EL USUARIO LA ADMINISTRACION DE DISCO SE QUEDA BLOQUEADO
//VERIFICAR: COMO SE GENERAN LOS REPORTES AHORA QUE CAMBIE EL COMANDO MOUNT

void adm_cap::mkdir(map<string, string> param_got, disco::User UserLoggedIn){
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
    //Se verifica que halla una sesion activa
    if (UserLoggedIn.id == -1){
        cout << csnt_cap.RED << "ERROR:" << csnt_cap.NC << " No hay una sesion activa " << csnt_cap.BLUE << comentario << csnt_cap.NC << endl;
        return;
    }

    //Se obtiene la montura
    disco::Mount actualMount = UserLoggedIn.montura;
    string path_montura = actualMount.path;
    if (actualMount.status == '0'){
        cout << csnt_cap.RED << "ERROR:" << csnt_cap.NC << " No existe el id " << csnt_cap.BLUE << comentario << csnt_cap.NC << endl;
        return;
    }

    //Se verifica si es un usuario root
    bool isRoot = (UserLoggedIn.usuario == "root" && UserLoggedIn.contrasenia == "123");    

    //Creo una variable de tipo FILE
    FILE *file = fopen(path_montura.c_str(), "rb+");

    //Obtengo el super bloque
    disco::Superblock *sp_user = new disco::Superblock();
    fseek(file, actualMount.part_start, SEEK_SET);
    fread(sp_user, csnt_cap.SIZE_SPB, 1, file);
    if (sp_user->s_filesystem_type == 0)    {
        cout << csnt_cap.RED << "ERROR:" << csnt_cap.NC << " No se ha formateado esta particion " << csnt_cap.BLUE << comentario << csnt_cap.NC << endl;
        return;
    }

    //Se obtiene el primer inodo de todo el sistema de archivos que en teoria es '/'
    disco::Inode *inode_user = new disco::Inode();
    fseek(file, sp_user->s_inode_start, SEEK_SET);
    fread(inode_user, csnt_cap.SIZE_I, 1, file);

    //cout << csnt_cap.GREEN << "RESPUESTA:" << csnt_cap.NC << " La carpeta ha sido creada correctamente " << csnt_cap.BLUE << comentario << csnt_cap.NC << endl;
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