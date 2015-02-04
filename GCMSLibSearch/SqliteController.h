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



#define PREPARED_STATEMENT_COUNT 7



class SqliteController {

public:
	SqliteController(const std::string &file);
	~SqliteController(void);

//接口
	// 预处理
	void SqliteController::preproccess();

	// 建
	void SqliteController::createPeakDataTable();

	// 查
	int SqliteController::totalCompoundCounts(); //化合物总数
	int SqliteController::maxPeakCount(); //库内最大峰个数
	Compound SqliteController::getCompound(int compoundID); //按ID获得对应化合物
	Peak SqliteController::getPeakData(int compoundID); //按ID获得对应化合物丰度数据
	std::vector<Compound> SqliteController::getCompounds(int startCompoundID, int limit); //按起始ID及limit 获取对应化合物

	// 增/改
	void SqliteController::storeCompound(const Compound& aCompound); //插入或替代化合物

	// 删
	//bool hasCompoundRecords(const std::string &tableName = TABLE_COMPOUND_INFO); //检验数据库表内是否存在化合物数据



	void queryCompoundData(std::vector<Compound> &selectedCompounds);
	

private:
	bool SqliteController::init_openSQLite(const std::string &file);
	int SqliteController::query_aSingleCount(sqlite3_stmt* pStatement);

private:
	sqlite3* _ppDB;
	sqlite3_stmt* _pStatements[PREPARED_STATEMENT_COUNT];
};

