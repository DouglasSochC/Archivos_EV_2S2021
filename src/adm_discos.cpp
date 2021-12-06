#include <iostream>
#include <map>
#include <cstring>
#include <limits.h>
#include <math.h>
#include "../util/util_p.h"
#include "../util/constant.h"
#include "adm_discos.h"
#include "adm_cap.h"
#include "disco.h"

using namespace std;

util_p util_admdcs;
constant csnt_admdcs;
adm_cap admcap_admdcs;
vector <disco::Mount> Memory_Mount;
const string PATH_TEMP  = "/tmp/douglas/Disco1.disk"; //Eliminar antes de ultimo push

//PENDIENTES:

//PREGUNTAR:
//1. A un inodo de tipo archivo se le debe de dejar un tamaño definido desde el principio?
//O debe de seguir aumentando?

//RECORDAR:
//1. Siempre hay que validar si el path o el name inicia con " para que funcione correctamente

//INI - METODOS PRINCIPALES

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

    /*Se verifica que no este montado su path*/
    disco::Mount tempMount = checkPartitionMounted(path);
    if (tempMount.status == '1'){
        cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No puede utilizar este comando debido a que el disco esta montado " << csnt_admdcs.BLUE << comentario << csnt_admdcs.NC << endl;
        return;
    }

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
    if (param_got.size() == 0){return;}
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

    /*Se verifica que no este montado su path*/
    disco::Mount tempMount = checkPartitionMounted(path);
    if (tempMount.status == '1'){
        cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No puede utilizar este comando debido a que el disco esta montado " << csnt_admdcs.BLUE << comentario << csnt_admdcs.NC << endl;
        return;
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
                fdisk_deletePrimaryExtended(delete_p, mbr, path, name);
            }else if(foundName == 'L'){
                fdisk_deleteLogic(delete_p, mbr, path, name);
            }
        }else{
            cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No existe una particion con el nombre '" << name << "' " << csnt_admdcs.BLUE << comentario << csnt_admdcs.NC << endl;
            return;
        }
    }else if(add_p != 0){
        if (foundName != '\0'){
            if (foundName == 'P' || foundName == 'E'){
                fdisk_addPrimaryExtended(add_p, mbr, path, name);
            }else if(foundName == 'L'){
                fdisk_addLogic(add_p, mbr, path, name);
            }
        }else{
            cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No existe una particion con el nombre '" << name << "' " << csnt_admdcs.BLUE << comentario << csnt_admdcs.NC << endl;
            return;
        }
    }else{        
        if (foundName == '\0'){
            fdisk_createPrimaryExtended(mbr, fit, type, size, name, path);
            fdisk_createLogic(mbr, fit, type, size, name, path);
        }else{
            cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " Ya existe una particion con el nombre '" << name << "' " << csnt_admdcs.BLUE << comentario << csnt_admdcs.NC << endl;
            return;
        }
    }
    cout << csnt_admdcs.BLUE << comentario << csnt_admdcs.NC << endl;
    return;
}

void adm_discos::mount(map<string, string> param_got){
    if (param_got.size() == 0){
        return;
    }
    /*Obteniendo datos*/
    string comentario = param_got["-comentario"].c_str();
    string name = param_got["-name"].c_str();
    string path = param_got["-path"].c_str();

    /*Formateo de datos*/
    path = (path.substr(0,1) == "\"") ? path.substr(1, path.size()-2): path;
    name = (name.substr(0,1) == "\"") ? name.substr(1, name.size()-2): name;

    /*Flujo de void*/
    disco::MBR mbr;
    FILE *file = fopen(path.c_str(), "rb");
    int read = fread(&mbr, csnt_admdcs.SIZE_MBR, 1, file);
    if (read > 0){
        fclose(file);
    }

    char foundName = find_namePartition(name, path);
    if (foundName == 'P' || foundName == 'L'){
        disco::Mount newMount = partitionToMounted(name, path);
        if (newMount.status != '0'){
            Memory_Mount.push_back(newMount);
            cout << csnt_admdcs.GREEN << "RESPUESTA:" << csnt_admdcs.NC << " La particion ha sido montada correctamente y se le ha asignado el siguiente id:\e[1m" << newMount.id << "\e[0m" << " "  << csnt_admdcs.BLUE << comentario << csnt_admdcs.NC << endl;
        }else{
            cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " Ya esta montada la particion " << name << " " << csnt_admdcs.BLUE << comentario << csnt_admdcs.NC << endl;
        }        
    }else if(foundName == 'E'){
        cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No se puede montar una particion extendida " << csnt_admdcs.BLUE << comentario << csnt_admdcs.NC << endl;
    }else{
        cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No existe la particion que desea montar" << csnt_admdcs.BLUE << comentario << csnt_admdcs.NC << endl;
    }
}

void adm_discos::unmount(map<string, string> param_got){
    if (param_got.size() == 0){
        return;
    }
    /*Obteniendo datos*/
    string comentario = param_got["-comentario"].c_str();
    string id = param_got["-id"].c_str();

    /*Flujo de void*/
    bool isUnmounted = false;
    for (int i = 0; i < Memory_Mount.size(); i++){
        if (util_admdcs.toLowerString(Memory_Mount[i].id) == util_admdcs.toLowerString(id)){
            isUnmounted = true;
            Memory_Mount.erase(Memory_Mount.begin() + i);
            break;
        }            
    }
    if (isUnmounted){
        cout << csnt_admdcs.GREEN << "RESPUESTA:" << csnt_admdcs.NC << " El id \e[1m" << id << "\e[0m ha sido desmontado correctamente " << csnt_admdcs.BLUE << comentario << csnt_admdcs.NC << endl;
    }else{
        cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No existe el id que desea desmontar " << csnt_admdcs.BLUE << comentario << csnt_admdcs.NC << endl;
    }
}

