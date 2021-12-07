#ifndef REPORTES_H
#define REPORTES_H

#include <map>
#include <vector>
#include <string>
#include "disco.h"
#include "adm_discos.h"

using namespace std;

class reportes
{
public:
    void mbr(map<string, string> param_got, vector<disco::Mount> list_mount);
    void disk(map<string, string> param_got, vector<disco::Mount> list_mount);
    void sb(map<string, string> param_got, vector<disco::Mount> list_mount);
    void journaling(map<string, string> param_got, vector<disco::Mount> list_mount);
    void bm_inode(map<string, string> param_got, vector<disco::Mount> list_mount);
    void bm_block(map<string, string> param_got, vector<disco::Mount> list_mount);
    disco::Mount getMount(string id, vector<disco::Mount> list_mount);
    void create_FileReport(string draw, string path);
private:
    adm_discos discosReportes;
};

#endif //REPORTES_H