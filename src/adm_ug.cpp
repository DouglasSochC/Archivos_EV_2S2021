#include <iostream>
#include <map>
#include <cmath>
#include <cstring>
#include "../util/util_p.h"
#include "../util/constant.h"
#include "adm_ug.h"
#include "adm_discos.h"
#include "disco.h"

using namespace std;

constant csnt_ug;
util_p util_ug;
adm_discos admdcs_admug;

disco::User adm_ug::login(map<string, string> param_got, vector<disco::Mount> list_mount, disco::User UserLoggedIn){
    if (param_got.size() == 0){
        return disco::User();
    }

    /*Obteniendo datos*/
    string comentario = param_got["-comentario"].c_str();
    string usr = param_got["-usr"].c_str();
    string pwd = param_got["-pwd"].c_str();
    string id = param_got["-id"].c_str();

    /*Formateo de datos*/
    usr = (usr.substr(0,1) == "\"") ? usr.substr(1, usr.size()-2): usr;

    /*Flujo del void*/
    //Se verifica que no halla una sesion activa
    if (UserLoggedIn.id != -1){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " Ya hay una sesion activa " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return UserLoggedIn;
    }

    //Se obtiene la montura
    disco::Mount actualMount;
    for (int i = 0; i < list_mount.size(); i++){
        if (list_mount[i].id == id){
            actualMount = list_mount[i];
        }
    }
    if (actualMount.status == '0'){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " No existe el id " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return disco::User();
    }
    
    string text = getArchiveUserTXT(actualMount.part_start, actualMount.path);
    disco::User response = checkUser(text, usr, pwd, true);
    response.montura = actualMount;

    if (!(response.usuario == usr && response.contrasenia == pwd)){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " No existe el usuario con el username y contraseña ingresada " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return disco::User();
    }

    cout << csnt_ug.GREEN << "RESPUESTA:" << csnt_ug.NC << " ¡Bienvenido! " << usr << " " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
    return response;
}

disco::User adm_ug::logout(disco::User UserLoggedIn){
    if (UserLoggedIn.id == -1){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " No hay una sesion activa" << endl;
    }else{
        cout << csnt_ug.GREEN << "RESPUESTA:" << csnt_ug.NC << " Sesion cerrada correctamente" << endl;
    }
    return disco::User();
}

void adm_ug::mkgrp(map<string, string> param_got, disco::User UserLoggedIn){
    if (param_got.size() == 0){return;}

    /*Obteniendo datos*/
    string comentario = param_got["-comentario"].c_str();
    string name = param_got["-name"].c_str();

    /*Formateo de datos*/
    name = (name.substr(0,1) == "\"") ? name.substr(1, name.size()-2): name;

    /*Flujo del void*/
    //Se verifica que halla una sesion activa
    if (UserLoggedIn.id == -1){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " No hay una sesion activa " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return;
    }

    //Se obtiene la montura
    disco::Mount actualMount = UserLoggedIn.montura;
    string path = actualMount.path;
    if (actualMount.status == '0'){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " No existe el id " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return;
    }

    //Se verifica que sea usuario root
    bool isRoot = (UserLoggedIn.usuario == "root" && UserLoggedIn.contrasenia == "123");
    if (!isRoot){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " Solo un usuario root puede realizar esta accion " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return;
    }
    
    //Creo una variable de tipo FILE
    FILE *file = fopen(path.c_str(), "rb+");

    //Obtengo el super bloque
    disco::Superblock *sp_user = new disco::Superblock();
    fseek(file, actualMount.part_start, SEEK_SET);
    fread(sp_user, csnt_ug.SIZE_SPB, 1, file);
    if (sp_user->s_filesystem_type == 0)    {
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " No se ha formateado esta particion " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return;
    }

    //Se obtiene el inodo del user.txt
    disco::Inode *inode_user = new disco::Inode();
    fseek(file, sp_user->s_inode_start + csnt_ug.SIZE_I, SEEK_SET);
    fread(inode_user, csnt_ug.SIZE_I, 1, file);
    
    string text = getArchiveUserTXT(actualMount.part_start, path);
    disco::Group response = checkGroup(text, name);
    if (response.nombre == name){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " Ya existe el grupo que desea crear " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return;
    }
    //Se insertan los datos en el inodo
    string registro = to_string(response.id+1) + ",G," + name + "\n";

    //Se almacena el registro para el journal, si es que es EXT3, antes de que sea modificado
    string registro_journal = registro;

    insertDatainInodeArchive(&registro, sp_user, inode_user, actualMount);    

    //Se actualiza el superbloque debido a que pudo tener cambios mientras se ingresaba algun registro
    fseek(file, actualMount.part_start, SEEK_SET);
    fwrite(sp_user, csnt_ug.SIZE_SPB, 1, file);

    //Se actualiza el inodo debido a que pudo tener cambios mientras se ingresaba algun registro
    fseek(file, sp_user->s_inode_start + csnt_ug.SIZE_I, SEEK_SET);
    fwrite(inode_user, csnt_ug.SIZE_I, 1, file);

    //Se almacenan los cambios
    fclose(file);

    //Se ingresa un journal en el caso de que sea un sistema EXT3
    if (sp_user->s_filesystem_type == 3){
        insertJournal(registro_journal, 'A', actualMount, *sp_user);
    }

    if (registro.size() == 0){
        cout << csnt_ug.GREEN << "RESPUESTA:" << csnt_ug.NC << " Se creado correctamente el grupo " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
    }else{
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " Ya no hay mas espacio para poder ingresar el grupo " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
    }
}