void adm_discos::mkfs(map<string, string> param_got, vector<disco::Mount> listMount){
    if (param_got.size() == 0){
        return;
    }
    /*Obteniendo datos*/
    string comentario = param_got["-comentario"].c_str();
    string id = param_got["-id"].c_str();
    string type = param_got["-type"].c_str();
    string fs = param_got["-fs"].c_str();

    /*Flujo del void*/
    //Se obtiene la particion que esta montada
    disco::Mount tempMount = getMountedLog(id); 
    if (tempMount.status == '0'){
        cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No existe el id que desea desmontar " << csnt_admdcs.BLUE << comentario << csnt_admdcs.NC << endl;
        return;
    }
    
    //Se crean variables que se utilizaran posteriormente
    string path = tempMount.path;
    string name = tempMount.name;
    time_t date_mounted = tempMount.date_mounted;
    int part_start_partition = -1;
    int size_partition = -1;

    //Se obtiene el MBR de la particion montada
    disco::MBR mbr = getMBR(path);
    if (mbr.mbr_tamano == -1){
        cout << csnt_admdcs.BLUE << comentario << csnt_admdcs.NC << endl;
        return;
    }
    
    //Se verifica que tipo de particion es, para obtener sus caracteristicas
    char foundName = find_namePartition(name, path);
    if (foundName == 'P' || foundName == 'E'){
        int before = -1; int after = -1;
        disco::Partition tempPartition = getPartitionEP(mbr, name, &before, &before);
        part_start_partition = tempPartition.part_start;
        size_partition = tempPartition.part_size;
    }else if(foundName == 'L'){
        disco::Partition partitionExtended;
        vector<disco::Partition> listPartitions = getListPartitionsEP(mbr);
        for (int i = 0; i < listPartitions.size(); i++){
            if (listPartitions[i].part_type == 'E'){
                partitionExtended = listPartitions[i];
                break;
            }            
        }
        int before = -1;
        disco::EBR tempPartition = getPartitionL(partitionExtended, path, name, &before);
        part_start_partition = tempPartition.part_start + csnt_admdcs.SIZE_EBR;
        size_partition = tempPartition.part_size - csnt_admdcs.SIZE_EBR;
    }else{
        cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No se ha encontrado el nombre de la particion " << csnt_admdcs.BLUE << comentario << csnt_admdcs.NC << endl;
        return;
    }
    
    int n_inodes;    
    if (fs == "2fs"){
        n_inodes = floor((size_partition - csnt_admdcs.SIZE_SPB) / (4 + csnt_admdcs.SIZE_I + 3 * csnt_admdcs.SIZE_AB));
    }else{
        n_inodes = floor((size_partition - csnt_admdcs.SIZE_SPB) / (4 + csnt_admdcs.SIZE_J + csnt_admdcs.SIZE_I + 3 * csnt_admdcs.SIZE_AB));
    }

    disco::Superblock temp_spb;
    temp_spb.s_inodes_count = n_inodes;
    //Se le resta un inodo con el fin de que no se sobrepase el espacio de la particion
    temp_spb.s_blocks_count = 3 * n_inodes;
    temp_spb.s_free_blocks_count = temp_spb.s_blocks_count;
    temp_spb.s_free_inodes_count = temp_spb.s_inodes_count;
    temp_spb.s_mtime = date_mounted;
    temp_spb.s_umtime = 0;
    temp_spb.s_mnt_count = 1;
    if (fs == "2fs") {
        temp_spb.s_filesystem_type = 2;
        mkfs_EXT2(temp_spb, part_start_partition, path, id, tempMount);
    } else {
        temp_spb.s_filesystem_type = 3;
        mkfs_EXT3(temp_spb, part_start_partition, path, id, tempMount);
    }
    cout << csnt_admdcs.GREEN << "RESPUESTA:" << csnt_admdcs.NC << " Formateo completo de la particion realizado correctamente " << csnt_admdcs.BLUE << comentario << csnt_admdcs.NC << endl;
}

//FIN - METODOS PRINCIPALES

//INI - SUB-METODOS

void adm_discos::fdisk_createPrimaryExtended(disco::MBR mbr, char part_fit, char part_type, int part_size, string part_name, string path){
    if (part_type == 'L'){
        return;
    }
    vector<disco::Partition> getPartitions = getListPartitionsEP(mbr);
    //Validando que solo halla una extendida
    int acum = 0;
    for (int i = 0; i < getPartitions.size(); i++){
        if (getPartitions[i].part_type == 'E'){
            acum++;
            break;
        }
    }
    if (acum > 0 && part_type == 'E'){
        cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No se puede crear mas de 1 particion extendida ";
        return;
    }

    //Construyendo el listado de espacios entre particiones
    vector<disco::Fit> listFit;
    disco::Fit tmpFit;
    if (getPartitions.size() > 0){
        tmpFit.start = csnt_admdcs.SIZE_MBR;
        tmpFit.size = getPartitions[0].part_start - csnt_admdcs.SIZE_MBR;
        listFit.push_back(tmpFit);
        for (int i = 0; i < getPartitions.size(); i++){
            if (i < (getPartitions.size()-1)){
                tmpFit.start = getPartitions[i].part_start + getPartitions[i].part_size;
                tmpFit.size = getPartitions[i+1].part_start - tmpFit.start;
                listFit.push_back(tmpFit);
            }else{
                tmpFit.start = getPartitions[i].part_start + getPartitions[i].part_size;
                tmpFit.size = mbr.mbr_tamano - tmpFit.start;
                listFit.push_back(tmpFit);
            }
        }
        tmpFit.start = mbr.mbr_tamano;
        tmpFit.size = 0;
        listFit.push_back(tmpFit);        
    }else{
        tmpFit.start = csnt_admdcs.SIZE_MBR;
        tmpFit.size = mbr.mbr_tamano - csnt_admdcs.SIZE_MBR;
        listFit.push_back(tmpFit);
        tmpFit.start = mbr.mbr_tamano;
        tmpFit.size = 0;
        listFit.push_back(tmpFit);
    }
    int response_Position = assign_positionByFit(listFit, part_size, mbr.disk_fit);
    if (response_Position == -1){
        cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No se ha podido realizar correctamente el ajuste ";
        return;
    }else if(response_Position == -2){
        cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No hay suficiente espacio para crear la particion ";
        return;
    }

    //Se crea una estructura temporal de tipo particion la cual se almacenara
    disco::Partition temp_partition;
    temp_partition.part_status = '1';
    temp_partition.part_type = part_type;
    temp_partition.part_fit = part_fit;
    temp_partition.part_start = response_Position;
    temp_partition.part_size = part_size;
    strcpy(temp_partition.part_name, part_name.c_str());

    //Se asigna la particion nueva a una particion del MBR que no se este utilizando
    if (mbr.mbr_partition_1.part_status == '0'){
        mbr.mbr_partition_1 = temp_partition;
    }else if(mbr.mbr_partition_2.part_status == '0'){
        mbr.mbr_partition_2 = temp_partition;
    }else if(mbr.mbr_partition_3.part_status == '0'){
        mbr.mbr_partition_3 = temp_partition;
    }else if(mbr.mbr_partition_4.part_status == '0'){
        mbr.mbr_partition_4 = temp_partition;
    }else{
        cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " Se ha llegado al limite de particiones (4) ";
        return;
    }
    
    //Ahora ya se almacenan la nueva particion en el disco
    FILE *save = fopen(path.c_str(), "rb+");
    fwrite(&mbr, csnt_admdcs.SIZE_MBR, 1, save);
    fclose(save);

    if (part_type == 'E'){
        if (fdisk_createLogicInit(temp_partition, path) == -1){return;}        
    }
    string type_partition = (part_type == 'E') ? "extendida": "primaria";
    cout << csnt_admdcs.GREEN << "RESPUESTA:" << csnt_admdcs.NC << " Se ha creado correctamente la particion " << type_partition << " ";
}

