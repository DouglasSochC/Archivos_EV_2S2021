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
    bool existencia_p = false;
    if (param_got.find("-p") == param_got.end()) {
        //NO SE ENCONTRO
    } else {
        existencia_p = true;
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
        fclose(file);
        return;
    }

    vector <vector<string>> path_niveles;
    vector <string> temporal;
    for (int i = 0; i < lista_path.size(); i++){
        temporal.push_back(lista_path[i]);
        path_niveles.push_back(temporal);
    }
    temporal.clear();

    //Se verificara hasta que diagonal del path ya no encuentra alguna carpeta
    int diagonal = -1;
    for (int i = 0; i < path_niveles.size(); i++){
        //Archivos y Carpetas no Encontrados
        vector <string> ACE = path_niveles[i];
        int pos_inodo_a_editar = 0;
        int pos_padre = -1;
        searchInode(path_montura, &pos_padre, &pos_inodo_a_editar, sp_user, &ACE);
        if (ACE.size() > 0){
            diagonal = i;
            break;
        }
    }

    //Se verifica si hay una carpeta que no se ha ingresado segun el path del comando
    if(diagonal == -1){
        cout << csnt_cap.RED << "ERROR:" << csnt_cap.NC << " El path ya esta ingresado dentro del sistema de archivos " << csnt_cap.BLUE << comentario << csnt_cap.NC << endl;
        fclose(file);
        return;
    }

    //Se busca cuantas carpetas falta por ingresar
    int diferencia = path_niveles.size() - diagonal;
    //Se verifica si hace falta ingresar mas de una carpeta y no se posee el parametro -p
    if (diferencia > 1 && !existencia_p){
        cout << csnt_cap.RED << "ERROR:" << csnt_cap.NC << " No se puede ingresar la carpeta debido a que no existe el path dentro del sistema de archivos " << csnt_cap.BLUE << comentario << csnt_cap.NC << endl;
        fclose(file);
        return;
    }

    bool response = false;
    for (int i = diagonal; i < path_niveles.size(); i++){
        //Archivos y Carpetas no Encontrados
        vector <string> ACE = path_niveles[i];
        vector <string> lista_path_completo = path_niveles[i];
        int pos_inodo_a_editar = 0;
        int pos_padre = -1;
        searchInode(path_montura, &pos_padre, &pos_inodo_a_editar, sp_user, &ACE);
        disco::Inode inodoPadre = getInodo(pos_inodo_a_editar, *sp_user, path_montura);
        string nombre_inodo_nuevo = ACE[0];

        /*Inicio - Verificacion de Permisos*/
        int id_propietario_inodo = inodoPadre.i_uid;
        int id_grupo_inodo = inodoPadre.i_gid;
        int permiso_padre_inodo = inodoPadre.i_perm;
        int permiso_propietario_inodo = (permiso_padre_inodo / 100) % 100;
        int permiso_grupo_inodo = (permiso_padre_inodo / 10) % 10;
        int permiso_otro_inodo = permiso_padre_inodo % 10;
        
        //Se busca el grupo del usuario
        string text = getArchiveUserTXT(actualMount.part_start, actualMount.path);
        //Se busca el id del grupo en el user.txt
        int id_grupo_UserLoggedIn = checkGroup(text, UserLoggedIn.grupo).id;
        int id_propietario_UserLoggedIn = UserLoggedIn.id;
        
        //Se verifica que tipo de permiso posee
        int tipo_permiso = -1;
        if (id_propietario_inodo == UserLoggedIn.id){
            tipo_permiso = permiso_propietario_inodo;
        }else if(id_grupo_inodo == id_grupo_UserLoggedIn){
            tipo_permiso = permiso_grupo_inodo;
        }else {
            tipo_permiso = permiso_otro_inodo;
        }
        //Se define todos los permisos para el usuario root
        tipo_permiso = isRoot ? 7: tipo_permiso;
        
        if (!(tipo_permiso == 2 || tipo_permiso == 3 || tipo_permiso == 6 || tipo_permiso == 7)){
            cout << csnt_cap.RED << "ERROR:" << csnt_cap.NC << " No se posee permisos de escritura " << csnt_cap.BLUE << comentario << csnt_cap.NC << endl;
            break;
        }
        /*Final - Verificacion de Permisos*/
                
        //Posiciones de los inodos y bloques a insertar
        int pos_nuevo_inodo_a_insertar = sp_user->s_first_ino;
        int pos_bloque_nuevo = sp_user->s_first_blo;
        bool encontro_puntero_disponible = false;
        editInodeFolder(comentario, inodoPadre, pos_inodo_a_editar, pos_nuevo_inodo_a_insertar, nombre_inodo_nuevo, path_montura, &encontro_puntero_disponible, sp_user, &pos_bloque_nuevo);
        if (encontro_puntero_disponible){
            if (sp_user->s_filesystem_type == 3){
                insertJournal(vectorToString(lista_path_completo), 'C', 664, '0', nombre_inodo_nuevo, UserLoggedIn.usuario, actualMount, *sp_user);
            }
            createNewFolder(pos_nuevo_inodo_a_insertar, pos_inodo_a_editar, path_montura, pos_bloque_nuevo, id_propietario_UserLoggedIn, id_grupo_UserLoggedIn, sp_user);
            fseek(file, actualMount.part_start, SEEK_SET);
            fwrite(sp_user, csnt_cap.SIZE_SPB, 1, file);
            response = true;
        }
    }
    
    if (response){
        cout << csnt_cap.GREEN << "RESPUESTA:" << csnt_cap.NC << " La carpeta ha sido creada correctamente " << csnt_cap.BLUE << comentario << csnt_cap.NC << endl;
    }

    fclose(file);

}

