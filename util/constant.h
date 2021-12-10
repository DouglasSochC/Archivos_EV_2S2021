#ifndef CONSTANT_H
#define CONSTANT_H
#include <string>
#include "../src/disco.h"

using namespace std;

class constant
{
public:
    const int AMOUNT_BITS  = 1024;
    const string EXTENSION_FILE  = ".disk";
    const string EXTENSION_EXEC  = ".sh";
    
    //ESTRUCTURAS
    
    const int SIZE_MBR = sizeof(disco::MBR);
    const int SIZE_PARTITION = sizeof(disco::Partition);
    const int SIZE_EBR = sizeof(disco::EBR);

    //FORMATEO

    const int SIZE_SPB = sizeof(disco::Superblock); //Super bloque
    const int SIZE_I = sizeof(disco::Inode); //Inodo
    const int SIZE_J = sizeof(disco::Journaling); //Journaling
    
    //MISMO TAMANIO
    
    const int SIZE_AB = sizeof(disco::Archiveblock); //Archive Block
    const int SIZE_FB = sizeof(disco::Folderblock); //File Block
    const int SIZE_PB = sizeof(disco::Pointerblock); //Pointer Block
    
    //COLORES - MENSAJES

    const string NC = "\e[0m";
    const string RED = "\e[0;31m";
    const string GREEN = "\e[0;32m";
    const string YELLOW = "\e[0;33m";
    const string BLUE = "\e[0;34m";
    const string CYAN = "\e[0;36m";

    //COLORES - TABLAS

    const string COLOR_INODE = "#84B1F2";
    const string COLOR_FOLDER = "#F2EA84";
    const string COLOR_ARCHIVE = "#F284ED";
    const string COLOR_POINTER = "#38F50E";    

    //USUARIOS Y GRUPOS INICIALES

    const string USER_TXT = "1,G,root\n1,U,root,root,123\n";
};

#endif