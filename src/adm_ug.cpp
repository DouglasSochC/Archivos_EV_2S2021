#include <iostream>
#include <map>
#include <cmath>
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
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " Ya hay una sesion activa" << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
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

    //Se crean variables que se utilizaran posteriormente
    string path = actualMount.path;
    string name = actualMount.name;
    int part_start_partition = -1;

    //Se obtiene el MBR de la particion montada
    disco::MBR mbr = admdcs_admug.getMBR(path);
    if (mbr.mbr_tamano == -1){
        cout << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return UserLoggedIn;
    }
    
    //Se verifica que tipo de particion es, para obtener sus caracteristicas
    char foundName = admdcs_admug.find_namePartition(name, path);
    if (foundName == 'P' || foundName == 'E'){
        int before = -1; int after = -1;
        disco::Partition tempPartition = admdcs_admug.getPartitionEP(mbr, name, &before, &before);
        part_start_partition = tempPartition.part_start;
    }else if(foundName == 'L'){
        disco::Partition partitionExtended;
        vector<disco::Partition> listPartitions = admdcs_admug.getListPartitionsEP(mbr);
        for (int i = 0; i < listPartitions.size(); i++){
            if (listPartitions[i].part_type == 'E'){
                partitionExtended = listPartitions[i];
                break;
            }            
        }
        int before = -1;
        disco::EBR tempPartition = admdcs_admug.getPartitionL(partitionExtended, path, name, &before);
        part_start_partition = tempPartition.part_start + csnt_ug.SIZE_EBR;
    }else{
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " No se ha encontrado el nombre de la particion " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
        return UserLoggedIn;
    }
    
    string text = getArchiveUserTXT(part_start_partition, path);
    disco::User response = checkUser(text, usr, pwd);

    if (response.id != -1){
        cout << csnt_ug.GREEN << "RESPUESTA:" << csnt_ug.NC << " ¡Bienvenido! " << usr << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
    }else{
        cout << csnt_ug.RED << "ERROR:" << csnt_ug.NC << " No existe un usuario con el username y contraseña ingresada " << csnt_ug.BLUE << comentario << csnt_ug.NC << endl;
    }
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
    
    //Se lee todo lo que tenga el inodo de tipo archivo, creando al principio una variable
    disco::Archiveblock tempArchive;
    //Apuntadores del 1 al 12
    for (int i = 0; i < 12; i++){
        if (inode_user.i_block[i] != -1){
            fseek(file, sp_user.s_block_start + (inode_user.i_block[i] * csnt_ug.SIZE_AB), SEEK_SET);
            fread(&tempArchive, csnt_ug.SIZE_AB, 1, file);
            response += tempArchive.b_content;
        }
    }

    //Apuntador 13 - Se apunta a una estructura de apuntadores
    if (inode_user.i_block[12] == -1){
        disco::Pointerblock tempPointer;
        fseek(file, sp_user.s_block_start + (inode_user.i_block[12] * csnt_ug.SIZE_AB), SEEK_SET);
        fread(&tempPointer, csnt_ug.SIZE_PB, 1, file);
        for (int i = 0; i < 15; i++){
            if (tempPointer.b_pointers[i] != -1){
                fseek(file, sp_user.s_block_start + (tempPointer.b_pointers[i] * csnt_ug.SIZE_AB), SEEK_SET);
                fread(&tempArchive, csnt_ug.SIZE_AB, 1, file);
                response += tempArchive.b_content;
            }
        }        
    }

    //Apuntador 14 - FALTA ANALISARLO

    //Apuntador 15 - FALTA ANALISARLO
    fclose(file);
    return response;
}

disco::User adm_ug::checkUser(string user_txt, string user, string psw){
    
    disco::User response;
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

            if (temp[1] == "U"){
                if (temp[3] == user && temp[4] == psw){
                    response.usuario = user;
                    response.contrasenia = psw;
                    response.grupo = temp[2];
                    response.id = atoi(temp[0].c_str());
                    response.tipo = temp[1][0];
                    break;
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