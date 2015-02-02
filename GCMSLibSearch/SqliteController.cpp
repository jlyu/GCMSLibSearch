#include <iostream>
//#include <assert.h>
#include "time.h"
//#include "match.h"
#include "SqliteController.h"


#define COUNT_TOTAL_ROWS  0
#define FIND_COMPOUND_BY_ID 1
#define FIND_COMPOUND_BY_RANK 2
#define STORE_COMPOUND_DATA 3


// -init & deinit
SqliteController::SqliteController(const std::string &file): //可以分散成3个文件。但是库内部的表结构一致
	_ppDB(NULL) {

		const std::string queries[] = { //TODO: CompoundInfo 必须可变
			// COUNT TOTAL ROWs 
			"SELECT COUNT(*) FROM CompoundInfo",
			// Find Compound By CompoundID
			"SELECT * FROM CompoundInfo" 
			"  WHERE CompoundID = ?",
			// Find Compounds By Rank
			"SELECT * FROM CompoundInfo"
			"  ORDER BY CompoundID"
			"  LIMIT ?" 
			"  OFFSET ?",
			// Store Or update new Compound
			"INSERT OR REPLACE INTO CompoundInfo ([CompoundName], [Formula], [MassWeight], [CasNo], [PeakCount], [PeakData])"
			" VALUES (?, ?, ?, ?, ?, ?);"

		};

		init_openSQLite(file); 

		for (size_t index = 0; index < PREPARED_STATEMENT_COUNT; index++) {
			const std::string query = queries[index];
			sqlite3_prepare_v2(_ppDB, query.c_str(), query.size(), &_pStatements[index], NULL);
		}
}
SqliteController::~SqliteController(void) { 
	for (size_t index = 0; index < PREPARED_STATEMENT_COUNT; index ++) {
		sqlite3_finalize(_pStatements[index]);
	}
	sqlite3_close(_ppDB);
}
bool SqliteController::init_openSQLite(const std::string &file) {

	int result = sqlite3_open_v2(file.c_str(), &_ppDB, SQLITE_OPEN_READWRITE, NULL);
	if (result != SQLITE_OK) {
		fprintf(stderr, "can't open db!\n", sqlite3_errmsg(_ppDB));
		sqlite3_close(_ppDB);
		return false;
	}

	printf("db open successfully!\n");
	return true;
}

// - 外部接口提供
int SqliteController::totalCompoundCounts() { 

	int count = 0;
	sqlite3_stmt* statement = _pStatements[COUNT_TOTAL_ROWS];

	int rc = sqlite3_step(statement);
	if (rc != SQLITE_ROW) {
		std::cerr << "sqlite3_step[" << rc << "] " << sqlite3_errmsg(_ppDB) << " " << sqlite3_errcode(_ppDB) << std::endl;
		return -1;
	}
	
	count = sqlite3_column_int(statement, 0);
	sqlite3_reset(statement);
	return count;
}
Compound SqliteController::getCompound(int compoundID) {

	Compound aCompound;
	sqlite3_stmt *statement = _pStatements[FIND_COMPOUND_BY_ID];

	int rc = sqlite3_bind_int(statement, 1, compoundID);
	if (rc != SQLITE_OK) {
		std::cerr << "sqlite3_bind_int[" << rc << "] " << sqlite3_errmsg(_ppDB) << " " << sqlite3_errcode(_ppDB) << std::endl;
		return aCompound;
	}

	rc = sqlite3_step(statement);
	if (sqlite3_data_count(statement) > 0) {
		aCompound._compoundID = sqlite3_column_int(statement, 0);
		aCompound._compoundName = (const char*)sqlite3_column_text(statement, 1);
		aCompound._formula = (const char*)sqlite3_column_text(statement, 2);
		aCompound._massWeight = sqlite3_column_int(statement, 3);
		aCompound._casNo = (const char*)sqlite3_column_text(statement, 4);
		aCompound._peakCount = sqlite3_column_int(statement, 5);
		aCompound._peakData = (const char*)sqlite3_column_text(statement, 7); // Col 6 = `Comment`
	}
	sqlite3_reset(statement);
	return aCompound;
}
std::vector<Compound> SqliteController::getCompounds(int startCompoundID, int limit) {
	std::vector<Compound> compounds;
	sqlite3_stmt *statement = _pStatements[FIND_COMPOUND_BY_RANK];

	if ((sqlite3_bind_int(statement, 1, limit) != SQLITE_OK) ||
		(sqlite3_bind_int(statement, 2, startCompoundID) != SQLITE_OK)) {
		std::cerr << "sqlite3_bind_int" << sqlite3_errmsg(_ppDB) << " " << sqlite3_errcode(_ppDB) << std::endl;
		return compounds;
	}

	while (sqlite3_step(statement) == SQLITE_ROW) {
		Compound aCompound;
		aCompound._compoundID = sqlite3_column_int(statement, 0);
		aCompound._compoundName = (const char*)sqlite3_column_text(statement, 1);
		aCompound._formula = (const char*)sqlite3_column_text(statement, 2);
		aCompound._massWeight = sqlite3_column_int(statement, 3);
		aCompound._casNo = (const char*)sqlite3_column_text(statement, 4);
		aCompound._peakCount = sqlite3_column_int(statement, 5);
		aCompound._peakData = (const char*)sqlite3_column_text(statement, 7); // Col 6 = `Comment`
		compounds.push_back(aCompound);
	}
	sqlite3_reset(statement);
	return compounds;
}

