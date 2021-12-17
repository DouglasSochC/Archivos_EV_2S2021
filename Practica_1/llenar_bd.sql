INSERT INTO practica1.LOCATION (
    type_code,
    type_name
) SELECT DISTINCT 
    location_type_code, 
    location_type_name
FROM practica1.tmp_locations;
SELECT CONCAT('CANTIDAD_LOCATION: ',COUNT(id)) AS descripcion FROM practica1.LOCATION;

-- ******************************************************************************************** --
-- ******************************************************************************************** --

INSERT IGNORE INTO practica1.COUNTRY_CODES (
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
    practica1.tmp_country_codes.name,
    practica1.tmp_country_codes.iso2,
    practica1.tmp_country_codes.name_name,
    practica1.tmp_country_codes.name_aiddata_code,
    practica1.tmp_country_codes.name_aiddata_name,
    practica1.tmp_country_codes.name_cow_alpha,
    practica1.tmp_country_codes.name_cow_numeric,
    practica1.tmp_country_codes.name_fao_code,
    practica1.tmp_country_codes.name_fips,
    practica1.tmp_country_codes.name_geonames_id,
    practica1.tmp_country_codes.name_imf_code,
    practica1.tmp_country_codes.name_iso2,
    practica1.tmp_country_codes.name_iso3,
    practica1.tmp_country_codes.name_iso_numeric,
    practica1.tmp_country_codes.name_oecd_code,
    practica1.tmp_country_codes.name_oecd_name,
    practica1.tmp_country_codes.name_un_code,
    practica1.tmp_country_codes.name_wb_code
FROM
    practica1.tmp_country_codes;
SELECT CONCAT('CANTIDAD_COUNTRY_CODES: ',COUNT(id)) AS descripcion FROM practica1.COUNTRY_CODES;

-- ******************************************************************************************** --
-- ******************************************************************************************** --

INSERT IGNORE INTO practica1.TYPE_STATUS (
    name
) SELECT DISTINCT
    status
FROM practica1.tmp_projects;
SELECT CONCAT('CANTIDAD_TYPE_STATUS: ',COUNT(id)) AS descripcion FROM practica1.TYPE_STATUS;

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
    practica1.tmp_geonames.geoname_id,
    practica1.tmp_geonames.place_name,
    practica1.tmp_geonames.latitude,
    practica1.tmp_geonames.longitude,
    practica1.tmp_geonames.gazetteer_adm_code,
    practica1.tmp_geonames.gazetteer_adm_name,
    practica1.tmp_geonames.location_class,
    practica1.tmp_geonames.geographic_exactness,
    (SELECT id FROM practica1.LOCATION WHERE practica1.LOCATION.type_code = practica1.tmp_geonames.location_type_code)
FROM 
    practica1.tmp_geonames;

SELECT CONCAT('CANTIDAD_GEONAME: ',COUNT(id)) AS descripcion FROM practica1.GEONAME;

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
    str_to_date(start_actual_isodate,'%d/%m/%y'),
    str_to_date(end_actual_isodate,'%d/%m/%y'),
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
SELECT CONCAT('CANTIDAD_PROJECT: ',COUNT(id)) AS descripcion FROM practica1.PROJECT;

-- ******************************************************************************************** --
-- ******************************************************************************************** --



-- ******************************************************************************************** --
-- ******************************************************************************************** --

-- ******************************************************************************************** --
-- ******************************************************************************************** --

/*
SELECT COUNT(id) FROM practica1.TRANSACTION;
SELECT COUNT(id) FROM practica1.CURRENCY;
SELECT COUNT(id) FROM practica1.LEVEL_1A;*/