void adm_ug::rmgrp(map<string, string> param_got, disco::User UserLoggedIn){
    if (param_got.size() == 0){return;}

    /*Obteniendo datos*/
    string comentario = param_got["-comentario"].c_str();
    string name = param_got["-name"].c_str();

    /*Formateo de datos*/
    name = (name.substr(0,1) == "\"") ? name.substr(1, name.size()-2): name;

    /*Flujo del void*/
    //Se verifica que halla una sesion activa
    if (UserLoggedIn.id == -1){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " No hay una sesion activa " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return;
    }

    //Se obtiene la montura
    disco::Mount actualMount = UserLoggedIn.montura;
    string path = actualMount.path;
    if (actualMount.status == '0'){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " No existe el id " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return;
    }

    //Se verifica que sea usuario root
    bool isRoot = (UserLoggedIn.usuario == "root" && UserLoggedIn.contrasenia == "123");
    if (!isRoot){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " Solo un usuario root puede realizar esta accion " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return;
    }
    
    if (name == "root"){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " No se puede eliminar el grupo root " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return;
    }

    //Creo una variable de tipo FILE
    FILE *file = fopen(path.c_str(), "rb+");

    //Obtengo el super bloque
    disco::Superblock *sp_user = new disco::Superblock();
    fseek(file, actualMount.part_start, SEEK_SET);
    fread(sp_user, csnt_ug.SIZE_SPB, 1, file);
    if (sp_user->s_filesystem_type == 0)    {
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " No se ha formateado esta particion " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return;
    }

    //Se obtiene el inodo del user.txt
    disco::Inode *inode_user = new disco::Inode();
    fseek(file, sp_user->s_inode_start + csnt_ug.SIZE_I, SEEK_SET);
    fread(inode_user, csnt_ug.SIZE_I, 1, file);
    
    string text = getArchiveUserTXT(actualMount.part_start, path);
    disco::Group response = checkGroup(text, name);
    if (response.nombre != name){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " No existe el grupo que desea eliminar " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return;
    }
    int posicion_registro_encontrado = response.posicion_registro;
    changeStatusRow(sp_user, inode_user, actualMount, &posicion_registro_encontrado);

    //Se almacenan los cambios
    fclose(file);

    //Se ingresa un journal en el caso de que sea un sistema EXT3
    if (sp_user->s_filesystem_type == 3){
        string registro = to_string(response.id)+","+response.nombre+";"+"0,"+response.nombre;
        insertJournal(registro, 'U', actualMount, *sp_user);
    }

    cout << csnt_ug.GREEN << "RESPUESTA:" << csnt_ug.NC << " Se eliminado correctamente el grupo " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;

}

void adm_ug::mkusr(map<string, string> param_got, disco::User UserLoggedIn){
    if (param_got.size() == 0){return;}

    /*Obteniendo datos*/
    string comentario = param_got["-comentario"].c_str();
    string usr = param_got["-usr"].c_str();
    string pwd = param_got["-pwd"].c_str();
    string grp = param_got["-grp"].c_str();

    /*Formateo de datos*/
    usr = (usr.substr(0,1) == "\"") ? usr.substr(1, usr.size()-2): usr;
    grp = (grp.substr(0,1) == "\"") ? grp.substr(1, grp.size()-2): grp;

    /*Flujo del void*/
    //Se verifica que halla una sesion activa
    if (UserLoggedIn.id == -1){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " No hay una sesion activa " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return;
    }

    //Se obtiene la montura
    disco::Mount actualMount = UserLoggedIn.montura;
    string path = actualMount.path;
    if (actualMount.status == '0'){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " No existe el id " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return;
    }

    //Se verifica que sea usuario root
    bool isRoot = (UserLoggedIn.usuario == "root" && UserLoggedIn.contrasenia == "123");
    if (!isRoot){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " Solo un usuario root puede realizar esta accion " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return;
    }

    //Creo una variable de tipo FILE
    FILE *file = fopen(path.c_str(), "rb+");

    //Obtengo el super bloque
    disco::Superblock *sp_user = new disco::Superblock();
    fseek(file, actualMount.part_start, SEEK_SET);
    fread(sp_user, csnt_ug.SIZE_SPB, 1, file);
    if (sp_user->s_filesystem_type == 0)    {
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " No se ha formateado esta particion " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        fclose(file);
        return;
    }

    //Se obtiene el inodo del user.txt
    disco::Inode *inode_user = new disco::Inode();
    fseek(file, sp_user->s_inode_start + csnt_ug.SIZE_I, SEEK_SET);
    fread(inode_user, csnt_ug.SIZE_I, 1, file);
    
    string text = getArchiveUserTXT(actualMount.part_start, path);
    disco::Group responseGroup = checkGroup(text, grp);
    if (responseGroup.nombre != grp){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " No existe el grupo que desea asignar al usuario " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        fclose(file);
        return;
    }
    disco::User responseUser = checkUser(text, usr, "", false);
    if (responseUser.usuario == usr){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " Ya existe el usuario que desea crear " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        fclose(file);
        return;
    }
    //Se insertan los datos en el inodo
    string registro = to_string(responseUser.id+1)+",U,"+grp+","+usr+","+pwd+"\n";

    //Se almacena el registro para el journal, si es que es EXT3, antes de que sea modificado
    string registro_journal = registro;

    insertDatainInodeArchive(&registro, sp_user, inode_user, actualMount);    

    //Se actualiza el superbloque debido a que pudo tener cambios mientras se ingresaba algun registro
    fseek(file, actualMount.part_start, SEEK_SET);
    fwrite(sp_user, csnt_ug.SIZE_SPB, 1, file);

    //Se actualiza el inodo debido a que pudo tener cambios mientras se ingresaba algun registro
    fseek(file, sp_user->s_inode_start + csnt_ug.SIZE_I, SEEK_SET);
    fwrite(inode_user, csnt_ug.SIZE_I, 1, file);

    //Se almacenan los cambios
    fclose(file);

    //Se ingresa un journal en el caso de que sea un sistema EXT3
    if (sp_user->s_filesystem_type == 3){
        insertJournal(registro_journal, 'A', actualMount, *sp_user);
    }

    if (registro.size() == 0){
        cout << csnt_ug.GREEN << "RESPUESTA:" << csnt_ug.NC << " Se creado correctamente el usuario " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
    }else{
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " Ya no hay mas espacio para poder ingresar el usuario " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
    }
}

