echo Carnet: 201807032
echo Nombre: Douglas Alexander Soch Catalan
echo Tema: Practica 1
#Ejecutar script como administrador
mysql --local_infile=1 -u administrador -p <  script.sql
#Loguearse como empleado para las consultas
mysql -u empleado -p