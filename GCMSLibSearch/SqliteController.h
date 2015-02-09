#pragma once

//#include "StdAfx.h" //
#include "sqlite3.h"
#include "Compound.h"
#include <vector>
#include <set>

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "sqlite3_D.lib")
#else
#pragma comment(lib, "sqlite3_R.lib")
#endif


#define PREPARED_STATEMENT_COUNT 9


class SqliteController {

public:
	SqliteController(const std::string &file);
	~SqliteController(void);

//接口
	// 预处理
	void SqliteController::preproccess();

	// 建
	void SqliteController::createPeakDataTable(); // +Index
	void SqliteController::dq_createMassHashTable(); // +Index

	// 查
	int SqliteController::totalCompoundCounts(); //化合物总数
	int SqliteController::maxPeakCount(); //库内最大峰个数
	Compound SqliteController::getCompound(int compoundID); //按ID获得对应化合物
	std::vector<Compound> SqliteController::getCompounds(int startCompoundID, int limit); //按起始ID及limit 获取对应化合物
	void SqliteController::getPeakData(int compoundID, Peak &aPeak); //按ID获得对应化合物丰度数据
	std::vector<Peak> SqliteController::getPeakDatas(int startCompoundID, int limit);
	void SqliteController::dq_getPeakDatas(std::set<int> &compoundIDsSet, std::vector<Peak>& peaks);
	void SqliteController::dq_getPeakDatas_v2(std::set<int> &compoundIDsSet, std::vector<Peak>& peaks);
	void SqliteController::getPeakPoints(int compoundID, /*int pointCount,*/ unsigned int* x, float* y);
	
	void SqliteController::dq_getPeakPoints(/*std::set<int> &compoundIDsSet,*/ std::vector<PeakPoint>& peakPoints);

	// 增/改
	void SqliteController::storeCompound(const Compound& aCompound); //插入或替代化合物
	void SqliteController::storePeakData(const PeakPoint& aPoint);

	// 删
	//bool hasCompoundRecords(const std::string &tableName = TABLE_COMPOUND_INFO); //检验数据库表内是否存在化合物数据



	void queryCompoundData(std::vector<Compound> &selectedCompounds);
	
	//Dirty & Quick
	std::vector<int> SqliteController::dq_getAllPeakCounts();
	void SqliteController::dq_filterPeakByTwoMass(const Compound &aCompound, std::set<int> &compoundIDsSet);

	void SqliteController::dq_pre_buildMassHash();

private:
	bool SqliteController::init_openSQLite(const std::string &file);
	bool SqliteController::checkConnectionError();

	int  SqliteController::query_aSingleCount(sqlite3_stmt* pStatement);
	// Parse TODO： 合成一个函数
	void SqliteController::pre_parsePeakDate(); //把CompoundInfo表的PeakData字段数据解析存入PeakData表内
	void SqliteController::pre_parsePeakDataString(const std::string& strPeakData, int peakCount, int *x, int *y);
	void SqliteController::pre_parsePeakDataString(const std::string& strPeakData, int peakCount, std::vector<PeakPoint> &peakPoints);
	void SqliteController::parseCompoundIDs(const std::string &strCompoundIDs, std::set<int> &compoundIDs);

	static bool peakPointCompare(const PeakPoint &p1, const PeakPoint &p2) {  
		return p1._y > p2._y;  
	}  
	

private:
	sqlite3* _ppDB;
	sqlite3_stmt* _pStatements[PREPARED_STATEMENT_COUNT];
};

