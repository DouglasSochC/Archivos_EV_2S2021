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

* Graphviz 2.43.0 o superior
```console
~$ dot -v
```

### ⚙️ Ejecucion 

Compilar el proyecto
```console
~$ g++ main.cpp "analizador/scanner.cpp" "analizador/parameters.cpp" "util/util_p.cpp" "src/adm_discos.cpp" "src/script_adm.cpp" "src/reportes.cpp" "src/adm_cap" "src/adm_ug" -o main
```

Ejecutar archivo compilado
```console
~$ ./main
```
## 📖 Documentacion

### 💿 Administracion de Discos

Estos comandos permitirán crear archivos que simularán discos duros en los que se podrá formatear más adelante con el sistema de archivos ext2 o ext3. Estos comandos estarán disponibles desde que se inicia el programa.

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

Este comando montará una partición del disco en el sistema. Cada partición se identificará por un id que tendrá la siguiente estructura: VD + LETRA + NUMERO.
Por ejemplo: vda1, vda2, vdb1, vdc1… La letra será la misma para particiones en el mismo disco y el número diferente para particiones en el mismo disco. Los parámetros admitidos por este comando son:

| Parámetro | Categoría   | Descripción                                                                                                             |
|-----------|-------------|-------------------------------------------------------------------------------------------------------------------------|
| -path     | Obligatorio | Este parámetro será la ruta en la que se encuentra el disco que se montará en el sistema. Este archivo ya debe existir. |
| -name     | Obligatorio | Indica el nombre de la partición a cargar. Si no existe debe mostrar error.                                             |

#### 5. UNMOUNT

Desmonta una partición del sistema. Se utilizará el id que se le asignó a la partición al momento de cargarla. Recibirá los siguientes parámetros:

| Parámetro | Categoría   | Descripción                                                                                     |
|-----------|-------------|-------------------------------------------------------------------------------------------------|
| -id       | Obligatorio | Especifica el id de la partición que se desmontará. Si no existe el id deberá mostrar un error. |

#### 6. MKFS

Este comando realiza un formateo completo de la partición, se formateará como ext2 por defecto si en caso el comando fs no está definido. También creará un archivo en la raíz llamado users.txt que tendrá los usuarios y contraseñas del sistema de archivos. La estructura de este archivo se explicará más adelante.

| Parámetro | Categoría   | Descripción                                                                                     |
|-----------|-------------|-------------------------------------------------------------------------------------------------|
| -id       | Obligatorio | Indicará el id que se generó con el comando mount de la fase anterior. Si no existe mostrará error. Se utilizará para saber la partición y el disco que se utilizará para hacer el sistema de archivos.|
| -type     | Opcional | Indicará que tipo de formateo se realizará. Ya que es opcional, se tomará como un formateo completo si no se especifica esta opción. Podrá tener los siguientes valores:<br>**Fast**: en este caso se realizará un formateo rápido.<br>**Full**: en este caso se realizará un formateo completo.<br> La diferencia entre estos dos tipos se explicará más adelante.|
| -fs       | Opcional | Indica el sistema de archivos a formatear ext2 / ext3. Por defecto será ext2. Y los valores serán. 2fs para ext2 o 3fs para ext3|

### 🎎 Administracion de Usuarios y Grupos

Estos comandos permitirán crear usuarios y grupos, asi como eliminarlos. Los permisos solo los tendrá el usuario root.

#### 1. LOGIN

Este comando se utiliza para iniciar sesión en el sistema. No se puede iniciar otra sesión sin haber hecho un logout antes, si no, debe mostrar un mensaje de error indicando que debe cerrar sesión antes. Recibirá los siguientes parámetros:

| Parámetro | Categoría   | Descripción                                                                                     |
|-----------|-------------|-------------------------------------------------------------------------------------------------|
| -usr       | Obligatorio | Especifica el nombre del usuario que iniciará sesión. Si no se encuentra mostrará un mensaje indicando que el usuario no existe. En este caso si distinguirá mayúsculas de minúsculas.|
| -pwd       | Obligatorio | Indicará la contraseña del usuario, si no coincide debe mostrar un mensaje de autenticación fallida. Distinguirá entre mayúsculas y minúsculas.|
| -id       | Obligatorio | Indicará el id de la partición montada de la cual van a iniciar sesión. De lograr iniciar sesión todas las acciones se realizarán sobre este id.|

#### 2. LOGOUT

Este comando se utiliza para cerrar sesión. Debe haber una sesión activa anteriormente para poder utilizarlo, si no, debe mostrar un mensaje de error. Este comando no recibe parámetros.

#### 3. MKGRP

Este comando creará un grupo para los usuarios de la partición y se guardará en el archivo users.txt de la partición, este comando solo lo puede utilizar el usuario root. Si otro usuario lo intenta ejecutar, deberá mostrar un mensaje de error, si el grupo a ingresar ya existe deberá mostrar un mensaje de error. Recibirá los siguientes parámetros:

| Parámetro | Categoría   | Descripción                                                                                     |
|-----------|-------------|-------------------------------------------------------------------------------------------------|
| -name       | Obligatorio | Indicará el nombre que tendrá el grupo |

#### 4. RMGRP

Este comando eliminará un grupo para los usuarios de la partición. Solo lo puede utilizar el usuario root, si lo utiliza alguien más debe mostrar un error. Recibirá los siguientes parámetros:

