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
| -fit      | Opcional    | Indicará el ajuste que utilizará el disco para crear las particiones dentro del disco Podrá tener los siguientes valores: <br>**BF**: Indicará el mejor ajuste (Best Fit)<br>**FF**: Utilizará el primer ajuste (First Fit)<br>**WF**: Utilizará el peor ajuste (Worst Fit)<br>Ya que es opcional, se tomará el primer ajuste si no está especificado en el comando. Si se utiliza otro valor que no sea alguno de los anteriores mostrará un mensaje de error. |
| -unit     | Opcional    | Este parámetro recibirá una letra que indicará las unidades que utilizará el parámetro size. Podrá tener los siguientes valores:<br>**k** que indicará que se utilizarán Kilobytes (1024 bytes)<br>**m** en el que se utilizarán Megabytes (1024 * 1024 bytes)<br>Este parámetro es opcional, si no se encuentra se creará un disco con tamaño en Megabytes. Si se utiliza otro valor debe mostrarse un mensaje de error.                                   |
| -path     | Obligatorio | Este parámetro será la ruta en el que se creará el archivo que representará el disco duro. Si las carpetas de la ruta no existen deberán crearse. 

#### 2. RMKDISK

Este parámetro elimina un archivo que representa a un disco duro mostrando un mensaje de confirmación para eliminar. Tendrá los siguientes parámetros:

| Parámetro | Categoría   | Descripción                                                                                                                                              |
|-----------|-------------|----------------------------------------------------------------------------------------------------------------------------------------------------------|
| -path     | Obligatorio | Este parámetro será la ruta en el que se eliminará el archivo que representará el disco duro. Si el archivo no existe, debe mostrar un mensaje de error. |

#### 3. FDISK

Este comando administra las particiones en el archivo que representa al disco duro. Deberá mostrar un error si no se pudo realizar la operación solicitada sobre la partición, especificando por qué razón no pudo crearse (Por espacio, por restricciones de particiones, etc.). No se considerará el caso de que se pongan parámetros incompatibles, por ejemplo, en un mismo comando fdisk llamar a delete y add. La estructura de cada disco se explicará más adelante. Tendrá los siguientes parámetros:

