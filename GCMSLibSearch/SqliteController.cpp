#include <iostream>
//#include <map>
#include <algorithm>
//#include <assert.h>
#include "time.h"
//#include "match.h"
#include "SqliteController.h"


//#define CREATE_TABLE_PEAKDATA	0
#define COUNT_TOTAL_ROWS		0
#define MAX_PEAK_COUNT			1
#define FIND_COMPOUND_BY_ID		2
#define FIND_PEAKDATA_BY_ID		3
#define FIND_COMPOUND_BY_RANK	4
#define FIND_PEAKDATA_BY_RANK	5
#define FIND_PEAKPOINTS_BY_ID	6
#define STORE_COMPOUND_DATA		7
#define STORE_PEAK_DATA			8

#define MAX_MASS 1659

#define MAX_Y_TO_X_EQUAL  1
#define SUB_Y_TO_X_EQUAL  2
#define MAX_X_EQUAL		  3


// -Table
#define CREATE_TABLE_PEAKDATA "CREATE TABLE IF NOT EXISTS [PeakData] ([ID] INTEGER PRIMARY KEY AUTOINCREMENT, [CompoundID] INTEGER, [x] INTEGER, [y] INTEGER);"
#define CREATE_TABLE_MASSHASH "CREATE TABLE IF NOT EXISTS [MassHash] ([Mass] INTEGER PRIMARY KEY, [IDs] CHAR);"
#define CREATE_TABLE_COMPOUND "CREATE TABLE IF NOT EXISTS [Compound] ([CompoundID] INTEGER PRIMARY KEY, [CompoundName] CHAR, [Formula] CHAR(255), [MassWeight] INTEGER, [CasNo] CHAR(255), [PeakCount] INTEGER, [MaxX] INTEGER, [PeakData] CHAR);"
// -Index
#define CREATE_INDEX_X_ON_PEAKDATA "CREATE INDEX IF NOT EXISTS idx_x ON [PeakData] (x);"
#define CREATE_INDEX_ID_ON_PEAKDATA "CREATE INDEX IF NOT EXISTS idx_id ON [PeakData] (CompoundID);"
#define CREATE_INDEX_MASSHASH "CREATE INDEX IF NOT EXISTS idx_mass ON [MassHash] (Mass);"
#define CREATE_INDEX_MAXX_ON_COMPOUND "CREATE INDEX IF NOT EXISTS idx_maxx ON [Compound] (MaxX);"
// -Count
#define COUNT_MASS_ROWS  "SELECT COUNT(Mass) FROM [MassHash]"
// -Select
#define SELECT_XY_FROM_PEAKDATA  "SELECT x, y FROM [PeakData] WHERE CompoundID = ?"


