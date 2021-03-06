# Integracion de Informacion

_Este es una practica universitaria del curso de Manejo e Implementacion de Archivos en donde la idea principal es la integracion de informacion en una base de datos que a sido almacenada en documentos con extension .csv y .tsv_

## 馃殌 Comenzando

El proyecto fue desarrollado en una distribucion GNU/Linux

### 馃搵 Requerimientos

* Ubuntu 20.04.3 LTS
```console
~$ lsb_release -a
```

* MySQL 8.0.27
```console
~$ mysql -V
```

### 鈿欙笍 Pre - Ejecucion 

#### Implementacion del rol Administrador y Empleado

Loguearse (desde consola) al usuario creado cuando se realizo la instalacion de MySQL
```console
~$ mysql -u *usuario* -p
```

Crear un usuario administrador

```console
mysql> CREATE USER 'administrador'@'localhost' IDENTIFIED BY 'Admin/*123';
```
```console
mysql> GRANT ALL PRIVILEGES ON * . * TO 'administrador'@'localhost';
```

Crear un usuario empleado

```console
mysql> CREATE USER 'empleado'@'localhost' IDENTIFIED BY 'Emplea/*123';
```
```console
mysql> GRANT SELECT ON *.* TO 'empleado'@'localhost';
```

Para que los cambios se vean inmediatamente

```console
mysql> FLUSH PRIVILEGES;
```

#### Habilitar local_infile de MySQL

Comprobar que el local_infile este habilitado
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
### 鈿欙笍 Ejecucion 
Ejecutar el script
```console
mysql> sudo sh execute.sh
```

## 馃摉 Documentacion

### 馃搰 Archivos a Almacenar

#### IDA maneja distintos archivos para almacenar su informaci贸n de forma separada en los diferentes departamentos de la asociaci贸n:

- geonamess.csv
Contiene datos geolocacionales seg煤n c贸digo geoname.
- projects.csv
Contienen toda la informaci贸n relevante a los proyectos que la asociaci贸n tiene y ha tenido.
- level_1a.csv
Contiene informaci贸n acerca de las rondas de financiamientos que se les han dado a los diferentes proyectos por lugar.

#### IBRD maneja una peque帽a base de datos para almacenar la informaci贸n relevante a las transacciones de dinero:

- transactions.csv
Contiene informacion acerca de todas las transaciones que se han realizado en los proyectos

#### Adem谩s, se manejan est谩ndares ISO para los c贸digos de pa铆ses y nombres geogr谩ficos:

- country_codes.tsv:
Contiene informaci贸n estandarizada sobre pa铆ses y sus c贸digos.
- locations.csv:
Contiene informaci贸n sobre los tipos de lugares.

### 鉁夛笍 Ejecutable

Un archivo .sh realiza lo siguiente:
1. Se loguea como usuario administrador y ejecuta un script.sql, el cual se explicara posteriormente su contenido
2. Se loguea como usuario empleado para poder realizar las consultas.

### 馃摐 Script.sql
Este archivo realiza lo siguiente:

鈥? Script de eliminaci贸n de tablas.

鈥? Script de creaci贸n de base de datos.

鈥? Script de tablas temporales.

鈥? 5 archivos de control.

鈥? Script de llenado de base de datos.


