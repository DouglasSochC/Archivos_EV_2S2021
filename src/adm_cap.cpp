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

    //Se verifica que el path sea correcto
    vector<string> lista_path = tokenizarPath(path, 'F');
    if (lista_path.size() == 0){
        cout << csnt_cap.RED << "ERROR:" << csnt_cap.NC << " El path ingresado es incorrecto; Los errores pueden ser 1. Debido a que excede los doce caracteres disponibles para una carpeta. 2. Es un problema de ruta " << csnt_cap.BLUE << comentario << csnt_cap.NC << endl;
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

    int pos_inodo_a_editar = 0;
    int pos_padre = -1;
    searchInode(path_montura, &pos_padre, &pos_inodo_a_editar, sp_user, &lista_path);
    disco::Inode inodoPadre = getInodo(pos_inodo_a_editar, *sp_user, path);
    
    /////////////Se verifican los permisos///////////
    int id_propietario = inodoPadre.i_uid;
    int id_grupo = inodoPadre.i_gid;
    //Se busca el grupo del usuario
    string text = getArchiveUserTXT(actualMount.part_start, actualMount.path);
    int id_grupo_UserLoggedIn = checkGroup(text, UserLoggedIn.grupo).id;
    //Se debe de buscar el id del grupo en el user.txt
    if (id_propietario == UserLoggedIn.id || id_grupo == id_grupo_UserLoggedIn){
        /* code */
    }
    //Obtener la decena
    cout << (123 / 10) % 10 << endl;
    

    //disco::Inode inodoNuevo = newInode()

    int pos_inodo_nuevo = getPositionBMInode(*sp_user, path_montura);


    cout << csnt_cap.GREEN << "RESPUESTA:" << csnt_cap.NC << " La carpeta ha sido creada correctamente " << csnt_cap.BLUE << comentario << csnt_cap.NC << endl;
}

void adm_cap::searchInode(string path, int *pos_padre, int *pos_inodo, disco::Superblock *spb, vector<string> *listado_path){
    
    //Se verifica que todavia halla un nombre que se pueda buscar
    string nombre_archivo_o_carpeta = (listado_path->size() == 0)? "": (*listado_path)[0];
    if (nombre_archivo_o_carpeta.empty()){
        return;
    }

    //Este nos servira para solo lectura
    FILE *file = fopen(path.c_str(), "rb");

    //Se obtiene el inodo
    disco::Inode *inodo = new disco::Inode();
    fseek(file, spb->s_inode_start + ((*pos_inodo) * csnt_cap.SIZE_I), SEEK_SET);
    fread(inodo, csnt_cap.SIZE_I, 1, file);
    
    //Esta variables nos sirve para que deje de buscar en otro apuntadores
    bool encontrado = false;

    if (inodo->i_type == '0'){
        //Se lee los primeros 12 apuntadores
        for (int i = 0; i < 12; i++){
            
            //En primera instancia se lee el apuntador 1 del inodo con el fin de conocer
            //el bloque de carpeta el cual contiene a su padre
            if (i == 0 && inodo->i_block[i] != -1 && encontrado == false){
                disco::Folderblock folder;
                fseek(file, spb->s_block_start + (inodo->i_block[i] * csnt_cap.SIZE_FB), SEEK_SET);
                fread(&folder, csnt_cap.SIZE_FB, 1, file);
                //i = 0: Indica la posicion del inodo del padre
                *pos_padre = folder.b_content[0].b_inodo;
                //i = 1: Indica la posicion del inodo mismo
                *pos_inodo = folder.b_content[1].b_inodo;
                //i = 2 e i = 3: Podria tener una carpeta o un archivo debido al b_name
                for (int i = 2; i < 4; i++){
                    //En el caso de encontrar un folder
                    if (folder.b_content[i].b_name == nombre_archivo_o_carpeta && typeText(nombre_archivo_o_carpeta) == 'F'){
                        //Se elimina el primer elemento del listado_path
                        listado_path->erase(listado_path->begin());
                        //Se asigna la posicion del inodo encontrado
                        *pos_inodo = folder.b_content[i].b_inodo;
                        //No hay mas datos en el path
                        if (listado_path->size() == 0){
                            encontrado = true;
                            break;
                        }
                        //Hay mas datos en el path
                        else if(listado_path->size() > 0){
                            searchInode(path, pos_padre, pos_inodo, spb, listado_path);
                        }
                    }
                    //En el caso de hallar un archivo
                    else if(folder.b_content[i].b_name == nombre_archivo_o_carpeta && typeText(nombre_archivo_o_carpeta) == 'A'){
                        listado_path->erase(listado_path->begin());
                        *pos_inodo = folder.b_content[i].b_inodo;                        
                        encontrado = true;
                        break;
                    }
                }
            }
            //Los apuntadores del 2 al 12 aqui no hay una forma de saber quien es el inodo padre
            else{
                disco::Folderblock folder;
                fseek(file, spb->s_block_start + (inodo->i_block[i] * csnt_cap.SIZE_FB), SEEK_SET);
                fread(&folder, csnt_cap.SIZE_FB, 1, file);
                for (int i = 0; i < 4; i++){
                    //En el caso de encontrar un folder
                    if (folder.b_content[i].b_name == nombre_archivo_o_carpeta && typeText(nombre_archivo_o_carpeta) == 'F'){
                        //Se elimina el primer elemento del listado_path
                        listado_path->erase(listado_path->begin());
                        //Se asigna la posicion del inodo encontrado
                        *pos_inodo = folder.b_content[i].b_inodo;
                        //No hay mas datos en el path
                        if (listado_path->size() == 0){
                            encontrado = true;
                            break;
                        }
                        //Hay mas datos en el path
                        else if(listado_path->size() > 0){
                            searchInode(path, pos_padre, pos_inodo, spb, listado_path);
                        }
                    }
                    //En el caso de hallar un archivo
                    else if(folder.b_content[i].b_name == nombre_archivo_o_carpeta && typeText(nombre_archivo_o_carpeta) == 'A'){
                        listado_path->erase(listado_path->begin());
                        *pos_inodo = folder.b_content[i].b_inodo;                        
                        encontrado = true;
                        break;
                    }
                }
            }
        }
        //Se lee el apuntador 13
        if(inodo->i_block[12] != -1 && encontrado == false){
            
        }
        //Se lee el apuntador 14
        else if(inodo->i_block[13] != -1 && encontrado == false){
            
        }
        //Se lee el apuntador 15
        else if(inodo->i_block[14] != -1 && encontrado == false){
            
        }
    }
    fclose(file);
}

