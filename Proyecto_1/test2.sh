#Contenido de Calificacion
#Douglas Soch - 201807032
#exec -path~:~/home/douglas/Documentos/ARCHIVOS/Proyecto_1/test2.sh

#ELIMINACION DE DISCO
rmdisk -path~:~/tmp/douglas/Disco1.disk #Se elimina el disco 1
rmdisk -path~:~/tmp/douglas/Disco2.disk #Se elimina el disco 2
rmdisk -path~:~/tmp/douglas/Disco3.disk #Se elimina el disco 3

#CREACION DE DISCO
Mkdisk    -Size~:~50 -fit~:~FF  -path~:~/tmp/douglas/Disco1.disk  #Se crea nuevo disco 1
Mkdisk    -Size~:~5 -fit~:~FF  -path~:~/tmp/douglas/Disco2.disk   #Se crea nuevo disco 2
Mkdisk    -Size~:~3 -fit~:~FF  -path~:~/tmp/douglas/Disco3.disk   #Se crea nuevo disco 3

#CREACION DE PARTICIONES DISCO 1
fdisk -type~:~P -unit~:~M -name~:~Part1 -size~:~10 -path~:~/tmp/douglas/Disco1.disk -fit~:~WF #Crea particion de 10MB
fdisk -type~:~E -unit~:~M -name~:~Part2 -size~:~20 -path~:~/tmp/douglas/Disco1.disk -fit~:~BF #Crea particion de 20MB
fdisk -type~:~P -unit~:~M -name~:~Part3 -size~:~12 -path~:~/tmp/douglas/Disco1.disk -fit~:~FF #Crea particion de 12MB
fdisk -type~:~P -unit~:~B -name~:~Part4 -size~:~800 -path~:~/tmp/douglas/Disco1.disk -fit~:~BF #Crea particion de 8388472B

fdisk -type~:~L -unit~:~M -name~:~Part5 -size~:~8 -path~:~/tmp/douglas/Disco1.disk -fit~:~WF #Crea particion nombre 5
fdisk -type~:~L -unit~:~M -name~:~Part6 -size~:~5 -path~:~/tmp/douglas/Disco1.disk -fit~:~FF #Crea particion nombre 6
fdisk -type~:~L -unit~:~M -name~:~Part7 -size~:~3 -path~:~/tmp/douglas/Disco1.disk -fit~:~FF #Crea particion nombre 7
fdisk -type~:~L -unit~:~M -name~:~Part8 -size~:~7 -path~:~/tmp/douglas/Disco1.disk -fit~:~BF #Error porque no hay mas espacio para ingresar; lo que queda son 4MB
fdisk -type~:~L -unit~:~M -name~:~Part8 -size~:~4 -path~:~/tmp/douglas/Disco1.disk -fit~:~BF #Crea particion nombre 8

# CREACION DE PARTICIONES DISCO 2
fdisk -type~:~P -unit~:~M -name~:~Part1 -size~:~1 -path~:~/tmp/douglas/Disco2.disk -fit~:~WF #Crea particion de 1MB
fdisk -type~:~E -unit~:~M -name~:~Part2 -size~:~2 -path~:~/tmp/douglas/Disco2.disk -fit~:~BF #Crea particion de 2MB
fdisk -type~:~P -unit~:~M -name~:~Part3 -size~:~1 -path~:~/tmp/douglas/Disco2.disk -fit~:~FF #Crea particion de 1MB

# CREACION DE PARTICIONES DISCO 3
fdisk -type~:~P -unit~:~M -name~:~Part1 -size~:~1 -path~:~/tmp/douglas/Disco3.disk -fit~:~WF #Crea particion de 1MB
fdisk -type~:~E -unit~:~M -name~:~Part2 -size~:~1 -path~:~/tmp/douglas/Disco3.disk -fit~:~BF #Crea particion de 2MB
fdisk -type~:~P -unit~:~K -name~:~Part3 -size~:~500 -path~:~/tmp/douglas/Disco3.disk -fit~:~FF #Crea particion de 1MB

#ELIMINACION DE PARTICIONES
#fdisk -delete~:~fast -name~:~Part5 -path~:~/tmp/douglas/Disco1.disk -f~:~BF -size~:~1
#fdisk -delete~:~full -name~:~Part6 -path~:~/tmp/douglas/Disco1.disk -f~:~BF -size~:~1
#fdisk -delete~:~full -name~:~Part7 -path~:~/tmp/douglas/Disco1.disk -f~:~BF -size~:~1
#fdisk -delete~:~full -name~:~Part8 -path~:~/tmp/douglas/Disco1.disk -f~:~BF -size~:~1

