#ifndef UTIL_P_H
#define UTIL_P_H
#include <string>
#include <vector>

using namespace std;

class util_p
{
public:
    string toLowerString(string cadena);
    string toUpperString(string cadena);
    bool isNumber(const string& str);
    vector<string> separateString(string param);
    bool check_correctExtFile(string path);
    bool check_correctExtEXEC(string path);
    bool check_existFile(string path);
    float round(float valor);
    bool hasSpecialCharacter(string texto);
    void insertDatainChar(string data, char* buffer);
    void createDirectory(string path);
    bool existFile(string path);
    void insertDatainVector(vector<string> *main_list, vector<string> data);
};

#endif //UTIL_P_H