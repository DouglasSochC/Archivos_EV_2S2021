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
};

#endif //PARAMETERS_H

