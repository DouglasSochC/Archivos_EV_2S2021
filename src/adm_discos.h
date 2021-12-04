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


    //AUXILIARES
    disco::MBR getMBR(string path);
    char find_namePartition(string name, string path);
    vector<disco::Partition> getListPartitionsEP(disco::MBR mbr);
    vector<disco::EBR> getListPartitionsL(disco::Partition partition, string path);
    int assign_positionByFit(vector<disco::Fit> list_Of_Fits, int size_new_partition, char fit);

    //METODOS PARA TEST - ELIMINAR ANTES DE ULTIMO PUSH
    void test_asignacionFit();
};

#endif //ADM_DISCOS_H