#include <iostream>
#include <map>
#include <cmath>
#include "../util/util_p.h"
#include "../util/constant.h"
#include "reportes.h"
#include "disco.h"

using namespace std;

constant csnt_rp;
util_p util_rp;

void reportes::mbr(map<string, string> param_got, vector<disco::Mount> list_mount){
    if (param_got.size() == 0){
        return;
    }
    //Obteniendo Datos
    string name = param_got["-name"].c_str();
    string path = param_got["-path"].c_str();
    string id = param_got["-id"].c_str();
    string ruta = param_got["-ruta"].c_str();
    int root = atoi(param_got["-root"].c_str());

    //Formateo de Datos
    path = (path.substr(0,1) == "\"") ? path.substr(1, path.size()-2): path;
    util_rp.createDirectory(path);

    //Flujo
    disco::Mount gettingMount = getMount(id, list_mount);
    if (gettingMount.status == '0'){
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha encontrado el id solicitado" << endl;
        return;
    }
    disco::MBR mbr;
    FILE *file = fopen(gettingMount.path.c_str(), "rb");
    int read = fread(&mbr, csnt_rp.SIZE_MBR, 1, file);
    if (read <= 0){
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha podido leer el disco" << endl;
        return;
    }else{
        fclose(file);
    }    
    
    string draw;
    struct tm *tm;
    tm = localtime(&mbr.mbr_fecha_creacion);
    char mostrar_fecha [20];

    strftime(mostrar_fecha, 20, "%Y/%m/%d %H:%M:%S", tm);
    draw = "digraph G{\n"
            "forcelabels= true;\n"
            "node [shape = plaintext];\n"
            "general [label = <\n"
            "<table>\n"
            "<tr><td COLSPAN = '2' BGCOLOR=\"#BFBFBF\"><font color=\"black\">MBR</font></td></tr>\n"
            "<tr><td BGCOLOR=\"#E0FFFF\">Nombre</td><td BGCOLOR=\"#E0FFFF\" >Valor</td></tr>\n"
            "<tr>"
                "<td>mbr_tamaño</td>\n"
                "<td>" + to_string(mbr.mbr_tamano) + "</td>\n"
            "</tr>\n"
            "<tr>\n"
                "<td>mbr_fecha_creacion</td>\n"
                "<td>" + string(mostrar_fecha) + "</td>\n"
            "</tr>\n"
            "<tr>\n"
                "<td>mbr_disk_signature</td>\n"
                "<td>" + to_string(mbr.mbr_disk_signature) + "</td>\n"
            "</tr>\n"
            "<tr>\n"
                "<td>Disk_fit</td>\n"
                "<td>" + string(1, mbr.disk_fit) + "</td>\n"
            "</tr>\n";

    vector<disco::Partition> listEP = discosReportes.getListPartitionsEP(mbr);
    int extended = -1;
    int acum = 1;
    for (int i = 0; i < listEP.size(); i++){
        draw += "<tr>\n"
                    "<td>part_status_" + to_string(i + 1) + "</td>\n"
                    "<td>" + listEP[i].part_status + " </td >\n"
                "</tr>\n"
                "<tr>\n"
                    "<td>part_type_" + to_string(i + 1) + "</td>\n"
                    "<td>" + listEP[i].part_type + "</td>\n"
                "</tr>\n"
                "<tr>\n"
                    "<td>part_fit_" + to_string(i + 1) + "</td>\n"
                    "<td>" + listEP[i].part_fit + "</td>\n"
                "</tr>\n"
                "<tr>\n"
                    "<td>part_start_" + to_string(i + 1) + "</td>\n"
                    "<td>" + to_string(listEP[i].part_start) + "</td>\n"
                "</tr>\n"
                "<tr>\n"
                    "<td>part_size_" + to_string(i + 1) + "</td>\n"
                    "<td>" + to_string(listEP[i].part_size) + "</td>\n"
                "</tr>\n"
                "<tr>\n"
                    "<td>part_name_" + to_string(i + 1) + "</td>\n"
                    "<td>" + listEP[i].part_name + "</td>\n"
                "</tr>\n";

        if (listEP[i].part_type == 'E'){
            extended = i;
        }
        acum++;
        
    }
    
    int val = 1;
    if (extended != -1){
        vector<disco::EBR> listEBR = discosReportes.getListPartitionsL(listEP[extended],gettingMount.path.c_str());
        for (int i = 0; i < listEBR.size(); i++){
            draw += "<tr>"
                    "<td BORDER=\"0\"></td><td BORDER=\"0\"></td>"
                "</tr>"
                "<tr>"
                    "<td COLSPAN = '2' BGCOLOR=\"#2F561F\"><font color=\"white\">EBR_" + to_string(val) + "</font></td>"
                "</tr>\n"
                "<tr>"
                    "<td BGCOLOR=\"#33FF90\">Nombre</td><td BGCOLOR=\"#33FF90\" >Valor</td>"
                "</tr>\n"
                "<tr>\n"
                    "<td>part_status_" + to_string(val) + "</td>\n"
                    "<td>" + listEBR[i].part_status + "</td>\n"
                "</tr>\n"
                "<tr>\n"
                    "<td>part_fit_" + to_string(val) + "</td>\n"
                    "<td>" + listEBR[i].part_fit + "</td>\n"
                "</tr>\n"
                "<tr>\n"
                    "<td>part_start_" + to_string(val) + "</td>\n"
                    "<td>" + to_string(listEBR[i].part_start) + "</td>\n"
                "</tr>\n"
                "<tr>\n"
                    "<td>part_size_" + to_string(val) + "</td>\n"
                    "<td>" + to_string(listEBR[i].part_size) + "</td>\n"
                "</tr>\n"
                "<tr>\n"
                    "<td>part_next" + to_string(val) + "</td>\n"
                    "<td>" + to_string(listEBR[i].part_next) + "</td>\n"
                "</tr>\n"
                "<tr>\n"
                    "<td>part_name" + to_string(val) + "</td>\n"
                    "<td>" + listEBR[i].part_name + "</td>\n"
                "</tr>\n";
                val++;
        }
    }
    draw += "</table>>];";
    draw += "\n\n}\n";
    create_FileReport(draw, path);
    cout << csnt_rp.GREEN << "RESPUESTA:" << csnt_rp.NC << " El reporte MBR ha sido generado correctamente" << endl;
}

