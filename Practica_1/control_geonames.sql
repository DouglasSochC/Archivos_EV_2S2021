LOAD DATA
LOCAL INFILE 'ArchivosCSV/geonames.csv'
INTO TABLE practica1.tmp_geonames
FIELDS TERMINATED BY ','
ENCLOSED BY '"'
LINES TERMINATED BY '\n'
IGNORE 1 LINES(
    geoname_id,
    place_name,
    latitude,
    longitude,
    location_type_code,
    location_type_name,
    gazetteer_adm_code,
    gazetteer_adm_name,
    location_class,
    geographic_exactness
);