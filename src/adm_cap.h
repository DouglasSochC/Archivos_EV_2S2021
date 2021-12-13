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
    
    //FALTA ANALIZAR LOS APUNTADORES INDIRECTOS
    void searchInode(string path, int *pos_padre, int *pos_inodo, disco::Superblock *spb, vector<string> *listado_path);
    /*
        Este metodo se encarga de buscar un espacio libre entre sus 15 apuntadores y asi pueda
        insertar un bloque de tipo carpeta y a este nuevo bloque de tipo carpeta se le asigna en su
        primer apuntador la posicion en la que se encuentra el nuevo inodo a ingresar
    */
    void editInodeFolder(disco::Inode inodo_a_editar, int pos_inodo_a_editar, int pos_nuevo_inodo_a_insertar, string nombre_inodo_a_insertar, string path, bool *encontro_puntero_disponible, disco::Superblock *spb, int *pos_block);
    /*
        Este metodo se encarga de crear una nueva carpeta con este procedimiento:
        1. Crea y almacena un bloque de tipo carpeta en el cual sus primeros el apuntador 1 
        apunta a su padre y el apuntador 2 apunta a su mismo inodo
        2. Crea y almacena un inodo en el cual su apuntador 1 apunta al bloque creado en el 
        inciso anterior
        3. Se disminuye la cantidad de bloques e inodos en una unidad y se asignan nuevas 
        posiciones del primer inodo y bloque libre
        Nota: Eso quiere decir que por lo menos es necesario tener un inodo y un bloque de espacio
        disponible en la particion para poder utilizar este metodo
    */
    void createNewFolder(int pos_nuevo_inodo_a_insertar, int pos_padre_del_inodo_a_insertar, string path, int pos_bloque_nuevo_a_insertar, int id_propietario_carpeta_nueva, int id_grupo_carpeta_nueva, disco::Superblock *spb);
    /*
        Se obtiene la estructura de un inodo, dependiendo de la posicion que se le asigne
        a traves del parametro 'pos_inode'
    */
    disco::Inode getInodo(int pos_inode, disco::Superblock spb, string path);
    /*
        Retorna una estructura de tipo Group con los datos de un grupo encontrado segun su nombre, 
        este necesita del texto completo del archivo user.txt
    */
    disco::Group checkGroup(string user_txt, string nombre);
    /*
        Este es un metodo auxiliar del metodo 'getArchiveUserTXT', aqui selecciona un rumbo de
        busqueda dependiendo del tipo de puntero que se ha escogido
    */
    void getContentByTypePointer(string *contenido, string tipo_apuntador, string path, disco::Superblock spb, int pos_block);
    /*
        Retorna el texto completo que tiene el archivo user.txt
    */
    string getArchiveUserTXT(int part_start_partition, string path);
    /*
        Este retorna la primera posicion disponible que encuentre en el bitmap de inodos
    */
    int getPositionBMInode(disco::Superblock spb, string path);
    /*
        Se setea un 1 en el bitmap de bloques en la posicion fijada por la variable position
    */
    void setPositionBMBlock(int position, string path, disco::Superblock spb);
    /*
        Se setea un 1 en el bitmap de inodos en la posicion fijada por la variable position
    */
    void setPositionBMInode(int position, string path, disco::Superblock spb);
    /*
        Este retorna la primera posicion disponible que encuentre en el bitmap de bloques
    */
    int getPositionBMBlock(disco::Superblock spb, string path);
    /*
        Este retorna un listado de string los cuales son los nombres separados por una '/', este
        dependera de que tipo de path se esta anlizando para que retorne el listado completo
        del path. En el caso de encontrar un error en el path, este metodo retorna un listado vacio.
        Valor para el parametro 'tipo_path' = A: Archive; F: Folder
    */
    vector<string> tokenizarPath(string path, char tipo_path);
    /*
        Este retorna el tipo de nombre que se esta ingresando, los valores que puede 
        retornar son: F = Folder; A = Archive; E = Error
    */
    char typeText(string nombre);
    /*
        Inserta el journal de la operacion
    */
    void insertJournal(string contenido, char operacion, int permiso, char tipo,  string nombre, string propietario, disco::Mount mount_temp, disco::Superblock spb);
    /*
        Este retorna el valor completo de un vector del path
    */
    string vectorToString(vector<string> path);
};

#endif //ADM_CAP_H