void adm_cap::editInodeFolder(string comentario, disco::Inode inodo_a_editar, int pos_inodo_a_editar, int pos_nuevo_inodo_a_insertar, string nombre_inodo_a_insertar, string path, bool *encontro_puntero_disponible, disco::Superblock *spb, int *pos_block){
    
    //Apuntadores del 1 al 12
    for (int i = 0; i < 12; i++){
        //Se verifica la existencia de un apuntador
        if(inodo_a_editar.i_block[i] != -1 && !(*encontro_puntero_disponible)){
            ////PARA TEST DE LOS DEMAS APUNTADORES ES NECESARIO COMENTAR ESTE BLOQUE DE CODIGO////
            /*
                Como minimo debe de haber espacio disponible: 1 Inodo y 1 Bloque
                Inodo1 = Es el nuevo inodo de tipo carpeta a crear
                Bloque1 = Es el bloque de tipo carpeta que tendra el inodo 1
            */
            if (spb->s_free_blocks_count <= 0 && spb->s_free_inodes_count <= 0){
                cout << csnt_cap.RED << "ERROR:" << csnt_cap.NC << " No hay mas espacio para ingresar carpetas o archivos " << csnt_cap.BLUE << comentario << csnt_cap.NC << endl;
                break;
            }else{
                FILE *file = fopen(path.c_str(), "rb+");
                //Se lee el bloque de tipo carpeta
                disco::Folderblock carpeta;            
                fseek(file, spb->s_block_start + (inodo_a_editar.i_block[i] * csnt_cap.SIZE_FB), SEEK_SET);
                fread(&carpeta, csnt_cap.SIZE_FB, 1, file);
                for (int i = 0; i < 4; i++){
                    if (carpeta.b_content[i].b_inodo == -1){
                        *encontro_puntero_disponible = true;
                        //Se asigna la posicion del bloque de carpeta al puntero del inodo
                        carpeta.b_content[i].b_inodo = pos_nuevo_inodo_a_insertar;
                        strcpy(carpeta.b_content[i].b_name, nombre_inodo_a_insertar.c_str());
                        break;
                    }
                }
                //Se actualiza
                fseek(file, spb->s_block_start + (inodo_a_editar.i_block[i] * csnt_cap.SIZE_FB), SEEK_SET);
                fwrite(&carpeta, csnt_cap.SIZE_FB, 1, file);
                fclose(file);
            }
        }
        //Si no existe un bloque de carpetas este lo crea
        else if(inodo_a_editar.i_block[i] == -1 && !(*encontro_puntero_disponible)){
            /*
                Como minimo debe de haber espacio disponible: 1 Inodo y 2 Bloques
                Bloque1 = Es el nuevo bloque de tipo carpeta en donde su primer apuntador apunta al Inodo1
                Inodo1 = Es el nuevo inodo de tipo carpeta a crear
                Bloque2 = Es el bloque de tipo carpeta que tendra el inodo 1 en su primer apuntador                
            */
            if (spb->s_free_blocks_count <= 1 && spb->s_free_inodes_count <= 0){
                cout << csnt_cap.RED << "ERROR:" << csnt_cap.NC << " No hay mas espacio para ingresar carpetas o archivos en esta particion " << csnt_cap.BLUE << comentario << csnt_cap.NC << endl;
                break;
            }else{
                *encontro_puntero_disponible = true;
                FILE *file = fopen(path.c_str(), "rb+");
                //Se crea el nuevo bloque de carpetas
                disco::Folderblock nuevo_bloque_carpeta;
                nuevo_bloque_carpeta.b_content[0].b_inodo = pos_nuevo_inodo_a_insertar;
                strcpy(nuevo_bloque_carpeta.b_content[0].b_name, nombre_inodo_a_insertar.c_str());
                //Se escribe el bloque de carpeta
                fseek(file, spb->s_block_start + ((*pos_block) * csnt_cap.SIZE_FB), SEEK_SET);
                fwrite(&nuevo_bloque_carpeta, csnt_cap.SIZE_FB, 1, file);
                //Se almacena el bloque de carpeta
                fclose(file);
                //Se asigna la posicion del bloque de carpeta al puntero del inodo
                inodo_a_editar.i_block[i] = (*pos_block);
                //Se setea un 1 en ese espacio disponible para confirmar su utilizacion en esa posicion
                setPositionBMBlock(*pos_block, path, *spb);
                //Se asigna una nueva posicion a la posicion del bloque
                *pos_block = getPositionBMBlock(*spb, path);
                spb->s_free_blocks_count -= 1;
            }
        }
    }

    //Apuntador 13 - Se apunta a una estructura de apuntadores
    //En el caso que encontro un apuntador que esta siendo utilizado y este apunta a un bloque de punteros
    if (inodo_a_editar.i_block[12] != -1 && !(*encontro_puntero_disponible)){
        /*
            Como minimo debe de haber espacio disponible: 1 Inodo y 2 Bloques
            Bloque1 = Es el nuevo bloque de tipo carpeta en donde su primer apuntador apunta al Inodo1, este es en el caso de que no se encuentre un bloque tipo carpeta
            Inodo1 = Es el nuevo inodo de tipo carpeta a crear
            Bloque2 = Es el bloque de tipo carpeta que tendra el inodo 1 en su primer apuntador         
        */
        if (spb->s_free_blocks_count <= 1 && spb->s_free_inodes_count <= 0){
            cout << csnt_cap.RED << "ERROR:" << csnt_cap.NC << " No hay mas espacio para ingresar carpetas o archivos en esta particion " << csnt_cap.BLUE << comentario << csnt_cap.NC << endl;
        }else{
            editBlockPointer("BSI", comentario, inodo_a_editar.i_block[12], pos_nuevo_inodo_a_insertar, nombre_inodo_a_insertar, path, encontro_puntero_disponible, spb, pos_block);
        }
    }
    //En este caso lo crea
    else if(inodo_a_editar.i_block[12] == -1 && !(*encontro_puntero_disponible)){
        /*
            Como minimo debe de haber espacio disponible: 1 Inodo y 3 Bloques
            Bloque1 = Es el nuevo bloque de tipo apuntador en donde su primer apuntador apunta al Bloque2
            Bloque2 = Es el nuevo bloque de tipo carpeta en donde su primer apuntador apunta al Inodo1
            Inodo1 = Es el nuevo inodo de tipo carpeta a crear
            Bloque3 = Es el bloque de tipo carpeta que tendra el inodo 1 en su primer apuntador                
        */
        if (spb->s_free_blocks_count <= 2 && spb->s_free_inodes_count <= 0){
            cout << csnt_cap.RED << "ERROR:" << csnt_cap.NC << " No hay mas espacio para ingresar carpetas o archivos en esta particion " << csnt_cap.BLUE << comentario << csnt_cap.NC << endl;
        }else{
            *encontro_puntero_disponible = true;
            FILE *file = fopen(path.c_str(), "rb+");
            //Se crea el nuevo bloque de apuntadores
            disco::Pointerblock nuevo_bloque_puntero;
            //Se escribe el bloque de apuntador
            fseek(file, spb->s_block_start + ((*pos_block) * csnt_cap.SIZE_PB), SEEK_SET);
            fwrite(&nuevo_bloque_puntero, csnt_cap.SIZE_PB, 1, file);
            //Se almacena el bloque de apuntadores
            fclose(file);
            //Se asigna la posicion del bloque de punteros al puntero 12 del inodo
            inodo_a_editar.i_block[12] = (*pos_block);
            int pos_bloque_a_editar = (*pos_block);
            //Se setea un 1 en ese espacio disponible para confirmar su utilizacion en esa posicion
            setPositionBMBlock(*pos_block, path, *spb);
            //Se asigna una nueva posicion a la posicion del bloque
            *pos_block = getPositionBMBlock(*spb, path);
            spb->s_free_blocks_count -= 1;
            //Lo que se debe de hacer a continuacion es editar el bloque
            editBlockPointer("BSI", comentario, pos_bloque_a_editar, pos_nuevo_inodo_a_insertar, nombre_inodo_a_insertar, path, encontro_puntero_disponible, spb, pos_block);            
        }        
    }

    //Apuntador 14 - Se apunta a una estructura de apuntadores
    if (inodo_a_editar.i_block[13] != -1 && !(*encontro_puntero_disponible)){
        //############################################################################
    }

    //Apuntador 15 - Se apunta a una estructura de apuntadores
    if (inodo_a_editar.i_block[14] != -1 && !(*encontro_puntero_disponible)){
        //############################################################################
    }

    if (*encontro_puntero_disponible){
        FILE *file = fopen(path.c_str(), "rb+");
        fseek(file, spb->s_inode_start + (pos_inodo_a_editar * csnt_cap.SIZE_I), SEEK_SET);
        fwrite(&inodo_a_editar, csnt_cap.SIZE_I, 1, file);
        fclose(file);
    }
}