void adm_ug::rmusr(map<string, string> param_got, disco::User UserLoggedIn){
    if (param_got.size() == 0){return;}

    /*Obteniendo datos*/
    string comentario = param_got["-comentario"].c_str();
    string usr = param_got["-usr"].c_str();

    /*Formateo de datos*/
    usr = (usr.substr(0,1) == "\"") ? usr.substr(1, usr.size()-2): usr;

    /*Flujo del void*/
    //Se verifica que halla una sesion activa
    if (UserLoggedIn.id == -1){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " No hay una sesion activa " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return;
    }

    //Se obtiene la montura
    disco::Mount actualMount = UserLoggedIn.montura;
    string path = actualMount.path;
    if (actualMount.status == '0'){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " No existe el id " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return;
    }

    //Se verifica que sea usuario root
    bool isRoot = (UserLoggedIn.usuario == "root" && UserLoggedIn.contrasenia == "123");
    if (!isRoot){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " Solo un usuario root puede realizar esta accion " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return;
    }
    
    if (usr == "root"){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " No se puede eliminar el usuario root " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return;
    }

    //Creo una variable de tipo FILE
    FILE *file = fopen(path.c_str(), "rb+");

    //Obtengo el super bloque
    disco::Superblock *sp_user = new disco::Superblock();
    fseek(file, actualMount.part_start, SEEK_SET);
    fread(sp_user, csnt_ug.SIZE_SPB, 1, file);
    if (sp_user->s_filesystem_type == 0)    {
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " No se ha formateado esta particion " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return;
    }

    //Se obtiene el inodo del user.txt
    disco::Inode *inode_user = new disco::Inode();
    fseek(file, sp_user->s_inode_start + csnt_ug.SIZE_I, SEEK_SET);
    fread(inode_user, csnt_ug.SIZE_I, 1, file);
    
    string text = getArchiveUserTXT(actualMount.part_start, path);
    disco::User response = checkUser(text, usr, "", false);
    if (response.usuario != usr){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " No existe el usuario que desea eliminar " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return;
    }
    int posicion_registro_encontrado = response.posicion_registro;
    changeStatusRow(sp_user, inode_user, actualMount, &posicion_registro_encontrado);

    //Se almacenan los cambios
    fclose(file);

    //Se ingresa un journal en el caso de que sea un sistema EXT3
    if (sp_user->s_filesystem_type == 3){
        string registro = to_string(response.id)+","+response.usuario+";"+"0,"+response.usuario;
        insertJournal(registro, 'U', actualMount, *sp_user);
    }
    cout << csnt_ug.GREEN << "RESPUESTA:" << csnt_ug.NC << " Se eliminado correctamente el grupo " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
}