int adm_discos::fdisk_createLogicInit(disco::Partition partition, string path){
    disco::EBR tempEBR;
    strcpy(tempEBR.part_name, "Vacio");
    FILE *file = fopen(path.c_str(), "rb+");
    if (file){
        fseek(file, partition.part_start, SEEK_SET);
        fwrite(&tempEBR, csnt_admdcs.SIZE_EBR, 1, file);
        fclose(file);
    }else{
        cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No se ha realizado la particion logica inicial correctamente ";
        return -1;
    }
    return 0;
}

void adm_discos::fdisk_createLogic(disco::MBR mbr, char part_fit, char part_type, int part_size, string part_name, string path){
    if (part_type != 'L'){
        return;
    }

    //Validando que exista una particion extendida
    vector<disco::Partition> getPartitions = getListPartitionsEP(mbr);
    int index = -1;
    for (int i = 0; i < getPartitions.size(); i++){
        if (getPartitions[i].part_type == 'E'){
            index = i;
            break;
        }
    }
    if (index == -1){
        cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No se puede crear una particion logica si no hay una extendida ";
        return;
    }

    //Construimos el listado de los ajustes para las particiones
    disco::Partition partitionExtended = getPartitions[index];
    vector<disco::Fit> listFit;
    disco::Fit tmpFit;
    vector<disco::EBR> listEBR = getListPartitionsL(partitionExtended, path);
    if (listEBR.size() > 0){
        tmpFit.start = partitionExtended.part_start;
        tmpFit.size = listEBR[0].part_start - partitionExtended.part_start;
        listFit.push_back(tmpFit);
        for (int i = 0; i < listEBR.size(); i++){
            if (i < (listEBR.size()-1)){
                tmpFit.start = listEBR[i].part_start + listEBR[i].part_size;
                tmpFit.size = listEBR[i+1].part_start - tmpFit.start;
                listFit.push_back(tmpFit);
            }else{
                tmpFit.start = listEBR[i].part_start + listEBR[i].part_size;
                tmpFit.size = (partitionExtended.part_start + partitionExtended.part_size) - tmpFit.start;
                listFit.push_back(tmpFit);
            }
        }
        tmpFit.start = partitionExtended.part_start + partitionExtended.part_size;
        tmpFit.size = 0;
        listFit.push_back(tmpFit);        
    }else{
        tmpFit.start = partitionExtended.part_start;
        tmpFit.size = partitionExtended.part_size;
        listFit.push_back(tmpFit);
        tmpFit.start = partitionExtended.part_start + partitionExtended.part_size;
        tmpFit.size = 0;
        listFit.push_back(tmpFit);
    }
    
    int response_Position = assign_positionByFit(listFit, part_size, partitionExtended.part_fit);
    if (response_Position == -1){
        cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No se ha podido realizar correctamente el ajuste ";
        return;
    }else if(response_Position == -2){
        cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No hay suficiente espacio para crear la particion logica ";
        return;
    }

    //Se crea una estructura temporal de EBR
    disco::EBR tempEBR;
    tempEBR.part_status = '1';
    tempEBR.part_fit = part_fit;
    tempEBR.part_start = response_Position;
    tempEBR.part_size = part_size;
    strcpy(tempEBR.part_name, part_name.c_str());

    //Se obtiene los EBR que estan antes y despues del actual que se va a ingresar
    int before_logic = -1;
    int after_logic = -1;
    getBA_PartitionL(partitionExtended, path, tempEBR.part_start, &before_logic, &after_logic);
    if (before_logic != -1){
        edit_beforeEBR(before_logic, tempEBR.part_start, path);
    }
    tempEBR.part_next = after_logic;

    //Ahora ya se almacenan los datos de las nuevas particiones, en el disco
    FILE *save = fopen(path.c_str(), "rb+");
    fseek(save, tempEBR.part_start, SEEK_SET);
    fwrite(&tempEBR, csnt_admdcs.SIZE_EBR, 1, save);
    fclose(save);
    cout << csnt_admdcs.GREEN << "RESPUESTA:" << csnt_admdcs.NC << " Se ha creado correctamente la particion logica ";
}

