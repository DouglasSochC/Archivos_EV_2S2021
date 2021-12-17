LOAD DATA
LOCAL INFILE 'ArchivosCSV/transactions.csv'
INTO TABLE practica1.tmp_transactions
FIELDS TERMINATED BY ','
LINES TERMINATED BY '\n'
IGNORE 1 LINES(
    transaction_id,
    project_id,
    transaction_isodate,
    transaction_year ,
    transaction_value_code ,
    transaction_currency ,
    transaction_value 
);