#include "StdAfx.h"
#include "SqliteSearchController.h"
#include <iostream>

SqliteSearchController::SqliteSearchController(void) {
	_ppDB = NULL;
}


SqliteSearchController::~SqliteSearchController(void)
{
}


bool SqliteSearchController::connectSQLite() {

	int result = sqlite3_open("..//ms.db", &_ppDB);

	if (result != SQLITE_OK) {
		fprintf(stderr, "can't open db!\n", sqlite3_errmsg(_ppDB));
		sqlite3_close(_ppDB);
		exit(1);
		return false;
	}
		
	printf("db open successfully!\n");
	return true;
}

void SqliteSearchController::queryCompoundData() {

	if (!connectSQLite()) { return; }

	int rc = 0;
	sqlite3_stmt *stmt;
	std::string query = "SELECT * FROM CompoundInfo WHERE CompoundID = 3 OR CompoundID = 4";

	rc = sqlite3_prepare(_ppDB, query.c_str(), query.size(), &stmt, NULL);
	if(rc != SQLITE_OK) {
		
		std::cerr << "sqlite3_prepare[" << rc << "] " << sqlite3_errmsg(_ppDB) << " " << sqlite3_errcode(_ppDB) << std::endl << query << std::endl;
		sqlite3_finalize(stmt);
		return;
	}

	rc = sqlite3_step(stmt);
	if (rc == SQLITE_DONE) { return; }

	while (rc == SQLITE_ROW) {
		std::cout << sqlite3_column_text(stmt, 1) << "|";
		std::cout << sqlite3_column_text(stmt, 2) << "|";
		std::cout << sqlite3_column_text(stmt, 7) << std::endl;
	}
	
	sqlite3_finalize(stmt);

	closeSQLite();
	return;
}

void SqliteSearchController::closeSQLite() {
	if (_ppDB) {
		sqlite3_close(_ppDB);
	}
	
}