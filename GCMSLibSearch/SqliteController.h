#pragma once

//#include "StdAfx.h" //
#include "sqlite3.h"
#include "Compound.h"
#include <vector>

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "sqlite3_D.lib")
#else
#pragma comment(lib, "sqlite3_R.lib")
#endif



#define PREPARED_STATEMENT_COUNT 5



class SqliteController {

public:
	SqliteController(const std::string &file);
	~SqliteController(void);

//接口
	int SqliteController::totalCompoundCounts(); //化合物总数
	int SqliteController::maxPeakCount(); //库内最大峰个数
	Compound SqliteController::getCompound(int compoundID); //按ID获得对应化合物
	std::vector<Compound> SqliteController::getCompounds(int startCompoundID, int limit); //按起始ID及limit 获取对应化合物
	void SqliteController::storeCompound(const Compound& aCompound); //插入或替代化合物

	//删除化合物

	//bool hasCompoundRecords(const std::string &tableName = TABLE_COMPOUND_INFO); //检验数据库表内是否存在化合物数据



	void queryCompoundData(std::vector<Compound> &selectedCompounds);
	

private:
	bool init_openSQLite(const std::string &file);


private:
	sqlite3* _ppDB;
	sqlite3_stmt* _pStatements[PREPARED_STATEMENT_COUNT];
};

