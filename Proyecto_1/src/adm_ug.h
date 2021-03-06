#ifndef ADM_UG_H
#define ADM_UG_H

#include <string>
#include <map>
#include <vector>
#include "disco.h"

using namespace std;

class adm_ug
{
public:
    /*METODOS PRINCIPALES*/

    disco::User login(map<string, string> param_got, vector<disco::Mount> list_mount, disco::User UserLoggedIn);
    disco::User logout(disco::User UserLoggedIn);
    void mkgrp(map<string, string> param_got, disco::User UserLoggedIn);
    void rmgrp(map<string, string> param_got, disco::User UserLoggedIn);
    void mkusr(map<string, string> param_got, disco::User UserLoggedIn);
    void rmusr(map<string, string> param_got, disco::User UserLoggedIn);
    /*METODOS AUXILIARES*/
    
    string getArchiveUserTXT(int part_start_partition, string path);
    disco::User checkUser(string user_txt, string user, string psw, bool needPassword);
    disco::Group checkGroup(string user_txt, string nombre);
    void getContentByTypePointer(string *contenido, string tipo_apuntador, string path, disco::Superblock spb, int pos_block);
    void insertJournal(string contenido, char operacion, int permiso, disco::Mount mount_temp, disco::Superblock spb);
    
    /*
    Este inserta registros en un inodo de tipo archivo
    */
    void insertDatainInodeArchive(string *registro, disco::Superblock *spb, disco::Inode *inode_user, disco::Mount mount_temp);
    /*
    Este metodo realiza la insercion de datos dentro de un inodo de tipo archivo.
    tipo_puntero: Este nos indica que tipo de puntero se esta leendo
    estructura_a_modificar: Nos indica que tipo de estructura va a modificar sus punteros; I = Inodo; P = Pointer
    */
    void setContentByTypePointer(string tipo_puntero, int pos_block, string path, char estructura_a_modificar, disco::Superblock *spb, disco::Inode *inode_user, disco::Pointerblock *pointer_user, string *registro);
    void setPositionBMBlock(int position, string path, disco::Superblock spb);
    int getPositionBMBlock(disco::Superblock spb, string path);
    /*
    Este cambia el estatus 1 a 0 de un registro dependiendo de la posicion en la que se encuentra
    */
    void changeStatusRow(disco::Superblock *spb, disco::Inode *inode_user, disco::Mount mount_temp, int *posicion_registro);
    void updateContentByTypePointer(string tipo_puntero, int pos_block, string path, disco::Superblock *spb, int *posicion_registro, bool *isUpdate);

    void setContentByTypePointer(string tipo_apuntador, string path, int pos_block, string *contenido, disco::Superblock *spb, disco::Inode *inodo);
    void createPointerBlock(int position, string path, disco::Superblock spb);
    void createArchiveBlock(int position, string path, disco::Superblock spb);
};

#endif //ADM_UG_H