disco::Inode adm_cap::getInodo(int pos_inode, disco::Superblock spb, string path){
    FILE *file = fopen(path.c_str(), "rb+");
    disco::Inode inodo_obtenido;
    fseek(file, spb.s_inode_start + (pos_inode * csnt_cap.SIZE_I), SEEK_SET);
    fread(&inodo_obtenido, csnt_cap.SIZE_I, 1, file);
    fclose(file);
    return inodo_obtenido;
}

disco::Inode adm_cap::newInode(int posicion_bloque_carpeta){
    
}

int adm_cap::getPositionBMInode(disco::Superblock spb, string path){
    int response;
    //Creo una variable de tipo FILE
    FILE *file = fopen(path.c_str(), "rb");
    char zero;
    //Seteo 0's en el bitmap de inodos
    fseek(file, spb.s_bm_inode_start, SEEK_SET);
    for (int i = 0; i < spb.s_inodes_count; i++){
        fread(&zero, sizeof(zero), 1, file);
        if (zero == '0'){
            response = i;
            break;
        }        
    }
    fclose(file);
    return response;
}

int adm_cap::getPositionBMBlock(disco::Superblock spb, string path){
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

string adm_cap::getArchiveUserTXT(int part_start_partition, string path){
    string response;
    //Creo una variable de tipo FILE
    FILE *file = fopen(path.c_str(), "rb");

    //Obtengo el super bloque
    disco::Superblock sp_user;
    fseek(file, part_start_partition, SEEK_SET);
    fread(&sp_user, csnt_cap.SIZE_SPB, 1, file);
    if (sp_user.s_filesystem_type == 0)    {
        cout << csnt_cap.RED << "ERROR:" << csnt_cap.NC << " No se ha formateado esta particion " << endl;
        return "";
    }

    //Obtengo el inodo del user.txt debido a que se debe de leer por completo
    disco::Inode inode_user;
    fseek(file, sp_user.s_inode_start + csnt_cap.SIZE_I, SEEK_SET);
    fread(&inode_user, csnt_cap.SIZE_I, 1, file);
    
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

void adm_cap::getContentByTypePointer(string *contenido, string tipo_apuntador, string path, disco::Superblock spb, int pos_block){

    FILE *file = fopen(path.c_str(), "rb");
    if (tipo_apuntador == "BD"){
        disco::Archiveblock tempArchive;
        fseek(file, spb.s_block_start + (pos_block * csnt_cap.SIZE_AB), SEEK_SET);
        fread(&tempArchive, csnt_cap.SIZE_AB, 1, file);
        *contenido += tempArchive.b_content;
    }else if(tipo_apuntador == "BSI"){
        disco::Pointerblock tempPointer;
        fseek(file, spb.s_block_start + (pos_block * csnt_cap.SIZE_AB), SEEK_SET);
        fread(&tempPointer, csnt_cap.SIZE_PB, 1, file);
        for (int i = 0; i < 15; i++){
            if (tempPointer.b_pointers[i] != -1){
                getContentByTypePointer(contenido, "BD", path, spb, tempPointer.b_pointers[i]);
            }
        }
    }else if(tipo_apuntador == "BDI"){
        disco::Pointerblock tempPointer;
        fseek(file, spb.s_block_start + (pos_block * csnt_cap.SIZE_AB), SEEK_SET);
        fread(&tempPointer, csnt_cap.SIZE_PB, 1, file);
        for (int i = 0; i < 15; i++){
            if (tempPointer.b_pointers[i] != -1){
                getContentByTypePointer(contenido, "BSI", path, spb, tempPointer.b_pointers[i]);
            }
        }       
    }else if(tipo_apuntador == "BTI"){
        disco::Pointerblock tempPointer;
        fseek(file, spb.s_block_start + (pos_block * csnt_cap.SIZE_AB), SEEK_SET);
        fread(&tempPointer, csnt_cap.SIZE_PB, 1, file);
        for (int i = 0; i < 15; i++){
            if (tempPointer.b_pointers[i] != -1){
                getContentByTypePointer(contenido, "BDI", path, spb, tempPointer.b_pointers[i]);
            }
        } 
    }
    fclose(file);
}

disco::Group adm_cap::checkGroup(string user_txt, string nombre){
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

vector<string> adm_cap::tokenizarPath(string path, char tipo_path){
    vector<string> response;
    if (path.substr(0, 1) != "/"){
        return response;
    }
    if (path.substr(0, path.size()) != "/"){
        path += "/";
    }

    int inicio = 1;
    int tamanio = 0;
    bool ya_existe_un_archivo = false;
    for (int i = 1; i < path.size(); i++){
        if (path.substr(i, 1) == "/"){
            string nombre = path.substr(inicio, tamanio);
            if (nombre.size() > 12){
                response.clear();
                break;
            }
            if (nombre.size() == 0){
                response.clear();
                break;
            }            
            char tipo = typeText(nombre);
            if (tipo == 'E'){
                response.clear();
                break;
            }else if(tipo == 'F' && !ya_existe_un_archivo){
                response.push_back(nombre);
                inicio = i+1;
                tamanio = 0;
            }else if(tipo == 'F' && ya_existe_un_archivo){
                response.clear();
                break;
            }else if(tipo == 'A' && tipo_path == 'F'){
                response.clear();
                break;
            }else if(tipo == 'A' && !ya_existe_un_archivo){
                response.push_back(nombre);
                inicio = i+1;
                tamanio = 0;
                ya_existe_un_archivo = true;
            }else if(tipo == 'A' && ya_existe_un_archivo){
                response.clear();
                break;
            }
        }else{
            tamanio++;
        }
    }    
    if (response.size() > 0 && tipo_path == 'A' && typeText(response[response.size()-1]) != 'A'){
        response.clear();
    }
    return response;
}

char adm_cap::typeText(string nombre){
    char response = '\0';
    int cantidad_puntos = 0;
    for (int i = 0; i < nombre.size(); i++){
        if (nombre.substr(i, 1) == "."){
            cantidad_puntos++;
        }        
    }
    if (cantidad_puntos == 0){
        response = 'F';
    }else if(cantidad_puntos == 1){
        response = 'A';
    }else if(cantidad_puntos > 1){
        response = 'E';
    }
    return response;
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