void adm_discos::fdisk_deletePrimaryExtended(string delete_p, disco::MBR mbr, string path, string name){
    
    vector<disco::Partition> listTemp;
    listTemp.push_back(mbr.mbr_partition_1);
    listTemp.push_back(mbr.mbr_partition_2);
    listTemp.push_back(mbr.mbr_partition_3);
    listTemp.push_back(mbr.mbr_partition_4);

    int before = -1;
    int after = -1;
    disco::Partition actualPartition = getPartitionEP(mbr, name, &before, &after);
    string type;
    for (int i = 0; i < listTemp.size(); i++){
        if (listTemp[i].part_start == actualPartition.part_start){
            type = listTemp[i].part_type;
            disco::Partition newPartition;
            listTemp[i] = newPartition;            
            break;
        }
    }
    mbr.mbr_partition_1 = listTemp[0];
    mbr.mbr_partition_2 = listTemp[1];
    mbr.mbr_partition_3 = listTemp[2];
    mbr.mbr_partition_4 = listTemp[3];
    FILE *save = fopen(path.c_str(), "rb+");
    fseek(save, 0, SEEK_SET);
    fwrite(&mbr, csnt_admdcs.SIZE_MBR, 1, save);
    fclose(save);
    type = (type == "P")?"primaria":"extendida";
    cout << csnt_admdcs.GREEN << "RESPUESTA:" << csnt_admdcs.NC << " Se ha eliminado correctamente la particion " << type << " ";
}

void adm_discos::fdisk_deleteLogic(string delete_p, disco::MBR mbr, string path, string name){

    //Buscamos la particion extendida
    disco::Partition partititionExtended;
    partititionExtended = (mbr.mbr_partition_1.part_type == 'E') ? mbr.mbr_partition_1: partititionExtended;
    partititionExtended = (mbr.mbr_partition_2.part_type == 'E') ? mbr.mbr_partition_2: partititionExtended;
    partititionExtended = (mbr.mbr_partition_3.part_type == 'E') ? mbr.mbr_partition_3: partititionExtended;
    partititionExtended = (mbr.mbr_partition_4.part_type == 'E') ? mbr.mbr_partition_4: partititionExtended;

    //Se obtiene la particion logica que se va a eliminar
    int before = -1;
    disco::EBR actualLogic = getPartitionL(partititionExtended, path, name, &before);
    if (before != -1){
        edit_beforeEBR(before, actualLogic.part_next, path);
    }
    if (partititionExtended.part_start == actualLogic.part_start){
        //Aqui se deshabilita la particion logica que se encuentra al inicio de la particion extendida
        FILE *save = fopen(path.c_str(), "rb+");
        actualLogic.part_status = '0';
        actualLogic.part_size = 0;
        fseek(save, actualLogic.part_start, SEEK_SET);
        fwrite(&actualLogic, csnt_admdcs.SIZE_EBR, 1, save);
        fclose(save);
    }else{
        //Aqui se deshabilita la particion posterior a la particion del inicio
        FILE *save = fopen(path.c_str(), "rb+");
        disco::EBR newEBR;
        fseek(save, actualLogic.part_start, SEEK_SET);
        fwrite(&newEBR, csnt_admdcs.SIZE_EBR, 1, save);
        fclose(save);
    }
    cout << csnt_admdcs.GREEN << "RESPUESTA:" << csnt_admdcs.NC << " Se ha eliminado correctamente la particion logica ";
}

void adm_discos::fdisk_addPrimaryExtended(int add_p, disco::MBR mbr, string path, string name){
    
    //Se almacena el listado de las particiones del MBR
    vector<disco::Partition> listTemp;
    listTemp.push_back(mbr.mbr_partition_1);
    listTemp.push_back(mbr.mbr_partition_2);
    listTemp.push_back(mbr.mbr_partition_3);
    listTemp.push_back(mbr.mbr_partition_4);

    //Se obtiene la particion segun el nombre ingresado por el usuario
    int before = -1;
    int after = -1;
    disco::Partition actualPartition = getPartitionEP(mbr,name, &before, &after);
    
    string type;
    string concepto;
    bool validate = false;
    for (int i = 0; i < listTemp.size(); i++){
        if (listTemp[i].part_start == actualPartition.part_start){
            if (add_p >= 0){
                //Verifica el inicio de la siguiente particion o el tamaño del disco
                int pos_tope = (i+1 <= listTemp.size()-1) ? listTemp[i+1].part_start: mbr.mbr_tamano;
                int pos_new = listTemp[i].part_start + listTemp[i].part_size + add_p;
                if (pos_new <= pos_tope){
                    listTemp[i].part_size = listTemp[i].part_size + add_p;
                    validate = true;
                    break;
                }
            }else{
                //Debe de verificar el inicio de la particion
                int pos_tope = listTemp[i].part_start;
                int pos_new = listTemp[i].part_start + listTemp[i].part_size + add_p;
                //Si se quiere dejar una particion con tamaño de 0 se debe poner >= enves de >
                if (pos_new > pos_tope){
                    listTemp[i].part_size = listTemp[i].part_size + add_p;
                    validate = true;
                    break;
                }
            }
        }
    }
    type = (type == "P")?"primaria":"extendida";
    concepto = (add_p >= 0) ? "aumentado": "disminuido";
    if (!validate){
        concepto = concepto == "aumentado"? "aumentado correctamente la particion por falta de espacio": "disminuido correctamente la particion debido a que se excede los limites del inicio de la particion";
        cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No se ha " << concepto << " ";
        return;
    }
    
    mbr.mbr_partition_1 = listTemp[0];
    mbr.mbr_partition_2 = listTemp[1];
    mbr.mbr_partition_3 = listTemp[2];
    mbr.mbr_partition_4 = listTemp[3];
    FILE *save = fopen(path.c_str(), "rb+");
    fseek(save, 0, SEEK_SET);
    fwrite(&mbr, csnt_admdcs.SIZE_MBR, 1, save);
    fclose(save);
    cout << csnt_admdcs.GREEN << "RESPUESTA:" << csnt_admdcs.NC << " Se ha " << concepto << " correctamente la particion " << type << " ";
}

