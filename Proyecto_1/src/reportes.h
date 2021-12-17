#ifndef REPORTES_H
#define REPORTES_H

#include <map>
#include <vector>
#include <string>
#include "disco.h"
#include "adm_discos.h"

using namespace std;

class reportes
{
public:
    void mbr(map<string, string> param_got, vector<disco::Mount> list_mount);
    void disk(map<string, string> param_got, vector<disco::Mount> list_mount);
    void sb(map<string, string> param_got, vector<disco::Mount> list_mount);
    void journaling(map<string, string> param_got, vector<disco::Mount> list_mount);
    void bm_inode(map<string, string> param_got, vector<disco::Mount> list_mount);
    void bm_block(map<string, string> param_got, vector<disco::Mount> list_mount);
    void file(map<string, string> param_got, vector<disco::Mount> list_mount);
    disco::Mount getMount(string id, vector<disco::Mount> list_mount);
    void tree(map<string, string> param_got, vector<disco::Mount> list_mount);    
    void drawInode(disco::Inode inodo, string path, string estructura_inicial, bool isInodeInit, int encabezado_inodo, int *contador_inodo, int *contador_bloque, string *draw, string *relations, disco::Superblock *spb);
    void drawFolderBlock(disco::Folderblock carpeta, string path, string estructura_inicial, bool isInodeInit, int encabezado_bloque, int *contador_inodo, int *contador_bloque, string *draw, string *relations, disco::Superblock *spb);
    void drawArchiveBlock(disco::Archiveblock archivo, string path, string estructura_inicial, bool isInodeInit, int encabezado_bloque, int *contador_inodo, int *contador_bloque, string *draw, string *relations, disco::Superblock *spb);
    void drawPointerBlock(disco::Pointerblock apuntador, string path, string estructura_inicial, bool isInodeInit, int encabezado_bloque, string tipo_apuntador, char tipo_inodo, int *contador_inodo, int *contador_bloque, string *draw, string *relations, disco::Superblock *spb);
    void create_FileReport(string draw, string path);
    vector<string> tokenizarPath(string path, char tipo_path);
    void searchBlock(string path, int *pos_padre, int *pos_inodo, int*pos_block, disco::Superblock *spb, vector<string> *listado_path);
    char typeText(string nombre);
    string getContentArchive(int position_inode, string path, disco::Superblock spb);
    void getContentByTypePointer(string *contenido, string tipo_apuntador, string path, disco::Superblock spb, int pos_block);
    disco::Inode getInodo(int pos_inode, disco::Superblock spb, string path);
    void searchInode(string path, int *pos_padre, int *pos_inodo, disco::Superblock *spb, vector<string> *listado_path);
    typedef struct {
        int num_puntero = -1; //Numero de puntero segun la estructura
        int posicion = -1; //Posicion en la que se encuentra el inodo
        string nombre = ""; //Nombre
    } Punteros_aux;

private:
    adm_discos discosReportes;
};

#endif //REPORTES_H