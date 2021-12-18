CREATE TABLE IF NOT EXISTS practica1.tmp_country_codes(
    name TEXT,
    iso2 TEXT,
    name_name TEXT,
    name_aiddata_code TEXT,
    name_aiddata_name TEXT,
    name_cow_alpha TEXT,
    name_cow_numeric TEXT,
    name_fao_code TEXT,
    name_fips TEXT,
    name_geonames_id TEXT,
    name_imf_code TEXT,
    name_iso2 TEXT,
    name_iso3 TEXT,
    name_iso_numeric TEXT,
    name_oecd_code TEXT,
    name_oecd_name TEXT,
    name_un_code TEXT,
    name_wb_code TEXT
);

CREATE TABLE IF NOT EXISTS practica1.tmp_geonames(
    geoname_id TEXT NOT NULL,
    place_name TEXT NOT NULL,
    latitude TEXT NOT NULL,
    longitude TEXT NOT NULL,
    location_type_code TEXT NOT NULL,
    location_type_name TEXT NOT NULL,
    gazetteer_adm_code TEXT NOT NULL,
    gazetteer_adm_name TEXT NOT NULL,
    location_class TEXT NOT NULL,
    geographic_exactness TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS practica1.tmp_level_1a(
    project_id TEXT NOT NULL,
    project_location_id TEXT NOT NULL,
    geoname_id TEXT NOT NULL,
    transactions_start_year TEXT NOT NULL,
    transactions_end_year TEXT NOT NULL,
    even_split_commitments TEXT NOT NULL,
    even_split_disbursements TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS practica1.tmp_locations(
    location_type_code TEXT,
    location_type_name TEXT
);

CREATE TABLE IF NOT EXISTS practica1.tmp_projects(
    project_id TEXT,
    is_geocoded TEXT,
    project_title TEXT,
    start_actual_isodate TEXT,
    end_actual_isodate TEXT,
    donors TEXT,
    donors_iso3 TEXT,
    recipients TEXT,
    recipients_iso3 TEXT,
    ad_sector_codes TEXT,
    ad_sector_names TEXT,
    status TEXT,
    transactions_start_year TEXT,
    transactions_end_year TEXT,
    total_commitments TEXT,
    total_disbursements TEXT
);

CREATE TABLE IF NOT EXISTS practica1.tmp_transactions(
    transaction_id TEXT NOT NULL,
    project_id TEXT NOT NULL,
    transaction_isodate TEXT NOT NULL,
    transaction_year  TEXT NOT NULL,
    transaction_value_code  TEXT NOT NULL,
    transaction_currency  TEXT NOT NULL,
    transaction_value  TEXT NOT NULL
);
