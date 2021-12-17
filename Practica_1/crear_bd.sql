DROP DATABASE IF EXISTS practica1;
CREATE DATABASE IF NOT EXISTS practica1;

CREATE TABLE IF NOT EXISTS practica1.LOCATION(
    id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    type_code TEXT NOT NULL,
    type_name TEXT
);

CREATE TABLE IF NOT EXISTS practica1.GEONAME(
    id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    geoname_id BIGINT NOT NULL,
    place_name VARCHAR(450) NOT NULL,
    latitude DOUBLE NOT NULL,
    longitude DOUBLE NOT NULL,
    gazetter_adm_code VARCHAR(450) NOT NULL,
    gazetter_adm_name VARCHAR(450) NOT NULL,
    location_class INT NOT NULL,
    geographic_exactness INT,
    id_location BIGINT UNSIGNED,
    FOREIGN KEY (id_location) REFERENCES practica1.LOCATION(id)
);

CREATE TABLE IF NOT EXISTS practica1.COUNTRY_CODES(
    id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(450) NOT NULL,
    iso2 VARCHAR(20),
    name_name VARCHAR(450),
    name_aiddata_code INT,
    name_aiddata_name VARCHAR(500),
    name_cow_alpha VARCHAR(20),
    name_cow_numeric INT,
    name_fao_code INT,
    name_fips VARCHAR(20),
    name_geonames_id INT,
    name_imf_code INT,
    name_iso2 VARCHAR(20),
    name_iso3 VARCHAR(20),
    name_iso_numeric INT,
    name_oecd_code INT,
    name_oecd_name VARCHAR(450),
    name_un_code INT,
    name_wb_code VARCHAR(20)
);

CREATE TABLE IF NOT EXISTS practica1.TYPE_STATUS(
    id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(300) NOT NULL
);

CREATE TABLE IF NOT EXISTS practica1.PROJECT(
    id VARCHAR(50) PRIMARY KEY,
    is_geocoded INT,
    project_title VARCHAR(500) NOT NULL,
    start_actual_isodate DATE NOT NULL,
    end_actual_isodate DATE,
    donors VARCHAR(50),
    donors_iso3 VARCHAR(10),
    recipients VARCHAR(450),
    recipients_iso3 VARCHAR(10),
    ad_sector_codes VARCHAR(450),
    ad_sector_names VARCHAR(450),
    transactions_start_year YEAR,
    transactions_end_year YEAR,
    total_commitments DOUBLE,
    total_disbursements DOUBLE,
    id_country_codes BIGINT UNSIGNED,
    id_status BIGINT UNSIGNED,
    FOREIGN KEY (id_country_codes) REFERENCES practica1.COUNTRY_CODES(id),
    FOREIGN KEY (id_status) REFERENCES practica1.TYPE_STATUS(id)
);

CREATE TABLE IF NOT EXISTS practica1.LEVEL_1A(
    id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    transactions_start_year YEAR NOT NULL,
    transactions_end_year YEAR NOT NULL,
    even_split_commitments DOUBLE NOT NULL,
    even_split_disbursements DOUBLE,
    id_project VARCHAR(50),
    id_geoname BIGINT UNSIGNED,
    FOREIGN KEY (id_project) REFERENCES practica1.PROJECT(id),
    FOREIGN KEY (id_geoname) REFERENCES practica1.GEONAME(id)
);

CREATE TABLE IF NOT EXISTS practica1.CURRENCY(
    id BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    transaction_currency VARCHAR(50)
);

CREATE TABLE IF NOT EXISTS practica1.TRANSACTION(
    id VARCHAR(100) NOT NULL PRIMARY KEY,
    transaction_isodate DATE NOT NULL,
    transaction_year YEAR NOT NULL,
    transaction_value_code VARCHAR(10) NOT NULL,
    transaction_value DOUBLE NOT NULL,
    id_project VARCHAR(50) NOT NULL,
    id_currency BIGINT UNSIGNED,
    FOREIGN KEY (id_project) REFERENCES practica1.PROJECT(id),
    FOREIGN KEY (id_currency) REFERENCES practica1.CURRENCY(id)
);