void adm_discos::fdisk_addLogic(int add_p, disco::MBR mbr, string path, string name){

    disco::Partition partititionExtended;
    partititionExtended = (mbr.mbr_partition_1.part_type == 'E') ? mbr.mbr_partition_1: partititionExtended;
    partititionExtended = (mbr.mbr_partition_2.part_type == 'E') ? mbr.mbr_partition_2: partititionExtended;
    partititionExtended = (mbr.mbr_partition_3.part_type == 'E') ? mbr.mbr_partition_3: partititionExtended;
    partititionExtended = (mbr.mbr_partition_4.part_type == 'E') ? mbr.mbr_partition_4: partititionExtended;

    int before = -1;
    disco::EBR actualPartition = getPartitionL(partititionExtended, path, name, &before);
    string concepto;
    bool validate = false;
    if (add_p >= 0){
        //Verifica el inicio de la siguiente particion
        int pos_tope = (actualPartition.part_next != -1) ? actualPartition.part_next: (partititionExtended.part_start + partititionExtended.part_size);
        int pos_new = actualPartition.part_start + actualPartition.part_size + add_p;
        if (pos_new <= pos_tope){
            actualPartition.part_size = actualPartition.part_size + add_p;
            validate = true;
        }
    }else{
        //Se obtiene el inicio de la particion actual en el cual se modificara su tamaño
        int pos_tope = actualPartition.part_start;
        int pos_new = actualPartition.part_start + actualPartition.part_size + add_p;
        //Si se quiere dejar una particion con tamaño de 0 se debe poner >= enves de >
        if (pos_new > pos_tope){
            actualPartition.part_size = actualPartition.part_size + add_p;
            validate = true;
        }
    }

    concepto = (add_p > 0) ? "aumentado": "disminuido";
    if (!validate){
        concepto = concepto == "aumentado"? "aumentado correctamente la particion por falta de espacio": "disminuido correctamente la particion debido a que se excede los limites del inicio de la particion";
        cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No se ha " << concepto << " ";
        return;
    }

    FILE *save = fopen(path.c_str(), "rb+");
    fseek(save, actualPartition.part_start, SEEK_SET);
    fwrite(&actualPartition, csnt_admdcs.SIZE_EBR, 1, save);
    fclose(save);
    cout << csnt_admdcs.GREEN << "RESPUESTA:" << csnt_admdcs.NC << " Se ha " << concepto << " correctamente la particion logica ";
}