| Parámetro | Categoría            | Descripción                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     |
|-----------|----------------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| -size     | Obligatorio al Crear | Este parámetro recibirá un número queindicará el tamaño de la partición a<br>crear. Debe ser positivo y mayor a cero,<br>de lo contrario se mostrará un mensaje<br>de error.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    |
| -unit     | Opcional             | Este parámetro recibirá una letra que indicará las unidades que utilizará el parámetro size. Podrá tener los siguientes valores:<br>**B**: indicará que se utilizarán bytes.<br>**K**: indicará que se utilizarán Kilobytes(1024 bytes)<br>**M**:indicará que se utilizarán Megabytes(1024 * 1024 bytes).<br>Este parámetro es opcional, si no se encuentra se creará una partición en Kilobytes. Si se utiliza un valor diferente mostrará un mensaje de error.                                                                                                                                                                                                                                                                                                                            |
| -path     | Obligatorio          | Este parámetro será la ruta en la que se encuentra el disco en el que se creará la partición. Este archivo ya debe existir, si no se mostrará un error.                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         |
| -type     | Opcional             | Indicará que tipo de partición se creará. Ya que es opcional, se tomará como primaria en caso de que no se indique. Podrá tener los siguientes valores:<br>**P**: en este caso se creará una partición primaria.<br>**E**: en este caso se creará una partición extendida.<br>**L**: Con este valor se creará una partición lógica.<br>* Las particiones lógicas sólo pueden estar dentro de la extendida sin sobrepasar su tamaño. Deberá tener en cuenta las restricciones de teoría de particiones:<br>* La suma de primarias y extendidas debe ser como máximo 4. Solo puede haber una partición extendida por disco.<br>* No se puede crear una partición lógica si no hay una extendida.<br>* Si se utiliza otro valor diferente a los anteriores deberá mostrar un mensaje de error. |
| -fit      | Opcional             | Indicará el ajuste que utilizará la partición para asignar espacio. Podrá tener los siguientes valores:<br>**BF**: Indicará el mejor ajuste (Best Fit)<br>**FF**: Utilizará el primer ajuste (First Fit)<br>**WF**: Utilizará el peor ajuste (Worst Fit)<br>Ya que es opcional, se tomará el peor ajuste si no está especificado en el comando. Si se utiliza otro valor que no sea alguno de los anteriores mostrará un mensaje de error.                                                                                                                                                                                                                                                                                                                                                  |
| -delete   | Opcional             | Este parámetro indica que se eliminará una partición. Este parámetro se utiliza junto con **-name** y **-path**. Se deberá mostrar un mensaje que permita confirmar la eliminación de dicha partición. Si la partición no existe deberá mostrar error. Si se elimina la partición extendida, deben eliminarse las particiones lógicas que tenga adentro. Recibirá los siguientes valores:<br>**Fast**: Esta opción marca como vacío el espacio en la tabla de particiones.<br>**Full**: Esta opción además marcar como vació el espacio en la tabla de particiones, rellena el espacio con el carácter \0. Si se utiliza otro valor diferente, mostrará un mensaje de error.                                                                                                                    |
| -name     | Obligatorio          | Indicará el nombre de la partición. El nombre no debe repetirse dentro de las particiones de cada disco. Si se va a eliminar, la partición ya debe existir, si no existe debe mostrar un mensaje de error                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       |
| -add      | Opcional             | Este parámetro se utilizará para agregar o quitar espacio de la partición. Puede ser positivo o negativo. Tomará el parámetro **-units** para las unidades a agregar o eliminar. En el caso de agregar espacio, deberá comprobar que exista espacio libre después de la partición. En el caso de quitar espacio se debe comprobar que quede espacio en la partición (no espacio negativo).

#### 4. MOUNT

Este comando montará una partición del disco en el sistema e imprimira en consola un resumen de todas las particiones montadas actualmente. Cada partición se identificará por un id que tendrá la siguiente estructura utilizando el número de carnet: 

*Últimos dos dígitos del Carnet + Numero + Letra

Ejemplo: carnet = 201536958Id ́s = 581A, 581B, 581C, 582A, 583A

Por cada partición montada, deberá imprimir una línea en consola con siguiente formato:
Path del disco utilizado|Nombre de la partición|ID de la partición montada.

El número será el misma para particiones en el mismo disco y la letra diferente para particiones en el mismo disco. (NOTA: Este Comando Debe Realizar el montaje en memoria ram no debe escribir esto en el disco) Los parámetros admitidos por este comando son:

| Parámetro | Categoría   | Descripción                                                                                                             |
|-----------|-------------|-------------------------------------------------------------------------------------------------------------------------|
| -path     | Obligatorio | Este parámetro será la ruta en la que se encuentra el disco que se montará en el sistema. Este archivo ya debe existir. |
| -name     | Obligatorio | Indica el nombre de la partición a cargar. Si no existe debe mostrar error.                                             |

#### 5. UMOUNT

Desmonta una partición del sistema. Se utilizará el id que se le asignó a la partición al momento de cargarla. Recibirá los siguientes parámetros:

| Parámetro | Categoría   | Descripción                                                                                     |
|-----------|-------------|-------------------------------------------------------------------------------------------------|
| -id       | Obligatorio | Especifica el id de la partición que se desmontará. Si no existe el id deberá mostrar un error. |

### 🎎 Administracion de Usuarios y Grupos

### 📁 Administracion de Carpetas, Archivos y Permisos

### 📜 Script

#### 1. EXEC

El programa podrá ejecutar scripts con el comando exec. Debe mostrar el contenido de la línea que está leyendo y su resultado. También debe mostrar los comentarios del script.

| Parámetro | Categoría   | Descripción                                           |
|-----------|-------------|-------------------------------------------------------|
| -path     | Obligatorio | Especifica el nombre del script que se va a ejecutar. |

### 📑 Reportes