#AUMENTANDO Y DISMINUYENDO TAMAÑO DE PARTICIONES
#fdisk -add~:~-500 -name~:~Part3 -path~:~/tmp/douglas/Disco1.disk -size~:~1 #Restando 500B
#fdisk -add~:~500 -name~:~Part3 -path~:~/tmp/douglas/Disco1.disk -size~:~1 #Agregando 500B
#fdisk -add~:~500 -name~:~Part1 -path~:~/tmp/douglas/Disco1.disk -size~:~1 #Error no hay espacio para aumentar
#fdisk -add~:~500 -name~:~Part8 -path~:~/tmp/douglas/Disco1.disk -size~:~1 #Error no hay espacio para aumentar

#MONTANDO LOS DISCOS EXISTENTES DISCO 1
mount -path~:~/tmp/douglas/Disco1.disk -name~:~Part1 #vda1
mount -path~:~/tmp/douglas/Disco1.disk -name~:~Part2 #Error no se puede montar una particion extendida
mount -path~:~/tmp/douglas/Disco1.disk -name~:~Part3 #vda2
mount -path~:~/tmp/douglas/Disco1.disk -name~:~Part4 #vda3
mount -path~:~/tmp/douglas/Disco1.disk -name~:~Part5 #vda4
mount -path~:~/tmp/douglas/Disco1.disk -name~:~Part6 #vda5
mount -path~:~/tmp/douglas/Disco1.disk -name~:~Part7 #vda6

#MONTANDO LOS DISCOS EXISTENTES DISCO 2
mount -path~:~/tmp/douglas/Disco2.disk -name~:~Part1 #vdb1
mount -path~:~/tmp/douglas/Disco2.disk -name~:~Part2 #Error no se puede montar una particion extendida
mount -path~:~/tmp/douglas/Disco2.disk -name~:~Part3 #vdb2

#MONTANDO LOS DISCOS EXISTENTES DISCO 3
mount -path~:~/tmp/douglas/Disco3.disk -name~:~Part1 #vdc1
mount -path~:~/tmp/douglas/Disco3.disk -name~:~Part2 #Error no se puede montar una particion extendida
mount -path~:~/tmp/douglas/Disco3.disk -name~:~Part3 #vdc2
mount -path~:~/tmp/douglas/Disco3.disk -name~:~Part3 #Error ya esta montado

#DESMONTANDO ALGUNOS ID
unmount -id~:~321B #Error no existe
#Error no es valido el parametro -iid
unmount -iid~:~321B
unmount -id~:~vda3 #Desmonta

#COMPROBACION DE ERRORES
rmdisk -path~:~/tmp/douglas/Disco1.disk #No se puede eliminar el disco ya que esta montado
#Error no existe el parametro -f
fdisk -delete~:~fast -name~:~Part5 -path~:~/tmp/douglas/Disco1.disk -f~:~BF -size~:~1

#GENERACION DE REPORTE MBR Y DISK
rep -id~:~vda1 -path~:~/tmp/douglas/reportes/MBR_A1.jpg -name~:~mbr
rep -id~:~vdb1 -path~:~/tmp/douglas/reportes/MBR_B1.jpg -name~:~mbr
rep -id~:~vda2 -path~:~/tmp/douglas/reportes/DISK_A2.jpg -name~:~disk
rep -id~:~vdb1 -path~:~/tmp/douglas/reportes/DISK_B1.jpg -name~:~disk
rep -id~:~vdB1 -path~:~/tmp/douglas/reportes/MBR_B1.jpg -name~:~mbr #Error no existe el id solicitado
rep -id~:~vda1 -path~:~/tmp/douglas/reportes/SB_A1.jpg -name~:~sb #Error debido a que no esta formateado 1
rep -id~:~vdb1 -path~:~/tmp/douglas/reportes/SB_B1.jpg -name~:~sb #Error debido a que no esta formateado 2

