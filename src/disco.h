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
        int i_uid = -1;
        int i_gid = -1;
        int i_size = -1;
        time_t i_atime;
        time_t i_ctime;
        time_t i_mtime;
        int i_block[15] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
        char i_type = -1;
        int i_perm = -1;
    } Inode;

    typedef struct {
        char b_name[12];
        int b_inodo = -1;
    } Content;
    
    typedef struct {
        Content b_content[4];
    } Folderblock;

    typedef struct {
        int s_filesystem_type;
        int s_inodes_count;
        int s_blocks_count;
        int s_free_blocks_count;
        int s_free_inodes_count;
        time_t s_mtime;
        time_t s_umtime;
        int s_mnt_count;
        int s_magic = 0xEF53;
        int s_inode_size = sizeof(Inode);
        int s_block_size= sizeof(Folderblock);
        int s_fist_ino = 0;
        int s_first_blo = 0;
        int s_bm_inode_start;
        int s_bm_block_start;
        int s_inode_start;
        int s_block_start;
    } Superblock;

    typedef struct {
        char operation[10] = "";
        char type = -1; //0 = Archivo, 1 = Carpeta
        char nombre[12] = ""; //Nombre Archivo o Directorio
        char content[64] = "";
        time_t date;
        char propietario[12];
        int permiso = 0;
    } Journaling;

    typedef struct {
        char b_content[64];
    } Archiveblock;

    typedef struct {
        int b_pointers[16] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    } Pointerblock;

    typedef struct {
        int start = -1;
        int size = -1;
        string name;
    } Fit;

    typedef struct {
        char status = '0'; //0 = Inactivo; 1 = Activo
        int num_partition;
        short letter;
        string name;
        string path;
        string id;
    } Mount;

};

#endif //DISCO_H