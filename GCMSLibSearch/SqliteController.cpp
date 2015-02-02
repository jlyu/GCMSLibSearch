//#include "StdAfx.h"

#include <iostream>
#include <vector>
#include "time.h"

//#include "match.h"

#include "SqliteController.h"

SqliteController::SqliteController(void) {
	_ppDB = NULL;
}


SqliteController::~SqliteController(void) { }

bool SqliteController::openSQLiteDB(const std::string &dbName) {
	int result = sqlite3_open(dbName.c_str(), &_ppDB);
	if (result != SQLITE_OK) {
		fprintf(stderr, "can't open db!\n", sqlite3_errmsg(_ppDB));
		sqlite3_close(_ppDB);
		exit(1);
		return false;
	}

	printf("db open successfully!\n");
	return true;
}


bool SqliteController::connectSQLite() {

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

void SqliteController::queryCompoundData(std::vector<Compound> &selectedCompounds) {

	if (!connectSQLite()) { return; }

	if (!selectedCompounds.empty()) { selectedCompounds.clear(); }


	//TODO: 实际检索数据代替随机测试数据
	int randomID[20] = { 0 };
	srand((unsigned)time(NULL));
	for (int i = 0; i < sizeof(randomID) / sizeof(int); i++) {
		randomID[i] = rand() % 191436 + 1;
		Compound aCompound = getCompound(randomID[i]);
		selectedCompounds.push_back(aCompound);
	}	
	
	closeSQLite();
	return;
}

Compound SqliteController::getCompound(int compoundID) {

	Compound aCompound;
	int rc = 0;
	sqlite3_stmt *stmt;
	std::string query = "SELECT * FROM CompoundInfo WHERE CompoundID = ?";

	rc = sqlite3_prepare(_ppDB, query.c_str(), query.size(), &stmt, NULL);
	if (rc != SQLITE_OK) {

		std::cerr << "sqlite3_prepare[" << rc << "] " << sqlite3_errmsg(_ppDB) << " " << sqlite3_errcode(_ppDB) << std::endl;
		sqlite3_finalize(stmt);
		return aCompound;
	}

	rc = sqlite3_bind_int(stmt, 1, compoundID);
	if (rc != SQLITE_OK) {
		std::cerr << "sqlite3_bind_int[" << rc << "] " << sqlite3_errmsg(_ppDB) << " " << sqlite3_errcode(_ppDB) << std::endl;
		sqlite3_finalize(stmt);
		return aCompound;
	}

	rc = sqlite3_step(stmt);
	if (rc == SQLITE_DONE) { return aCompound; }

	
	if (sqlite3_data_count(stmt) > 0) {
		aCompound._compoundID = sqlite3_column_int(stmt, 0);
		aCompound._compoundName = (const char*)sqlite3_column_text(stmt, 1);
		aCompound._formula = (const char*)sqlite3_column_text(stmt, 2);
		aCompound._massWeight = sqlite3_column_int(stmt, 3);
		aCompound._casNo = (const char*)sqlite3_column_text(stmt, 4);
		aCompound._peakCount = sqlite3_column_int(stmt, 5);
		aCompound._peakData = (const char*)sqlite3_column_text(stmt, 7); // Col 6 = `Comment`
	}

	sqlite3_finalize(stmt);
	return aCompound;
}


void SqliteController::closeSQLite() {
	if (_ppDB) {
		sqlite3_close(_ppDB);
	}
	
}