#FORMATEO DE PARTICION
mkfs -type~:~fast -id~:~vdb1 #Realiza un formateo completo de la particion que ocupa el id vdb1
mkfs -fs~:~3fs  -id~:~vda1 #Realiza un formateo rápido de la partición en el id vda1 en ext2
rep -id~:~vda1 -path~:~/tmp/douglas/reportes/SB_A1.jpg -name~:~sb
rep -id~:~vdb1 -path~:~/tmp/douglas/reportes/SB_B1.jpg -name~:~sb
rep -id~:~vda1 -path~:~/tmp/douglas/reportes/JOURNALING_A1.jpg -name~:~journaling #Hay un reporte
rep -id~:~vdb1 -path~:~/tmp/douglas/reportes/JOURNALING_B1.jpg -name~:~journaling #Es un sistema EXT2, no hay journaling
rep -id~:~vda1 -path~:~/tmp/douglas/reportes/BMINODE_A1.plain -name~:~bm_inode
rep -id~:~vdb1 -path~:~/tmp/douglas/reportes/BMINODE_B2.plain -name~:~bm_inode
rep -id~:~vda1 -path~:~/tmp/douglas/reportes/BMBLOCK_A1.plain -name~:~bm_block
rep -id~:~vdb1 -path~:~/tmp/douglas/reportes/BMBLOCK_B1.plain -name~:~bm_block

#LOG IN Y LOG OUT
logout #Error no hay una sesion activa
login -usr~:~root -pwd~:~123 -id~:~vdb1 #Logueo Exitoso
login -usr~:~"mi usuario" -pwd~:~"mi pwd" -id~:~vdb2 #Error - Ya hay una sesion activa
login -usr~:~root -pwd~:~123 -id~:~vda1 #Error - Ya hay una sesion activa
logout #Sesion cerrada correctamente
login -usr~:~root -pwd~:~1234 -id~:~vda1 #Contraseña incorrecta
login -usr~:~root -pwd~:~123 -id~:~vda1 #Logueo exitoso del root

#CREACION DE GRUPOS
mkgrp -name~:~"root" #Error ya existe grupo:root
mkgrp -name~:~"grupo11111" #Crea el grupo:grupo11111
mkgrp -name~:~"grupo11111" #Error ya existe grupo:grupo11111
mkgrp -name~:~"grupo22222" #Crea el grupo:grupo22222
mkgrp -name~:~"grupo33333" #Crea el grupo:grupo33333
mkgrp -name~:~"grupo44444" #Crea el grupo:grupo44444
mkgrp -name~:~"grupo22222" #Debe mostrar mensaje de error ya que el grupo ya existe

#ELIMINACION DE GRUPOS
rmgrp -name~:~grupo11111 #Elimina el grupo de usuarios en la partición de la sesión actual
rmgrp -name~:~grupo11111 #Error ya que el grupo no existe porque ya fue eliminado
mkgrp -name~:~"grupo11111" #Crea el grupo: grupo11111
rmgrp -name~:~grupo11111 #Elimina el grupo de usuarios en la partición de la sesión actual
rmgrp -name~:~grupo11111 #Error ya que el grupo no existe porque ya fue eliminado
mkgrp -name~:~usuarios #Crea el grupo:usuarios

#CREANDO USUARIOS
Mkusr -usr~:~user1 -grp~:~usuarios -pwd~:~contra1 #Crea al usuario:user1
Mkusr -usr~:~user2 -grp~:~usuarios -pwd~:~contra2 #Crea al usuario:user2
Mkusr -usr~:~admin -grp~:~root -pwd~:~admin #Crea al usuario:user2
Mkusr -usr~:~user2 -grp~:~usuarios -pwd~:~usuari1 #Error, ya existe el user2
Mkusr -usr~:~user1 -grp~:~usuarios -pwd~:~usuario #Error, ya existe el user3

#ELIMINANDO USUARIO
rmusr -usr~:~user1 #Se elimina el usuario: user1
rmusr -usr~:~user2 #Se elimina el usuario: user2
rmusr -usr~:~root #Error, no se puede eliminar el usuario root
Mkusr -usr~:~user1 -grp~:~usuarios -pwd~:~contra1 #Se crea otra vez el usuario:user1
rmusr -usr~:~user1 #Se elimina el usuario:user1
rmusr -usr~:~user1 #Error, no existe este usuario
rmusr -usr~:~user2 #Error, no existe este usuario

#LOG OUT
logout #Sesion cerrada correctamente
#LOG IN
login -usr~:~root -pwd~:~123 -id~:~vda1 #Logueo exitoso del root

