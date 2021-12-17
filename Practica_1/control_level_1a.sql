LOAD DATA
LOCAL INFILE 'ArchivosCSV/level_1a.csv'
INTO TABLE practica1.tmp_level_1a
CHARACTER SET latin1
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n'
IGNORE 1 LINES(
    project_id,
    project_location_id,
    geoname_id,
    transactions_start_year,
    transactions_end_year,
    even_split_commitments,
    even_split_disbursements
);