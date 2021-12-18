# Integracion de Informacion

_Este es una practica universitaria del curso de Manejo e Implementacion de Archivos en donde la idea principal es la integracion de informacion en una base de datos que a sido almacenada en documentos con extension .csv y .tsv_

## 🚀 Comenzando

El proyecto fue desarrollado en una distribucion GNU/Linux

### 📋 Requerimientos

* Ubuntu 20.04.3 LTS
```console
~$ lsb_release -a
```

* MySQL 8.0.27
```console
~$ mysql -V
```

### ⚙️ Ejecucion 

Loguearse al usuario creado en MySQL desde consola
```console
~$ mysql -u *usuario* -p
```

Comprobar que el **local_infile** este habilitado
```console
mysql> show global variables like 'local_infile';
```

*En el caso que este deshabilitado se hace lo siguiente*
```console
mysql> set global local_infile=true;
```
```console
mysql> exit
```
```console
mysql> mysql --local_infile=1 -u *usuario* -p
```

Ejecutar el script
```console
mysql> source *path_del_script.sql*
```

## 📖 Documentacion

### 📄 Archivos a Almacenar

IDA maneja distintos archivos para almacenar su información de forma separada en los diferentes departamentos de la asociación:

#### • geonamess.csv
Contiene datos geolocacionales según código geoname.
#### • projects.csv
Contienen toda la información relevante a los proyectos que la asociación tiene y ha tenido.
#### • level_1a.csv
Contiene información acerca de las rondas de financiamientos que se les han dado a los diferentes proyectos por lugar.