void adm_discos::mkfs_EXT2(disco::Superblock superblock, int part_start_partition, string path, string id_mount, disco::Mount partitionMount){

    //Inicio del bitmap de inodos
    superblock.s_bm_inode_start = part_start_partition + csnt_admdcs.SIZE_SPB;
    //Inicio del bitmap de bloques
    superblock.s_bm_block_start = superblock.s_bm_inode_start + superblock.s_inodes_count;
    //Inicio de la tabla de inodos
    superblock.s_inode_start = superblock.s_bm_block_start + superblock.s_blocks_count;
    //Inicio de la tabla de bloques
    superblock.s_block_start = superblock.s_inode_start + (superblock.s_inodes_count * csnt_admdcs.SIZE_I);

    FILE *format_partition = fopen(path.c_str(), "rb+");
    
    //Se almacena el superbloque
    fseek(format_partition, part_start_partition, SEEK_SET);
    fwrite(&superblock, csnt_admdcs.SIZE_SPB, 1, format_partition);
    
    char zero = '0';
    //Seteo 0's en el bitmap de inodos
    fseek(format_partition, superblock.s_bm_inode_start, SEEK_SET);
    for (int i = 0; i < superblock.s_inodes_count; i++){
        fwrite(&zero, sizeof(zero), 1, format_partition);
    }
    
    //Seteo 0's en el bitmap de bloques
    fseek(format_partition, superblock.s_bm_block_start, SEEK_SET);
    for (int i = 0; i < superblock.s_blocks_count; i++){
        fwrite(&zero, sizeof(zero), 1, format_partition);
    }

    //Seteo estructura-inodo al espacio de inodos
    disco::Inode seteando_inodo;
    fseek(format_partition, superblock.s_inode_start, SEEK_SET);
    for (int i = 0; i < superblock.s_inodes_count; i++){
        fwrite(&seteando_inodo, csnt_admdcs.SIZE_I, 1, format_partition);
    }
    
    //Seteo estructura-block al espacio para los bloques
    disco::Archiveblock seteando_bloque;
    fseek(format_partition, superblock.s_block_start, SEEK_SET);
    for (size_t i = 0; i < superblock.s_blocks_count; i++){
        fwrite(&seteando_bloque, csnt_admdcs.SIZE_AB, 1, format_partition);
    }    
    
    //Cierro el archivo de formateo
    fclose(format_partition);
    
    /*En la parte superior ya se realizo el formateo del sistema de archivo ahora en la parte 
    inferior se agrega la carpeta root y se agrega un archivo llamado user.txt*/
    
    //Defino una variable que almacene las estructuras en el disco
    FILE *save_structs = fopen(path.c_str(), "rb+");
    
    //Ahora se crea el primer inodo el cual sera la carpeta root
    disco::Inode inode_root;
    inode_root.i_uid = 1;
    inode_root.i_gid = 1;
    inode_root.i_size = 0;
    inode_root.i_atime = superblock.s_umtime;
    inode_root.i_ctime = time(nullptr);
    inode_root.i_mtime = superblock.s_umtime;
    inode_root.i_block[0] = 0;
    inode_root.i_type = '0';
    inode_root.i_perm = 664;
    //Se almacena el primer inodo en el bitmap de inodos
    char caracter = '1';
    fseek(save_structs, superblock.s_bm_inode_start, SEEK_SET);
    fwrite(&caracter, sizeof(caracter), 1, save_structs);
    //Se almacena el primer inodo en la tabla de inodos
    fseek(save_structs, superblock.s_inode_start, SEEK_SET);
    fwrite(&inode_root, csnt_admdcs.SIZE_I, 1, save_structs);

    //Se crea un bloque de carpeta para la ruta "/"
    disco::Folderblock temp_folderblock;
    //Padre
    strcpy(temp_folderblock.b_content[0].b_name, ".");
    temp_folderblock.b_content[0].b_inodo = 0;
    //Nombre de la carpeta
    strcpy(temp_folderblock.b_content[1].b_name, "..");
    temp_folderblock.b_content[1].b_inodo = 0;
    //Nombre del archivo que contiene la carpeta - Apunta al inodo del archivo
    strcpy(temp_folderblock.b_content[2].b_name, "user.txt");
    temp_folderblock.b_content[2].b_inodo = 1;
    //Se almacena el primer bloque en el bitmap
    fseek(save_structs, superblock.s_bm_block_start, SEEK_SET);
    fwrite(&caracter, sizeof(caracter), 1, save_structs);
    //Se almacena el primer bloque en la tabla de bloques
    fseek(save_structs, superblock.s_block_start, SEEK_SET);
    fwrite(&temp_folderblock, csnt_admdcs.SIZE_FB, 1, save_structs);

    //Defino los datos predeterminados que posee el archivo user.txt
    string data = "1,G,root\n1,U,root,root,123\n";
    //Se crea el segundo inodo el cual sera para el archivo user.txt
    disco::Inode inode_archive;
    inode_archive.i_uid = 1;
    inode_archive.i_gid = 1;
    inode_archive.i_size = data.length();
    inode_archive.i_atime = superblock.s_umtime;
    inode_archive.i_ctime = time(nullptr);
    inode_archive.i_mtime = superblock.s_umtime;
    inode_archive.i_block[0] = 1;
    inode_archive.i_type = '1';    
    inode_archive.i_perm = 664;
    //Se almacena el segundo inodo en el bitmap de inodos
    fseek(save_structs, superblock.s_bm_inode_start + 1, SEEK_SET);
    fwrite(&caracter, sizeof(caracter), 1, save_structs);
    //Se almacena el segundo inodo en la tabla de inodos
    fseek(save_structs, superblock.s_inode_start + csnt_admdcs.SIZE_I, SEEK_SET);
    fwrite(&inode_archive, csnt_admdcs.SIZE_I, 1, save_structs);

    //Se crea un bloque de archivo
    disco::Archiveblock temp_archiveblock;
    //Informacion del bloque - Se ingresa la data
    strcpy(temp_archiveblock.b_content, data.c_str());
    //Se almacena el segundo bloque en el bitmap
    fseek(save_structs, superblock.s_bm_block_start + 1, SEEK_SET);
    fwrite(&caracter, sizeof(caracter), 1, save_structs);
    //Se almacena el segundo bloque en la tabla de bloques
    fseek(save_structs, superblock.s_block_start + csnt_admdcs.SIZE_AB, SEEK_SET);
    fwrite(&temp_archiveblock, csnt_admdcs.SIZE_AB, 1, save_structs);
    fclose(save_structs);
}