void adm_cap::editBlockPointer(string tipo_bloque, string comentario, int pos_bloque_a_editar, int pos_nuevo_inodo_a_insertar, string nombre_inodo_a_insertar, string path, bool *encontro_puntero_disponible, disco::Superblock *spb, int *pos_block){
    
    bool puntero_disponible = false;
    if (tipo_bloque == "BSI"){
        FILE *file = fopen(path.c_str(), "rb+");
        //Se lee el bloque de apuntadores
        disco::Pointerblock bloque_puntero;
        fseek(file, spb->s_block_start + (pos_bloque_a_editar * csnt_cap.SIZE_PB), SEEK_SET);
        fread(&bloque_puntero, csnt_cap.SIZE_PB, 1, file);

        for (int i = 0; i < 15; i++){
            if (bloque_puntero.b_pointers[i] != -1 && !puntero_disponible){
                ////PARA TEST DE LOS DEMAS APUNTADORES ES NECESARIO COMENTAR ESTE BLOQUE DE CODIGO////
                disco::Folderblock bloque_carpeta;
                //Se lee el bloque de carpeta
                fseek(file, spb->s_block_start + ((bloque_puntero.b_pointers[i]) * csnt_cap.SIZE_FB), SEEK_SET);
                fread(&bloque_carpeta, csnt_cap.SIZE_FB, 1, file);
                for (int i = 0; i < 4; i++){
                    if (bloque_carpeta.b_content[i].b_inodo == -1 && !puntero_disponible){
                        *encontro_puntero_disponible = true;
                        puntero_disponible = true;
                        bloque_carpeta.b_content[i].b_inodo = pos_nuevo_inodo_a_insertar;
                        strcpy(bloque_carpeta.b_content[i].b_name, nombre_inodo_a_insertar.c_str());
                    }
                }
                fseek(file, spb->s_block_start + ((bloque_puntero.b_pointers[i]) * csnt_cap.SIZE_FB), SEEK_SET);
                fwrite(&bloque_carpeta, csnt_cap.SIZE_FB, 1, file);
            }else if(bloque_puntero.b_pointers[i] == -1 && !puntero_disponible){
                *encontro_puntero_disponible = true;
                puntero_disponible = true;
                bloque_puntero.b_pointers[i] = (*pos_block);
                disco::Folderblock nuevo_bloque_carpeta;
                nuevo_bloque_carpeta.b_content[0].b_inodo = pos_nuevo_inodo_a_insertar;
                strcpy(nuevo_bloque_carpeta.b_content[0].b_name, nombre_inodo_a_insertar.c_str());
                //Se escribe el bloque de carpeta
                fseek(file, spb->s_block_start + ((*pos_block) * csnt_cap.SIZE_FB), SEEK_SET);
                fwrite(&nuevo_bloque_carpeta, csnt_cap.SIZE_FB, 1, file);
                //Se setea un 1 en ese espacio disponible para confirmar su utilizacion en esa posicion
                setPositionBMBlock(*pos_block, path, *spb);
                //Se asigna una nueva posicion a la posicion del bloque
                *pos_block = getPositionBMBlock(*spb, path);
                spb->s_free_blocks_count -= 1;
            }
        }   
        
        //Se almacena el bloque de apuntadores
        fseek(file, spb->s_block_start + (pos_bloque_a_editar * csnt_cap.SIZE_PB), SEEK_SET);
        fwrite(&bloque_puntero, csnt_cap.SIZE_PB, 1, file);
        fclose(file);
    }
    //
    else if(tipo_bloque == "BDI"){

    }
    //
    else if(tipo_bloque == "BTI"){

    }
}

