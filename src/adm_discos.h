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
};

#endif //ADM_DISCOS_H