void reportes::disk(map<string, string> param_got, vector<disco::Mount> list_mount){
    if (param_got.size() == 0){
        return;
    }
    //Obteniendo Datos
    string name = param_got["-name"].c_str();
    string path = param_got["-path"].c_str();
    string id = param_got["-id"].c_str();
    string ruta = param_got["-ruta"].c_str();
    int root = atoi(param_got["-root"].c_str());

    //Formateo de Datos
    path = (path.substr(0,1) == "\"") ? path.substr(1, path.size()-2): path;
    util_rp.createDirectory(path);

    //Flujo
    disco::Mount gettingMount = getMount(id, list_mount);
    if (gettingMount.status == '0'){
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha encontrado el id solicitado" << endl;
        return;
    }
    string path_mount = gettingMount.path;
    disco::MBR mbr;
    FILE *file = fopen(path_mount.c_str(), "rb");
    int read = fread(&mbr, csnt_rp.SIZE_MBR, 1, file);
    if (read <= 0){
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha podido leer el disco" << endl;
        return;
    }else{
        fclose(file);
    }

    //Se obtiene las particiones activas (EP) que tiene el disco
    vector<disco::Partition> listPartitionsEP = discosReportes.getListPartitionsEP(mbr);
    disco::Partition partitionExtended;
    for (int i = 0; i < listPartitionsEP.size(); i++){
        if (listPartitionsEP[i].part_type == 'E'){
            partitionExtended = listPartitionsEP[i];
        }        
    }
    
    //Ahora se construira con la estructura FIT cada uno de los espacios en el reporte 
    //para las particiones activas (EP)
    disco::Fit tempFit;
    vector<disco::Fit> listFitEP;
    if (listPartitionsEP.size() > 0){
        tempFit.name = "Libre";
        tempFit.size = listPartitionsEP[0].part_start - csnt_rp.SIZE_MBR;
        listFitEP.push_back(tempFit);
        for (int i = 0; i < listPartitionsEP.size(); i++){
            //Espacio actual de la particion
            tempFit.name = (listPartitionsEP[i].part_type == 'E')? "Extendida": "Primaria";
            tempFit.size = listPartitionsEP[i].part_size;
            listFitEP.push_back(tempFit);
            
            //Se revisa si no hay un espacio libre entre esta particion y la que sigue
            int siguiente = (listPartitionsEP.size()-1) >= (i+1) ? listPartitionsEP[i+1].part_start: -1;
            if (siguiente != -1){
                //No ha llegado al tope
                tempFit.name = "Libre";
                tempFit.size = listPartitionsEP[i+1].part_start - (listPartitionsEP[i].part_start + listPartitionsEP[i].part_size);
                listFitEP.push_back(tempFit);
            }else{
                //Llego al tope
                tempFit.name = "Libre";
                tempFit.size = mbr.mbr_tamano - (listPartitionsEP[i].part_start + listPartitionsEP[i].part_size);
                listFitEP.push_back(tempFit);
            }
        }
    }else{
        tempFit.name = "Libre";
        tempFit.size = mbr.mbr_tamano;
        listFitEP.push_back(tempFit);
    }
    
    vector<disco::Fit> listFitL;
    if (partitionExtended.part_status == '1'){
        //Se obtiene las particiones activas (L) que tiene el disco si es que existe
        vector<disco::EBR> listPartitionsL = discosReportes.getListPartitionsL(partitionExtended, path_mount);
        //Ahora se construira con la estructura FIT cada uno de los espacios en el reporte 
        //para las particiones activas (L)
        if (listPartitionsL.size() > 0){
            tempFit.name = "Libre";
            tempFit.size = listPartitionsL[0].part_start - partitionExtended.part_start;
            listFitL.push_back(tempFit);
            for (int i = 0; i < listPartitionsL.size(); i++){
                //Espacio actual de la particion
                tempFit.name = "Logica";
                tempFit.size = listPartitionsL[i].part_size;
                listFitL.push_back(tempFit);

                //Se revisa si no hay un espacio libre entre esta particion y la que sigue
                int siguiente = (listPartitionsL.size()-1) >= (i+1) ? listPartitionsL[i+1].part_start: -1;
                if (siguiente != -1){
                    //No ha llegado al tope
                    tempFit.name = "Libre";
                    tempFit.size = listPartitionsL[i+1].part_start - (listPartitionsL[i].part_start + listPartitionsL[i].part_size);
                    listFitL.push_back(tempFit);
                }else{
                    //Llego al tope
                    tempFit.name = "Libre";
                    tempFit.size = (partitionExtended.part_start + partitionExtended.part_size) - (listPartitionsL[i].part_start + listPartitionsL[i].part_size);
                    listFitL.push_back(tempFit);
                }
            }
        }else{
            tempFit.name = "Libre";
            tempFit.size = partitionExtended.part_size;
            listFitL.push_back(tempFit);
        }
    }

    //Se cuenta la cantidad de particiones que hay en la particion extendida, esto es debido
    //a que hay que acomodar la tabla del reporte con el colspan
    int colspan = 0;
    if (partitionExtended.part_status == '1'){
        for (int i = 0; i < listFitL.size(); i++){
            if (listFitL[i].size > 0){
                if (listFitL[i].name == "Libre"){
                    colspan++;
                }else if(listFitL[i].name == "Logica"){
                    colspan = colspan + 2;
                }
            }            
        }
    }
    string draw = "digraph G{\n"
            "forcelabels= true;\n"
            "node [shape = plaintext];\n"
            "tbl [\n"
            "shape=plaintext \n"
            "label=< \n"
            "<table border='0' cellborder='1' color='blue' cellspacing='0'> \n";

    draw += "<tr>\n\n"
                "<td rowspan='2'>\n"
                "<table cellspacing='0' >\n"
                "<tr><td>MBR</td></tr>\n"
                "</table>\n"
                "</td>\n";    
    
    for (int i = 0; i < listFitEP.size(); i++){
        if (listFitEP[i].size > 0){
            float size_disk = mbr.mbr_tamano;
            if (listFitEP[i].name == "Extendida" && listFitL.size() > 0){
                draw += "\n<td rowspan='2'>\n"
                        "<table cellspacing='0'>\n"
                        "<tr>\n"
                        "<td colspan='" + to_string(colspan) + "'> Extendida </td>\n"
                        "</tr>\n"
                        "<tr>\n";
                for (int i = 0; i < listFitL.size(); i++){
                    if (listFitL[i].size > 0){
                        float size_l = listFitL[i].size;
                        float percentage = util_rp.round(size_l * 100 / size_disk);
                        if (listFitL[i].name == "Libre"){
                            draw += "<td><br/>" + listFitL[i].name + "<br/> " + to_string(percentage) + "% del disco<br/> </td>\n";
                        }else{
                            draw += "<td><br/> EBR <br/></td>\n";
                            draw += "<td><br/>" + listFitL[i].name + "<br/> " + to_string(percentage) + "% del disco<br/> </td>\n";
                        }
                    }
                }
                draw += "</tr>"
                    "</table>"
                    "</td>\n";
            }else{
                float size = listFitEP[i].size;
                float percentage = util_rp.round(size * 100 / size_disk);
                cout << percentage << endl;
                draw += "\n<td rowspan='2'>\n"
                "<table cellspacing='0'>\n"
                "<tr><td><br/>" + listFitEP[i].name + "<br/> " + to_string(percentage) + "% del disco<br/> </td></tr>\n"
                "</table>\n"
                "</td>\n";
            }
        }        
    }
    draw += "\n</tr>\n"
            "</table>\n"
            ">]; \n"
            "} \n";
        
    create_FileReport(draw, path);
    cout << csnt_rp.GREEN << "RESPUESTA:" << csnt_rp.NC << " El reporte DISK ha sido generado correctamente" << endl;
}

disco::Mount reportes::getMount(string id, vector<disco::Mount> list_mount){
    disco::Mount response;
    response.status = '0';
    for (int i = 0; i < list_mount.size(); i++){
        if (list_mount[i].id == id){
            response = list_mount[i];
            break;
        }        
    }
    return response;
}

void reportes::create_FileReport(string draw, string path){

    util_rp.createDirectory(path);
    string extension = "-T" + path.substr(path.size()-3,3);
    int size = path.size() - 1;
    int acum = 1;
    while (path.substr(size, 1) != "/"){
        size--;
        acum++;
    }
    size++;
    string ubicacion_salida = path.substr(0, path.size() - 4);
    string ubicacion_dot = path.substr(0, path.size() - 4)+".dot";
    FILE* file = fopen(ubicacion_dot.c_str(), "w+");
    fwrite(draw.c_str(), draw.size(), 1, file);
    fclose(file);
    string function = "dot " + extension + " " + ubicacion_dot +" -o " + ubicacion_salida;
    system(function.c_str());
}