void adm_cap::createNewFolder(int pos_nuevo_inodo_a_insertar, int pos_padre_del_inodo_a_insertar, string path, int pos_bloque_nuevo_a_insertar, int id_propietario_carpeta_nueva, int id_grupo_carpeta_nueva, disco::Superblock *spb){
    FILE *file = fopen(path.c_str(), "rb+");
    //Se crea una estructura de la nueva carpeta
    disco::Folderblock nueva_carpeta;
    //Padre
    strcpy(nueva_carpeta.b_content[0].b_name, ".");
    nueva_carpeta.b_content[0].b_inodo = pos_padre_del_inodo_a_insertar;
    //Carpeta hija
    strcpy(nueva_carpeta.b_content[1].b_name, "..");
    nueva_carpeta.b_content[1].b_inodo = pos_nuevo_inodo_a_insertar;
    //Se almacena el bloque de carpeta
    fseek(file, spb->s_block_start + (pos_bloque_nuevo_a_insertar * csnt_cap.SIZE_FB), SEEK_SET);
    fwrite(&nueva_carpeta, csnt_cap.SIZE_FB, 1, file);

    //Se crea una estructura del inodo
    disco::Inode nuevo_inodo;
    nuevo_inodo.i_uid = id_propietario_carpeta_nueva;
    nuevo_inodo.i_gid = id_grupo_carpeta_nueva;
    nuevo_inodo.i_size = 0;
    nuevo_inodo.i_ctime = time(nullptr);
    nuevo_inodo.i_mtime = time(nullptr);
    nuevo_inodo.i_atime = 0;
    nuevo_inodo.i_type = '0';
    nuevo_inodo.i_perm = 664;
    nuevo_inodo.i_block[0] = pos_bloque_nuevo_a_insertar;
    //Se almacena el inodo
    fseek(file, spb->s_inode_start + (pos_nuevo_inodo_a_insertar * csnt_cap.SIZE_I), SEEK_SET);
    fwrite(&nuevo_inodo, csnt_cap.SIZE_I, 1, file);

    //Se disminuye la cantidad de inodos y bloques
    spb->s_free_blocks_count -= 1;
    spb->s_free_inodes_count -= 1;

    //Se setea un 1 a las posiciones del bitmap de inodos y bloques para que ya no sean tomadas en cuenta
    setPositionBMInode(pos_nuevo_inodo_a_insertar, path, *spb);
    setPositionBMBlock(pos_bloque_nuevo_a_insertar, path, *spb);

    //Se asigna nuevas posiciones disponibles para el inodo y bloque
    spb->s_first_ino = getPositionBMInode(*spb, path);
    spb->s_first_blo = getPositionBMBlock(*spb, path);

    //Se cierra el FILE
    fclose(file);
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
            else if(inodo->i_block[i] != -1 && encontrado == false){
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
            //#############################################################
        }
        //Se lee el apuntador 14
        else if(inodo->i_block[13] != -1 && encontrado == false){
            //#############################################################
        }
        //Se lee el apuntador 15
        else if(inodo->i_block[14] != -1 && encontrado == false){
            //#############################################################
        }
    }
    fclose(file);
}

