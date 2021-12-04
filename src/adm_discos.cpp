#include <iostream>
#include <map>
#include <cstring>
#include <limits.h>
#include <math.h>
#include "../util/util_p.h"
#include "../util/constant.h"
#include "adm_discos.h"
#include "disco.h"

using namespace std;

util_p util_admdcs;
constant csnt_admdcs;
const string PATH_TEMP  = "/tmp/douglas/Disco1.disk"; //Eliminar antes de ultimo push

void adm_discos::mkdisk(map<string, string> param_got){
    if (param_got.size() == 0)
    {
        return;
    }
    /*Obteniendo datos*/
    string comentario = param_got["-comentario"];
    int size = atoi(param_got["-size"].c_str());
    char unit = util_admdcs.toLowerString(param_got["-unit"].c_str())[0];
    string fit = util_admdcs.toUpperString(param_got["-fit"].c_str());
    string path = param_got["-path"].c_str();

    /*Formateo de datos*/
    path = (path.substr(0,1) == "\"") ? path.substr(1, path.size()-2): path;
    size = (unit == 'k') ? (size * csnt_admdcs.AMOUNT_BITS): (size * csnt_admdcs.AMOUNT_BITS * csnt_admdcs.AMOUNT_BITS);

    /*Flujo del void*/
    //Se crea la carpeta del archivo si es que no existe
    util_admdcs.createDirectory(path);

    //Se crea un mbr el cual va a ser escrito en el disco
    disco::MBR temp_mbr;
    temp_mbr.mbr_tamano = size;
    temp_mbr.mbr_fecha_creacion = time(nullptr);
    temp_mbr.mbr_disk_signature = 1 + rand() % (10001 - 1);
    temp_mbr.disk_fit = fit[0];
    temp_mbr.mbr_partition_1 = disco::Partition();
    temp_mbr.mbr_partition_2 = disco::Partition();
    temp_mbr.mbr_partition_3 = disco::Partition();
    temp_mbr.mbr_partition_4 = disco::Partition();

    //A esto se le conoce como descriptor
    FILE *file;

    //El wb+ nos permite abrir el archivo para lectura/escritura además
    //el cursor (stream) inicia en la parte inicial del archivo y el archivo sera binario
    file = fopen(path.c_str(), "wb+");

    if (file){
        //Primero ingreso una cadena vacia que me indique donde empieza el archivo
        fwrite("\0", 1, 1, file);
        //Creo una cadena vacia en la posicion (size-1) para que me cree el archivo con el tamaño asignado
        fseek(file, size - 1, SEEK_SET);
        fwrite("\0", 1, 1, file);
        //Muevo el cursor al principio del archivo
        rewind(file);
        //Realizo el ingreso de la estructura
        fwrite(&temp_mbr, csnt_admdcs.SIZE_MBR, 1, file);
        fclose(file);
    }
    cout << csnt_admdcs.GREEN << "RESPUESTA:" << csnt_admdcs.NC << " La creacion del disco ha sido exitosa " << csnt_admdcs.BLUE << comentario << csnt_admdcs.NC << endl;
}

void adm_discos::rmdisk(map<string, string> param_got){
    if (param_got.size() == 0)
    {
        return;
    }
    /*Obteniendo datos*/
    string comentario = param_got["-comentario"].c_str();
    string path = param_got["-path"].c_str();
    
    /*Formateo de datos*/
    path = (path.substr(0,1) == "\"") ? path.substr(1, path.size()-2): path;

    /*Flujo del void*/
    cout << csnt_admdcs.CYAN << "ELECCION:" << csnt_admdcs.NC << " ¿Desea eliminar el archivo? S/N" << endl;
    string response;
    getline(cin, response);
    if (util_admdcs.toUpperString(response) == "S"){
        remove(path.c_str());
        cout << csnt_admdcs.GREEN << "RESPUESTA:" << csnt_admdcs.NC << " El disco ha sido eliminado correctamente " << csnt_admdcs.BLUE << comentario << csnt_admdcs.NC << endl;
    }else{
        cout << csnt_admdcs.YELLOW << "AVISO:" << csnt_admdcs.NC << " No se realizo la eliminacion " << csnt_admdcs.BLUE << comentario << csnt_admdcs.NC << endl;
    }
}

