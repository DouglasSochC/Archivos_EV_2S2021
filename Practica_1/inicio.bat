@echo off
mysql -u root -p
*enter in the password*
DROP DATABASE IF EXISTS practica1;
CREATE DATABASE practica1;
USE practica1;
EXIT
echo Exit from MySQL...