disco::Inode adm_cap::getInodo(int pos_inode, disco::Superblock spb, string path){
    disco::Inode inodo_obtenido;
    FILE *file = fopen(path.c_str(), "rb");
    fseek(file, spb.s_inode_start + (pos_inode * csnt_cap.SIZE_I), SEEK_SET);
    fread(&inodo_obtenido, csnt_cap.SIZE_I, 1, file);
    fclose(file);
    return inodo_obtenido;
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

void adm_cap::setPositionBMBlock(int position, string path, disco::Superblock spb){
    
    //Creo una variable de tipo FILE
    FILE *file = fopen(path.c_str(), "rb+");
    char c_one = '1';
    //Seteo 1 en el bitmap de inodos segun la posicion
    fseek(file, spb.s_bm_block_start + position, SEEK_SET);
    fwrite(&c_one, sizeof(c_one), 1, file);
    fclose(file);    
}

void adm_cap::setPositionBMInode(int position, string path, disco::Superblock spb){
    
    //Creo una variable de tipo FILE
    FILE *file = fopen(path.c_str(), "rb+");
    char c_one = '1';
    //Seteo 1 en el bitmap de inodos segun la posicion
    fseek(file, spb.s_bm_inode_start + position, SEEK_SET);
    fwrite(&c_one, sizeof(c_one), 1, file);
    fclose(file);    
}

void adm_cap::insertJournal(string contenido, char operacion, int permiso, char tipo,  string nombre, string propietario, disco::Mount mount_temp, disco::Superblock spb){

    //Creo una variable de tipo FILE
    FILE *file = fopen(mount_temp.path.c_str(), "rb+");
    fseek(file, mount_temp.part_start + csnt_cap.SIZE_SPB, SEEK_SET);

    disco::Journaling *journal_lectura = new disco::Journaling();
    int posicion = -1;
    for (int i = 0; i < spb.s_inodes_count; i++){
        //Se lee el journaling
        fread(journal_lectura, csnt_cap.SIZE_J, 1, file);
        if (journal_lectura->id_journal == -1){
            posicion = i;
            break;
        }        
    }
    
    if (posicion == -1){
        cout << csnt_cap.RED << "ERROR:" << csnt_cap.NC << " Ya no hay mas espacio para ingresar journaling " << endl;
        return;
    }

    //Se posiciona el puntero del file
    fseek(file, mount_temp.part_start + csnt_cap.SIZE_SPB + (posicion * csnt_cap.SIZE_J), SEEK_SET);

    //Journal nuevo
    disco::Journaling *journal = new disco::Journaling();
    journal->id_journal = posicion + 1;
    journal->operation = operacion;
    journal->type = tipo;
    strcpy(journal->nombre, nombre.c_str());
    journal->content = contenido;
    journal->date = time(nullptr);
    strcpy(journal->propietario, propietario.c_str());
    journal->permiso = permiso;
    
    fwrite(journal, csnt_cap.SIZE_J, 1, file);
    fclose(file);
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

string adm_cap::vectorToString(vector<string> path){
    string response;
    for (int i = 0; i < path.size(); i++){
        response += "/"+path[i];
    }
    return response;
}


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