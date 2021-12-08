#ifndef DISCO_H
#define DISCO_H

#include "ctime"
#include <string>

using namespace std;

class disco
{
public:

    typedef struct
    {
        char part_status = '0'; //0 = Inactivo; 1 = Activo
        char part_type; // P, E
        char part_fit; // B, F, W
        int part_start = -1;
        int part_size = 0;
        char part_name[16];
    } Partition;
    
    typedef struct
    {
        int mbr_tamano;
        time_t mbr_fecha_creacion;
        int mbr_disk_signature;
        char disk_fit; //B, F, W
        Partition mbr_partition_1;
        Partition mbr_partition_2;
        Partition mbr_partition_3;
        Partition mbr_partition_4;
    } MBR;
    
    typedef struct
    {
        char part_status = '0'; //0 = Inactivo; 1 = Activo;
        char part_fit; //B, F, W
        int part_start;
        int part_size = 0;
        int part_next = -1;
        char part_name[16];
    } EBR;
    
    typedef struct {
        int i_uid = -1; //UID del usuario propietario del archivo o carpeta
        int i_gid = -1; //GID del grupo al que pertenece el archivo o carpeta.
        int i_size = -1; //Tamaño del archivo en bytes
        time_t i_atime; //Última fecha en que se leyó el inodo sin modificarlo
        time_t i_ctime; //Fecha en la que se creó el inodo
        time_t i_mtime; //Ultima fecha en la que se modificó el inodo
        int i_block[15] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}; //Registros 1-12 = Bloque Directo; Registro 13 = Bloque Simple Indirecto; Registro 14 = Bloque Doble Indirecto; Registro 15 = Bloque Triple Indirecto; Si no son utilizados tendra el valor -1
        char i_type; //0 = Carpeta; 1 = Archivo
        int i_perm = -1;
    } Inode;

    typedef struct {
        char b_name[12]; //Nombre de la carpeta o archivo
        int b_inodo = -1; //Apuntador hacia un inodo asociado al archivo o carpeta
    } Content;
    
    typedef struct {
        Content b_content[4]; //Array con el contenido de la carpeta
    } Folderblock;

    typedef struct {
        int s_filesystem_type = 0; //Guarda el numero que identifica el sistema de archivos utilizado; 0 = Ninguna; 2 = EXT2; 3 = EXT3
        int s_inodes_count; //Guarda el número total de inodos
        int s_blocks_count; //Guarda el número total de bloques
        int s_free_blocks_count; //Contiene el número de bloques libres
        int s_free_inodes_count; //Contiene el número de inodos libres
        time_t s_mtime; //Ultima fecha en el que el sistema fue montado
        time_t s_umtime; //Ultima fecha en que el sistema fue desmontado
        int s_mnt_count; //Indica cuantas veces se ha montado el sistema
        int s_magic = 0xEF53; //Valor que identifica al sistema de archivos, tendrá el valor 0xEF53
        int s_inode_size = sizeof(Inode); //Tamaño del inodo
        int s_block_size= sizeof(Folderblock); //Tamaño del bloque
        int s_first_ino = 0; //Primer inodo libre
        int s_first_blo = 0; //Primer bloque libre
        int s_bm_inode_start; //Guardará el inicio del bitmap de inodos
        int s_bm_block_start; //Guardará el inicio del bitmap de bloques
        int s_inode_start; //Guardará el inicio de la tabla de inodos
        int s_block_start; //Guardará el inico de la tabla de bloques
    } Superblock;

    typedef struct {
        int id_journal = -1; // Debe iniciar en 0
        char operation; //C = Creacion; R = Lectura; U = Edicion; D = Eliminacion
        char type; //0 = Carpeta, 1 = Archivo
        char nombre[12]; //Nombre archivo o directorio
        string content; //Datos contenidos
        time_t date; //Fecha de transaccion
        char propietario[10]; //Propietario de archivo o directorio
        int permiso = 0; //Permisos que posee el archivo o directorio
    } Journaling;

    typedef struct {
        char b_content[64]; //Array con el contenido del archivo
    } Archiveblock;

    typedef struct {
        int b_pointers[16] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}; //Array con los apuntadores hacia bloques (de archivo o carpeta)
    } Pointerblock;

    typedef struct {
        int start = -1;
        int size = -1;
        string name;
    } Fit;

    typedef struct {
        char status = '0'; //0 = Inactivo; 1 = Activo
        int num_partition; //Numero con el que se identifica la particion
        short letter; //Letra con la que se identifica la particion
        string name; //Nombre de la particion
        string path; //Path del disco de la particion
        string id; //Id asignado por el sistema
        char typeP; //Tipo de particion que se monto
        int part_start; //Posicion de inicio de la particion
        int size_partition; //Tamanio de la particion
        time_t date_mounted;
    } Mount;

    typedef struct {
        int id = -1; // -1 = Sin uso; 0 = Eliminado; >0 = Id activo
        char tipo; // G = Grupo; U = Usuario
        string grupo = ""; // Nombre del grupo al que pertence
        string usuario = ""; // Nombre del usuario
        string contrasenia = ""; // Contrasenia del usuario
        Mount montura; //Montura
        int posicion_registro = -1; // Es la posicion en la que se encuentra ubicado el registro dentro del archivo user.txt
        int tamanio_registro = -1; //Es el tamanio del registro encontrado
    } User;

    typedef struct {
        int id = -1; // -1 = Sin uso; 0 = Eliminado; >0 = Id activo
        string nombre = ""; // Nombre del grupo
        int posicion_registro = -1; // Es la posicion en la que se encuentra ubicado el registro dentro del archivo user.txt
        int tamanio_registro = -1; //Es el tamanio del registro encontrado
    } Group;

};

#endif //DISCO_H