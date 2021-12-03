# Sistema de Archivos EXT2 y EXT3

_Este es un proyecto universitario del curso de Manejo e Implementacion de Archivos en donde la idea principal es imitar un sistema de archivos EXT2 y EXT3 en el lenguaje C/C++_

## 🚀 Comenzando

El proyecto fue desarrollado en una distribucion GNU/Linux

### 📋 Requerimientos

* Ubuntu 20.04.3 LTS o superior.
```console
~$ lsb_release -a
```

* Paquete g++ 9.3.0 o superior
```console
~$ g++ -v
```

### ⚙️ Ejecucion 

Compilar el proyecto
```console
~$ g++ main.cpp "analizador/scanner.cpp" "analizador/parameters.cpp" "util/util_p.cpp" "src/adm_discos.cpp" "src/script_adm.cpp" -o main
```

Ejecutar archivo compilado
```console
~$ ./main
```
## 📖 Documentacion

### 💿 Administracion de Discos

#### 1. MKDISK

Este comando creará un archivo binario que simulará undisco duro, estos archivos binarios tendrán la extensión disk y su contenido al inicio será 0. Deberá ocupar físicamente el tamaño indicado por los parámetros, (no importa que el sistema operativo no muestre el tamaño exacto). Recibirá el nombre del archivo que simulará el disco duro y tendrá los siguientes parámetros

| Parámetro | Categoría   | Descripción                                                                                                                                                                                                                                                                                                                                                                                                                                         |
|-----------|-------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| -size     | Obligatorio | Este parámetro recibirá un número que indicará el tamaño del disco a crear. Debe ser positivo y mayor que cero, si no se mostrará un error.                                                                                                                                                                                                                                                                                                         |
| -f        | Opcional    | Indicará el ajuste que utilizará el disco para crear las particiones dentro del disco Podrá tener los siguientes valores: <br>BF: Indicará el mejor ajuste (Best Fit)<br>FF: Utilizará el primer ajuste (First Fit)<br>WF: Utilizará el peor ajuste (Worst Fit)<br>Ya que es opcional, se tomará el primer ajuste si no está especificado en el comando. Si se utiliza otro valor que no sea alguno de los anteriores mostrará un mensaje de error. |
| -u        | Opcional    | Este parámetro recibirá una letra que indicará las unidades que utilizará el parámetro size. Podrá tener los siguientes valores:<br>k que indicará que se utilizarán Kilobytes (1024 bytes)<br>m en el que se utilizarán Megabytes (1024 * 1024 bytes)<br>Este parámetro es opcional, si no se encuentra se creará un disco con tamaño en Megabytes. Si se utiliza otro valor debe mostrarse un mensaje de error.                                   |
| -path     | Obligatorio | Este parámetro será la ruta en el que se creará el archivo que representará el disco duro. Si las carpetas de la ruta no existen deberán crearse. 

#### 2. RMKDISK

Este parámetro elimina un archivo que representa a un disco duro mostrando un mensaje de confirmación para eliminar. Tendrá los siguientes parámetros:

| Parámetro | Categoría   | Descripción                                                                                                                                              |
|-----------|-------------|----------------------------------------------------------------------------------------------------------------------------------------------------------|
| -path     | Obligatorio | Este parámetro será la ruta en el que se eliminará el archivo que representará el disco duro. Si el archivo no existe, debe mostrar un mensaje de error. |

### 📜 Script

#### 1. EXEC

El programa podrá ejecutar scripts con el comando exec. Debe mostrar el contenido de la línea que está leyendo y su resultado. También debe mostrar los comentarios del script.

| Parámetro | Categoría   | Descripción                                           |
|-----------|-------------|-------------------------------------------------------|
| -path     | Obligatorio | Especifica el nombre del script que se va a ejecutar. |