void adm_discos::mkfs_EXT3(disco::Superblock superblock, int part_start_partition, string path, string id_mount, disco::Mount partitionMount){
    
    //Inicio del Journaling
    int journaling_start = part_start_partition + csnt_admdcs.SIZE_SPB;
    //Cantidad de Journaling
    int total_block_journaling = superblock.s_inodes_count;

    //Inicio del bitmap de inodos    
    superblock.s_bm_inode_start = journaling_start + (total_block_journaling * csnt_admdcs.SIZE_J);
    //Inicio del bitmap de bloques
    superblock.s_bm_block_start = superblock.s_bm_inode_start + superblock.s_inodes_count;
    //Inicio de la tabla de inodos
    superblock.s_inode_start = superblock.s_bm_block_start + superblock.s_blocks_count;
    //Inicio de la tabla de bloques
    superblock.s_block_start = superblock.s_inode_start + (superblock.s_inodes_count * csnt_admdcs.SIZE_I);

    FILE *format_partition = fopen(path.c_str(), "rb+");
    //Se almacena la estructura de superbloque
    fseek(format_partition, part_start_partition, SEEK_SET);
    fwrite(&superblock, sizeof(disco::Superblock), 1, format_partition);

    //Seteo de estructuras journaling
    disco::Journaling seteando_journal;
    for (int i = 0; i < total_block_journaling; i++){
        fwrite(&seteando_journal, csnt_admdcs.SIZE_J, 1, format_partition);
    }
    
    char zero = '0';
    //Seteo 0's en el bitmap de inodos
    fseek(format_partition, superblock.s_bm_inode_start, SEEK_SET);
    for (int i = 0; i < superblock.s_inodes_count; i++){
        fwrite(&zero, sizeof(zero), 1, format_partition);
    }

    //Seteo 0's en el bitmap de bloques
    fseek(format_partition, superblock.s_bm_block_start, SEEK_SET);
    for (int i = 0; i < superblock.s_blocks_count; i++){
        fwrite(&zero, sizeof(zero), 1, format_partition);
    }

    //Seteo estructuras de tipo Inodo
    disco::Inode seteando_inodo;
    fseek(format_partition, superblock.s_inode_start, SEEK_SET);
    for (int i = 0; i < superblock.s_inodes_count; i++){
        fwrite(&seteando_inodo, csnt_admdcs.SIZE_I, 1, format_partition);
    }

    //Seteo 0's al espacio para los bloques
    disco::Archiveblock seteando_block;
    fseek(format_partition, superblock.s_block_start, SEEK_SET);
    for (size_t i = 0; i < superblock.s_blocks_count; i++){
        fwrite(&seteando_block, csnt_admdcs.SIZE_AB, 1, format_partition);
    }
    
    //Cierro el archivo de formateo
    fclose(format_partition);

    /*En la parte superior ya se realizo el formateo del sistema de archivo ahora en la parte 
    inferior se agrega la carpeta root y se agrega un archivo llamado user.txt*/
    
    //Defino una variable que almacene las estructuras en el disco
    FILE *save_structs = fopen(path.c_str(), "rb+");
    
    //Ahora se crea el primer inodo el cual sera la carpeta root
    disco::Inode inode_root;
    inode_root.i_uid = 1;
    inode_root.i_gid = 1;
    inode_root.i_size = 0;
    inode_root.i_atime = superblock.s_umtime;
    inode_root.i_ctime = time(nullptr);
    inode_root.i_mtime = superblock.s_umtime;
    inode_root.i_block[0] = 0;
    inode_root.i_type = '0';
    inode_root.i_perm = 664;
    //Se almacena el primer inodo en el bitmap de inodos
    char caracter = '1';
    fseek(save_structs, superblock.s_bm_inode_start, SEEK_SET);
    fwrite(&caracter, sizeof(caracter), 1, save_structs);
    //Se almacena el primer inodo en la tabla de inodos
    fseek(save_structs, superblock.s_inode_start, SEEK_SET);
    fwrite(&inode_root, csnt_admdcs.SIZE_I, 1, save_structs);

    //Se crea un bloque de carpeta para la ruta "/"
    disco::Folderblock temp_folderblock;
    //Padre
    strcpy(temp_folderblock.b_content[0].b_name, ".");
    temp_folderblock.b_content[0].b_inodo = 0;
    //Nombre de la carpeta
    strcpy(temp_folderblock.b_content[1].b_name, "..");
    temp_folderblock.b_content[1].b_inodo = 0;
    //Nombre del archivo que contiene la carpeta - Apunta al inodo del archivo
    strcpy(temp_folderblock.b_content[2].b_name, "user.txt");
    temp_folderblock.b_content[2].b_inodo = 1;
    //Se almacena el primer bloque en el bitmap
    fseek(save_structs, superblock.s_bm_block_start, SEEK_SET);
    fwrite(&caracter, sizeof(caracter), 1, save_structs);
    //Se almacena el primer bloque en la tabla de bloques
    fseek(save_structs, superblock.s_block_start, SEEK_SET);
    fwrite(&temp_folderblock, csnt_admdcs.SIZE_FB, 1, save_structs);

    //Se crea el journaling de la primera transaccion
    disco::Journaling journaling1;
    journaling1.operation = 'C';
    journaling1.type = '0';
    strcpy(journaling1.nombre, "/");
    journaling1.date = time(nullptr);
    strcpy(journaling1.propietario, "root");
    journaling1.permiso = 664;
    //Se almacena el journaling
    fseek(save_structs, journaling_start, SEEK_SET);
    fwrite(&journaling1, csnt_admdcs.SIZE_J, 1, save_structs);

    //Defino los datos predeterminados que posee el archivo user.txt
    string data = "1,G,root\n1,U,root,root,123\n";
    //Se crea el segundo inodo el cual sera para el archivo user.txt
    disco::Inode inode_archive;
    inode_archive.i_uid = 1;
    inode_archive.i_gid = 1;
    inode_archive.i_size = data.length();
    inode_archive.i_atime = superblock.s_umtime;
    inode_archive.i_ctime = time(nullptr);
    inode_archive.i_mtime = superblock.s_umtime;
    inode_archive.i_block[0] = 1;
    inode_archive.i_type = '1';    
    inode_archive.i_perm = 664;
    //Se almacena el segundo inodo en el bitmap de inodos
    fseek(save_structs, superblock.s_bm_inode_start + 1, SEEK_SET);
    fwrite(&caracter, sizeof(caracter), 1, save_structs);
    //Se almacena el segundo inodo en la tabla de inodos
    fseek(save_structs, superblock.s_inode_start + csnt_admdcs.SIZE_I, SEEK_SET);
    fwrite(&inode_archive, csnt_admdcs.SIZE_I, 1, save_structs);

    //Se crea un bloque de archivo
    disco::Archiveblock temp_archiveblock;
    //Informacion del bloque - Se ingresa la data
    strcpy(temp_archiveblock.b_content, data.c_str());
    //Se almacena el segundo bloque en el bitmap
    fseek(save_structs, superblock.s_bm_block_start + 1, SEEK_SET);
    fwrite(&caracter, sizeof(caracter), 1, save_structs);
    //Se almacena el segundo bloque en la tabla de bloques
    fseek(save_structs, superblock.s_block_start + csnt_admdcs.SIZE_AB, SEEK_SET);
    fwrite(&temp_archiveblock, csnt_admdcs.SIZE_AB, 1, save_structs);

    //Se crea el journaling de la segunda transaccion
    disco::Journaling journaling2;
    journaling2.operation = 'C';
    journaling2.type = '1';
    strcpy(journaling2.nombre, "user.txt");
    journaling2.content = data;
    journaling2.date = time(nullptr);
    strcpy(journaling2.propietario, "root");
    journaling2.permiso = 664;
    //Se almacena el journaling
    fseek(save_structs, journaling_start + csnt_admdcs.SIZE_J, SEEK_SET);
    fwrite(&journaling2, csnt_admdcs.SIZE_J, 1, save_structs);
    fclose(save_structs);
}

//FIN - SUB-METODOS

//INIT - METODOS AUXILIARES

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

