#ifndef ADM_UG_H
#define ADM_UG_H

#include <string>
#include <map>
#include <vector>
#include "disco.h"

using namespace std;

class adm_ug
{
public:
    /*METODOS PRINCIPALES*/

    disco::User login(map<string, string> param_got, vector<disco::Mount> list_mount, disco::User UserLoggedIn);
    disco::User logout(disco::User UserLoggedIn);

    /*METODOS AUXILIARES*/

    string getArchiveUserTXT(int part_start_partition, string path);
    disco::User checkUser(string user_txt, string user, string psw);
};

#endif //ADM_UG_H