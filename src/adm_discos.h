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

    //SUB-METODOS
    void fdisk_createPrimaryExtended(disco::MBR mbr, char part_fit, char part_type, int size, string name, string path);
    int fdisk_createLogicInit(disco::Partition partition, string path);
    void fdisk_createLogic(disco::MBR mbr, char part_fit, char part_type, int part_size, string part_name, string path);
    void fdisk_deletePrimaryExtended(string delete_p, disco::MBR mbr, string path, string name);
    void fdisk_deleteLogic(string delete_p, disco::MBR mbr, string path, string name);

    //AUXILIARES
    /*
    Return struct MBR of the path
    */
    disco::MBR getMBR(string path);

    /*
    Return type of partition if name is found case opposite returns \0
    */
    char find_namePartition(string name, string path);

    /*
    Get partitions extended and primary where status is = 1, this orders the partitions
    */
    vector<disco::Partition> getListPartitionsEP(disco::MBR mbr);
    
    /*
    Get partitions logics where status is = 1
    */
    vector<disco::EBR> getListPartitionsL(disco::Partition partition, string path);

    /*
    Esta funcion necesita un vector de estructura Fit, el tamaño de la particion que se va a
    ingresar en un espacio, y el fit. 
    En el vector de estructura fit debe venir los espacios existentes que hay entre particiones osea pueden
    venir fit.size >0 o fit.size =0, apartir de ahi se empieza con el analisis de asignacion de espacio 
    de una particion nueva, es por eso que el metodo necesita el tamaño de la particion nueva para
    ir comparando.

    Los posibles valores que retorna son los siguientes:
    -1 = Error al asignar posicion
    -2 = Ya no hay espacio disponible
    n = Retorna el valor fit.start debido a que encontro un espacio disponible
    */
    int assign_positionByFit(vector<disco::Fit> list_Of_Fits, int size_new_partition, char fit);

    /*
    Get position before and after of position empty in partition logic
    */
    void getBA_PartitionL(disco::Partition partition_extended, string path, int actual, int *before, int *after);

    /*
    Edits a before EBR of one selected
    */
    void edit_beforeEBR(int pos_ini, int part_next, string path);

    /*
    Get partition logic where status is = 1
    */
    disco::EBR getPartitionL(disco::Partition partition_extended, string path, string name, int *before);

    /*
    Get partition extended or primary where status is = 1
    */
    disco::Partition getPartitionEP(disco::MBR mbr, string name, int *before, int *after);

    //METODOS PARA TEST - ELIMINAR ANTES DE ULTIMO PUSH
    void test_asignacionFit();
};

#endif //ADM_DISCOS_H