| Parámetro | Categoría   | Descripción                                                                                     |
|-----------|-------------|-------------------------------------------------------------------------------------------------|
| -name       | Obligatorio | Indicará el nombre del grupo a eliminar. Si el grupo no se encuentra dentro de la partición debe mostrar un error. |

#### 5. MKUSR

Este comando crea un usuario en la partición. Solo lo puede ejecutar el usuario root, si lo utiliza otro usuario deberá mostrar un error. Recibirá los siguientes parámetros:

| Parámetro | Categoría   | Descripción                                                                                     |
|-----------|-------------|-------------------------------------------------------------------------------------------------|
| -usr       | Obligatorio | Indicará el nombre del usuario a crear, si ya existe, deberá mostrar un error indicando que ya existe el usuario. Máximo: 10 caracteres.|
| -pwd       | Obligatorio | Indicará la contraseña del usuario. Máximo: 10 caracteres. |
| -grp       | Obligatorio | Indicará el grupo al que pertenecerá el usuario. Debe de existir en la partición en la que se está creando el usuario, si no debe mostrar un mensaje de error. Máximo: 10 caracteres.|

#### 6. RMUSR

Este comando elimina un usuario en la partición. Solo lo puede ejecutar el usuario root, si lo utiliza otro usuario deberá mostrar un error. Recibirá los siguientes parámetros:

| Parámetro | Categoría   | Descripción                                                                                     |
|-----------|-------------|-------------------------------------------------------------------------------------------------|
| -usr       | Obligatorio | Indicará el nombre del usuario a eliminar, si no existe, deberá mostrar un error indicando que el usuario no existe. |

### 📁 Administracion de Carpetas, Archivos y Permisos

Estos comandos permitirán crear archivos y carpetas, así como editarlos, copiarlos, moverlos y eliminarlos. Los permisos serán para el usuario propietario del archivo, para el grupo al que pertenece y para otros usuarios, así como en Linux.

#### 1. MKDIR

Este comando es similar a mkfile, pero no crea archivos, sino carpetas. El propietario será el usuario que actualmente ha iniciado sesión. Tendrá los permisos 664. El usuario deberá tener el permiso de escritura en la carpeta padre, si no debe mostrar un error. Tendrá los siguientes parámetros:

| Parámetro | Categoría   | Descripción                                                                                     |
|-----------|-------------|-------------------------------------------------------------------------------------------------|
| -id       | Obligatorio | Especifica el id de la partición. Se utilizará para saber la partición y el disco que se utilizará para hacer crear la carpeta.|
| -path       | Obligatorio | Este parámetro será la ruta de la carpeta que se creará. Si lleva espacios en blanco deberá encerrarse entre comillas.<br>Si no existen las carpetas padres, debe mostrar error, a menos que se utilice el parámetro –p, que se explica posteriormente.|
| -p       | Obligatorio | Si se utiliza este parámetro y las carpetas padres en el parametro **path** no existen, entonces deben crearse. <br>Si ya existen, no realizara nada. No recibirá ningún valor, si lo recibe debe mostrar error.|

### 📜 Script

#### 1. EXEC

El programa podrá ejecutar scripts con el comando exec. Debe mostrar el contenido de la línea que está leyendo y su resultado. También debe mostrar los comentarios del script.

| Parámetro | Categoría   | Descripción                                           |
|-----------|-------------|-------------------------------------------------------|
| -path     | Obligatorio | Especifica el nombre del script que se va a ejecutar. |

### 📑 Reportes

#### 1. REP

Recibirá el nombre del reporte que se desea y lo generará con graphviz en una carpeta existente. El uso de este comando no depende de una sesion activa.

| Parámetro | Categoría   | Descripción                                                                                                                                                                                                                   |
|-----------|-------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| -name     | Obligatorio | Nombre del reporte a generar. Tendrá los siguientes valores:<br>MBR<br>DISK<br>SB<br>JOURNALING<br>BLOCK<br>BM_INODE<br>BM_BLOCK<br>TREE<br>INODE<br>FILE<br>LS<br>Si recibe otro valor que no sea alguno de los anteriores, debe mostrar un error.                        |
| -path     | Obligatorio | Si recibe otro valor que no sea alguno de los anteriores, debe mostrar un error. Indica una carpeta y el nombre que tendrá el reporte. Si no existe la carpeta, deberá crearla. Si lleva espacios se encerrará entre comillas |
| -id       | Obligatorio | Indica el id de la partición que se utilizará. Si el reporte es sobre la información del disco, se utilizará el disco al que pertenece la partición. Si no existe debe mostrar un error.                                      |

##### 1.1 Reporte MBR
Mostrará tablas con toda la información del MBR, así como de los EBR que se pudieron haber creado.

##### 1.2 Reporte DISK
Este reporte mostrará la estructura de las particiones, el mbr del disco y el porcentaje que cada partición o espacio libre tiene dentro del disco (La sumatoria de los porcentajes debe de ser 100%).

##### 1.3 Reporte SB
Muestra toda la información del superbloque en una tabla.

##### 1.4 Reporte JOURNALING
Muestra toda la información del superbloque en una tabla.

##### 1.5 Reporte BM_INODE
Este reporte mostrará la información del bit map de inodos, mostará todos los bits, libres o utilizados. Este reporte se generara en un archivo de texto mostrando 20 registros por línea.

##### 1.6 Reporte BM_BLOCK
Este reporte mostrará la información del bit map de bloques, mostará todos los bits, libres o utilizados. Este reporte se generara en un archivo de texto que mostrara 20 registros por línea.