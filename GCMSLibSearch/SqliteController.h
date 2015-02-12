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


//
#define COUNT_COMPOUNDS 191438
#define COMPOUNDS_SIZES 191439

class SqliteController {

public:
	SqliteController(const std::string &file);
	~SqliteController(void);

//接口
	// 预
	void SqliteController::pre_proccess();

	// 建
	void SqliteController::createTable(const char* table, const char* index);

	// 查
	int SqliteController::totalCompoundCounts(); //化合物总数
	int SqliteController::maxPeakCount(); //库内最大峰个数
	Compound SqliteController::getCompound(int compoundID); //按ID获得对应化合物
	std::vector<Compound> SqliteController::getCompounds(int startCompoundID, int limit); //按起始ID及limit 获取对应化合物
	void SqliteController::queryCompoundData(std::vector<Compound> &selectedCompounds);
	void SqliteController::getPeakData(int compoundID, Peak &aPeak); //按ID获得对应化合物丰度数据
	std::vector<Peak> getPeakDatas(int startCompoundID, int limit);
	void SqliteController::dq_getPeakDatas(std::set<int> &compoundIDsSet, std::vector<Peak>& peaks);
	void SqliteController::dq_getPeakDatas_v2(int*, std::vector<Peak>& peaks);
	void SqliteController::dq_getPeakDatas_v3(int*, std::vector<Peak>& peaks);
	void SqliteController::getPeakPoints(int compoundID, unsigned int* x, float* y);
	void SqliteController::dq_getPeakPoints(std::vector<PeakPoint>& peakPoints);

	// 增/改
	void SqliteController::storeCompound(const Compound& aCompound); //插入或替代化合物
	void SqliteController::storePeakData(const PeakPoint& aPoint);

	// 删
	
	
	//Dirty & Quick
	std::vector<int> SqliteController::dq_getAllPeakCounts();
	void SqliteController::dq_pre_getCompoundInfo(std::vector<Compound> &compounds);

	void SqliteController::dq_filterPeakByTwoMass(const Compound &aCompound, int* compoundIDs);
	void SqliteController::dq_filterPeakByMaxX(const int maxX, int* compoundIDs);

	void SqliteController::dq_pre_buildMassHash();
	void SqliteController::dq_pre_buildCompound(std::vector<Compound> &compounds);
	void SqliteController::dq_pre_buildFilter();

	bool SqliteController::init_openSQLite(const std::string &file);
	bool SqliteController::checkConnectionError();
	int  SqliteController::query_aSingleCount(sqlite3_stmt* pStatement);
	// Parse TODO： 合成一个函数
	void SqliteController::pre_parsePeakDate(); //把CompoundInfo表的PeakData字段数据解析存入PeakData表内
	void SqliteController::pre_parsePeakDataString(const std::string& strPeakData, int peakCount, int *x, int *y);
	void SqliteController::pre_parsePeakDataString(const std::string& strPeakData, int peakCount, std::vector<PeakPoint> &peakPoints);
	int  SqliteController::pre_parsePeakDataString(const std::string& strPeakData, int peakCount);
	void SqliteController::parseCompoundIDs(const std::string &strCompoundIDs, int* compoundIDs);


private:
	sqlite3* _ppDB;

	static bool peakPointCompare(const PeakPoint &p1, const PeakPoint &p2) {  return p1._y > p2._y; }  
};