void SqliteController::storeCompound(const Compound& aCompound) {

}
// - 内部测试接口

void SqliteController::queryCompoundData(std::vector<Compound> &selectedCompounds) {

	if (!selectedCompounds.empty()) { selectedCompounds.clear(); }


	//TODO: 实际检索数据代替随机测试数据
	int randomID[20] = { 0 };
	srand((unsigned)time(NULL));
	for (int i = 0; i < sizeof(randomID) / sizeof(int); i++) {
		randomID[i] = rand() % 191436 + 1;
		Compound aCompound = getCompound(randomID[i]);
		selectedCompounds.push_back(aCompound);
	}	
	
	return;
}

//Compound SqliteController::getCompound(int compoundID) {
//
//	Compound aCompound;
//	int rc = 0;
//	sqlite3_stmt *stmt;
//	std::string query = "SELECT * FROM CompoundInfo WHERE CompoundID = ?";
//
//	rc = sqlite3_prepare(_ppDB, query.c_str(), query.size(), &stmt, NULL);
//	if (rc != SQLITE_OK) {
//
//		std::cerr << "sqlite3_prepare[" << rc << "] " << sqlite3_errmsg(_ppDB) << " " << sqlite3_errcode(_ppDB) << std::endl;
//		sqlite3_finalize(stmt);
//		return aCompound;
//	}
//
//	rc = sqlite3_bind_int(stmt, 1, compoundID);
//	if (rc != SQLITE_OK) {
//		std::cerr << "sqlite3_bind_int[" << rc << "] " << sqlite3_errmsg(_ppDB) << " " << sqlite3_errcode(_ppDB) << std::endl;
//		sqlite3_finalize(stmt);
//		return aCompound;
//	}
//
//	rc = sqlite3_step(stmt);
//	if (rc == SQLITE_DONE) { return aCompound; }
//
//	
//	if (sqlite3_data_count(stmt) > 0) {
//		aCompound._compoundID = sqlite3_column_int(stmt, 0);
//		aCompound._compoundName = (const char*)sqlite3_column_text(stmt, 1);
//		aCompound._formula = (const char*)sqlite3_column_text(stmt, 2);
//		aCompound._massWeight = sqlite3_column_int(stmt, 3);
//		aCompound._casNo = (const char*)sqlite3_column_text(stmt, 4);
//		aCompound._peakCount = sqlite3_column_int(stmt, 5);
//		aCompound._peakData = (const char*)sqlite3_column_text(stmt, 7); // Col 6 = `Comment`
//	}
//
//	sqlite3_finalize(stmt);
//	return aCompound;
//}