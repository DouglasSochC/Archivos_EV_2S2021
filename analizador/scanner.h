#ifndef SCANNER_H
#define SCANNER_H
#include <vector>
#include <string>

using namespace std;

class scanner
{
public:
    void init();
    vector<string> tokens(string comando);
    void exec_command(vector<string> tokens);
};

#endif //SCANNER_H