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

void adm_discos::mkdisk(map<string, string> param_got){
    if (param_got.size() == 0)
    {
        return;
    }
    /*Obteniendo Datos*/
    string comentario = param_got["-comentario"];
    int size = atoi(param_got["-size"].c_str());
    char unit = util_admdcs.toLowerString(param_got["-unit"].c_str())[0];
    string fit = util_admdcs.toUpperString(param_got["-fit"].c_str());
    string path = param_got["-path"].c_str();

    /*Formateo de Datos*/
    path = (path.substr(0,1) == "\"") ? path.substr(1, path.size()-2): path;
    size = (unit == 'k') ? (size * csnt_admdcs.AMOUNT_BITS): (size * csnt_admdcs.AMOUNT_BITS * csnt_admdcs.AMOUNT_BITS);
    size = size + csnt_admdcs.SIZE_MBR;

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
    cout << csnt_admdcs.GREEN << "RESPUESTA:" << csnt_admdcs.NC << " La creacion del disco ha sido exitosa" << endl;
    if (!comentario.empty()){
        cout << csnt_admdcs.BLUE << comentario << endl;
    }    
}