void adm_discos::fdisk(map<string, string> param_got){
    if (param_got.size() == 0)
    {
        return;
    }
    /*Obteniendo datos*/
    string comentario = param_got["-comentario"];
    int size = atoi(param_got["-size"].c_str());
    char unit = param_got["-unit"][0];
    string path = param_got["-path"];
    char type = param_got["-type"][0];
    char fit = param_got["-fit"][0];
    string delete_p = param_got["-delete"];
    string name = param_got["-name"];
    int add_p = atoi(param_got["-add"].c_str());

    /*Formateo de datos*/
    path = (path.substr(0,1) == "\"") ? path.substr(1, path.size()-2): path;
    name = (name.substr(0,1) == "\"") ? name.substr(1, name.size()-2): name;
    if (unit == 'K'){
        size = size * csnt_admdcs.AMOUNT_BITS;
        add_p = add_p * csnt_admdcs.AMOUNT_BITS;
    }else if(unit == 'M'){
        size = size * csnt_admdcs.AMOUNT_BITS * csnt_admdcs.AMOUNT_BITS;
        add_p = add_p * csnt_admdcs.AMOUNT_BITS * csnt_admdcs.AMOUNT_BITS;
    }

    /*Flujo de void*/
    //Obtenemos el MBR
    disco::MBR mbr = getMBR(path);
    if (mbr.mbr_tamano == -1){
        cout << csnt_admdcs.BLUE << comentario << csnt_admdcs.NC << endl;
        return;
    }

    char foundName = find_namePartition(name, path);
    if (!delete_p.empty()){
        if (foundName != '\0'){
            if (foundName == 'P' || foundName == 'E'){
                
            }else if(foundName == 'L'){
                
            }
        }else{
            cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No existe una particion con el nombre '" << name << "' " << csnt_admdcs.BLUE << comentario << csnt_admdcs.NC << endl;
        }
    }else if(add_p != 0){
        if (foundName != '\0'){
            if (foundName == 'P' || foundName == 'E'){
                
            }else if(foundName == 'L'){
                
            }
        }else{
            cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No existe una particion con el nombre '" << name << "' " << csnt_admdcs.BLUE << comentario << csnt_admdcs.NC << endl;
        }
    }else{        
        if (foundName == '\0'){
            
        }else{
            cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " Ya existe una particion con el nombre '" << name << "' " << csnt_admdcs.BLUE << comentario << csnt_admdcs.NC << endl;
        }
    }
}

disco::MBR adm_discos::getMBR(string path){
    disco::MBR response;
    FILE *file = fopen(path.c_str(), "rb");
    int read = fread(&response, csnt_admdcs.SIZE_MBR, 1, file);
    if (read <= 0){
        response.mbr_tamano = -1;
        cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No se ha podido leer el disco ";
    }else{
        fclose(file);
    }
    return response;
}

/*
Return type of partition if name is found case opposite returns \0
*/
char adm_discos::find_namePartition(string name, string path){
    
    char response = '\0';
    disco::MBR mbr = getMBR(path);
    if (mbr.mbr_tamano == -1){
        cout << endl; return response;
    }
    vector<disco::Partition> getPartitions = getListPartitionsEP(mbr);
    int index = -1;
    for (int i = 0; i < getPartitions.size(); i++){
        if (getPartitions[i].part_name == name){
            response = getPartitions[i].part_type;
            break;
        }
        if (getPartitions[i].part_type == 'E'){
            index = i;
        }
    }
    if (index != -1 && response == '\0'){
        disco::Partition partitionExtended = getPartitions[index];
        vector<disco::EBR> listEBR = getListPartitionsL(partitionExtended, path);
        for (int i = 0; i < listEBR.size(); i++){
            if (listEBR[i].part_name == name){
                response = 'L';
                break;
            }
        }
    }
    return response;
}

/*
Get partitions extended and primary where status = 1
*/
vector<disco::Partition> adm_discos::getListPartitionsEP(disco::MBR mbr){
    vector<disco::Partition> response;
    map<int, disco::Partition> tempMap;
    if (mbr.mbr_partition_1.part_status == '1'){
        tempMap[mbr.mbr_partition_1.part_start] = mbr.mbr_partition_1;
    }
    if(mbr.mbr_partition_2.part_status == '1'){
        tempMap[mbr.mbr_partition_2.part_start] = mbr.mbr_partition_2;
    }
    if(mbr.mbr_partition_3.part_status == '1'){
        tempMap[mbr.mbr_partition_3.part_start] = mbr.mbr_partition_3;
    }
    if(mbr.mbr_partition_4.part_status == '1'){
        tempMap[mbr.mbr_partition_4.part_start] = mbr.mbr_partition_4;
    }
    auto iter = tempMap.begin();
    while (iter != tempMap.end()) {
        response.push_back(iter->second);
        ++iter;
    }
    return response;
}