#CREANDO OTRAS CARPETAS
Mkdir -path~:~/home/user/docs//usac1 #Error - problema de ruta
Mkdir -P~:~a -path~:~/home/user/docs/usac #Error ya que el parametro -p no debe llevar asignacion
Mkdir -path~:~/home/user/docs.txt/usac #Error ya que busca dentro de un archivo
Mkdir -path~:~/home/user/docs/usac.txt/hola.txt #Error existen dos archivos
Mkdir -path~:~/home/user/docs/1234567890123 #Error excede los 12
Mkdir -P=a -path~:~/home/user/docs/usac #Error al parametro -P=a
Mkdir -path~:~/home/user/docs/usac.txt #Error ya que solo esta creando carpetas
Mkdir -p -path~:~/home/user/docs/12345678901 #Lo ejecuta
Mkdir -P -path~:~/home/user/docs/usac #Lo ejecuta
Mkdir -path~:~/home/user/docs/usac1 #Lo ejecuta
mkdir -path~:~/drive #Lo crea:drive
mkdir -path~:~/carpeta1 #Lo crea:carpeta1
mkdir -path~:~/carpeta2 #Lo crea:carpeta2
mkdir -path~:~/carpeta3 #Lo crea:carpeta3
mkdir -path~:~/carpeta4 #Lo crea:carpeta4
mkdir -path~:~/carpeta5 #Lo crea:carpeta5
mkdir -path~:~/carpeta6 #Lo crea:carpeta6
mkdir -path~:~/carpeta7 #Lo crea:carpeta7
mkdir -path~:~/carpeta8 #Lo crea:carpeta8
mkdir -path~:~/carpeta9 #Lo crea:carpeta9
mkdir -path~:~/carpeta10 #Lo crea:carpeta10
mkdir -path~:~/carpeta11 #Lo crea:carpeta11
mkdir -path~:~/carpeta12 #Lo crea:carpeta12
mkdir -path~:~/carpeta13 #Lo crea:carpeta13
mkdir -path~:~/carpeta14 #Lo crea:carpeta14
mkdir -path~:~/carpeta15 #Lo crea:carpeta15
mkdir -path~:~/carpeta16 #Lo crea:carpeta16
mkdir -path~:~/carpeta17 #Lo crea:carpeta17
mkdir -path~:~/carpeta18 #Lo crea:carpeta18
mkdir -path~:~/carpeta19 #Lo crea:carpeta19
mkdir -path~:~/carpeta20 #Lo crea:carpeta20
mkdir -path~:~/carpeta21 #Lo crea:carpeta21
mkdir -path~:~/carpeta22 #Lo crea:carpeta22
mkdir -path~:~/carpeta23 #Lo crea:carpeta23
mkdir -path~:~/carpeta24 #Lo crea:carpeta24
mkdir -path~:~/carpeta25 #Lo crea:carpeta25
mkdir -path~:~/carpeta26 #Lo crea:carpeta26
mkdir -path~:~/carpeta27 #Lo crea:carpeta27
mkdir -path~:~/carpeta28 #Lo crea:carpeta28
mkdir -path~:~/carpeta29 #Lo crea:carpeta29
mkdir -path~:~/carpeta30 #Lo crea:carpeta30
mkdir -path~:~/carpeta31 #Lo crea:carpeta31
mkdir -path~:~/carpeta32 #Lo crea:carpeta32
mkdir -path~:~/carpeta33 #Lo crea:carpeta33
mkdir -path~:~/carpeta34 #Lo crea:carpeta34
mkdir -path~:~/carpeta35 #Lo crea:carpeta35
mkdir -path~:~/carpeta36 #Lo crea:carpeta36
mkdir -path~:~/carpeta37 #Lo crea:carpeta37
mkdir -path~:~/carpeta38 #Lo crea:carpeta38
mkdir -path~:~/carpeta39 #Lo crea:carpeta39
mkdir -path~:~/carpeta40 #Lo crea:carpeta40
mkdir -path~:~/carpeta41 #Lo crea:carpeta41
mkdir -path~:~/carpeta42 #Lo crea:carpeta42
mkdir -path~:~/carpeta43 #Lo crea:carpeta43
mkdir -path~:~/carpeta44 #Lo crea:carpeta44
mkdir -path~:~/carpeta45 #Lo crea:carpeta45
mkdir -path~:~/carpeta46 #Lo crea:carpeta46
mkdir -path~:~/carpeta47 #Lo crea:carpeta47
mkdir -path~:~/carpeta48 #Lo crea:carpeta48
mkdir -path~:~/carpeta49 #Lo crea:carpeta49
mkdir -path~:~/carpeta50 #Lo crea:carpeta50
mkdir -path~:~/temp1/temp1 #Error - No existe la carpeta temp1 para crear la temp2
mkdir -p -path~:~/temp1/temp2 #Lo crea
mkdir -p -path~:~/temp/temp1/temp1 #Lo crea
mkdir -p -path~:~/temp/temp1/temp1 #Error - Ya existe la carpeta temp1
mkdir -p -path~:~/temp/temp1/temp3 #Lo crea
mkdir -p -path~:~/temp/temp1/temp4 #Lo crea
mkdir -p -path~:~/temp/temp1/temp1 #Error -Ya existe la carpeta temp1

