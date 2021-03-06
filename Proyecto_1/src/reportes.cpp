#include <iostream>
#include <map>
#include <cmath>
#include <cstring>
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
    /*Obteniendo datos*/
    string comentario = param_got["-comentario"].c_str();
    string name = param_got["-name"].c_str(); //Este es el nombre del reporte
    string path = param_got["-path"].c_str();
    string id = param_got["-id"].c_str();
    string ruta = param_got["-ruta"].c_str();
    int root = atoi(param_got["-root"].c_str());

    /*Formateo de datos*/
    path = (path.substr(0,1) == "\"") ? path.substr(1, path.size()-2): path;
    util_rp.createDirectory(path);

    /*Flujo del void*/
    disco::Mount gettingMount = getMount(id, list_mount);
    if (gettingMount.status == '0'){
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha encontrado el id solicitado " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
        return;
    }
    disco::MBR mbr;
    FILE *file = fopen(gettingMount.path.c_str(), "rb");
    int read = fread(&mbr, csnt_rp.SIZE_MBR, 1, file);
    if (read <= 0){
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha podido leer el disco " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
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
                "<td>mbr_tama??o</td>\n"
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
    cout << csnt_rp.GREEN << "RESPUESTA:" << csnt_rp.NC << " El reporte MBR ha sido generado correctamente " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
}