/*
Get partitions logics where status = 1
*/
vector<disco::EBR> adm_discos::getListPartitionsL(disco::Partition partition, string path){
    vector<disco::EBR> response;
    disco::EBR tempEBR;
    FILE *file = fopen(path.c_str(), "rb+");
    fseek(file, partition.part_start, SEEK_SET);
    fread(&tempEBR, csnt_admdcs.SIZE_EBR, 1, file);
    int acum = 0;
    while (true){
        if (tempEBR.part_status != '0'){
            response.push_back(tempEBR);
            if (tempEBR.part_next == -1){
                break;
            }else{
                fseek(file, tempEBR.part_next, SEEK_SET);
                fread(&tempEBR, csnt_admdcs.SIZE_EBR, 1, file);
            }
        }else{
            if (tempEBR.part_next == -1){
                break;
            }else{
                fseek(file, tempEBR.part_next, SEEK_SET);
                fread(&tempEBR, csnt_admdcs.SIZE_EBR, 1, file);
            }
        }
    }
    return response;
}


/*
This function get a vector of the struct Fit, the struct helps
return the next values:
-1 = Error
-2 = There isn't enough space
Its function principal is assign a position 
*/
int adm_discos::assign_positionByFit(vector<disco::Fit> list_Of_Fits, int size_new_partition, char fit){
    int response = -1;
    if (list_Of_Fits.size() <= 0){
        return response;
    }
    response = -2;
    if (fit == 'F'){
        for (int i = 0; i < list_Of_Fits.size(); i++){
            if (list_Of_Fits[i].size >= size_new_partition){
                response = list_Of_Fits[i].start;
                break;
            }
        }
    }else if(fit == 'W'){
        int size = 0;
        for (int i = 0; i < list_Of_Fits.size(); i++){
            if (list_Of_Fits[i].size >= size_new_partition && list_Of_Fits[i].size >= size){
                size = list_Of_Fits[i].size;
                response = list_Of_Fits[i].start;
            }
        }
    }else if(fit == 'B'){
        int dif_actual = INT_MAX;
        for (int i = 0; i < list_Of_Fits.size(); i++){
            int aux_dif = list_Of_Fits[i].size - size_new_partition;
            if (list_Of_Fits[i].size >= size_new_partition && aux_dif >= 0 && aux_dif <= dif_actual){
                dif_actual = aux_dif;
                response = list_Of_Fits[i].start;                
            }
        }
    }
    return response;
}

void adm_discos::test_asignacionFit(){
    char fit = 'B';
    int size_temp = 100;
    vector<disco::Fit> list_fit;
    disco::Fit tempFit;
    tempFit.name = "nombre 1";
    tempFit.start = 10000;
    tempFit.size = tempFit.start;
    list_fit.push_back(tempFit);
    tempFit.name = "nombre 2";
    tempFit.start = 5000;
    tempFit.size = tempFit.start;
    list_fit.push_back(tempFit);
    tempFit.name = "nombre 3";
    tempFit.start = 7500;
    tempFit.size = tempFit.start;
    list_fit.push_back(tempFit);
    tempFit.name = "nombre 4";
    tempFit.start = 2300;
    tempFit.size = tempFit.start;
    list_fit.push_back(tempFit);
    tempFit.name = "nombre 5";
    tempFit.start = 15000;
    tempFit.size = tempFit.start;
    list_fit.push_back(tempFit);
    tempFit.name = "nombre 6";
    tempFit.start = 400;
    tempFit.size = tempFit.start;
    list_fit.push_back(tempFit);
    tempFit.name = "nombre 7";
    tempFit.start = 400;
    tempFit.size = tempFit.start;
    list_fit.push_back(tempFit);
    tempFit.name = "nombre 8";
    tempFit.start = 1234;
    tempFit.size = tempFit.start;
    list_fit.push_back(tempFit);

    int response_Position = assign_positionByFit(list_fit, size_temp, fit);
    if (response_Position == -1){
        cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No se ha podido realizar correctamente el ajuste" << endl;
        return;
    }else if(response_Position == -2){
        cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No hay suficiente espacio para crear la particion logica" << endl;
        return;
    }
    cout << "RESPONSE:" << response_Position << endl;

}