#ifndef ADM_CAP_H
#define ADM_CAP_H

#include <string>
#include <map>
#include <vector>

using namespace std;

class adm_cap
{
public:

    void mkdir(map<string, string> param_got, disco::User UserLoggedIn);
    void cat(map<string, string> param_got, disco::Mount partitionMount, disco::User UserLoggedIn);

    void searchInode(string path, int *pos_padre, int *pos_inodo, disco::Superblock *spb, vector<string> *listado_path);
    void showPointerBlock();
    void editInode(int pos_inode, disco::Superblock spb, string path);
    disco::Inode newInode(int posicion_bloque_carpeta);
    disco::Inode getInodo(int pos_inode, disco::Superblock spb, string path);
    string getArchiveUserTXT(int part_start_partition, string path);
    disco::Group checkGroup(string user_txt, string nombre);
    void getContentByTypePointer(string *contenido, string tipo_apuntador, string path, disco::Superblock spb, int pos_block);
    /*
        Este retorna la primera posicion disponible que encuentre en el bitmap de inodos
    */
    int getPositionBMInode(disco::Superblock spb, string path);
    /*
        Este retorna la primera posicion disponible que encuentre en el bitmap de bloques
    */
    int getPositionBMBlock(disco::Superblock spb, string path);
    /*
        tipo_path = A: Archive; F: Folder
    */
    vector<string> tokenizarPath(string path, char tipo_path);
    /*
        F = Folder; A = Archive; E = Error
    */
    char typeText(string nombre);
};

#endif //ADM_CAP_H