void reportes::disk(map<string, string> param_got, vector<disco::Mount> list_mount){
    if (param_got.size() == 0){
        return;
    }
    /*Obteniendo datos*/
    string comentario = param_got["-comentario"].c_str();
    string name = param_got["-name"].c_str(); //Este es el nombre del reporte
    string path = param_got["-path"].c_str();
    string id = param_got["-id"].c_str();
    string ruta = param_got["-ruta"].c_str();
    int root = atoi(param_got["-root"].c_str());

    /*Formateo de datos*/
    path = (path.substr(0,1) == "\"") ? path.substr(1, path.size()-2): path;
    util_rp.createDirectory(path);

    /*Flujo del void*/
    disco::Mount gettingMount = getMount(id, list_mount);
    if (gettingMount.status == '0'){
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha encontrado el id solicitado " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
        return;
    }
    string path_mount = gettingMount.path;
    disco::MBR mbr;
    FILE *file = fopen(path_mount.c_str(), "rb");
    int read = fread(&mbr, csnt_rp.SIZE_MBR, 1, file);
    if (read <= 0){
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha podido leer el disco " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
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
    cout << csnt_rp.GREEN << "RESPUESTA:" << csnt_rp.NC << " El reporte DISK ha sido generado correctamente " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
}

void reportes::sb(map<string, string> param_got, vector<disco::Mount> list_mount){
    if (param_got.size() == 0){
        return;
    }
    /*Obteniendo datos*/
    string comentario = param_got["-comentario"].c_str();
    string name = param_got["-name"].c_str();
    string path = param_got["-path"].c_str();
    string id = param_got["-id"].c_str();
    string ruta = param_got["-ruta"].c_str();
    int root = atoi(param_got["-root"].c_str());

    /*Formateo de Datos*/
    path = (path.substr(0,1) == "\"") ? path.substr(1, path.size()-2): path;
    util_rp.createDirectory(path);

    /*Flujo del void*/
    //Se obtiene la particion que esta montada
    disco::Mount tempMount = getMount(id, list_mount);
    if (tempMount.status == '0'){
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No existe el id que desea " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
        return;
    }
    
    //Se crean variables que se utilizaran posteriormente
    string path_aux = tempMount.path;
    string name_aux = tempMount.name;
    time_t date_mounted = tempMount.date_mounted;
    int part_start_partition = tempMount.part_start;
    
    //Obtengo la estructura del superbloque
    disco::Superblock spb;
    FILE *file = fopen(path_aux.c_str(), "rb");
    fseek(file, part_start_partition, SEEK_SET);
    int read = fread(&spb, csnt_rp.SIZE_SPB, 1, file);
    if (read <= 0){
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha podido leer el disco " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
        return;
    }else{
        fclose(file);
    }
    if (spb.s_filesystem_type == 0)    {
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha formateado esta particion " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
        return;
    }    
    
    struct tm *tm;
    char s_mtime[20];
    tm = localtime(&spb.s_mtime);
    strftime(s_mtime, 20, "%Y/%m/%d %H:%M:%S", tm);
    char s_umtime[20];
    tm = localtime(&spb.s_umtime);
    strftime(s_umtime, 20, "%Y/%m/%d %H:%M:%S", tm);

    string draw;
    draw = "digraph G{\n"
            "forcelabels= true;\n"
            "node [shape = plaintext];\n"
            "general [label = <\n"
            "<table>\n"
            "<tr><td COLSPAN = '2' BGCOLOR=\"#BFBFBF\"><font color=\"black\">SUPER BLOQUE - " + name_aux + " en " + path_aux + "</font></td></tr>\n"
            "<tr><td BGCOLOR=\"#E0FFFF\">Nombre</td><td BGCOLOR=\"#E0FFFF\" >Valor</td></tr>\n"
            "<tr>"
                "<td>s_filesystem_type</td>\n"
                "<td>" + to_string(spb.s_filesystem_type) + "</td>\n"
            "</tr>\n"
            "<tr>\n"
                "<td>s_inodes_count</td>\n"
                "<td>" + to_string(spb.s_inodes_count) + "</td>\n"
            "</tr>\n"
            "<tr>\n"
                "<td>s_blocks_count</td>\n"
                "<td>" + to_string(spb.s_blocks_count) + "</td>\n"
            "</tr>\n"
            "<tr>\n"
                "<td>s_free_blocks_count</td>\n"
                "<td>" + to_string(spb.s_free_blocks_count) + "</td>\n"
            "</tr>\n"
            "<tr>\n"
                "<td>s_free_inodes_count</td>\n"
                "<td>" + to_string(spb.s_free_inodes_count) + "</td>\n"
            "</tr>\n"
            "<tr>\n"
                "<td>s_mtime</td>\n"
                "<td>" + string(s_mtime) + "</td>\n"
            "</tr>\n"
            "<tr>\n"
                "<td>s_umtime</td>\n"
                "<td>" + string(s_umtime) + "</td>\n"
            "</tr>\n"
            "<tr>\n"
                "<td>s_mnt_count</td>\n"
                "<td>" + to_string(spb.s_mnt_count) + "</td>\n"
            "</tr>\n"
            "<tr>\n"
                "<td>s_magic</td>\n"
                "<td>" + to_string(spb.s_magic) + "</td>\n"
            "</tr>\n"
            "<tr>\n"
                "<td>s_inode_size</td>\n"
                "<td>" + to_string(spb.s_inode_size) + "</td>\n"
            "</tr>\n"
            "<tr>\n"
                "<td>s_block_size</td>\n"
                "<td>" + to_string(spb.s_block_size) + "</td>\n"
            "</tr>\n"
            "<tr>\n"
                "<td>s_first_ino</td>\n"
                "<td>" + to_string(spb.s_first_ino) + "</td>\n"
            "</tr>\n"
            "<tr>\n"
                "<td>s_first_blo</td>\n"
                "<td>" + to_string(spb.s_first_blo) + "</td>\n"
            "</tr>\n"
            "<tr>\n"
                "<td>s_bm_inode_start</td>\n"
                "<td>" + to_string(spb.s_bm_inode_start) + "</td>\n"
            "</tr>\n"
            "<tr>\n"
                "<td>s_bm_block_start</td>\n"
                "<td>" + to_string(spb.s_bm_block_start) + "</td>\n"
            "</tr>\n"
            "<tr>\n"
                "<td>s_inode_start</td>\n"
                "<td>" + to_string(spb.s_inode_start) + "</td>\n"
            "</tr>\n"
            "<tr>\n"
                "<td>s_block_start</td>\n"
                "<td>" + to_string(spb.s_block_start) + "</td>\n"
            "</tr>\n";
    draw += "</table>>];";
    draw += "\n\n}\n";
    create_FileReport(draw, path);
    cout << csnt_rp.GREEN << "RESPUESTA:" << csnt_rp.NC << " El reporte SB ha sido generado correctamente " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
}

void reportes::journaling(map<string, string> param_got, vector<disco::Mount> list_mount){

    if (param_got.size() == 0){
        return;
    }
    /*Obteniendo datos*/
    string comentario = param_got["-comentario"].c_str();
    string name = param_got["-name"].c_str(); //Este es el nombre del reporte
    string path = param_got["-path"].c_str();
    string id = param_got["-id"].c_str();
    string ruta = param_got["-ruta"].c_str();

    /*Formateo de datos*/
    path = (path.substr(0,1) == "\"") ? path.substr(1, path.size()-2): path;
    util_rp.createDirectory(path);

    /*Flujo del void*/
    disco::Mount tempMount = getMount(id, list_mount);
    if (tempMount.status == '0'){
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha encontrado el id solicitado " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
        return;
    }

    //Se crean variables que se utilizaran posteriormente
    string path_aux = tempMount.path;
    string name_aux = tempMount.name;
    time_t date_mounted = tempMount.date_mounted;
    int part_start_partition = tempMount.part_start;
    
    //Obtengo la estructura del superbloque
    disco::Superblock spb;
    FILE *file_journaling = fopen(path_aux.c_str(), "rb+");
    fseek(file_journaling, part_start_partition, SEEK_SET);
    int read = fread(&spb, csnt_rp.SIZE_SPB, 1, file_journaling);
    if (read <= 0){
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha podido leer el disco " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
        return;
    }
    if (spb.s_filesystem_type == 0)    {
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha formateado esta particion " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
        return;
    }else if(spb.s_filesystem_type == 2){
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " Es un sistema EXT2 por lo tanto no se tiene un journaling " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
        return;
    }
    
    //Se busca los journaling realizados
    vector<disco::Journaling> listJournal;
    fseek(file_journaling, part_start_partition + csnt_rp.SIZE_SPB, SEEK_SET);
    while (true){
        disco::Journaling *tempJournal = new disco::Journaling();
        fread(tempJournal, csnt_rp.SIZE_J, 1, file_journaling);
        if (tempJournal->id_journal != -1){
            listJournal.push_back(*tempJournal);
        }else{
            break;
        }
    }
    fclose(file_journaling);

    struct tm *tm;
    char fecha_transaccion[20];
    string draw;
    draw = "digraph G{\n"
            "forcelabels= true;\n"
            "node [shape = plaintext];\n"
            "general [label = <\n"
            "<table>\n"
            "<tr><td COLSPAN = '8' BGCOLOR=\"#BFBFBF\"><font color=\"black\">JOURNALING - " + name_aux + " en " + path_aux + "</font></td></tr>\n"
            "<tr><td BGCOLOR=\"#E0FFFF\">Id</td><td BGCOLOR=\"#E0FFFF\">Operacion</td><td BGCOLOR=\"#E0FFFF\" >Tipo</td><td BGCOLOR=\"#E0FFFF\">Nombre</td><td BGCOLOR=\"#E0FFFF\">Argumento</td><td BGCOLOR=\"#E0FFFF\">Fecha Transaccion</td><td BGCOLOR=\"#E0FFFF\">Propietario</td><td BGCOLOR=\"#E0FFFF\">Permisos</td></tr>\n";
    for (int i = 0; i < listJournal.size(); i++){
        tm = localtime(&listJournal[i].date);
        strftime(fecha_transaccion, 20, "%Y/%m/%d %H:%M:%S", tm);
        string type_block = (listJournal[i].type == '0')? "Carpeta": "Archivo";
        string operation_block(1,listJournal[i].operation);
        draw += "<tr>"
                "<td>" + to_string(listJournal[i].id_journal) + "</td>\n"
                "<td>" + operation_block + "</td>\n"
                "<td>" + type_block + "</td>\n"
                "<td>" + listJournal[i].nombre + "</td>\n"
                "<td>" + listJournal[i].content + "</td>\n"
                "<td>" + fecha_transaccion + "</td>\n"
                "<td>" + listJournal[i].propietario + "</td>\n"
                "<td>" + to_string(listJournal[i].permiso) + "</td>\n"
                "</tr>\n";
    }
    draw += "</table>>];";
    draw += "\n\n}\n";
    create_FileReport(draw, path);
    cout << csnt_rp.GREEN << "RESPUESTA:" << csnt_rp.NC << " El reporte JOURNALING ha sido generado correctamente " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
    return;
}

void reportes::bm_inode(map<string, string> param_got, vector<disco::Mount> list_mount){

    if (param_got.size() == 0){
        return;
    }
    /*Obteniendo datos*/
    string comentario = param_got["-comentario"].c_str();
    string name = param_got["-name"].c_str(); //Este es el nombre del reporte
    string path = param_got["-path"].c_str();
    string id = param_got["-id"].c_str();
    string ruta = param_got["-ruta"].c_str();

    /*Formateo de datos*/
    path = (path.substr(0,1) == "\"") ? path.substr(1, path.size()-2): path;
    util_rp.createDirectory(path);

    /*Flujo del void*/
    disco::Mount tempMount = getMount(id, list_mount);
    if (tempMount.status == '0'){
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha encontrado el id solicitado " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
        return;
    }

    //Se crean variables que se utilizaran posteriormente
    string path_aux = tempMount.path;
    string name_aux = tempMount.name;
    time_t date_mounted = tempMount.date_mounted;
    int part_start_partition = tempMount.part_start;
    
    //Obtengo la estructura del superbloque
    disco::Superblock spb;
    FILE *file = fopen(path_aux.c_str(), "rb");
    fseek(file, part_start_partition, SEEK_SET);
    int read = fread(&spb, csnt_rp.SIZE_SPB, 1, file);
    if (read <= 0){
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha podido leer el disco " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
        return;
    }

    unsigned char buffer[spb.s_inodes_count];
    fseek(file, spb.s_bm_inode_start, SEEK_SET);
    fread(&buffer, sizeof(char), spb.s_inodes_count, file);
    fclose(file);

    if (spb.s_filesystem_type == 0)    {
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha formateado esta particion " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
        return;
    }
    
    int columnas = 20;
    int filas = round(spb.s_inodes_count/columnas);

    string draw;
    draw = "digraph G{\n"
            "forcelabels= true;\n"
            "node [shape = plaintext];\n"
            "general [label = <\n";
    for (int i = 0; i < spb.s_inodes_count; i++){
        if (i % columnas== 0){
            draw += "\n";
            draw += buffer[i];
        }else if(i % columnas != 0){
            draw += buffer[i];
        }
    }
    draw += "\n>];";
    draw += "\n}\n";
    create_FileReport(draw, path);
    cout << csnt_rp.GREEN << "RESPUESTA:" << csnt_rp.NC << " El reporte BM_INODE ha sido generado correctamente " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;

}

void reportes::bm_block(map<string, string> param_got, vector<disco::Mount> list_mount){

    if (param_got.size() == 0){
        return;
    }
    /*Obteniendo datos*/
    string comentario = param_got["-comentario"].c_str();
    string name = param_got["-name"].c_str(); //Este es el nombre del reporte
    string path = param_got["-path"].c_str();
    string id = param_got["-id"].c_str();
    string ruta = param_got["-ruta"].c_str();

    /*Formateo de datos*/
    path = (path.substr(0,1) == "\"") ? path.substr(1, path.size()-2): path;
    util_rp.createDirectory(path);

    /*Flujo del void*/
    disco::Mount tempMount = getMount(id, list_mount);
    if (tempMount.status == '0'){
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha encontrado el id solicitado " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
        return;
    }

    //Se crean variables que se utilizaran posteriormente
    string path_aux = tempMount.path;
    string name_aux = tempMount.name;
    time_t date_mounted = tempMount.date_mounted;
    int part_start_partition = tempMount.part_start;
    
    //Obtengo la estructura del superbloque
    disco::Superblock spb;
    FILE *file = fopen(path_aux.c_str(), "rb");
    fseek(file, part_start_partition, SEEK_SET);
    int read = fread(&spb, csnt_rp.SIZE_SPB, 1, file);
    if (read <= 0){
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha podido leer el disco " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
        return;
    }

    unsigned char buffer[spb.s_blocks_count];
    fseek(file, spb.s_bm_block_start, SEEK_SET);
    fread(&buffer, sizeof(char), spb.s_blocks_count, file);
    fclose(file);

    if (spb.s_filesystem_type == 0)    {
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha formateado esta particion " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
        return;
    }
    
    int columnas = 20;
    int filas = spb.s_blocks_count/columnas;

    string draw;
    draw = "digraph G{\n"
            "forcelabels= true;\n"
            "node [shape = plaintext];\n"
            "general [label = <\n";
    for (int i = 0; i < spb.s_blocks_count; i++){
        if (i % columnas == 0){
            draw += "\n";
            draw += buffer[i];
        }else if(i % columnas != 0){
            draw += buffer[i];
        }
    }
    draw += "\n>];";
    draw += "\n}\n";
    
    create_FileReport(draw, path);
    cout << csnt_rp.GREEN << "RESPUESTA:" << csnt_rp.NC << " El reporte BM_BLOCK ha sido generado correctamente " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;

}

void reportes::file(map<string, string> param_got, vector<disco::Mount> list_mount){
    if (param_got.size() == 0){
        return;
    }
    /*Obteniendo datos*/
    string comentario = param_got["-comentario"].c_str();
    string name = param_got["-name"].c_str(); //Este es el nombre del reporte
    string path = param_got["-path"].c_str();
    string id = param_got["-id"].c_str();
    string ruta = param_got["-ruta"].c_str();

    if (ruta.empty()){
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No ha ingresado la ruta de busqueda " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
        return;
    }    

    /*Formateo de datos*/
    path = (path.substr(0,1) == "\"") ? path.substr(1, path.size()-2): path;
    ruta = (ruta.substr(0,1) == "\"") ? ruta.substr(1, ruta.size()-2): ruta;
    util_rp.createDirectory(path);

    /*Flujo del void*/
    disco::Mount gettingMount = getMount(id, list_mount);
    if (gettingMount.status == '0'){
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha encontrado el id solicitado " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
        return;
    }

    //Se verifica que el path sea correcto
    vector<string> lista_path = tokenizarPath(ruta, 'A');
    if (lista_path.size() == 0){
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " El path ingresado es incorrecto; Los errores pueden ser 1. Debido a que excede los doce caracteres disponibles para una carpeta. 2. Es un problema de ruta " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
        return;
    }    

    //Creo una variable de tipo FILE
    FILE *file = fopen(gettingMount.path.c_str(), "rb+");

    //Obtengo el super bloque
    disco::Superblock *sp_user = new disco::Superblock();
    fseek(file, gettingMount.part_start, SEEK_SET);
    fread(sp_user, csnt_rp.SIZE_SPB, 1, file);
    if (sp_user->s_filesystem_type == 0)    {
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha formateado esta particion " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
        fclose(file);
        return;
    }

    //INI - SE VERIFICA LA EXISTENCIA DEL PATH INGRESADO
    vector <vector<string>> path_niveles;
    vector <string> temporal;
    for (int i = 0; i < lista_path.size(); i++){
        temporal.push_back(lista_path[i]);
        path_niveles.push_back(temporal);
    }
    temporal.clear();

    //Se verificara hasta que diagonal del path ya no encuentra alguna carpeta
    int diagonal = -1;
    for (int i = 0; i < path_niveles.size(); i++){
        //Archivos y Carpetas no Encontrados
        vector <string> ACE = path_niveles[i];
        int pos_inodo_a_editar = 0;
        int pos_padre = -1;
        searchInode(gettingMount.path.c_str(), &pos_padre, &pos_inodo_a_editar, sp_user, &ACE);
        if (ACE.size() > 0){
            diagonal = i;
            break;
        }
    }
    //Se busca cuantas carpetas falta por ingresar
    int diferencia = (diagonal == -1)? 0: path_niveles.size() - diagonal;
    //Se verifica si hace falta ingresar mas de una carpeta
    if (diferencia > 0){
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No existe el archivo segun el path ingresado " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
        fclose(file);
        return;
    }
    //FIN - SE VERIFICA LA EXISTENCIA DEL PATH INGRESADO
    
    //Se busca el bloque de archivo
    int pos_inodo_a_editar = 0;
    int pos_inodo_padre = -1;
    int pos_bloque_encontrado = -1;
    string nombre_archivo = lista_path[lista_path.size() - 1];
    searchBlock(gettingMount.path.c_str(), &pos_inodo_padre, &pos_inodo_a_editar, &pos_bloque_encontrado, sp_user, &lista_path);
    disco::Inode inodoPadre = getInodo(pos_inodo_padre, *sp_user, gettingMount.path.c_str());

    string contenido = getContentArchive(pos_inodo_a_editar, gettingMount.path.c_str(), *sp_user);
    cout << "\e[1m" << "ARCHIVO: " << nombre_archivo << "\e[0m" << endl;
    cout << csnt_rp.YELLOW << contenido << csnt_rp.NC << endl;
    fclose(file);
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
    
    int size = path.size() - 1;
    while (path.substr(size, 1) != "/"){
        size--;
    }
    int posicion_extension = path.size()-1;
    while (path.substr(posicion_extension, 1) != "."){
        posicion_extension--;
    }
    string extension = "-T" + path.substr(posicion_extension+1, path.size()-1);
    string ubicacion_salida = path.substr(0, path.size() - 4);
    string ubicacion_dot = path.substr(0, path.size() - 4)+".dot";
    FILE* file_report = fopen(ubicacion_dot.c_str(), "w+");
    fwrite(draw.c_str(), draw.size(), 1, file_report);
    fclose(file_report);
    string function = "dot " + extension + " " + ubicacion_dot +" -o " + ubicacion_salida;
    system(function.c_str());
    remove(ubicacion_dot.c_str());
}

void reportes::tree(map<string, string> param_got, vector<disco::Mount> list_mount){
    if (param_got.size() == 0){
        return;
    }
    /*Obteniendo datos*/
    string comentario = param_got["-comentario"].c_str();
    string name = param_got["-name"].c_str(); //Este es el nombre del reporte
    string path = param_got["-path"].c_str();
    string id = param_got["-id"].c_str();
    string ruta = param_got["-ruta"].c_str();
    int root = atoi(param_got["-root"].c_str());

    /*Formateo de datos*/
    path = (path.substr(0,1) == "\"") ? path.substr(1, path.size()-2): path;
    util_rp.createDirectory(path);

    /*Flujo del void*/
    //Se busca la montura segun el id
    disco::Mount tempMount = getMount(id, list_mount);
    if (tempMount.status == '0'){
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha encontrado el id solicitado " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
        return;
    }
    
    //Se crean variables que se utilizaran posteriormente
    string path_aux = tempMount.path;
    string name_aux = tempMount.name;
    time_t date_mounted = tempMount.date_mounted;
    int part_start_partition = tempMount.part_start;
    
    //Se crea un file para leer las estructuras iniciales
    FILE *file_tree = fopen(path_aux.c_str(), "rb");

    //Obtengo la estructura del superbloque
    disco::Superblock spb;    
    fseek(file_tree, part_start_partition, SEEK_SET);
    int read = fread(&spb, csnt_rp.SIZE_SPB, 1, file_tree);
    if (read <= 0){
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha podido leer el disco " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
        return;
    }
    if (spb.s_filesystem_type == 0)    {
        cout << csnt_rp.RED << "ERROR:" << csnt_rp.NC << " No se ha formateado esta particion " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
        return;
    }
    
    disco::Inode inodo_inicial;
    fseek(file_tree, spb.s_inode_start, SEEK_SET);
    fread(&inodo_inicial, csnt_rp.SIZE_I, 1, file_tree);
    fclose(file_tree);

    string draw = "";
    draw += "digraph {\n"
    "graph [pad=\"0.5\", nodesep=\"0.5\", ranksep=\"2\"];\n"
    "node [shape=plain]\n"
    "rankdir=LR;\n";
    string relations = "";
    int contador_inodo = 0;
    int contador_bloque = 0;
    int encabezado_inodo = 0;
    drawInode(inodo_inicial, path_aux, "", true, encabezado_inodo, &contador_inodo, &contador_bloque, &draw, &relations, &spb);
    draw += relations + "\n";
    draw += "}";

    create_FileReport(draw, path);
    cout << csnt_rp.GREEN << "RESPUESTA:" << csnt_rp.NC << " El reporte TREE ha sido generado correctamente " << csnt_rp.BLUE << comentario << csnt_rp.NC << endl;
}

void reportes::drawInode(disco::Inode inodo, string path, string estructura_inicial, bool isInodeInit, int encabezado_inodo, int *contador_inodo, int *contador_bloque, string *draw, string *relations, disco::Superblock *spb){
    
    vector<Punteros_aux> listado_punteros;
    int cantidad_punteros_utilizados = 0;
    for (int i = 0; i < 15; i++){
        if (inodo.i_block[i] != -1){
            cantidad_punteros_utilizados++;
        }
    }    
    struct tm *tm;
    char fecha_transaccion[20];
    string nombre_estructura = "inodo"+to_string(*contador_inodo);
    *draw += nombre_estructura+" [label=<\n"
    "<TABLE border='0' cellborder='1' cellspacing='0'>\n"
    "<TR>\n"
        "<TD COLSPAN='3' BGCOLOR = '"+ csnt_rp.COLOR_INODE +"'>INODO " + to_string(encabezado_inodo) + "</TD>\n"
    "</TR>\n";
    if (!isInodeInit){
        *relations += estructura_inicial + "->" + nombre_estructura + "\n";
    }
    *contador_inodo = *contador_inodo + 1;
    *draw += "<TR>\n"
        "<TD BGCOLOR = '"+ csnt_rp.COLOR_INODE +"'>i_uid</TD>\n"
        "<TD COLSPAN='2' BGCOLOR = '"+ csnt_rp.COLOR_INODE +"'>"+ to_string(inodo.i_uid) + "</TD>\n"
    "</TR>\n"
    "<TR>\n"
        "<TD BGCOLOR = '"+ csnt_rp.COLOR_INODE +"'>i_gid</TD>\n"
        "<TD COLSPAN='2' BGCOLOR = '"+ csnt_rp.COLOR_INODE +"'>"+ to_string(inodo.i_gid) + "</TD>\n"
    "</TR>\n"
    "<TR>\n"
        "<TD BGCOLOR = '"+ csnt_rp.COLOR_INODE +"'>i_size</TD>\n"
        "<TD COLSPAN='2' BGCOLOR = '"+ csnt_rp.COLOR_INODE +"'>"+ to_string(inodo.i_size) + "</TD>\n"
    "</TR>\n";
    tm = localtime(&inodo.i_atime);
    strftime(fecha_transaccion, 20, "%Y/%m/%d %H:%M:%S", tm);
    *draw += "<TR>\n"
        "<TD BGCOLOR = '"+ csnt_rp.COLOR_INODE +"'>i_atime</TD>\n"
        "<TD COLSPAN='2' BGCOLOR = '"+ csnt_rp.COLOR_INODE +"'>"+ fecha_transaccion + "</TD>\n"
    "</TR>\n";
    tm = localtime(&inodo.i_ctime);
    strftime(fecha_transaccion, 20, "%Y/%m/%d %H:%M:%S", tm);
    *draw += "<TR>\n"
        "<TD BGCOLOR = '"+ csnt_rp.COLOR_INODE +"'>i_ctime</TD>\n"
        "<TD COLSPAN='2' BGCOLOR = '"+ csnt_rp.COLOR_INODE +"'>"+ fecha_transaccion + "</TD>\n"
    "</TR>\n";
    tm = localtime(&inodo.i_mtime);
    strftime(fecha_transaccion, 20, "%Y/%m/%d %H:%M:%S", tm);
    *draw += "<TR>\n"
        "<TD BGCOLOR = '"+ csnt_rp.COLOR_INODE +"'>i_mtime</TD>\n"
        "<TD COLSPAN='2' BGCOLOR = '"+ csnt_rp.COLOR_INODE +"'>"+ fecha_transaccion + "</TD>\n"
    "</TR>\n";
    if (cantidad_punteros_utilizados > 0){
        *draw += "<TR>\n"
            "<TD ROWSPAN='"+to_string(cantidad_punteros_utilizados+1)+"' BGCOLOR = '"+ csnt_rp.COLOR_INODE +"'>i_block</TD>\n"
        "</TR>\n";
    }
    for (int i = 0; i < 15; i++){
        if (inodo.i_block[i] != -1){
            *draw += "<TR>\n"
                "<TD BGCOLOR = '"+ csnt_rp.COLOR_INODE +"'>P_"+ to_string(i+1) +"</TD>\n"
                "<TD BGCOLOR = '"+ csnt_rp.COLOR_INODE +"' port='"+to_string(i)+"'>"+to_string(inodo.i_block[i])+"</TD>\n"
            "</TR>\n";
            Punteros_aux temp_p;
            temp_p.num_puntero = i;
            temp_p.posicion = inodo.i_block[i];
            listado_punteros.push_back(temp_p);
        }
    }
    *draw += "<TR>\n"
        "<TD BGCOLOR = '"+ csnt_rp.COLOR_INODE +"'>i_type</TD>\n"
        "<TD COLSPAN='2' BGCOLOR = '"+ csnt_rp.COLOR_INODE +"'>"+ inodo.i_type +"</TD>\n"
    "</TR>\n"
    "<TR>\n"
        "<TD BGCOLOR = '"+ csnt_rp.COLOR_INODE +"'>i_perm</TD>\n"
        "<TD COLSPAN='2' BGCOLOR = '"+ csnt_rp.COLOR_INODE +"'>"+ to_string(inodo.i_perm) + "</TD>\n"
    "</TR>\n"
    "</TABLE>\n"
    ">];\n";

    if (inodo.i_type == '0'){
        FILE *file_inode = fopen(path.c_str(), "rb");
        for (int i = 0; i < listado_punteros.size(); i++){
            int posicion_bloque = listado_punteros[i].posicion;
            //Apuntadores 1 al 12 de un inodo de tipo carpeta en donde lo que se va a leer son bloques de tipo folder
            if (listado_punteros[i].num_puntero >= 0 && listado_punteros[i].num_puntero < 12){
                disco::Folderblock folder;
                fseek(file_inode, spb->s_block_start + (posicion_bloque * csnt_rp.SIZE_FB), SEEK_SET);
                fread(&folder, csnt_rp.SIZE_FB, 1, file_inode);
                //Dibuja carpeta
                drawFolderBlock(folder, path, nombre_estructura+":"+to_string(listado_punteros[i].num_puntero), false, posicion_bloque, contador_inodo, contador_bloque, draw, relations, spb);
            }else if(listado_punteros[i].num_puntero == 12){
                //Se lee el apuntador 13
                disco::Pointerblock pointer;
                fseek(file_inode, spb->s_block_start + (posicion_bloque * csnt_rp.SIZE_PB), SEEK_SET);
                fread(&pointer, csnt_rp.SIZE_PB, 1, file_inode);
                //Dibuja el apuntador
                drawPointerBlock(pointer, path, nombre_estructura+":"+to_string(listado_punteros[i].num_puntero), false, posicion_bloque, "BSI", '0', contador_inodo, contador_bloque, draw, relations, spb);
            }else if(listado_punteros[i].num_puntero == 13){
                //Se lee el apuntador 14
                disco::Pointerblock pointer;
                fseek(file_inode, spb->s_block_start + (posicion_bloque * csnt_rp.SIZE_PB), SEEK_SET);
                fread(&pointer, csnt_rp.SIZE_PB, 1, file_inode);
                //Dibuja el apuntador
                drawPointerBlock(pointer, path, nombre_estructura+":"+to_string(listado_punteros[i].num_puntero), false, posicion_bloque, "BDI", '0', contador_inodo, contador_bloque, draw, relations, spb);
            }else if(listado_punteros[i].num_puntero == 14){
                //Se lee el apuntador 15
                disco::Pointerblock pointer;
                fseek(file_inode, spb->s_block_start + (posicion_bloque * csnt_rp.SIZE_PB), SEEK_SET);
                fread(&pointer, csnt_rp.SIZE_PB, 1, file_inode);
                //Dibuja el apuntador
                drawPointerBlock(pointer, path, nombre_estructura+":"+to_string(listado_punteros[i].num_puntero), false, posicion_bloque, "BTI", '0', contador_inodo, contador_bloque, draw, relations, spb);
            }
            
        }
        fclose(file_inode);
    }else if(inodo.i_type == '1'){
        FILE *file_inode = fopen(path.c_str(), "rb");
        for (int i = 0; i < listado_punteros.size(); i++){
            int posicion_bloque = listado_punteros[i].posicion;
            if (listado_punteros[i].num_puntero >= 0 && listado_punteros[i].num_puntero < 12){
                //Se leen directamente estructuras de tipo archivo
                disco::Archiveblock archive;
                fseek(file_inode, spb->s_block_start + (posicion_bloque * csnt_rp.SIZE_AB), SEEK_SET);
                fread(&archive, csnt_rp.SIZE_AB, 1, file_inode);
                //Dibuja el archivo
                drawArchiveBlock(archive, path, nombre_estructura+":"+to_string(listado_punteros[i].num_puntero), false, posicion_bloque, contador_inodo, contador_bloque, draw, relations, spb);          
            }else if(listado_punteros[i].num_puntero == 12){
                //Se lee el apuntador 13
                disco::Pointerblock pointer;//En el metodo del apuntador hay que indicar que tipo es
                fseek(file_inode, spb->s_block_start + (posicion_bloque * csnt_rp.SIZE_PB), SEEK_SET);
                fread(&pointer, csnt_rp.SIZE_PB, 1, file_inode);
                //Dibuja el apuntador
                drawPointerBlock(pointer, path, nombre_estructura+":"+to_string(listado_punteros[i].num_puntero), false, posicion_bloque, "BSI", '1', contador_inodo, contador_bloque, draw, relations, spb);
            }else if(listado_punteros[i].num_puntero == 13){
                //Se lee el apuntador 14
                disco::Pointerblock pointer;
                fseek(file_inode, spb->s_block_start + (posicion_bloque * csnt_rp.SIZE_PB), SEEK_SET);
                fread(&pointer, csnt_rp.SIZE_PB, 1, file_inode);
                //Dibuja el apuntador
                drawPointerBlock(pointer, path, nombre_estructura+":"+to_string(listado_punteros[i].num_puntero), false, posicion_bloque, "BDI", '1', contador_inodo, contador_bloque, draw, relations, spb);
            }else if(listado_punteros[i].num_puntero == 14){
                //Se lee el apuntador 15
                disco::Pointerblock pointer;
                fseek(file_inode, spb->s_block_start + (posicion_bloque * csnt_rp.SIZE_PB), SEEK_SET);
                fread(&pointer, csnt_rp.SIZE_PB, 1, file_inode);
                //Dibuja el apuntador
                drawPointerBlock(pointer, path, nombre_estructura+":"+to_string(listado_punteros[i].num_puntero), false, posicion_bloque, "BTI", '1', contador_inodo, contador_bloque, draw, relations, spb);
            }
        }
        fclose(file_inode);
    }
}

void reportes::drawFolderBlock(disco::Folderblock carpeta, string path, string estructura_inicial, bool isInodeInit, int encabezado_bloque, int *contador_inodo, int *contador_bloque, string *draw, string *relations, disco::Superblock *spb){
    vector<Punteros_aux> listado_punteros;
    int cantidad_punteros_utilizados = 0;
    for (int i = 0; i < 4; i++){
        if (carpeta.b_content[i].b_inodo != -1){
            cantidad_punteros_utilizados++;
        }
    }    
    struct tm *tm;
    char fecha_transaccion[20];
    string nombre_estructura = "bloque"+to_string(*contador_bloque);
    *draw += nombre_estructura+" [label=<\n"
    "<TABLE border='0' cellborder='1' cellspacing='0'>\n"
    "<TR>\n"
        "<TD COLSPAN='3' BGCOLOR = '"+ csnt_rp.COLOR_FOLDER +"'>BLOQUE " + to_string(encabezado_bloque) + "</TD>\n"
    "</TR>\n";
    if (!isInodeInit){
        *relations += estructura_inicial + "->" + nombre_estructura + "\n";
    }
    *contador_bloque = *contador_bloque + 1;

    if (cantidad_punteros_utilizados > 0){
        *draw += "<TR>\n"
            "<TD ROWSPAN='"+to_string(cantidad_punteros_utilizados+1)+"' BGCOLOR = '"+ csnt_rp.COLOR_FOLDER +"'>b_content</TD>\n"
        "</TR>\n";
    }
    for (int i = 0; i < 4; i++){
        if (carpeta.b_content[i].b_inodo != -1){
            *draw += "<TR>\n"
                "<TD BGCOLOR = '"+ csnt_rp.COLOR_FOLDER +"'>"+ carpeta.b_content[i].b_name +"</TD>\n"
                "<TD BGCOLOR = '"+ csnt_rp.COLOR_FOLDER +"' port='"+to_string(i)+"'>"+to_string(carpeta.b_content[i].b_inodo)+"</TD>\n"
            "</TR>\n";
            Punteros_aux temp_p;
            temp_p.num_puntero = i;
            temp_p.posicion = carpeta.b_content[i].b_inodo;
            temp_p.nombre = carpeta.b_content[i].b_name;
            listado_punteros.push_back(temp_p);
        }
    }
    *draw += "</TABLE>\n"
    ">];\n";

    FILE *file_folder = fopen(path.c_str(), "rb");
    for (int i = 0; i < listado_punteros.size(); i++){
        int posicion_inodo = listado_punteros[i].posicion;
        if (!(listado_punteros[i].nombre == "." || listado_punteros[i].nombre == "..")){
            disco::Inode tempInodo;
            fseek(file_folder, spb->s_inode_start + (posicion_inodo * csnt_rp.SIZE_I), SEEK_SET);
            fread(&tempInodo, csnt_rp.SIZE_I, 1, file_folder);
            //Dibuja Inodo
            drawInode(tempInodo, path, nombre_estructura+":"+to_string(listado_punteros[i].num_puntero), false, posicion_inodo, contador_inodo, contador_bloque, draw, relations, spb);
        }
    }
    fclose(file_folder);
}

void reportes::drawArchiveBlock(disco::Archiveblock archivo, string path, string estructura_inicial, bool isInodeInit, int encabezado_bloque, int *contador_inodo, int *contador_bloque, string *draw, string *relations, disco::Superblock *spb){

    string nombre_estructura = "bloque"+to_string(*contador_bloque);
    *draw += nombre_estructura+" [label=<\n"
    "<TABLE border='0' cellborder='1' cellspacing='0'>\n"
    "<TR>\n"
        "<TD COLSPAN='2' BGCOLOR = '"+ csnt_rp.COLOR_ARCHIVE +"'>BLOQUE " + to_string(encabezado_bloque) + "</TD>\n"
    "</TR>\n";
    if (!isInodeInit){
        *relations += estructura_inicial + "->" + nombre_estructura + "\n";
    }
    *contador_bloque = *contador_bloque + 1;
    *draw += "<TR>\n"
        "<TD BGCOLOR = '"+ csnt_rp.COLOR_ARCHIVE +"'>b_content</TD>\n"
        "<TD BGCOLOR = '"+ csnt_rp.COLOR_ARCHIVE +"'>"+ string(archivo.b_content) + "</TD>\n"
    "</TR>\n";
    *draw += "</TABLE>\n"
    ">];\n";

}

void reportes::drawPointerBlock(disco::Pointerblock apuntador, string path, string estructura_inicial, bool isInodeInit, int encabezado_bloque, string tipo_apuntador, char tipo_inodo, int *contador_inodo, int *contador_bloque, string *draw, string *relations, disco::Superblock *spb){
    vector<Punteros_aux> listado_punteros;
    int cantidad_punteros_utilizados = 0;
    for (int i = 0; i < 15; i++){
        if (apuntador.b_pointers[i] != -1){
            cantidad_punteros_utilizados++;
        }
    }
    string nombre_estructura = "bloque"+to_string(*contador_bloque);
    *draw += nombre_estructura+" [label=<\n"
    "<TABLE border='0' cellborder='1' cellspacing='0'>\n"
    "<TR>\n"
        "<TD COLSPAN='3' BGCOLOR = '"+ csnt_rp.COLOR_POINTER +"'>BLOQUE " + to_string(encabezado_bloque) + "</TD>\n"
    "</TR>\n";
    if (!isInodeInit){
        *relations += estructura_inicial + "->" + nombre_estructura + "\n";
    }
    *contador_bloque = *contador_bloque + 1;

    if (cantidad_punteros_utilizados > 0){
        *draw += "<TR>\n"
            "<TD ROWSPAN='"+to_string(cantidad_punteros_utilizados+1)+"' BGCOLOR = '"+ csnt_rp.COLOR_POINTER +"'>b_content</TD>\n"
        "</TR>\n";
    }
    for (int i = 0; i < 15; i++){
        if (apuntador.b_pointers[i] != -1){
            *draw += "<TR>\n"
                "<TD BGCOLOR = '"+ csnt_rp.COLOR_POINTER +"'>"+ to_string(i) +"</TD>\n"
                "<TD BGCOLOR = '"+ csnt_rp.COLOR_POINTER +"' port='"+to_string(i)+"'>"+to_string(apuntador.b_pointers[i])+"</TD>\n"
            "</TR>\n";
            Punteros_aux temp_p;
            temp_p.num_puntero = i;
            temp_p.posicion = apuntador.b_pointers[i];
            listado_punteros.push_back(temp_p);
        }
    }
    *draw += "</TABLE>\n"
    ">];\n";

    FILE *file_pointer = fopen(path.c_str(), "rb");
    if (tipo_apuntador == "BSI"){
        if (tipo_inodo == '0'){
            //El tipo_inodo = 0 solo me indica que a lo que se apunta es a un bloque de tipo folder
            for (int i = 0; i < listado_punteros.size(); i++){
                int posicion_bloque = listado_punteros[i].posicion;
                disco::Folderblock folder;
                fseek(file_pointer, spb->s_block_start + (posicion_bloque * csnt_rp.SIZE_FB), SEEK_SET);
                fread(&folder, csnt_rp.SIZE_FB, 1, file_pointer);
                //Dibuja carpeta
                drawFolderBlock(folder, path, nombre_estructura+":"+to_string(listado_punteros[i].num_puntero), false, posicion_bloque, contador_inodo, contador_bloque, draw, relations, spb);
            }
        }else if(tipo_inodo == '1'){
            //El tipo_inodo = 1 solo me indica que a lo que se apunta es a un archivo
            for (int i = 0; i < listado_punteros.size(); i++){
                int posicion_bloque = listado_punteros[i].posicion;
                disco::Archiveblock archive;
                fseek(file_pointer, spb->s_block_start + (posicion_bloque * csnt_rp.SIZE_AB), SEEK_SET);
                fread(&archive, csnt_rp.SIZE_AB, 1, file_pointer);
                //Dibuja el archivo
                drawArchiveBlock(archive, path, nombre_estructura+":"+to_string(listado_punteros[i].num_puntero), false, posicion_bloque, contador_inodo, contador_bloque, draw, relations, spb);                
            }
        }
    }else if(tipo_apuntador == "BDI"){
        for (int i = 0; i < listado_punteros.size(); i++){
            int posicion_bloque = listado_punteros[i].posicion;
            disco::Pointerblock pointer;
            fseek(file_pointer, spb->s_block_start + (posicion_bloque * csnt_rp.SIZE_PB), SEEK_SET);
            fread(&pointer, csnt_rp.SIZE_PB, 1, file_pointer);
            //Dibuja los apuntadores
            drawPointerBlock(pointer, path, nombre_estructura+":"+to_string(listado_punteros[i].num_puntero), false, posicion_bloque, "BSI", tipo_inodo, contador_inodo, contador_bloque, draw, relations, spb);
        }
    }else if(tipo_apuntador == "BTI"){
        for (int i = 0; i < listado_punteros.size(); i++){
            int posicion_bloque = listado_punteros[i].posicion;
            disco::Pointerblock pointer;
            fseek(file_pointer, spb->s_block_start + (posicion_bloque * csnt_rp.SIZE_PB), SEEK_SET);
            fread(&pointer, csnt_rp.SIZE_PB, 1, file_pointer);
            //Dibuja los apuntadores
            drawPointerBlock(pointer, path, nombre_estructura+":"+to_string(listado_punteros[i].num_puntero), false, posicion_bloque, "BDI", tipo_inodo, contador_inodo, contador_bloque, draw, relations, spb);
        }
    }
    fclose(file_pointer);
}

vector<string> reportes::tokenizarPath(string path, char tipo_path){
    vector<string> response;
    if (path.substr(0, 1) != "/"){
        return response;
    }
    if (path.substr(0, path.size()) != "/"){
        path += "/";
    }

    int inicio = 1;
    int tamanio = 0;
    bool ya_existe_un_archivo = false;
    for (int i = 1; i < path.size(); i++){
        if (path.substr(i, 1) == "/"){
            string nombre = path.substr(inicio, tamanio);
            if (nombre.size() > 11){
                response.clear();
                break;
            }
            if (nombre.size() == 0){
                response.clear();
                break;
            }            
            char tipo = typeText(nombre);
            if (tipo == 'E'){
                response.clear();
                break;
            }else if(tipo == 'F' && !ya_existe_un_archivo){
                response.push_back(nombre);
                inicio = i+1;
                tamanio = 0;
            }else if(tipo == 'F' && ya_existe_un_archivo){
                response.clear();
                break;
            }else if(tipo == 'A' && tipo_path == 'F'){
                response.clear();
                break;
            }else if(tipo == 'A' && !ya_existe_un_archivo){
                response.push_back(nombre);
                inicio = i+1;
                tamanio = 0;
                ya_existe_un_archivo = true;
            }else if(tipo == 'A' && ya_existe_un_archivo){
                response.clear();
                break;
            }
        }else{
            tamanio++;
        }
    }    
    if (response.size() > 0 && tipo_path == 'A' && typeText(response[response.size()-1]) != 'A'){
        response.clear();
    }
    return response;
}

char reportes::typeText(string nombre){
    char response = '\0';
    int cantidad_puntos = 0;
    for (int i = 0; i < nombre.size(); i++){
        if (nombre.substr(i, 1) == "."){
            cantidad_puntos++;
        }        
    }
    if (cantidad_puntos == 0){
        response = 'F';
    }else if(cantidad_puntos == 1){
        response = 'A';
    }else if(cantidad_puntos > 1){
        response = 'E';
    }
    return response;
}

void reportes::searchBlock(string path, int *pos_padre, int *pos_inodo, int*pos_block, disco::Superblock *spb, vector<string> *listado_path){
    
    //Se verifica que todavia halla un nombre que se pueda buscar
    string nombre_archivo_o_carpeta = (listado_path->size() == 0)? "": (*listado_path)[0];
    if (nombre_archivo_o_carpeta.empty()){
        return;
    }

    //Este nos servira para solo lectura
    FILE *file = fopen(path.c_str(), "rb");

    //Se obtiene el inodo
    disco::Inode *inodo = new disco::Inode();
    fseek(file, spb->s_inode_start + ((*pos_inodo) * csnt_rp.SIZE_I), SEEK_SET);
    fread(inodo, csnt_rp.SIZE_I, 1, file);
    
    //Esta variable nos sirve para que deje de buscar en otro apuntadores
    bool encontrado = false;

    if (inodo->i_type == '0'){
        //Se lee los primeros 12 apuntadores
        for (int i = 0; i < 12; i++){
            
            //En primera instancia se lee el apuntador 1 del inodo con el fin de conocer
            //el bloque de carpeta el cual contiene a su padre
            if (i == 0 && inodo->i_block[i] != -1 && encontrado == false){
                disco::Folderblock folder;
                fseek(file, spb->s_block_start + (inodo->i_block[i] * csnt_rp.SIZE_FB), SEEK_SET);
                fread(&folder, csnt_rp.SIZE_FB, 1, file);
                //i = 0: Indica la posicion del inodo del padre
                *pos_padre = folder.b_content[0].b_inodo;
                //i = 1: Indica la posicion del inodo mismo
                *pos_inodo = folder.b_content[1].b_inodo;
                //i = 2 e i = 3: Podria tener una carpeta o un archivo debido al b_name
                for (int j = 2; j < 4; j++){
                    //En el caso de encontrar un folder
                    if (folder.b_content[j].b_name == nombre_archivo_o_carpeta && typeText(nombre_archivo_o_carpeta) == 'F'){
                        //Se elimina el primer elemento del listado_path
                        listado_path->erase(listado_path->begin());
                        //Se asigna la posicion del inodo encontrado
                        *pos_inodo = folder.b_content[j].b_inodo;
                        //Se asigna la posicion del bloque encontrado
                        *pos_block = inodo->i_block[i];
                        //No hay mas datos en el path
                        if (listado_path->size() == 0){
                            encontrado = true;
                            break;
                        }
                        //Hay mas datos en el path
                        else if(listado_path->size() > 0){
                            searchBlock(path, pos_padre, pos_inodo, pos_block, spb, listado_path);
                        }
                    }
                    //En el caso de hallar un archivo
                    else if(folder.b_content[j].b_name == nombre_archivo_o_carpeta && typeText(nombre_archivo_o_carpeta) == 'A'){
                        listado_path->erase(listado_path->begin());
                        //Se asigna la posicion del inodo encontrado
                        *pos_inodo = folder.b_content[j].b_inodo;  
                        //Se asigna la posicion del bloque encontrado
                        *pos_block = inodo->i_block[i];                      
                        encontrado = true;
                        break;
                    }
                }
            }
            //Los apuntadores del 2 al 12 aqui no hay una forma de saber quien es el inodo padre
            else if(inodo->i_block[i] != -1 && encontrado == false){
                disco::Folderblock folder;
                fseek(file, spb->s_block_start + (inodo->i_block[i] * csnt_rp.SIZE_FB), SEEK_SET);
                fread(&folder, csnt_rp.SIZE_FB, 1, file);
                for (int j = 0; j < 4; j++){
                    //En el caso de encontrar un folder
                    if (folder.b_content[j].b_name == nombre_archivo_o_carpeta && typeText(nombre_archivo_o_carpeta) == 'F'){
                        //Se elimina el primer elemento del listado_path
                        listado_path->erase(listado_path->begin());
                        //Se asigna la posicion del inodo encontrado
                        *pos_inodo = folder.b_content[j].b_inodo;
                        //Se asigna la posicion del bloque encontrado
                        *pos_block = inodo->i_block[i];
                        //No hay mas datos en el path
                        if (listado_path->size() == 0){
                            encontrado = true;
                            break;
                        }
                        //Hay mas datos en el path
                        else if(listado_path->size() > 0){
                            searchBlock(path, pos_padre, pos_inodo, pos_block, spb, listado_path);
                        }
                    }
                    //En el caso de hallar un archivo
                    else if(folder.b_content[j].b_name == nombre_archivo_o_carpeta && typeText(nombre_archivo_o_carpeta) == 'A'){
                        listado_path->erase(listado_path->begin());
                        *pos_inodo = folder.b_content[j].b_inodo;
                        //Se asigna la posicion del bloque encontrado
                        *pos_block = inodo->i_block[i];
                        encontrado = true;
                        break;
                    }
                }
            }
        }
        //Se lee el apuntador 13
        if(inodo->i_block[12] != -1 && encontrado == false){
            disco::Pointerblock pointer;
            fseek(file, spb->s_block_start + (inodo->i_block[12] * csnt_rp.SIZE_PB), SEEK_SET);
            fread(&pointer, csnt_rp.SIZE_PB, 1, file);
            for (int i = 0; i < 15; i++){
                //Se encontro un bloque de carpeta
                if (pointer.b_pointers[i] != -1){
                    disco::Folderblock folder;
                    fseek(file, spb->s_block_start + (pointer.b_pointers[i] * csnt_rp.SIZE_FB), SEEK_SET);
                    fread(&folder, csnt_rp.SIZE_FB, 1, file);
                    for (int j = 0; j < 4; j++){
                        //En el caso de encontrar un folder
                        if (folder.b_content[j].b_name == nombre_archivo_o_carpeta && typeText(nombre_archivo_o_carpeta) == 'F'){
                            //Se elimina el primer elemento del listado_path
                            listado_path->erase(listado_path->begin());
                            //Se asigna la posicion del inodo encontrado
                            *pos_inodo = folder.b_content[j].b_inodo;
                            //Se asigna la posicion del bloque encontrado
                            *pos_block = pointer.b_pointers[i];
                            //No hay mas datos en el path
                            if (listado_path->size() == 0){
                                encontrado = true;
                                break;
                            }
                            //Hay mas datos en el path
                            else if(listado_path->size() > 0){
                                searchBlock(path, pos_padre, pos_inodo, pos_block, spb, listado_path);
                            }
                        }
                        //En el caso de hallar un archivo
                        else if(folder.b_content[j].b_name == nombre_archivo_o_carpeta && typeText(nombre_archivo_o_carpeta) == 'A'){
                            listado_path->erase(listado_path->begin());
                            //Se asigna la posicion del inodo encontrado
                            *pos_inodo = folder.b_content[j].b_inodo; 
                            //Se asigna la posicion del bloque encontrado
                            *pos_block = pointer.b_pointers[i];                       
                            encontrado = true;
                            break;
                        }
                    }                    
                }                
            }
        }
        //Se lee el apuntador 14
        else if(inodo->i_block[13] != -1 && encontrado == false){
            disco::Pointerblock pointer_principal;
            fseek(file, spb->s_block_start + (inodo->i_block[13] * csnt_rp.SIZE_PB), SEEK_SET);
            fread(&pointer_principal, csnt_rp.SIZE_PB, 1, file);
            for (int i = 0; i < 15; i++){
                if (pointer_principal.b_pointers[i] != -1){
                    disco::Pointerblock sub_pointer;
                    fseek(file, spb->s_block_start + (pointer_principal.b_pointers[i] * csnt_rp.SIZE_PB), SEEK_SET);
                    fread(&sub_pointer, csnt_rp.SIZE_PB, 1, file);
                    for (int j = 0; j < 15; j++){
                        //Se encontro un bloque de carpeta
                        if (sub_pointer.b_pointers[j] != -1){
                            disco::Folderblock folder;
                            fseek(file, spb->s_block_start + (sub_pointer.b_pointers[j] * csnt_rp.SIZE_FB), SEEK_SET);
                            fread(&folder, csnt_rp.SIZE_FB, 1, file);
                            for (int k = 0; k < 4; k++){
                                //En el caso de encontrar un folder
                                if (folder.b_content[k].b_name == nombre_archivo_o_carpeta && typeText(nombre_archivo_o_carpeta) == 'F'){
                                    //Se elimina el primer elemento del listado_path
                                    listado_path->erase(listado_path->begin());
                                    //Se asigna la posicion del inodo encontrado
                                    *pos_inodo = folder.b_content[k].b_inodo;
                                    //Se asigna la posicion del bloque encontrado
                                    *pos_block = sub_pointer.b_pointers[j];
                                    //No hay mas datos en el path
                                    if (listado_path->size() == 0){
                                        encontrado = true;
                                        break;
                                    }
                                    //Hay mas datos en el path
                                    else if(listado_path->size() > 0){
                                        searchBlock(path, pos_padre, pos_inodo, pos_block, spb, listado_path);
                                    }
                                }
                                //En el caso de hallar un archivo
                                else if(folder.b_content[k].b_name == nombre_archivo_o_carpeta && typeText(nombre_archivo_o_carpeta) == 'A'){
                                    listado_path->erase(listado_path->begin());
                                    //Se asigna la posicion del inodo encontrado
                                    *pos_inodo = folder.b_content[k].b_inodo;
                                    //Se asigna la posicion del bloque encontrado
                                    *pos_block = sub_pointer.b_pointers[j];
                                    encontrado = true;
                                    break;
                                }
                            }                    
                        }                
                    }
                }                
            }
        }
        //Se lee el apuntador 15
        else if(inodo->i_block[14] != -1 && encontrado == false){
            //#############################################################
        }
    }
    fclose(file);
}

string reportes::getContentArchive(int position_inode, string path, disco::Superblock spb){
    string response;
    //Creo una variable de tipo FILE
    FILE *file = fopen(path.c_str(), "rb");

    //Obtengo el inodo del archivo debido a que se debe de leer por completo
    disco::Inode inode_user;
    fseek(file, spb.s_inode_start + (position_inode * csnt_rp.SIZE_I), SEEK_SET);
    fread(&inode_user, csnt_rp.SIZE_I, 1, file);
    
    //Apuntadores del 1 al 12
    for (int i = 0; i < 12; i++){
        if (inode_user.i_block[i] != -1){
            getContentByTypePointer(&response, "BD", path, spb, inode_user.i_block[i]);
        }
    }
    
    //Apuntador 13 - Se apunta a una estructura de apuntadores
    if (inode_user.i_block[12] != -1){
        getContentByTypePointer(&response, "BSI", path, spb, inode_user.i_block[12]);
    }

    //Apuntador 14 - Se apunta a una estructura de apuntadores
    if (inode_user.i_block[13] != -1){
        getContentByTypePointer(&response, "BDI", path, spb, inode_user.i_block[13]);
    }

    //Apuntador 15 - Se apunta a una estructura de apuntadores
    if (inode_user.i_block[14] != -1){
        getContentByTypePointer(&response, "BTI", path, spb, inode_user.i_block[14]);
    }

    fclose(file);
    return response;
}

void reportes::getContentByTypePointer(string *contenido, string tipo_apuntador, string path, disco::Superblock spb, int pos_block){

    FILE *file = fopen(path.c_str(), "rb");
    if (tipo_apuntador == "BD"){
        disco::Archiveblock tempArchive;
        fseek(file, spb.s_block_start + (pos_block * csnt_rp.SIZE_AB), SEEK_SET);
        fread(&tempArchive, csnt_rp.SIZE_AB, 1, file);
        *contenido += tempArchive.b_content;
    }else if(tipo_apuntador == "BSI"){
        disco::Pointerblock tempPointer;
        fseek(file, spb.s_block_start + (pos_block * csnt_rp.SIZE_AB), SEEK_SET);
        fread(&tempPointer, csnt_rp.SIZE_PB, 1, file);
        for (int i = 0; i < 15; i++){
            if (tempPointer.b_pointers[i] != -1){
                getContentByTypePointer(contenido, "BD", path, spb, tempPointer.b_pointers[i]);
            }
        }
    }else if(tipo_apuntador == "BDI"){
        disco::Pointerblock tempPointer;
        fseek(file, spb.s_block_start + (pos_block * csnt_rp.SIZE_AB), SEEK_SET);
        fread(&tempPointer, csnt_rp.SIZE_PB, 1, file);
        for (int i = 0; i < 15; i++){
            if (tempPointer.b_pointers[i] != -1){
                getContentByTypePointer(contenido, "BSI", path, spb, tempPointer.b_pointers[i]);
            }
        }       
    }else if(tipo_apuntador == "BTI"){
        disco::Pointerblock tempPointer;
        fseek(file, spb.s_block_start + (pos_block * csnt_rp.SIZE_AB), SEEK_SET);
        fread(&tempPointer, csnt_rp.SIZE_PB, 1, file);
        for (int i = 0; i < 15; i++){
            if (tempPointer.b_pointers[i] != -1){
                getContentByTypePointer(contenido, "BDI", path, spb, tempPointer.b_pointers[i]);
            }
        } 
    }
    fclose(file);
}

disco::Inode reportes::getInodo(int pos_inode, disco::Superblock spb, string path){
    disco::Inode inodo_obtenido;
    FILE *file = fopen(path.c_str(), "rb");
    fseek(file, spb.s_inode_start + (pos_inode * csnt_rp.SIZE_I), SEEK_SET);
    fread(&inodo_obtenido, csnt_rp.SIZE_I, 1, file);
    fclose(file);
    return inodo_obtenido;
}

void reportes::searchInode(string path, int *pos_padre, int *pos_inodo, disco::Superblock *spb, vector<string> *listado_path){
    
    //Se verifica que todavia halla un nombre que se pueda buscar
    string nombre_archivo_o_carpeta = (listado_path->size() == 0)? "": (*listado_path)[0];
    if (nombre_archivo_o_carpeta.empty()){
        return;
    }

    //Este nos servira para solo lectura
    FILE *file = fopen(path.c_str(), "rb");

    //Se obtiene el inodo
    disco::Inode *inodo = new disco::Inode();
    fseek(file, spb->s_inode_start + ((*pos_inodo) * csnt_rp.SIZE_I), SEEK_SET);
    fread(inodo, csnt_rp.SIZE_I, 1, file);
    
    //Esta variable nos sirve para que deje de buscar en otro apuntadores
    bool encontrado = false;

    if (inodo->i_type == '0'){
        //Se lee los primeros 12 apuntadores
        for (int i = 0; i < 12; i++){
            
            //En primera instancia se lee el apuntador 1 del inodo con el fin de conocer
            //el bloque de carpeta el cual contiene a su padre
            if (i == 0 && inodo->i_block[i] != -1 && encontrado == false){
                disco::Folderblock folder;
                fseek(file, spb->s_block_start + (inodo->i_block[i] * csnt_rp.SIZE_FB), SEEK_SET);
                fread(&folder, csnt_rp.SIZE_FB, 1, file);
                //i = 0: Indica la posicion del inodo del padre
                *pos_padre = folder.b_content[0].b_inodo;
                //i = 1: Indica la posicion del inodo mismo
                *pos_inodo = folder.b_content[1].b_inodo;
                //i = 2 e i = 3: Podria tener una carpeta o un archivo debido al b_name
                for (int i = 2; i < 4; i++){
                    //En el caso de encontrar un folder
                    if (folder.b_content[i].b_name == nombre_archivo_o_carpeta && typeText(nombre_archivo_o_carpeta) == 'F'){
                        //Se elimina el primer elemento del listado_path
                        listado_path->erase(listado_path->begin());
                        //Se asigna la posicion del inodo encontrado
                        *pos_inodo = folder.b_content[i].b_inodo;
                        //No hay mas datos en el path
                        if (listado_path->size() == 0){
                            encontrado = true;
                            break;
                        }
                        //Hay mas datos en el path
                        else if(listado_path->size() > 0){
                            searchInode(path, pos_padre, pos_inodo, spb, listado_path);
                        }
                    }
                    //En el caso de hallar un archivo
                    else if(folder.b_content[i].b_name == nombre_archivo_o_carpeta && typeText(nombre_archivo_o_carpeta) == 'A'){
                        listado_path->erase(listado_path->begin());
                        *pos_inodo = folder.b_content[i].b_inodo;                        
                        encontrado = true;
                        break;
                    }
                }
            }
            //Los apuntadores del 2 al 12 aqui no hay una forma de saber quien es el inodo padre
            else if(inodo->i_block[i] != -1 && encontrado == false){
                disco::Folderblock folder;
                fseek(file, spb->s_block_start + (inodo->i_block[i] * csnt_rp.SIZE_FB), SEEK_SET);
                fread(&folder, csnt_rp.SIZE_FB, 1, file);
                for (int i = 0; i < 4; i++){
                    //En el caso de encontrar un folder
                    if (folder.b_content[i].b_name == nombre_archivo_o_carpeta && typeText(nombre_archivo_o_carpeta) == 'F'){
                        //Se elimina el primer elemento del listado_path
                        listado_path->erase(listado_path->begin());
                        //Se asigna la posicion del inodo encontrado
                        *pos_inodo = folder.b_content[i].b_inodo;
                        //No hay mas datos en el path
                        if (listado_path->size() == 0){
                            encontrado = true;
                            break;
                        }
                        //Hay mas datos en el path
                        else if(listado_path->size() > 0){
                            searchInode(path, pos_padre, pos_inodo, spb, listado_path);
                        }
                    }
                    //En el caso de hallar un archivo
                    else if(folder.b_content[i].b_name == nombre_archivo_o_carpeta && typeText(nombre_archivo_o_carpeta) == 'A'){
                        listado_path->erase(listado_path->begin());
                        *pos_inodo = folder.b_content[i].b_inodo;                        
                        encontrado = true;
                        break;
                    }
                }
            }
        }
        //Se lee el apuntador 13
        if(inodo->i_block[12] != -1 && encontrado == false){
            disco::Pointerblock pointer;
            fseek(file, spb->s_block_start + (inodo->i_block[12] * csnt_rp.SIZE_PB), SEEK_SET);
            fread(&pointer, csnt_rp.SIZE_PB, 1, file);
            for (int i = 0; i < 15; i++){
                //Se encontro un bloque de carpeta
                if (pointer.b_pointers[i] != -1){
                    disco::Folderblock folder;
                    fseek(file, spb->s_block_start + (pointer.b_pointers[i] * csnt_rp.SIZE_FB), SEEK_SET);
                    fread(&folder, csnt_rp.SIZE_FB, 1, file);
                    for (int i = 0; i < 4; i++){
                        //En el caso de encontrar un folder
                        if (folder.b_content[i].b_name == nombre_archivo_o_carpeta && typeText(nombre_archivo_o_carpeta) == 'F'){
                            //Se elimina el primer elemento del listado_path
                            listado_path->erase(listado_path->begin());
                            //Se asigna la posicion del inodo encontrado
                            *pos_inodo = folder.b_content[i].b_inodo;
                            //No hay mas datos en el path
                            if (listado_path->size() == 0){
                                encontrado = true;
                                break;
                            }
                            //Hay mas datos en el path
                            else if(listado_path->size() > 0){
                                searchInode(path, pos_padre, pos_inodo, spb, listado_path);
                            }
                        }
                        //En el caso de hallar un archivo
                        else if(folder.b_content[i].b_name == nombre_archivo_o_carpeta && typeText(nombre_archivo_o_carpeta) == 'A'){
                            listado_path->erase(listado_path->begin());
                            *pos_inodo = folder.b_content[i].b_inodo;                        
                            encontrado = true;
                            break;
                        }
                    }                    
                }                
            }
        }
        //Se lee el apuntador 14
        else if(inodo->i_block[13] != -1 && encontrado == false){
            disco::Pointerblock pointer_principal;
            fseek(file, spb->s_block_start + (inodo->i_block[13] * csnt_rp.SIZE_PB), SEEK_SET);
            fread(&pointer_principal, csnt_rp.SIZE_PB, 1, file);
            for (int i = 0; i < 15; i++){
                if (pointer_principal.b_pointers[i] != -1){
                    disco::Pointerblock sub_pointer;
                    fseek(file, spb->s_block_start + (pointer_principal.b_pointers[i] * csnt_rp.SIZE_PB), SEEK_SET);
                    fread(&sub_pointer, csnt_rp.SIZE_PB, 1, file);
                    for (int i = 0; i < 15; i++){
                        //Se encontro un bloque de carpeta
                        if (sub_pointer.b_pointers[i] != -1){
                            disco::Folderblock folder;
                            fseek(file, spb->s_block_start + (sub_pointer.b_pointers[i] * csnt_rp.SIZE_FB), SEEK_SET);
                            fread(&folder, csnt_rp.SIZE_FB, 1, file);
                            for (int i = 0; i < 4; i++){
                                //En el caso de encontrar un folder
                                if (folder.b_content[i].b_name == nombre_archivo_o_carpeta && typeText(nombre_archivo_o_carpeta) == 'F'){
                                    //Se elimina el primer elemento del listado_path
                                    listado_path->erase(listado_path->begin());
                                    //Se asigna la posicion del inodo encontrado
                                    *pos_inodo = folder.b_content[i].b_inodo;
                                    //No hay mas datos en el path
                                    if (listado_path->size() == 0){
                                        encontrado = true;
                                        break;
                                    }
                                    //Hay mas datos en el path
                                    else if(listado_path->size() > 0){
                                        searchInode(path, pos_padre, pos_inodo, spb, listado_path);
                                    }
                                }
                                //En el caso de hallar un archivo
                                else if(folder.b_content[i].b_name == nombre_archivo_o_carpeta && typeText(nombre_archivo_o_carpeta) == 'A'){
                                    listado_path->erase(listado_path->begin());
                                    *pos_inodo = folder.b_content[i].b_inodo;                        
                                    encontrado = true;
                                    break;
                                }
                            }                    
                        }                
                    }
                }                
            }
        }
        //Se lee el apuntador 15
        else if(inodo->i_block[14] != -1 && encontrado == false){
            //#############################################################
        }
    }
    fclose(file);
}