string adm_ug::getArchiveUserTXT(int part_start_partition, string path){
    string response;
    //Creo una variable de tipo FILE
    FILE *file = fopen(path.c_str(), "rb");

    //Obtengo el super bloque
    disco::Superblock sp_user;
    fseek(file, part_start_partition, SEEK_SET);
    fread(&sp_user, csnt_ug.SIZE_SPB, 1, file);
    if (sp_user.s_filesystem_type == 0)    {
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " No se ha formateado esta particion " << endl;
        return "";
    }

    //Obtengo el inodo del user.txt debido a que se debe de leer por completo
    disco::Inode inode_user;
    fseek(file, sp_user.s_inode_start + csnt_ug.SIZE_I, SEEK_SET);
    fread(&inode_user, csnt_ug.SIZE_I, 1, file);
    
    //Apuntadores del 1 al 12
    for (int i = 0; i < 12; i++){
        if (inode_user.i_block[i] != -1){
            getContentByTypePointer(&response, "BD", path, sp_user, inode_user.i_block[i]);
        }
    }
    
    //Apuntador 13 - Se apunta a una estructura de apuntadores
    if (inode_user.i_block[12] != -1){
        getContentByTypePointer(&response, "BSI", path, sp_user, inode_user.i_block[12]);
    }

    //Apuntador 14 - Se apunta a una estructura de apuntadores
    if (inode_user.i_block[13] != -1){
        getContentByTypePointer(&response, "BDI", path, sp_user, inode_user.i_block[13]);
    }

    //Apuntador 15 - Se apunta a una estructura de apuntadores
    if (inode_user.i_block[14] != -1){
        getContentByTypePointer(&response, "BTI", path, sp_user, inode_user.i_block[14]);
    }

    fclose(file);
    return response;
}

void adm_ug::insertDatainInodeArchive(string *registro, disco::Superblock *spb, disco::Inode *inode_user, disco::Mount mount_temp){

    //Apuntadores del 1 al 12
    for (int i = 0; i < 12; i++){
        if (registro->size() > 0){
            setContentByTypePointer("BD", inode_user->i_block[i], mount_temp.path, 'I', spb, inode_user, NULL, registro);
        }
    }

    //Apuntador 13 - Se apunta a una estructura de apuntadores
    if (registro->size() > 0){
        setContentByTypePointer("BSI", inode_user->i_block[12], mount_temp.path, 'I', spb, inode_user, NULL, registro);
    }

    //Apuntador 14 - Se apunta a una estructura de apuntadores
    if (registro->size() > 0){
        setContentByTypePointer("BDI", inode_user->i_block[13], mount_temp.path, 'I', spb, inode_user, NULL, registro);
    }

    //Apuntador 15 - Se apunta a una estructura de apuntadores
    if (registro->size() > 0){
        setContentByTypePointer("BTI", inode_user->i_block[14], mount_temp.path, '\0', spb, inode_user, NULL, registro);
    }

}

void adm_ug::getContentByTypePointer(string *contenido, string tipo_apuntador, string path, disco::Superblock spb, int pos_block){

    FILE *file = fopen(path.c_str(), "rb");
    if (tipo_apuntador == "BD"){
        disco::Archiveblock tempArchive;
        fseek(file, spb.s_block_start + (pos_block * csnt_ug.SIZE_AB), SEEK_SET);
        fread(&tempArchive, csnt_ug.SIZE_AB, 1, file);
        *contenido += tempArchive.b_content;
    }else if(tipo_apuntador == "BSI"){
        disco::Pointerblock tempPointer;
        fseek(file, spb.s_block_start + (pos_block * csnt_ug.SIZE_AB), SEEK_SET);
        fread(&tempPointer, csnt_ug.SIZE_PB, 1, file);
        for (int i = 0; i < 15; i++){
            if (tempPointer.b_pointers[i] != -1){
                getContentByTypePointer(contenido, "BD", path, spb, tempPointer.b_pointers[i]);
            }
        }
    }else if(tipo_apuntador == "BDI"){
        disco::Pointerblock tempPointer;
        fseek(file, spb.s_block_start + (pos_block * csnt_ug.SIZE_AB), SEEK_SET);
        fread(&tempPointer, csnt_ug.SIZE_PB, 1, file);
        for (int i = 0; i < 15; i++){
            if (tempPointer.b_pointers[i] != -1){
                getContentByTypePointer(contenido, "BSI", path, spb, tempPointer.b_pointers[i]);
            }
        }       
    }else if(tipo_apuntador == "BTI"){
        disco::Pointerblock tempPointer;
        fseek(file, spb.s_block_start + (pos_block * csnt_ug.SIZE_AB), SEEK_SET);
        fread(&tempPointer, csnt_ug.SIZE_PB, 1, file);
        for (int i = 0; i < 15; i++){
            if (tempPointer.b_pointers[i] != -1){
                getContentByTypePointer(contenido, "BDI", path, spb, tempPointer.b_pointers[i]);
            }
        } 
    }
    fclose(file);
}

