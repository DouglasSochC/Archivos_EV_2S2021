INSERT INTO practica1.LOCATION (
    type_code,
    type_name
) SELECT DISTINCT 
    location_type_code, 
    location_type_name
FROM practica1.tmp_locations;
SELECT CONCAT('CANTIDAD_LOCATION: ',COUNT(id)) AS cantidad FROM practica1.LOCATION;

-- ******************************************************************************************** --
-- ******************************************************************************************** --

INSERT INTO practica1.COUNTRY_CODES (
    name,
    iso2,
    name_name,
    name_aiddata_code,
    name_aiddata_name,
    name_cow_alpha,
    name_cow_numeric,
    name_fao_code,
    name_fips,
    name_geonames_id,
    name_imf_code,
    name_iso2,
    name_iso3,
    name_iso_numeric,
    name_oecd_code,
    name_oecd_name,
    name_un_code,
    name_wb_code
) SELECT DISTINCT
    name,
    iso2,
    name_name,
    name_aiddata_code * 1,
    name_aiddata_name,
    name_cow_alpha,
    name_cow_numeric * 1,
    name_fao_code * 1,
    name_fips,
    name_geonames_id * 1,
    name_imf_code * 1,
    name_iso2,
    name_iso3,
    name_iso_numeric * 1,
    name_oecd_code * 1,
    name_oecd_name,
    name_un_code * 1,
    name_wb_code
FROM
    practica1.tmp_country_codes;
SELECT CONCAT('CANTIDAD_COUNTRY_CODES: ',COUNT(id)) AS cantidad FROM practica1.COUNTRY_CODES;

-- ******************************************************************************************** --
-- ******************************************************************************************** --

INSERT INTO practica1.TYPE_STATUS (
    name
) SELECT DISTINCT
    status
FROM practica1.tmp_projects;
SELECT CONCAT('CANTIDAD_TYPE_STATUS: ',COUNT(id)) AS cantidad FROM practica1.TYPE_STATUS;

-- ******************************************************************************************** --
-- ******************************************************************************************** --

INSERT IGNORE INTO practica1.GEONAME(
    geoname_id,
    place_name,
    latitude,
    longitude,
    gazetter_adm_code,
    gazetter_adm_name,
    location_class,
    geographic_exactness,
    id_location
) SELECT 
    geoname_id,
    place_name,
    latitude,
    longitude,
    gazetteer_adm_code,
    gazetteer_adm_name,
    location_class,
    geographic_exactness,
    (SELECT id FROM practica1.LOCATION WHERE practica1.LOCATION.type_code = practica1.tmp_geonames.location_type_code)
FROM 
    practica1.tmp_geonames;
SELECT CONCAT('CANTIDAD_GEONAME: ',COUNT(id)) AS cantidad FROM practica1.GEONAME;

-- ******************************************************************************************** --
-- ******************************************************************************************** --

INSERT IGNORE INTO practica1.PROJECT (
    id,
    is_geocoded,
    project_title,
    start_actual_isodate,
    end_actual_isodate,
    donors,
    donors_iso3,
    recipients,
    recipients_iso3,
    ad_sector_codes,
    ad_sector_names,
    transactions_start_year,
    transactions_end_year,
    total_commitments,
    total_disbursements,
    id_country_codes,
    id_status
) SELECT DISTINCT
    project_id,
    is_geocoded,
    project_title,
    str_to_date(start_actual_isodate,'%d/%m/%Y'),
    str_to_date(end_actual_isodate,'%d/%m/%Y'),
    donors,
    donors_iso3,
    recipients,
    recipients_iso3,
    ad_sector_codes,
    ad_sector_names,
    transactions_start_year,
    transactions_end_year,
    total_commitments,
    total_disbursements,
    (SELECT id FROM practica1.COUNTRY_CODES WHERE practica1.COUNTRY_CODES.name = practica1.tmp_projects.recipients),
    (SELECT id FROM practica1.TYPE_STATUS WHERE practica1.TYPE_STATUS.name = practica1.tmp_projects.status)
FROM 
    practica1.tmp_projects;
SELECT CONCAT('CANTIDAD_PROJECT: ',COUNT(id)) AS cantidad FROM practica1.PROJECT;

-- ******************************************************************************************** --
-- ******************************************************************************************** --

INSERT IGNORE INTO practica1.CURRENCY (
    transaction_currency
) SELECT DISTINCT
    transaction_currency
FROM practica1.tmp_transactions;
SELECT CONCAT('CANTIDAD_CURRENCY: ',COUNT(id)) AS cantidad FROM practica1.CURRENCY;

-- ******************************************************************************************** --
-- ******************************************************************************************** --

INSERT INTO practica1.TRANSACTION(
    id,
    transaction_isodate,
    transaction_year,
    transaction_value_code,
    transaction_value,
    id_project,
    id_currency
) SELECT DISTINCT 
    transaction_id,
    str_to_date(transaction_isodate,'%d/%m/%Y'),
    transaction_year,
    transaction_value_code,
    transaction_value * 1,    
    (SELECT id FROM practica1.PROJECT WHERE practica1.PROJECT.id = practica1.tmp_transactions.project_id),
    (SELECT id FROM practica1.CURRENCY WHERE practica1.CURRENCY.transaction_currency = practica1.tmp_transactions.transaction_currency)
FROM 
    practica1.tmp_transactions;
SELECT CONCAT('CANTIDAD_TRANSACTION: ',COUNT(id)) AS cantidad FROM practica1.TRANSACTION;

-- ******************************************************************************************** --
-- ******************************************************************************************** --

INSERT INTO practica1.LEVEL_1A(
    transactions_start_year,
    transactions_end_year,
    even_split_commitments,
    even_split_disbursements,
    id_project/*,
    id_geoname*/
) SELECT
    transactions_start_year,
    transactions_end_year,
    CAST(even_split_commitments AS DOUBLE),
    CAST(even_split_disbursements AS DOUBLE),
    (SELECT id FROM practica1.PROJECT WHERE practica1.PROJECT.id = practica1.tmp_level_1a.project_id)/*,
    (SELECT DISTINCT id FROM practica1.GEONAME WHERE practica1.GEONAME.geoname_id = practica1.tmp_level_1a.geoname_id LIMIT 1)*/
FROM 
    practica1.tmp_level_1a;
SELECT CONCAT('CANTIDAD_LEVEL_1A: ',COUNT(id)) AS cantidad FROM practica1.LEVEL_1A;

-- ******************************************************************************************** --
-- ******************************************************************************************** --
