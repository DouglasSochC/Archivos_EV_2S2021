#include <iostream>
#include <fstream>
#include "scanner.h"
#include "../util/util_p.h"
#include "../util/constant.h"
#include "../src/adm_discos.h"
#include "../src/adm_cap.h"
#include "../src/reportes.h"
#include "../src/script_adm.h"
#include "parameters.h"

using namespace std;

util_p util_scnr;
constant cnst_scnr;
parameters prmts_scnr;
adm_discos admdcs_scnr;
adm_cap admcap_scnr;
reportes rp_scnr;
script_adm scrp_scnr;

void scanner::init(){
    
    string comando;
    getline(cin, comando);
    vector<string> spliting = tokens(comando);
    exec_command(spliting);
}

/*
Si el path solo tiene una doble comilla de apertura pero no del cerrado ya no reconoce mas tokens
*/
vector<string> scanner::tokens(string comando){
    
    comando += " ";
    vector<string> response_tokens;
    int pos_init = 0;
    int length = 0;
    bool route = false;  

    for (int i = 0; i < comando.size(); i++)
    {
        route = (comando.substr(i, 1) == "\"") ? !route : route;
        if (comando.substr(i, 1) == " " && !route)
        {
            if (!comando.substr(pos_init, length).empty())
            {
                response_tokens.push_back(comando.substr(pos_init, length));
            }
            pos_init = i + 1;
            length = 0;
        }else if (comando.substr(i, 1) == "#"){
            response_tokens.push_back(comando.substr(i, comando.size()));
            break;
        }else{
            length++;
        }
    }
    return response_tokens;
}

void scanner::exec_command(vector<string> tokens){

    if (tokens.size() > 0){
        string comando = tokens[0];
        string comentario = comando;
        comando = util_scnr.toLowerString(comando);        
        if (comando.substr(0,1) == "#"){
            cout << cnst_scnr.BLUE << comentario << cnst_scnr.NC << endl;
        }else if (comando == "mkdisk"){
            map<string, string> param_got = prmts_scnr.param_mkdisk(tokens);
            admdcs_scnr.mkdisk(param_got);
        }else if (comando == "rmdisk"){
            map<string, string> param_got = prmts_scnr.param_rmdisk(tokens);
            admdcs_scnr.rmdisk(param_got);
        }else if (comando == "fdisk"){            
            map<string, string> param_got = prmts_scnr.param_fdisk(tokens);
            admdcs_scnr.fdisk(param_got);
        }else if(comando == "mount"){
            map<string, string> param_got = prmts_scnr.param_mount(tokens);
            admdcs_scnr.mount(param_got);
        }else if(comando == "unmount"){
            map<string, string> param_got = prmts_scnr.param_unmount(tokens);
            admdcs_scnr.unmount(param_got);
        }else if(comando == "exec"){
            map<string, string> param_got = prmts_scnr.param_exec(tokens);
            scrp_scnr.exec(param_got);
        }else if(comando == "mkfs"){
            map<string, string> param_got = prmts_scnr.param_mkfs(tokens);
            admdcs_scnr.mkfs(param_got, admdcs_scnr.getListMount());
        }else if(comando == "mkdir"){
            map<string, string> param_got = prmts_scnr.param_mkdir(tokens);
            admcap_scnr.mkdir(param_got, admdcs_scnr.getListMount());
        }else if(comando == "rep"){
            map<string, string> param_got = prmts_scnr.param_rep(tokens);
            if (param_got.size() > 0){
                string name_report = param_got["-name"].c_str();
                if (name_report == "mbr"){
                    rp_scnr.mbr(param_got, admdcs_scnr.getListMount());
                }else if(name_report == "disk"){
                    rp_scnr.disk(param_got, admdcs_scnr.getListMount());
                }else if(name_report == "sb"){
                    rp_scnr.sb(param_got, admdcs_scnr.getListMount());
                }
            }
        }else if(comando == "test1"){
            admdcs_scnr.test_asignacionFit();
        }else{
            cout << cnst_scnr.RED << "ERROR:" << cnst_scnr.NC << " No existe el comando " << comando << endl;
        }
    }
}