void adm_ug::setContentByTypePointer(string tipo_puntero, int pos_block, string path, char estructura_a_modificar, disco::Superblock *spb, disco::Inode *inode_user, disco::Pointerblock *pointer_user, string *registro){
    FILE *file = fopen(path.c_str(), "rb+");
    if (tipo_puntero == "BD"){
        
        if (pos_block == -1 && spb->s_free_blocks_count > 0){
            //Se obtiene la posicion de un bloque vacio
            int new_block_position = spb->s_first_blo;
            //Se hace una comparacion sobre que tipo de estructura va a modificar algunos de sus punteros
            if (estructura_a_modificar == 'P'){
                //Se modifica un puntero vacio del bloque de apuntadores
                for (int i = 0; i < 15; i++){
                    if (pointer_user->b_pointers[i] == -1){
                        pointer_user->b_pointers[i] = new_block_position;
                        break;
                    }
                }
            }else if(estructura_a_modificar == 'I'){
                for (int i = 0; i < 12; i++){
                    //Se modifica un puntero vacio del inodo
                    if (inode_user->i_block[i] == -1){
                        inode_user->i_block[i] = new_block_position;
                        break;
                    }
                }
            }
            //Se setea un 1 en la nueva posicion ya que va a ser ocupada
            setPositionBMBlock(new_block_position, path, *spb);
            //Se encuentra una nueva posicion de un bloque vacio y se le asigna al superbloque
            spb->s_first_blo = getPositionBMBlock(*spb, path);
            //Se crea un nuevo bloque de tipo puntero
            createArchiveBlock(new_block_position, path, *spb);
            //Se modifica la cantidad de bloques libres
            spb->s_free_blocks_count -= 1;
            //Entra al mismo ciclo con la nueva posicion
            setContentByTypePointer("BD", new_block_position, path, '\0', spb, inode_user, NULL, registro);
        }else if(pos_block != -1 && spb->s_free_blocks_count > 0){
            //Leo una estructura de tipo archivo segun la variable pos_block
            disco::Archiveblock *archive_bd = new disco::Archiveblock();
            fseek(file, spb->s_block_start + (pos_block * csnt_ug.SIZE_AB), SEEK_SET);
            fread(archive_bd, csnt_ug.SIZE_AB, 1, file);
            
            //cc = cantidad de caracteres
            //Esta variable define la cantidad de caracteres que se estan utilizando dentro del b_content
            int cc_utilizados = strlen(archive_bd->b_content);
            //Esta variable define la cantidad de caracteres que aun se pueden ingresar en el b_content
            int cc_disponible = 63 - cc_utilizados;
            if (cc_disponible <= 0){
                return;
            }            

            //Esta variable define el tamanio del nuevo contenido que se va a escribir
            int tamanio_contenido = registro->size();
            //Esta variable define la cantidad de caracteres que se van a utilizar
            int cc_a_utilizar = (cc_disponible - tamanio_contenido) >= 0 ? registro->size(): cc_disponible;
            string contenido_a_ingresar = registro->substr(0, cc_a_utilizar);
            string contenido_faltante = registro->substr(cc_a_utilizar, registro->size()-cc_a_utilizar);
            *registro = contenido_faltante;
            strcat(archive_bd->b_content, contenido_a_ingresar.c_str());
            
            //Se modifica el tamanio del inodo
            inode_user->i_size += cc_a_utilizar;

            //Se almacena la estructura con los nuevos datos
            fseek(file, spb->s_block_start + (pos_block * csnt_ug.SIZE_AB), SEEK_SET);
            fwrite(archive_bd, csnt_ug.SIZE_AB, 1, file);
        }
    }else if (tipo_puntero == "BSI"){
        if (pos_block == -1 && spb->s_free_blocks_count > 0){
            //Se obtiene la posicion de un bloque vacio
            int new_block_position = spb->s_first_blo;
            //Se hace una comparacion sobre que tipo de estructura va a modificar algunos de sus punteros
            if (estructura_a_modificar == 'P'){
                //Se modifica un puntero vacio del bloque de apuntadores
                for (int i = 0; i < 15; i++){
                    if (pointer_user->b_pointers[i] == -1){
                        pointer_user->b_pointers[i] = new_block_position;
                        break;
                    }
                }
            }else if(estructura_a_modificar == 'I'){
                //Se modifica el puntero numero 14 del inodo_user
                inode_user->i_block[12] = new_block_position;
            }
            //Se setea un 1 en la nueva posicion ya que va a ser ocupada
            setPositionBMBlock(new_block_position, path, *spb);
            //Se encuentra una nueva posicion de un bloque vacio y se le asigna al superbloque
            spb->s_first_blo = getPositionBMBlock(*spb, path);
            //Se modifica la cantidad de bloques libres
            spb->s_free_blocks_count -= 1;
            //Se crea un nuevo bloque de tipo puntero
            createPointerBlock(new_block_position, path, *spb);
            //Entra al mismo ciclo con la nueva posicion
            setContentByTypePointer("BSI", new_block_position, path, '\0', spb, inode_user, NULL, registro);
        }else if(pos_block != -1 && spb->s_free_blocks_count > 0){
            disco::Pointerblock *pointer_bsi = new disco::Pointerblock();
            fseek(file, spb->s_block_start + (pos_block * csnt_ug.SIZE_PB), SEEK_SET);
            fread(pointer_bsi, csnt_ug.SIZE_PB, 1, file);
            for (int i = 0; i < 15; i++){
                if (registro->size() > 0){
                    setContentByTypePointer("BD", pointer_bsi->b_pointers[i], path, 'P', spb, inode_user, pointer_bsi, registro);
                }
            }
            //Se almacenan los cambios que halla pasado el bloque de tipo apuntador
            fseek(file, spb->s_block_start + (pos_block * csnt_ug.SIZE_PB), SEEK_SET);
            fwrite(pointer_bsi, csnt_ug.SIZE_PB, 1, file);
        }
    }else if (tipo_puntero == "BDI"){
        if (pos_block == -1 && spb->s_free_blocks_count > 0){
            //Se obtiene la posicion de un bloque vacio
            int new_block_position = spb->s_first_blo;
            //Se hace una comparacion sobre que tipo de estructura va a modificar algunos de sus punteros
            if (estructura_a_modificar == 'P'){
                //Se modifica un puntero vacio del bloque de apuntadores
                for (int i = 0; i < 15; i++){
                    if (pointer_user->b_pointers[i] == -1){
                        pointer_user->b_pointers[i] = new_block_position;
                        break;
                    }
                }
            }else if(estructura_a_modificar == 'I'){
                //Se modifica el puntero numero 14 del inodo_user
                inode_user->i_block[13] = new_block_position;
            }
            //Se setea un 1 en la nueva posicion ya que va a ser ocupada
            setPositionBMBlock(new_block_position, path, *spb);
            //Se encuentra una nueva posicion de un bloque vacio y se le asigna al superbloque
            spb->s_first_blo = getPositionBMBlock(*spb, path);
            //Se modifica la cantidad de bloques libres
            spb->s_free_blocks_count -= 1;
            //Se crea un nuevo bloque de tipo puntero
            createPointerBlock(new_block_position, path, *spb);
            //Entra al mismo ciclo con la nueva posicion
            setContentByTypePointer("BDI", new_block_position, path, '\0', spb, inode_user, NULL, registro);
        }else if(pos_block != -1 && spb->s_free_blocks_count > 0){
            disco::Pointerblock *pointer_bdi = new disco::Pointerblock();
            fseek(file, spb->s_block_start + (pos_block * csnt_ug.SIZE_PB), SEEK_SET);
            fread(pointer_bdi, csnt_ug.SIZE_PB, 1, file);
            for (int i = 0; i < 15; i++){
                if (registro->size() > 0){
                    setContentByTypePointer("BSI", pointer_bdi->b_pointers[i], path, 'P', spb, inode_user, pointer_bdi, registro);
                }
            }
            //Se almacenan los cambios que halla pasado el bloque de tipo apuntador
            fseek(file, spb->s_block_start + (pos_block * csnt_ug.SIZE_PB), SEEK_SET);
            fwrite(pointer_bdi, csnt_ug.SIZE_PB, 1, file);
        }
    }else if (tipo_puntero == "BTI"){
        if (pos_block == -1 && spb->s_free_blocks_count > 0){
            //Se obtiene la posicion de un bloque vacio
            int new_block_position = spb->s_first_blo;
            //Se modifica el puntero numero 15 del inodo_user
            inode_user->i_block[14] = new_block_position;
            //Se setea un 1 en la nueva posicion ya que va a ser ocupada
            setPositionBMBlock(new_block_position, path, *spb);
            //Se encuentra una nueva posicion de un bloque vacio y se le asigna al superbloque
            spb->s_first_blo = getPositionBMBlock(*spb, path);
            //Se modifica la cantidad de bloques libres
            spb->s_free_blocks_count -= 1;
            //Se crea un nuevo bloque de tipo puntero
            createPointerBlock(new_block_position, path, *spb);
            //Entra al mismo ciclo con la nueva posicion
            setContentByTypePointer("BTI", new_block_position, path, '\0', spb, inode_user, NULL, registro);
        }else if(pos_block != -1 && spb->s_free_blocks_count > 0){
            disco::Pointerblock *pointer_bti = new disco::Pointerblock();
            fseek(file, spb->s_block_start + (pos_block * csnt_ug.SIZE_PB), SEEK_SET);
            fread(pointer_bti, csnt_ug.SIZE_PB, 1, file);
            for (int i = 0; i < 15; i++){
                if (registro->size() > 0){
                    setContentByTypePointer("BDI", pointer_bti->b_pointers[i], path, 'P', spb, inode_user, pointer_bti, registro);
                }
            }
            //Se almacenan los cambios que halla pasado el bloque de tipo apuntador
            fseek(file, spb->s_block_start + (pos_block * csnt_ug.SIZE_PB), SEEK_SET);
            fwrite(pointer_bti, csnt_ug.SIZE_PB, 1, file);
        }
    }
    fclose(file);
}

