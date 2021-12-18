-- ************************************************************************************************** --
-- ****************************************** CONSULTA 1 ******************************************** --
-- *************************** ¿Cuántos proyectos iniciaron en el 2011? ***************************** --
-- ************************************************************************************************** --

SELECT COUNT(practica1.PROJECT.id) AS proyectos
FROM practica1.PROJECT
WHERE YEAR(practica1.PROJECT.start_actual_isodate) = 2011;

-- ************************************************************************************************** --
-- ****************************************** CONSULTA 2 ******************************************** --
-- *************************** ¿Cuál fue el proyecto de mayor duración? ***************************** --
-- ************************************************************************************************** --

SELECT 
DATEDIFF(practica1.PROJECT.end_actual_isodate, practica1.PROJECT.start_actual_isodate) AS duracion_dias,
DATEDIFF(practica1.PROJECT.end_actual_isodate, practica1.PROJECT.start_actual_isodate)/365 AS duracion_anios,
PROJECT.project_title AS nombre_proyecto
FROM practica1.PROJECT
ORDER BY duracion_dias DESC LIMIT 1;

-- ************************************************************************************************** --
-- ****************************************** CONSULTA 3 ******************************************** --
-- *************************** ¿Cuál fue el proyecto de menor duración? ***************************** --
-- ************************************************************************************************** --

SELECT 
DATEDIFF(practica1.PROJECT.end_actual_isodate, practica1.PROJECT.start_actual_isodate) AS duracion_dias,
DATEDIFF(practica1.PROJECT.end_actual_isodate, practica1.PROJECT.start_actual_isodate)/365 AS duracion_anios,
PROJECT.project_title AS nombre_proyecto
FROM practica1.PROJECT
WHERE practica1.PROJECT.end_actual_isodate IS NOT NULL
ORDER BY duracion_dias ASC LIMIT 1;

-- ************************************************************************************************** --
-- ****************************************** CONSULTA 4 ******************************************** --
-- *************************** Top 5 de países con más proyectos asignados ************************** --
-- ************************************************************************************************** --

SELECT
practica1.COUNTRY_CODES.name AS pais,
COUNT(practica1.COUNTRY_CODES.id) AS cantidad_proyectos
FROM practica1.COUNTRY_CODES
INNER JOIN practica1.PROJECT ON practica1.COUNTRY_CODES.id = practica1.PROJECT.id_country_codes
GROUP BY practica1.COUNTRY_CODES.name
ORDER BY cantidad_proyectos DESC
LIMIT 5;

-- ************************************************************************************************** --
-- ****************************************** CONSULTA 5 ******************************************** --
-- ************************** Top 5 de países con menos proyectos asignados ************************* --
-- ************************************************************************************************** --

SELECT
practica1.COUNTRY_CODES.name AS pais,
COUNT(practica1.COUNTRY_CODES.id) AS cantidad_proyectos
FROM practica1.COUNTRY_CODES
INNER JOIN practica1.PROJECT ON practica1.COUNTRY_CODES.id = practica1.PROJECT.id_country_codes
GROUP BY practica1.COUNTRY_CODES.name
ORDER BY cantidad_proyectos ASC
LIMIT 5;

-- ************************************************************************************************** --
-- ****************************************** CONSULTA 6 ******************************************** --
-- ***************************** ¿Cuál fue el proyecto con más costo? ******************************* --
-- ************************************************************************************************** --

SELECT
practica1.PROJECT.id AS identificador,
practica1.PROJECT.project_title AS proyecto,
MAX(practica1.PROJECT.total_commitments) AS costo
FROM practica1.PROJECT
GROUP BY practica1.PROJECT.id
ORDER BY costo DESC
LIMIT 1;

-- ************************************************************************************************** --
-- ****************************************** CONSULTA 7 ******************************************** --
-- ***************************** ¿Cuál fue el proyecto menos costoso? ******************************* --
-- ************************************************************************************************** --

SELECT
practica1.PROJECT.id AS identificador,
practica1.PROJECT.project_title AS proyecto,
MIN(practica1.PROJECT.total_commitments) AS costo
FROM practica1.PROJECT
GROUP BY practica1.PROJECT.id
ORDER BY costo ASC
LIMIT 1;

-- ************************************************************************************************** --
-- ****************************************** CONSULTA 8 ******************************************** --
-- ******************************* Top 5 de proyectos más costosos ********************************** --
-- ************************************************************************************************** --

SELECT
practica1.PROJECT.id AS identificador,
practica1.PROJECT.project_title AS proyecto,
MAX(practica1.PROJECT.total_commitments) AS costo
FROM practica1.PROJECT
GROUP BY practica1.PROJECT.id
ORDER BY costo DESC
LIMIT 5;

-- ************************************************************************************************** --
-- ****************************************** CONSULTA 9 ******************************************** --
-- ****************************** Costo Total de Todos los Proyectos ******************************** --
-- ************************************************************************************************** --

SELECT
SUM(practica1.PROJECT.total_commitments) AS costo_total
FROM practica1.PROJECT;

-- ************************************************************************************************** --
-- ****************************************** CONSULTA 10 ******************************************* --
-- Top 3 de las Transacciones más Costosas RI-East Africa Public Health Laboratory Networking Project --
-- ************************************************************************************************** --

SELECT
practica1.TRANSACTION.id AS id_transaccion,
practica1.TRANSACTION.transaction_value AS cantidad
FROM practica1.PROJECT
INNER JOIN practica1.TRANSACTION ON practica1.TRANSACTION.id_project = practica1.PROJECT.id
WHERE practica1.PROJECT.project_title = 'RI-East Africa Public Health Laboratory Networking Project'
ORDER BY cantidad DESC
LIMIT 3;
