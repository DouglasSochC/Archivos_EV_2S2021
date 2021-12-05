#include "iostream"
#include "util_p.h"
#include "constant.h"

using namespace std;

constant cnst_util;

string util_p::toLowerString(string cadena){
    for (int i = 0; i < cadena.length(); i++) cadena[i] = tolower(cadena[i]);
    return cadena;
}

string util_p::toUpperString(string cadena) {
  for (int i = 0; i < cadena.length(); i++) cadena[i] = toupper(cadena[i]);
  return cadena;
}

bool util_p::isNumber(const string& str)
{    
    int acum = 0;
    if (str[0] == '-')
    {
        acum++;
    }
    for (char const &c : str) {
        if (acum != 1)
        {
            if (std::isdigit(c) == 0) return false;
        }else{
            acum--;
        }
    }
    return true;
}

vector<string> util_p::separateString(string param){
    //Comentario que venga despues del comando
    if (param.substr(0,1) == "#"){
        param = "-comentario~:~" + param;
    }
    
    vector<string> response;
    for (int i = 0; i < param.size(); i++)
    {
        if (param.substr(i, 3) == "~:~")
        {
            response.push_back(param.substr(0, i));
            string valor = param.substr(i + 3, param.size());
            if (!valor.empty())
            {
                response.push_back(param.substr(i + 3, param.size()));
            }            
        }
    }
    return response;
}

bool util_p::check_correctExtFile(string path){
    bool response = false;

    path = (path.substr(0,1) == "\"") ? path.substr(1, path.size()-2): path;
    int pos = path.size();
    string val = "";

    while (val != "." && pos > 0)
    {
        val = path.substr(pos, 1);
        pos--;
    }
    string extension = path.substr(pos+1, path.size());
    response = (cnst_util.EXTENSION_FILE == extension);
    return response;
}

bool util_p::check_correctExtEXEC(string path){
    bool response = false;

    path = (path.substr(0,1) == "\"") ? path.substr(1, path.size()-2): path;
    int pos = path.size();
    string val = "";

    while (val != "." && pos > 0)
    {
        val = path.substr(pos, 1);
        pos--;
    }
    string extension = path.substr(pos+1, path.size());
    response = (cnst_util.EXTENSION_EXEC == extension);
    return response;
}

bool util_p::check_existFile(string path){
    bool response = false;
    path = (path.substr(0,1) == "\"") ? path.substr(1, path.size()-2): path;
    FILE *aux_file = fopen(path.c_str(), "r");
    if (aux_file != NULL)
    {
        fclose(aux_file);
        response = true;
    }
    return response;
}

void util_p::createDirectory(string path){
    path = (path.substr(0,1) == "\"") ? path.substr(1, path.size()-2): path;
    int posicion = path.size();
    string val = "";
    while (val != "/")
    {
        val = path.substr(posicion, 1);
        posicion--;
    }    
    string comando = "mkdir -p \"" + path.substr(0, posicion+1) + "\"";
    system(comando.c_str());
}

float util_p::round(float valor){
    char str[40];
    sprintf(str, "%.3f", valor);
    sscanf(str, "%f", &valor); 
    return valor;
}