disco::User adm_ug::checkUser(string user_txt, string user, string psw, bool needPassword){
    
    disco::User response;
    vector<string> temp;
    int actual = 0;
    int tamanio = 0;
    for (int i = 0; i < user_txt.size(); i++){
        if (user_txt.substr(i, 1) == "\n"){
            string linea = user_txt.substr(actual, tamanio);            
            int inicio_valor = 0;
            int tamanio_valor = 0;

            //Ingreso en un vector todos los registro que encuentre
            for (int j = 0; j < linea.size(); j++){
                if (linea.substr(j, 1) == ","){
                    temp.push_back(linea.substr(inicio_valor, tamanio_valor));
                    inicio_valor = j+1;
                    tamanio_valor = 0;
                }else if(j == (linea.size()-1)){
                    temp.push_back(linea.substr(inicio_valor, tamanio_valor+1));
                    inicio_valor = 0;
                    tamanio_valor = 0;
                }else{
                    tamanio_valor += 1;
                }
            }

            if (temp[1] == "U"){
                if ((temp[3] == user && temp[4] == psw && atoi(temp[0].c_str()) > 0) || (temp[3] == user && needPassword == false && atoi(temp[0].c_str()) > 0)){
                    response.usuario = user;
                    response.contrasenia = psw;
                    response.grupo = temp[2];
                    response.id = atoi(temp[0].c_str());
                    response.tipo = temp[1][0];
                    response.posicion_registro = actual;
                    response.tamanio_registro = tamanio;
                    break;
                }else if(atoi(temp[0].c_str()) >= response.id){
                    response.usuario = "";
                    response.contrasenia = "";
                    response.grupo = temp[2];
                    response.id = atoi(temp[0].c_str());
                    response.tipo = temp[1][0];
                    response.posicion_registro = actual;
                    response.tamanio_registro = tamanio;
                }
            }
            actual = i + 1;
            tamanio = 0;
            temp.clear();
        }else{
            tamanio += 1;
        }
    }    
    return response;
}

