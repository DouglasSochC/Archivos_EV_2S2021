#ifndef ADM_CAP_H
#define ADM_CAP_H

#include <string>
#include <map>
#include <vector>

using namespace std;

class adm_cap
{
public:        
    void mkdir(map<string, string> param_got, vector<disco::Mount> listMount);
};

#endif //ADM_CAP_H