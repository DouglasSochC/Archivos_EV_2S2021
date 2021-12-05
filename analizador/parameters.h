#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <string>
#include <map>
#include <vector>

using namespace std;

class parameters
{
public:
    map<string, string> param_mkdisk(vector<string> tokens);
    map<string, string> param_rmdisk(vector<string> tokens);
    map<string, string> param_fdisk(vector<string> tokens);
    map<string, string> param_exec(vector<string> tokens);
    map<string, string> param_mount(vector<string> tokens);
    map<string, string> param_unmount(vector<string> tokens);
    map<string, string> param_mkfs(vector<string> tokens);
    map<string, string> param_rep(vector<string> tokens);
};

#endif //PARAMETERS_H

