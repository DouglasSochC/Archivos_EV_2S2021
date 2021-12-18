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

### ⚙️ Pre - Ejecucion 

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
### ⚙️ Ejecucion 
Ejecutar el script
```console
mysql> sudo sh execute.sh
```

## 📖 Documentacion

### 📇 Archivos a Almacenar

#### IDA maneja distintos archivos para almacenar su información de forma separada en los diferentes departamentos de la asociación:

- geonamess.csv
Contiene datos geolocacionales según código geoname.
- projects.csv
Contienen toda la información relevante a los proyectos que la asociación tiene y ha tenido.
- level_1a.csv
Contiene información acerca de las rondas de financiamientos que se les han dado a los diferentes proyectos por lugar.

#### IBRD maneja una pequeña base de datos para almacenar la información relevante a las transacciones de dinero:

- transactions.csv
Contiene informacion acerca de todas las transaciones que se han realizado en los proyectos

#### Además, se manejan estándares ISO para los códigos de países y nombres geográficos:

- country_codes.tsv:
Contiene información estandarizada sobre países y sus códigos.
- locations.csv:
Contiene información sobre los tipos de lugares.

### ✉️ Ejecutable

Un archivo .sh realiza lo siguiente:
1. Se loguea como usuario administrador y ejecuta un script.sql, el cual se explicara posteriormente su contenido
2. Se loguea como usuario empleado para poder realizar las consultas.

### 📜 Script.sql
Este archivo realiza lo siguiente:

• Script de eliminación de tablas.

• Script de creación de base de datos.

• Script de tablas temporales.

• 5 archivos de control.

• Script de llenado de base de datos.