disco::Group adm_ug::checkGroup(string user_txt, string nombre){
    disco::Group response;
    vector<string> temp;
    int actual = 0;
    int tamanio = 0;
    for (int i = 0; i < user_txt.size(); i++){
        if (user_txt.substr(i, 1) == "\n"){
            string linea = user_txt.substr(actual, tamanio);            
            int inicio_valor = 0;
            int tamanio_valor = 0;
            for (int j = 0; j < linea.size(); j++){
                if (linea.substr(j, 1) == ","){
                    temp.push_back(linea.substr(inicio_valor, tamanio_valor));
                    inicio_valor = j+1;
                    tamanio_valor = 0;
                }else if(j == (linea.size()-1)){
                    temp.push_back(linea.substr(inicio_valor, tamanio_valor+1));
                    inicio_valor = 0;
                    tamanio_valor = 0;
                }else{
                    tamanio_valor += 1;
                }
            }

            if (temp[1] == "G"){
                if (temp[2] == nombre && atoi(temp[0].c_str()) > 0){
                    response.id = atoi(temp[0].c_str());
                    response.nombre = temp[2].c_str();
                    response.posicion_registro = actual;
                    response.tamanio_registro = tamanio;
                    break;
                }else if(atoi(temp[0].c_str()) >= response.id){
                    response.id = atoi(temp[0].c_str());
                    response.nombre = temp[2].c_str();
                    response.posicion_registro = actual;
                    response.tamanio_registro = tamanio;
                }
            }
            actual = i + 1;
            tamanio = 0;
            temp.clear();
        }else{
            tamanio += 1;
        }
    }    
    return response;
}

void adm_ug::insertJournal(string contenido, char operacion, disco::Mount mount_temp, disco::Superblock spb){

    //Creo una variable de tipo FILE
    FILE *file = fopen(mount_temp.path.c_str(), "rb+");
    fseek(file, mount_temp.part_start + csnt_ug.SIZE_SPB, SEEK_SET);

    disco::Journaling *journal_lectura = new disco::Journaling();
    int posicion = -1;
    for (int i = 0; i < spb.s_inodes_count; i++){
        //Se lee el journaling
        fread(journal_lectura, csnt_ug.SIZE_J, 1, file);
        if (journal_lectura->id_journal == -1){
            posicion = i;
            break;
        }        
    }
    
    if (posicion == -1){
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " Ya no hay mas espacio para ingresar journaling " << endl;
        return;
    }

    //Se posiciona el puntero del file
    fseek(file, mount_temp.part_start + csnt_ug.SIZE_SPB + (posicion * csnt_ug.SIZE_J), SEEK_SET);

    //Journal nuevo
    disco::Journaling *journal = new disco::Journaling();
    journal->id_journal = posicion + 1;
    journal->operation = operacion;
    journal->type = '1';
    strcpy(journal->nombre, "user.txt");
    journal->content = contenido;
    journal->date = time(nullptr);
    strcpy(journal->propietario, "root");
    journal->permiso = 664;
    
    fwrite(journal, csnt_ug.SIZE_J, 1, file);
    fclose(file);
}

int adm_ug::getPositionBMBlock(disco::Superblock spb, string path){
    int response;
    //Creo una variable de tipo FILE
    FILE *file = fopen(path.c_str(), "rb");
    char zero;
    //Seteo 0's en el bitmap de inodos
    fseek(file, spb.s_bm_block_start, SEEK_SET);
    for (int i = 0; i < spb.s_blocks_count; i++){
        fread(&zero, sizeof(zero), 1, file);
        if (zero == '0'){
            response = i;
            break;
        }        
    }
    fclose(file);
    return response;
}

void adm_ug::setPositionBMBlock(int position, string path, disco::Superblock spb){
    
    //Creo una variable de tipo FILE
    FILE *file = fopen(path.c_str(), "rb+");
    char c_one = '1';
    //Seteo 1 en el bitmap de inodos segun la posicion
    fseek(file, spb.s_bm_block_start + position, SEEK_SET);
    fwrite(&c_one, sizeof(c_one), 1, file);
    fclose(file);    
}