void adm_discos::getBA_PartitionL(disco::Partition partition_extended, string path, int actual, int *before, int *after){
    vector<disco::EBR> listLogics = getListPartitionsL(partition_extended, path);
    if (listLogics.size() <= 0){
        *before = -1;
        *after = -1;
        return;
    }
    if (partition_extended.part_start == actual){
        *before = -1;
        *after = listLogics[0].part_start;
        return;
    }
    for (int i = 0; i < listLogics.size(); i++){
        int next = listLogics[i].part_start + listLogics[i].part_size;
        if (next == actual){
            *before = listLogics[i].part_start;
            *after = (i+1 <= listLogics.size()-1) ? listLogics[i+1].part_start: -1;
            break;
        }        
    }
}

void adm_discos::edit_beforeEBR(int part_start, int part_next, string path){
    disco::EBR tempEBR;
    FILE *file = fopen(path.c_str(), "rb+");
    //Se obtiene el EBR
    fseek(file, part_start, SEEK_SET);
    fread(&tempEBR, csnt_admdcs.SIZE_EBR, 1, file);
    //Se modifica su next
    tempEBR.part_next = part_next;
    //Se sobrescribe
    fseek(file, part_start, SEEK_SET);
    fwrite(&tempEBR, csnt_admdcs.SIZE_EBR, 1, file);
    fclose(file);
}

disco::EBR adm_discos::getPartitionL(disco::Partition partition_extended, string path, string name, int *before){
    
    disco::EBR response;
    FILE *file = fopen(path.c_str(), "rb+");
    fseek(file, partition_extended.part_start, SEEK_SET);
    fread(&response, csnt_admdcs.SIZE_EBR, 1, file);
    while (true){
        if (response.part_status != '0'){
            if (response.part_name == name){
                break;
            }else{
                *before = response.part_start;
                if (response.part_next == -1){
                    break;
                }
                fseek(file, response.part_next, SEEK_SET);
                fread(&response, csnt_admdcs.SIZE_EBR, 1, file);
            }
        }else{
            if (response.part_next == -1){
                break;
            }else{
                *before = response.part_start;
                fseek(file, response.part_next, SEEK_SET);
                fread(&response, csnt_admdcs.SIZE_EBR, 1, file);
            }
        }
    }
    return response;
}

disco::Partition adm_discos::getPartitionEP(disco::MBR mbr, string name, int *before, int *after){
    disco::Partition response;
    vector<disco::Partition> listPartitions = getListPartitionsEP(mbr);
    for (int i = 0; i < listPartitions.size(); i++){
        *before = (i-1) >= 0 ? listPartitions[i-1].part_start: -1;
        if (listPartitions[i].part_name == name){
            response = listPartitions[i];
            *after = (listPartitions.size()-1) >= (i+1) ? listPartitions[i+1].part_start: -1;
            break;
        }        
    }    
    return response;
}

disco::Mount adm_discos::partitionToMounted(string name, string path){
    
    disco::Mount response;
    int partition_next = 1;
    short code_ASCII = 97;
    bool foundMount = false;
    bool pathFound = false; 

    for (int i = 0; i < Memory_Mount.size(); i++){
        if (Memory_Mount[i].name == name && Memory_Mount[i].path == path){
            response.status = '0';
            foundMount = true;
            break;
        }
        //Si encuentra un path que ya esta montado
        if (Memory_Mount[i].path == path && Memory_Mount[i].num_partition >= partition_next){
            code_ASCII = Memory_Mount[i].letter;
            partition_next = Memory_Mount[i].num_partition + 1;
            pathFound = true;
        }
        //Si no encuentra un path montado
        if (Memory_Mount[i].path != path && Memory_Mount[i].letter >= code_ASCII && !pathFound){
            code_ASCII = Memory_Mount[i].letter + 1;
            partition_next = 1;
        }        
    }

    char letter = code_ASCII;
    if (!foundMount){
        response.name = name;
        response.path = path;
        response.num_partition = partition_next;
        response.letter = code_ASCII;
        response.status = '1';
        response.id = letter+to_string(response.num_partition);
        response.id = "vd"+response.id;
        response.date_mounted = time(nullptr);
    }
    return response;
}

disco::Mount adm_discos::checkPartitionMounted(string path){
    disco::Mount response;
    for (int i = 0; i < Memory_Mount.size(); i++){
        if (Memory_Mount[i].path == path){
            response = Memory_Mount[i];
            break;
        }
    }
    return response;
}

disco::Mount adm_discos::getMountedLog(string id){
    disco::Mount response;
    for (int i = 0; i < Memory_Mount.size(); i++){
        if (Memory_Mount[i].id == id){
            response = Memory_Mount[i];
            break;
        }
    }
    return response;
}

vector<disco::Mount> adm_discos::getListMount(){
    return Memory_Mount;
}

//FIN - METODOS AUXILIARES

//METODOS PARA TEST - ELIMINAR ANTES DE ULTIMO PUSH

void adm_discos::test_asignacionFit(){
    char fit = 'W';
    int size_temp = 100;
    vector<disco::Fit> list_fit;
    disco::Fit tempFit;
    tempFit.name = "nombre 1";
    tempFit.start = 1000000;
    tempFit.size = tempFit.start;
    list_fit.push_back(tempFit);
    tempFit.name = "nombre 2";
    tempFit.start = 5200000;
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
    tempFit.start = 99;
    tempFit.size = tempFit.start;
    list_fit.push_back(tempFit);
    tempFit.name = "nombre 7";
    tempFit.start = 300;
    tempFit.size = tempFit.start;
    list_fit.push_back(tempFit);
    tempFit.name = "nombre 8";
    tempFit.start = 100000;
    tempFit.size = tempFit.start;
    list_fit.push_back(tempFit);

    int response_Position = assign_positionByFit(list_fit, size_temp, fit);
    if (response_Position == -1){
        cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No se ha podido realizar correctamente el ajuste" << endl;
        return;
    }else if(response_Position == -2){
        cout << csnt_admdcs.RED << "ERROR:" << csnt_admdcs.NC << " No hay suficiente espacio para crear la particion" << endl;
        return;
    }
    cout << "RESPONSE:" << response_Position << endl;

}