#CREANDO USUARIOS NUEVOS
Mkusr -usr~:~temporal1 -grp~:~usuarios -pwd~:~dato_nuevo #Crea al usuario:temporal1
Mkusr -usr~:~temporal2 -grp~:~usuarios -pwd~:~dato_nuev1 #Crea al usuario:temporal2
Mkusr -usr~:~temporal3 -grp~:~usuarios -pwd~:~dato_nuev2 #Crea al usuario:temporal3
rep -id~:~vda1 -path~:~/tmp/douglas/reportes/TREE_I.svg -name~:~tree #Este es el reporte inicial

#MODIFICANDO NOMBRE DE CARPETAS
ren -PatH~:~/temp/temp/temp/temp -name~:~temp2 #Error no existe el path ingresado
ren -PatH~:~/temp/temp1/temp2 -name~:~temp3 #Error ya que la carpeta temp3 ya existe
ren -PatH~:~/temp1/hola.txt -name~:~temp3 #Error no se puede modificar el nombre debido a que no son el mismo tipo
ren -PatH~:~/temp1/temp2 -name~:~hola.txt #Error no se puede modificar el nombre debido a que no son el mismo tipo
ren -PatH~:~/temp1/temp2 -name~:~douglas123 #Cambia el nombre de la carpeta temp2 a temp3
rep -id~:~vda1 -path~:~/tmp/douglas/reportes/TREE_U1.svg -name~:~tree #Se muestra el cambio de nombre debido al REN

#LEER ARCHIVOS
Cat #Error porque no esta completo
Cat -filea1~:~/home/user/docs/a.txt #Error porque no existe -filea
Cat -file2~:~/home/user/docs/a.txt #Error porque no esta en orden
Cat -file1~:~/home/user/docs/a.txt #Error no existe el documento a.txt

#CREAR ARCHIVO
mkfile -path~:~/home/user/docs/b.txt -size~:~-123 #Error - El size debe ser >0
mkfile -path~:~"/home/user/docs/eje 1.txt" -cont~:~/home/douglas/Documentos/texto.txt #Crea el archivo con datos
mkfile -p -path~:~/home/user/docs1/libre.txt -size~:~15939 #Crea bloques vacios
Cat -file1~:~"/home/user/docs/eje 1.txt" #Lee el archivo 'eje 1.txt'
rep -id~:~vda1 -path~:~/tmp/douglas/reportes/TREE_U2.svg -name~:~tree #Este muestra los nuevos archivos debido al MKFILE

#MODIFICANDO NOMBRE DE ARCHIVOS
ren -PatH~:~"/home/user/docs/eje 1.txt" -name~:~prueba.txt #Cambia el nombre del archivo 'eje 1.txt' a 'prueba.txt'
rep -id~:~vda1 -path~:~/tmp/douglas/reportes/TREE_U3.svg -name~:~tree #Aqui debe de verse el cambio de nombre del archivo 'eje 1.txt' a 'ejemplo.txt'

#REPORTES FINALES
rep -id~:~vda1 -path~:~/tmp/douglas/reportes/JOURNALING_GRUPOS.jpg -name~:~journaling #Exito
rep -id~:~vda1 -path~:~/tmp/douglas/reportes/BMINODE_GRUPOS.plain -name~:~bm_inode #Exito
rep -id~:~vda1 -path~:~/tmp/douglas/reportes/BMBLOCK_GRUPOS.plain -name~:~bm_block #Exito
rep -id~:~vda1 -path~:~/tmp/douglas/reportes/SB_GRUPOS.jpg -name~:~sb #Exito
rep -id~:~vda1 -path~:~/tmp/douglas/reportes/TEXTO.plain -name~:~file -ruta~:~"/home/user/docs/prueba.txt" #Exito