void adm_ug::createPointerBlock(int position, string path, disco::Superblock spb){
    //Creo una variable de tipo FILE
    FILE *file = fopen(path.c_str(), "rb+");
    disco::Pointerblock tempBlock;
    fseek(file, spb.s_block_start + (position * csnt_ug.SIZE_PB), SEEK_SET);
    fwrite(&tempBlock, csnt_ug.SIZE_PB, 1, file);
    fclose(file);
}

void adm_ug::createArchiveBlock(int position, string path, disco::Superblock spb){
    //Creo una variable de tipo FILE
    FILE *file = fopen(path.c_str(), "rb+");
    disco::Archiveblock tempBlock;
    fseek(file, spb.s_block_start + (position * csnt_ug.SIZE_AB), SEEK_SET);
    fwrite(&tempBlock, csnt_ug.SIZE_AB, 1, file);
    fclose(file);
}

void adm_ug::changeStatusRow(disco::Superblock *spb, disco::Inode *inode_user, disco::Mount mount_temp, int *posicion_registro){

    bool isUpdate = false;
    //Apuntadores del 1 al 12
    for (int i = 0; i < 12; i++){
        if (inode_user->i_block[i] != -1 && isUpdate == false){
            updateContentByTypePointer("BD", inode_user->i_block[i], mount_temp.path, spb, posicion_registro, &isUpdate);
        }
    }

    //Apuntador 13 - Se apunta a una estructura de apuntadores
    if (inode_user->i_block[12] != -1 && isUpdate == false){
        updateContentByTypePointer("BSI", inode_user->i_block[12], mount_temp.path, spb, posicion_registro, &isUpdate);
    }

    //Apuntador 14 - Se apunta a una estructura de apuntadores
    if (inode_user->i_block[13] != -1 && isUpdate == false){
        updateContentByTypePointer("BDI", inode_user->i_block[13], mount_temp.path, spb, posicion_registro, &isUpdate);
    }

    //Apuntador 15 - Se apunta a una estructura de apuntadores
    if (inode_user->i_block[14] != -1 && isUpdate == false){
        updateContentByTypePointer("BTI", inode_user->i_block[14], mount_temp.path, spb, posicion_registro, &isUpdate);
    }

}

void adm_ug::updateContentByTypePointer(string tipo_puntero, int pos_block, string path, disco::Superblock *spb, int *posicion_registro, bool *isUpdate){
    FILE *file = fopen(path.c_str(), "rb+");
    if (tipo_puntero == "BD"){
        disco::Archiveblock tempArchive;
        fseek(file, spb->s_block_start + (pos_block * csnt_ug.SIZE_AB), SEEK_SET);
        fread(&tempArchive, csnt_ug.SIZE_AB, 1, file);
        
        int posicion_inicial_lectura = 0;
        for (int i = 0; i < 63; i++){
            *posicion_registro -= 1;
            posicion_inicial_lectura += 1;
            if (*posicion_registro == 0){
                break;
            }
        }
        if (posicion_inicial_lectura == 63){
            return;
        }else{
            tempArchive.b_content[posicion_inicial_lectura] = '0';
            fseek(file, spb->s_block_start + (pos_block * csnt_ug.SIZE_AB), SEEK_SET);
            fwrite(&tempArchive, csnt_ug.SIZE_AB, 1, file);
            *isUpdate = true;
        }
    }else if(tipo_puntero == "BSI"){
        disco::Pointerblock tempPointer;
        fseek(file, spb->s_block_start + (pos_block * csnt_ug.SIZE_PB), SEEK_SET);
        fread(&tempPointer, csnt_ug.SIZE_PB, 1, file);
        for (int i = 0; i < 15; i++){
            if (tempPointer.b_pointers[i] != -1){
                updateContentByTypePointer("BD", tempPointer.b_pointers[i], path, spb, posicion_registro, isUpdate);
            }
        }
    }else if(tipo_puntero == "BDI"){
        disco::Pointerblock tempPointer;
        fseek(file, spb->s_block_start + (pos_block * csnt_ug.SIZE_PB), SEEK_SET);
        fread(&tempPointer, csnt_ug.SIZE_PB, 1, file);
        for (int i = 0; i < 15; i++){
            if (tempPointer.b_pointers[i] != -1){
                updateContentByTypePointer("BSI", tempPointer.b_pointers[i], path, spb, posicion_registro, isUpdate);
            }
        }       
    }else if(tipo_puntero == "BTI"){
        disco::Pointerblock tempPointer;
        fseek(file, spb->s_block_start + (pos_block * csnt_ug.SIZE_PB), SEEK_SET);
        fread(&tempPointer, csnt_ug.SIZE_PB, 1, file);
        for (int i = 0; i < 15; i++){
            if (tempPointer.b_pointers[i] != -1){
                updateContentByTypePointer("BDI", tempPointer.b_pointers[i], path, spb, posicion_registro, isUpdate);
            }
        } 
    }
    fclose(file);
}


