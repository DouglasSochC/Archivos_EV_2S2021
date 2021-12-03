#ifndef ADM_DISCOS_H
#define ADM_DISCOS_H

#include <string>
#include <map>
#include <vector>

using namespace std;

class adm_discos
{
public:
    //METODOS PRINCIPALES
    void mkdisk(map<string, string> param_got);
    void rmdisk(map<string, string> param_got);
    void fdisk(map<string, string> param_got);
    void mount(map<string, string> param_got);
    void umount(map<string, string> param_got);
    void mkfs(map<string, string> param_got, vector<disco::Mount> listMount);

    //SUB METODOS
    void fdisk_createPrimaryExtended(disco::MBR mbr, char part_fit, char part_type, int size, string name, string path);
    void fdisk_createLogic(disco::MBR mbr, char part_fit, char part_type, int part_size, string part_name, string path);
    void fdisk_deletePrimaryExtended(string delete_p, disco::MBR mbr, string path, string name);
    void fdisk_deleteLogic(string delete_p, disco::MBR mbr, string path, string name);
    void fdisk_addPrimaryExtended(int add_p, disco::MBR mbr, string path, string name);
    void fdisk_addLogic(int add_p, disco::MBR mbr, string path, string name);
    void mkfs_EXT2(disco::Superblock superblock, int part_start_partition, int n_inodes, string path);
    void mkfs_EXT3(disco::Superblock superblock, int part_start_partition, int n_inodes, string path);

    //UTIL
    vector<disco::Partition> getListPartitionsEP(disco::MBR mbr);
    disco::Partition getPartitionEP(disco::MBR mbr, string name, int *before, int *after);
    vector<disco::EBR> getListPartitionsL(disco::Partition partition, string path);
    disco::EBR getPartitionL(disco::Partition partition_extended, string path, string name, int *before);
    void edit_beforeEBR(int pos_ini, int part_next, string path);
    void edit_afterEBR(int pos_ini, int part_next, string path);
    void create_logicInit(disco::Partition partition, string path);
    char find_namePartition(string name, string path);
    int assign_positionByFit(vector<disco::Fit> list_Of_Fits, int size_new_partition, char fit);
    void getBA_PartitionL(disco::Partition partition_extended, string path, int actual, int *before, int *after);
    disco::Mount partitionToMounted(string name, string path);
    void showPartitionsMounteds();
    vector<disco::Mount> getListMount();

    void verLogicas();
};

#endif //ADM_DISCOS_H