// -init & deinit
SqliteController::SqliteController(const std::string &file): //可以分散成3个文件。但是库内部的表结构一致
	_ppDB(NULL) { //TODO: 表必须已经存在

		const std::string queries[] = { //TODO: CompoundInfo 必须可变
			// COUNT TOTAL ROWs 
			"SELECT COUNT(*) FROM CompoundInfo",
			// COUNT MAX PEAK COUNT
			"SELECT max(PeakCount) FROM CompoundInfo LIMIT 1",
			// Find Compound By CompoundID
			"SELECT * FROM CompoundInfo" 
			"  WHERE CompoundID = ?",
			// Find peakData By CompoundID
			"SELECT PeakCount, PeakData FROM CompoundInfo" 
			"  WHERE CompoundID = ? LIMIT 1",
			// Find Compounds By Rank
			"SELECT * FROM CompoundInfo"
			"  ORDER BY CompoundID"
			"  LIMIT ?" 
			"  OFFSET ?",
			// Find PeakData By Rank
			"SELECT PeakCount, PeakData FROM CompoundInfo"
			"  ORDER BY CompoundID"
			"  LIMIT ?" 
			"  OFFSET ?",
			// Find PeakPoints By CompoundID
			"SELECT x, y FROM [PeakData]"
			"  WHERE CompoundID = ?"
			"  LIMIT ?", //
			// Store Or update new Compound
			"INSERT OR REPLACE INTO CompoundInfo ([CompoundID], [CompoundName], [Formula], [MassWeight], [CasNo], [PeakCount], [PeakData])"
			" VALUES (?, ?, ?, ?, ?, ?, ?);",
			// Store or update new PeakData
			"INSERT OR REPLACE INTO [PeakData] ([CompoundID], [x], [y])"
			"  VALUES (?, ?, ?);"
		};

		init_openSQLite(file); 

		for (size_t index = 0; index < PREPARED_STATEMENT_COUNT; index++) {
			const std::string query = queries[index];
			int rc = sqlite3_prepare_v2(_ppDB, query.c_str(), query.size(), &_pStatements[index], NULL);
			if (rc != SQLITE_OK) {
				std::cerr << "sqlite3_prepare_v2[" << rc << "] " << sqlite3_errmsg(_ppDB) << " " << sqlite3_errcode(_ppDB) << std::endl << query.c_str() << std::endl;
			}
		}

		preproccess();
}
SqliteController::~SqliteController(void) { 
	for (size_t index = 0; index < PREPARED_STATEMENT_COUNT; index ++) {
		sqlite3_finalize(_pStatements[index]);
	}
	sqlite3_close(_ppDB);
}
bool SqliteController::init_openSQLite(const std::string &file) {

	int result = sqlite3_open_v2(file.c_str(), &_ppDB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	if (result != SQLITE_OK) {
		std::cerr << "init_openSQLite -> sqlite3_open_v2[" << result << "] " << sqlite3_errmsg(_ppDB) << " " << sqlite3_errcode(_ppDB) << std::endl;
		sqlite3_close(_ppDB);
		return false;
	}

	printf("open ---> OK \n\n");

	return true;
}
bool SqliteController::checkConnectionError() {
	int code = sqlite3_errcode(_ppDB);
	if ((code > SQLITE_OK) && (code < SQLITE_ROW)) {
		const char* message = sqlite3_errmsg(_ppDB);
		std::cout << "SQLite error: " << message << std::endl;
		return true;
	}
	return false;
}

// - 外部接口提供
void SqliteController::preproccess() {
	// -Create TABLE [PeakData] [MassHash]
	//createPeakDataTable(); 
	//dq_createMassHashTable();
	dq_createCompoundTable();

	// -Fill in dates
	//pre_parsePeakDate();
	//dq_pre_buildMassHash();
	dq_pre_buildCompound();
}
void SqliteController::createPeakDataTable() {

	int rc;
	sqlite3_stmt* statement;

	// TABLE
	rc = sqlite3_prepare_v2(_ppDB, CREATE_TABLE_PEAKDATA, -1, &statement, NULL);
	if (rc != SQLITE_OK) { }

	rc = sqlite3_step(statement);
	if (rc != SQLITE_DONE) {
		std::cerr << "createPeakDataTable() -> sqlite3_step[" << rc << "] " << sqlite3_errmsg(_ppDB) << " " << sqlite3_errcode(_ppDB) << std::endl;
	}
	sqlite3_finalize(statement);

	// INDEX
	rc = sqlite3_prepare_v2(_ppDB, CREATE_INDEX_ID_ON_PEAKDATA, -1, &statement, NULL);
	if (rc != SQLITE_OK) { }
	rc = sqlite3_step(statement);
	if (rc != SQLITE_DONE) {
		std::cerr << "createPeakDataTable() -> sqlite3_step[" << rc << "] " << sqlite3_errmsg(_ppDB) << " " << sqlite3_errcode(_ppDB) << std::endl;
	}
	sqlite3_finalize(statement);
	checkConnectionError();
}
void SqliteController::dq_createMassHashTable() {
	int rc;
	sqlite3_stmt* statement;

	rc = sqlite3_prepare_v2(_ppDB, CREATE_TABLE_MASSHASH, -1, &statement, NULL);
	if (rc != SQLITE_OK) { }

	rc = sqlite3_step(statement);
	if (rc != SQLITE_OK && rc != SQLITE_DONE) {
		std::cerr << "dq_createMassHashTable() -> sqlite3_step[" << rc << "] " << sqlite3_errmsg(_ppDB) << " " << sqlite3_errcode(_ppDB) << std::endl;
	}
	sqlite3_finalize(statement);

	// INDEX
	rc = sqlite3_prepare_v2(_ppDB, CREATE_INDEX_MASSHASH, -1, &statement, NULL);
	if (rc != SQLITE_OK) { }
	rc = sqlite3_step(statement);
	if (rc != SQLITE_DONE) {
		std::cerr << "dq_createMassHashTable() -> sqlite3_step[" << rc << "] " << sqlite3_errmsg(_ppDB) << " " << sqlite3_errcode(_ppDB) << std::endl;
	}
	sqlite3_finalize(statement);
	checkConnectionError();
}
void SqliteController::dq_createCompoundTable() {
	int rc;
	sqlite3_stmt* statement;
	rc = sqlite3_prepare_v2(_ppDB, CREATE_TABLE_COMPOUND, -1, &statement, NULL);
	if (rc != SQLITE_OK) { }

	rc = sqlite3_step(statement);
	if (rc != SQLITE_OK && rc != SQLITE_DONE) {
		std::cerr << "dq_createCompoundTable() -> sqlite3_step[" << rc << "] " << sqlite3_errmsg(_ppDB) << " " << sqlite3_errcode(_ppDB) << std::endl;
	}
	sqlite3_finalize(statement);

	// INDEX
	rc = sqlite3_prepare_v2(_ppDB, CREATE_INDEX_MAXX_ON_COMPOUND, -1, &statement, NULL);
	if (rc != SQLITE_OK) { }
	rc = sqlite3_step(statement);
	if (rc != SQLITE_DONE) {
		std::cerr << "dq_createMassHashTable() -> sqlite3_step[" << rc << "] " << sqlite3_errmsg(_ppDB) << " " << sqlite3_errcode(_ppDB) << std::endl;
	}
	sqlite3_finalize(statement);
	checkConnectionError();
}
// 查
int SqliteController::totalCompoundCounts() { 

	sqlite3_stmt* statement = _pStatements[COUNT_TOTAL_ROWS];
	return query_aSingleCount(statement);

}
int SqliteController::maxPeakCount() {

	sqlite3_stmt* statement = _pStatements[MAX_PEAK_COUNT];
	return query_aSingleCount(statement);
}
void SqliteController::getPeakData(int compoundID, Peak& aPeak) {

	sqlite3_stmt* statement = _pStatements[FIND_PEAKDATA_BY_ID];
	int rc = sqlite3_bind_int(statement, 1, compoundID);
	if (rc != SQLITE_OK) {
		std::cerr << "sqlite3_bind_int[" << rc << "] " << sqlite3_errmsg(_ppDB) << " " << sqlite3_errcode(_ppDB) << std::endl;
		return;
	}

	rc = sqlite3_step(statement);
	if (sqlite3_data_count(statement) > 0) {
		aPeak._peakCount = sqlite3_column_int(statement, 0);
		aPeak._peakData = (const char*)sqlite3_column_text(statement, 1);
	}

	sqlite3_reset(statement);
	
}
std::vector<Peak> SqliteController::getPeakDatas(int startCompoundID, int limit) {
	std::vector<Peak> peaks;
	sqlite3_stmt *statement = _pStatements[FIND_PEAKDATA_BY_RANK];

	if ((sqlite3_bind_int(statement, 1, limit) != SQLITE_OK) ||
		(sqlite3_bind_int(statement, 2, startCompoundID) != SQLITE_OK)) {
			std::cerr << "sqlite3_bind_int" << sqlite3_errmsg(_ppDB) << " " << sqlite3_errcode(_ppDB) << std::endl;
			return peaks;
	}

	sqlite3_exec(_ppDB, "BEGIN;", 0, 0, 0);

	while (sqlite3_step(statement) == SQLITE_ROW) {
		Peak aPeak;
		aPeak._peakCount = sqlite3_column_int(statement, 0);
		aPeak._peakData = (const char*)sqlite3_column_text(statement, 1);
		peaks.push_back(aPeak);
	}

	sqlite3_exec(_ppDB, "COMMIT;", 0, 0, 0);

	sqlite3_reset(statement);
	return peaks;

	/*

	char sql[1024];
	sprintf(sql, "SELECT PeakCount, PeakData FROM CompoundInfo ORDER BY CompoundID LIMIT %d OFFSET %d;", limit, startCompoundID);

	char **pazResult;
	int nRow, nCol;
	int rc = sqlite3_get_table(_ppDB, sql, &pazResult, &nRow, &nCol, 0);
	if (rc != SQLITE_OK) {
	std::cerr << "getPeakDatas - > sqlite3_get_table" << sqlite3_errmsg(_ppDB) << " " << sqlite3_errcode(_ppDB) << std::endl;
	return peaks;
	}

	for (int row = 0; row < nRow; row++) {
	Peak aPeak;
	aPeak._peakCount = atoi(pazResult[nCol * row + nCol]);
	aPeak._peakData = pazResult[nCol * row + nCol + 1];
	peaks.push_back(aPeak);
	}

	sqlite3_free_table(pazResult);

	sqlite3_exec(_ppDB, "COMMIT;", 0, 0, 0);*/
}
void SqliteController::dq_getPeakDatas_v2(int* compoundIDs, std::vector<Peak>& peaks) {

	// 按slice单位 WHERE x=1 OR x=3 OR x=9 .. 一次性取数据
	const size_t peakSize = compoundIDs[0];
	peaks.resize(peakSize);

	const int slice = 256;
	const int pages = peakSize / slice; 
	const int remain = peakSize % slice;
	
	char c[32];
	sqlite3_stmt *statement;
	std::string query = "SELECT PeakCount, PeakData FROM CompoundInfo WHERE ";

	int index = 0;
	for(int i = 1; i < COUNT_COMPOUNDS; i++) { // 191438

		if (compoundIDs[i] != 2 && i != COUNT_COMPOUNDS - 1) { continue; }

		const int compoundID = i;
		sprintf_s(c, "CompoundID = %d ", compoundID); 
		query += c;
		if ((index != peakSize - 1) && (index % slice != 0) || (index == 0))  { query += "OR "; }

		if ((index != 0 && index % slice == 0)||(i == COUNT_COMPOUNDS - 1)) {

			sqlite3_exec(_ppDB, "BEGIN;", NULL, NULL, NULL);
			sqlite3_prepare_v2(_ppDB, query.c_str(), query.size(), &statement, NULL);
			
			//std::cout << query.c_str() << std::endl;
			while (sqlite3_step(statement) == SQLITE_ROW) {
				
				static int n = 0;
				peaks[n]._peakCount = sqlite3_column_int(statement, 0);
				peaks[n]._peakData = (const char*)sqlite3_column_text(statement, 1);
				n++;
			}
			sqlite3_exec(_ppDB, "COMMIT;", NULL, NULL, NULL);
			sqlite3_reset(statement);
			query = "SELECT PeakCount, PeakData FROM CompoundInfo WHERE ";
		}
		index++;
	}
	sqlite3_finalize(statement);
}
void SqliteController::dq_getPeakDatas_v3(int* compoundIDs, std::vector<Peak>& peaks) {
	// CompoundInfo 全部读入内存，事务批量读
	const size_t peakSize = compoundIDs[0];
	peaks.resize(COUNT_COMPOUNDS);

	sqlite3_stmt *statement;
	std::string query = "SELECT PeakCount, PeakData FROM CompoundInfo ORDER BY CompoundID LIMIT ? OFFSET ?";
	sqlite3_prepare_v2(_ppDB, query.c_str(), query.size(), &statement, NULL);

	const int slice = 10000;
	const int pages = COUNT_COMPOUNDS / slice; //每次读10K行
	//const int remain = peakSize % slice;

	for (int page = 0; page != pages + 1; page++) {

		sqlite3_exec(_ppDB, "BEGIN;", NULL, NULL, NULL);
		if ((sqlite3_bind_int(statement, 1, slice) == SQLITE_OK) &&
			(sqlite3_bind_int(statement, 2, slice * page) == SQLITE_OK)) {

				while (sqlite3_step(statement) == SQLITE_ROW) {
					static int n = 0;
					peaks[n]._peakCount = sqlite3_column_int(statement, 0);
					peaks[n]._peakData = (const char*)sqlite3_column_text(statement, 1);
					n++;
				}
		}
		sqlite3_exec(_ppDB, "COMMIT;", NULL, NULL, NULL);
		sqlite3_reset(statement);
	}

	sqlite3_finalize(statement);
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
void SqliteController::getPeakPoints(int compoundID, unsigned int* x, float* y) {

	int rc;
	sqlite3_stmt *statement;

	sqlite3_exec(_ppDB, "BEGIN;", 0, 0, 0);
	rc = sqlite3_prepare_v2(_ppDB, SELECT_XY_FROM_PEAKDATA, -1, &statement, NULL);


	for (int i=1; i <= 191436; i++) { //TODO: 批量取
		if (sqlite3_bind_int(statement, 1, compoundID) == SQLITE_OK) {
			
			int index = 0;
			while (sqlite3_step(statement) == SQLITE_ROW) {
				unsigned int x_ = static_cast<unsigned int>(sqlite3_column_int(statement, 0));
				float y_ = static_cast<float>(sqlite3_column_int(statement, 1));

				x[index] = x_;
				y[index] = y_;

				index++;
			}
		}
	}

	sqlite3_exec(_ppDB, "COMMIT;", 0, 0, 0);
	sqlite3_finalize(statement);
}
void SqliteController::dq_getPeakPoints(int compoundID, std::vector<PeakXY>& peakXYs) {
	int rc;
	sqlite3_stmt *statement;

	//sqlite3_exec(_ppDB, "BEGIN;", 0, 0, 0);
	


	for (int i=1; i <= 1000; i++) { //TODO: 批量取
		rc = sqlite3_prepare_v2(_ppDB, SELECT_XY_FROM_PEAKDATA, -1, &statement, NULL);

		if (sqlite3_bind_int(statement, 1, i) == SQLITE_OK) {

			//PeakXY aPeakXY;
			int index = 0;
			while (sqlite3_step(statement) == SQLITE_ROW) {

				//aPeakXY._compoundID = i;
				unsigned int x_ = static_cast<unsigned int>(sqlite3_column_int(statement, 0));
				float y_ = static_cast<float>(sqlite3_column_int(statement, 1));

				//aPeakXY._x[index] = x_;
				//aPeakXY._y[index] = y_;

				index++;
			}

			//peakXYs.push_back(aPeakXY);
			sqlite3_reset(statement);
		}
	}

	//sqlite3_exec(_ppDB, "COMMIT;", 0, 0, 0);
	sqlite3_finalize(statement);
}
void SqliteController::dq_getPeakPoints(std::vector<PeakPoint>& peakPoints) {
	int rc;
	sqlite3_stmt* statement;
	const std::string queryXY = "SELECT [x], [y] FROM [PeakData] WHERE CompoundID = ?";
	//const std::string queryXY = "SELECT [CompoundID] FROM [PeakData] WHERE x = ?";

	sqlite3_exec(_ppDB, "BEGIN;", 0, 0, 0);
	rc = sqlite3_prepare_v2(_ppDB, queryXY.c_str(), queryXY.size(), &statement, NULL);

	for (int i=1; i <= 200000; i++) {
		if (sqlite3_bind_int(statement, 1, i) == SQLITE_OK) {

			while (sqlite3_step(statement) == SQLITE_ROW) {

				//compoundIDs += (const char*)sqlite3_column_text(statementPeakData, 0); 
				//compoundIDs += ",";
				//count++;
			}
			sqlite3_reset(statement);
		}
	}
	
	sqlite3_exec(_ppDB, "COMMIT;", 0, 0, 0);
	sqlite3_finalize(statement);


}
// 存 / 改
void SqliteController::storeCompound(const Compound& aCompound) {

	// TODO: VERIFY compound datas
	sqlite3_stmt *statement = _pStatements[STORE_COMPOUND_DATA];
	
	if ((sqlite3_bind_int(statement,  1, aCompound._compoundID) == SQLITE_OK) &&
		(sqlite3_bind_text(statement, 2, aCompound._compoundName.c_str(), -1, SQLITE_STATIC) == SQLITE_OK) &&
		(sqlite3_bind_text(statement, 3, aCompound._formula.c_str(), -1, SQLITE_STATIC) == SQLITE_OK) &&
		(sqlite3_bind_int(statement,  4, aCompound._massWeight) == SQLITE_OK) &&
		(sqlite3_bind_text(statement, 5, aCompound._casNo.c_str(), -1, SQLITE_STATIC) == SQLITE_OK) &&
		(sqlite3_bind_int(statement,  6, aCompound._peakCount) == SQLITE_OK) &&
		(sqlite3_bind_text(statement, 7, aCompound._peakData.c_str(), -1, SQLITE_STATIC) == SQLITE_OK)) {

		   sqlite3_step(statement);
		   sqlite3_reset(statement);
	}
}
void SqliteController::storePeakData(const PeakPoint& aPoint) {

	sqlite3_stmt *statement = _pStatements[STORE_PEAK_DATA];

	if ((sqlite3_bind_int(statement, 1, aPoint._compoundID) == SQLITE_OK) &&
		(sqlite3_bind_int(statement, 2, aPoint._x) == SQLITE_OK) &&
		(sqlite3_bind_int(statement, 3, aPoint._y) == SQLITE_OK) ) {

			sqlite3_step(statement);
			sqlite3_reset(statement);
	}

}
// - 内部接口
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
int  SqliteController::query_aSingleCount(sqlite3_stmt* pStatement) {
	int count = 0;
	int rc = sqlite3_step(pStatement);
	if (rc != SQLITE_ROW) {
		std::cerr << "query_aSingleCount -> sqlite3_step[" << rc << "] " << sqlite3_errmsg(_ppDB) << " " << sqlite3_errcode(_ppDB) << std::endl;
		return -1;
	}

	count = sqlite3_column_int(pStatement, 0);
	sqlite3_reset(pStatement);
	return count;
}
void SqliteController::pre_parsePeakDataString(const std::string& strPeakData, int peakCount, int *x, int *y) {
	std::string::size_type i = 0;
	std::string::size_type j = strPeakData.find(';');

	for (int index = 0; index < peakCount; index++) {

		std::string strXY = strPeakData.substr(i, j-i);
		std::string::size_type w = strXY.find(' ');
		std::string strX = strXY.substr(0, w);
		std::string strY = strXY.substr(w+1, strXY.length());

		x[index] = atoi(strX.c_str());
		y[index] = (float)atoi(strY.c_str());

		i = ++j;
		j = strPeakData.find(';', j);

	}
}
void SqliteController::pre_parsePeakDataString(const std::string& strPeakData, int peakCount, std::vector<PeakPoint> &peakPoints) {

	std::string::size_type i = 0;
	std::string::size_type j = strPeakData.find(';');

	for (int index = 0; index < peakCount; index++) {

		std::string strXY = strPeakData.substr(i, j-i);
		std::string::size_type w = strXY.find(' ');
		std::string strX = strXY.substr(0, w);
		std::string strY = strXY.substr(w+1, strXY.length());

		PeakPoint aPeakPoint;
		aPeakPoint._x = atoi(strX.c_str());
		aPeakPoint._y = atoi(strY.c_str());
		peakPoints.push_back(aPeakPoint);

		i = ++j;
		j = strPeakData.find(';', j);

	}
}
int SqliteController::pre_parsePeakDataString(const std::string& strPeakData, int peakCount) {

	std::string::size_type i = 0;
	std::string::size_type j = strPeakData.find(';');

	for (int index = 0; index < peakCount; index++) {

		if (index == peakCount - 1) {
			std::string strXY = strPeakData.substr(i, j-i);
			std::string::size_type w = strXY.find(' ');
			std::string strX = strXY.substr(0, w);
			std::string strY = strXY.substr(w+1, strXY.length());

			return atoi(strX.c_str());
		}

		i = ++j;
		j = strPeakData.find(';', j);
	}
}
void SqliteController::pre_parsePeakDate() {

	sqlite3_exec(_ppDB, "BEGIN;", 0, 0, 0);

	int* x = new int[800];
	int* y = new int[800];

	const int totalCompounds = totalCompoundCounts();
	for (int compoundID = 1; compoundID < totalCompounds; compoundID++) {

		if (compoundID % 10000 == 0) { 
			std::cout << compoundID << " done." << std::endl;
		}

		Peak aPeak;
		getPeakData(compoundID, aPeak);
		const std::string strPeakData = aPeak._peakData;
		const int peakCount = aPeak._peakCount;

		pre_parsePeakDataString(strPeakData, peakCount, x, y);

		for (int peakIndex = 0; peakIndex < peakCount; peakIndex++) {
			PeakPoint aPoint;
			aPoint._compoundID = compoundID;
			aPoint._x = x[peakIndex];
			aPoint._y = y[peakIndex];
			storePeakData(aPoint);
		}
	}

	sqlite3_exec(_ppDB, "COMMIT;", 0, 0, 0);

	std::cout << "all " << totalCompounds << " done." << std::endl;
	delete [] y;
	delete [] x;
}
void SqliteController::parseCompoundIDs(const std::string &strCompoundIDs, int* compoundIDs) {

	std::string::size_type i = 0;
	std::string::size_type j = strCompoundIDs.find(',');

	while ( j != std::string::npos) {

		std::string strCompoundID = strCompoundIDs.substr(i, j-i);
		int compoundID = atoi(strCompoundID.c_str());

		if (compoundIDs[compoundID] == 0) {
			compoundIDs[compoundID] = 1;
		}
		if (compoundIDs[compoundID] == 1) {
			compoundIDs[compoundID] = 2;
			compoundIDs[0]++;
		}
			
		i = ++j;
		j = strCompoundIDs.find(',', j);
	}
}
// Dirty & Quick
std::vector<int> SqliteController::dq_getAllPeakCounts() {
	std::vector<int> counts;

	const std::string query = "SELECT PeakCount FROM CompoundInfo";
	sqlite3_stmt *statement;
	sqlite3_prepare_v2(_ppDB, query.c_str(), query.size(), &statement, NULL);

	
	while (sqlite3_step(statement) == SQLITE_ROW) {
		int count = sqlite3_column_int(statement, 0);
		counts.push_back(count);
	}

	sqlite3_finalize(statement);
	return counts;
}
void SqliteController::dq_pre_buildMassHash() {

	sqlite3_stmt* statement;
	
	sqlite3_prepare_v2(_ppDB, COUNT_MASS_ROWS, -1, &statement, NULL);
	int massCount = 0;
	if (sqlite3_step(statement) == SQLITE_ROW) {
		massCount = sqlite3_column_int(statement, 0);
	}
	sqlite3_finalize(statement);

	// 按升序填充 Mass 序号
	if (massCount == 0) {
		const std::string query = "INSERT OR REPLACE INTO [MassHash](Mass, IDs) VALUES (?, '')";
		sqlite3_prepare_v2(_ppDB, query.c_str(), query.size(), &statement, NULL);

		sqlite3_exec(_ppDB, "BEGIN;", 0, 0, 0);
		for (int i=1; i <= MAX_MASS; i++) {
			if (sqlite3_bind_int(statement, 1, i) == SQLITE_OK) {
				sqlite3_step(statement);
				sqlite3_reset(statement);
			}
		}
		sqlite3_exec(_ppDB, "COMMIT;", 0, 0, 0);
		sqlite3_finalize(statement);
		std::cout << "dq_pre_buildMassHash() -> OK" << std::endl;
	}

	// 填充 IDs 	
	
	sqlite3_stmt* statementPeakData;
	const std::string queryPeakData = "SELECT [compoundID] FROM [PeakData] WHERE x = ?";
	

	sqlite3_stmt* statementMassHash;
	const std::string insertMassHash = "INSERT OR REPLACE INTO [MassHash](Mass, IDs) VALUES (?, ?)";
	
	for (int massIndex = 1; massIndex <= MAX_MASS; massIndex++) {

		// 读取PeakData 中的对应关系
		int count = 0;
		double timeStart = (double)clock();
		double timeFinish = (double)clock();

		std::string compoundIDs = "";

		sqlite3_exec(_ppDB, "BEGIN;", 0, 0, 0);
		sqlite3_prepare_v2(_ppDB, queryPeakData.c_str(), queryPeakData.size(), &statementPeakData, NULL);
		if (sqlite3_bind_int(statementPeakData, 1, massIndex) == SQLITE_OK) {

			while (sqlite3_step(statementPeakData) == SQLITE_ROW) {

				//compoundIDs += (const char*)sqlite3_column_text(statementPeakData, 0); 
				//compoundIDs += ",";
				//count++;
			}
		}
		sqlite3_exec(_ppDB, "COMMIT;", 0, 0, 0);
		sqlite3_finalize(statementPeakData);

		// 存入MassHash
		sqlite3_prepare_v2(_ppDB, insertMassHash.c_str(), insertMassHash.size(), &statementMassHash, NULL);
		if ((sqlite3_bind_int(statementMassHash, 1, massIndex) == SQLITE_OK) &&
			(sqlite3_bind_text(statementMassHash, 2, compoundIDs.c_str(), -1, SQLITE_STATIC) == SQLITE_OK) ) {

				sqlite3_step(statementMassHash);
				sqlite3_finalize(statementMassHash);
		}

		
		std::cout << massIndex << " done ("<< count <<") "<< MAX_MASS - massIndex << " to go. =" << timeFinish-timeStart <<" ms" << std::endl;
		
	}
}
void SqliteController::dq_pre_buildCompound() {
	//读取所有 CompoundInfo 条目 解析计算出 MaxX 后，插入Compound 表内
	std::vector<Compound> compounds;
	//compounds.resize(COUNT_COMPOUNDS);

	sqlite3_stmt *statement;
	std::string query = "SELECT * FROM CompoundInfo ORDER BY CompoundID LIMIT ? OFFSET ?";
	sqlite3_prepare_v2(_ppDB, query.c_str(), query.size(), &statement, NULL);
	
	const int slice = 10000;
	const int pages = COUNT_COMPOUNDS / slice;
	for (int page = 0; page != pages + 1; page++) {

		sqlite3_exec(_ppDB, "BEGIN;", NULL, NULL, NULL);
		if ((sqlite3_bind_int(statement, 1, slice) == SQLITE_OK) &&
			(sqlite3_bind_int(statement, 2, slice * page) == SQLITE_OK)) {

				while (sqlite3_step(statement) == SQLITE_ROW) {
					Compound aCompound;
					aCompound._compoundID = sqlite3_column_int(statement, 0);
					aCompound._compoundName = (const char*)sqlite3_column_text(statement, 1);
					aCompound._formula = (const char*)sqlite3_column_text(statement, 2);
					aCompound._massWeight = sqlite3_column_int(statement, 3);
					aCompound._casNo = (const char*)sqlite3_column_text(statement, 4);
					aCompound._peakCount = sqlite3_column_int(statement, 5);
					aCompound._peakData = (const char*)sqlite3_column_text(statement, 7); // Col 6 = `Comment`
					aCompound._maxX = pre_parsePeakDataString(aCompound._peakData, aCompound._peakCount);
					compounds.push_back(aCompound);
				}
		}

		sqlite3_exec(_ppDB, "COMMIT;", 0, 0, 0);
		sqlite3_reset(statement);
	}

	// 存入 Compound 表
	query = "INSERT OR REPLACE INTO Compound ([CompoundID], [CompoundName], [Formula], [MassWeight], [CasNo], [PeakCount], [MaxX], [PeakData])"
			" VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
	sqlite3_prepare_v2(_ppDB, query.c_str(), query.size(), &statement, NULL);

	const size_t compoundSize = compounds.size();
	if (compoundSize != COUNT_COMPOUNDS - 2) { std::cout << "Compound Size goes wrong" << std::endl; }

	sqlite3_exec(_ppDB, "BEGIN;", NULL, NULL, NULL);
	for (size_t i = 0; i != compoundSize; i++) {
		if ((sqlite3_bind_int(statement,  1, compounds[i]._compoundID) == SQLITE_OK) &&
			(sqlite3_bind_text(statement, 2, compounds[i]._compoundName.c_str(), -1, SQLITE_STATIC) == SQLITE_OK) &&
			(sqlite3_bind_text(statement, 3, compounds[i]._formula.c_str(), -1, SQLITE_STATIC) == SQLITE_OK) &&
			(sqlite3_bind_int(statement,  4, compounds[i]._massWeight) == SQLITE_OK) &&
			(sqlite3_bind_text(statement, 5, compounds[i]._casNo.c_str(), -1, SQLITE_STATIC) == SQLITE_OK) &&
			(sqlite3_bind_int(statement,  6, compounds[i]._peakCount) == SQLITE_OK) &&
			(sqlite3_bind_int(statement,  7, compounds[i]._maxX) == SQLITE_OK) &&
			(sqlite3_bind_text(statement, 8, compounds[i]._peakData.c_str(), -1, SQLITE_STATIC) == SQLITE_OK)) {

				sqlite3_step(statement);
				sqlite3_reset(statement);
		}
	}
	sqlite3_exec(_ppDB, "COMMIT;", 0, 0, 0);
	
}
void SqliteController::dq_filterPeakByTwoMass(const Compound &aCompound, int* compoundIDs) {

	const std::string strPeakData = aCompound._peakData;
	const int peakCount = aCompound._peakCount;
	

	// 解析字符串 找出最大和次最大的Y值对应的X
	std::vector<PeakPoint> peakPoints;
	pre_parsePeakDataString(strPeakData, peakCount, peakPoints);
	if (peakPoints.size() < 2) { return; }

	nth_element(peakPoints.begin(), peakPoints.begin() + 2, peakPoints.end(), SqliteController::peakPointCompare); // 最大的2个数
	const int maxXSubXSize = 2;
	int maxXSubX[maxXSubXSize] = { peakPoints[0]._x, peakPoints[1]._x };

	sqlite3_stmt* statement;
	const std::string query = "SELECT IDs FROM MassHash WHERE Mass = ? LIMIT 1;"; 
	sqlite3_prepare_v2(_ppDB, query.c_str(), query.size(), &statement, NULL);

	// 选用std::set<int> 速度太慢且会在后期的造成 5s 的析构时间消耗
	// 直接开一个内存存放所有情况 //compoundIDs = new int[COUNT_COMPOUNDS];

	for (int i = 0; i != maxXSubXSize; i++) {
		if ((sqlite3_bind_int(statement, 1, maxXSubX[i]) == SQLITE_OK)) {
			if (sqlite3_step(statement) == SQLITE_ROW) {
				const std::string strCompoundIDs = (const char*)sqlite3_column_text(statement, 0);
				parseCompoundIDs(strCompoundIDs, compoundIDs);
				sqlite3_reset(statement);
			}
		}
	}


	//if ((sqlite3_bind_int(statement, 1, subX) == SQLITE_OK)) {
	//	if (sqlite3_step(statement) == SQLITE_ROW) {
	//		const std::string strCompoundIDs = (const char*)sqlite3_column_text(statement, 0);
	//		parseCompoundIDs(strCompoundIDs, compoundIDs);
	//		sqlite3_reset(statement);
	//	}
	//}

	sqlite3_finalize(statement);
}

