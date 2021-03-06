#pragma once
//#include "StdAfx.h" //
#include "sqlite3.h"
#include "Compound.h"

#include <vector>
#include <set>
#include <stdarg.h>

//#if defined(DEBUG) || defined(_DEBUG)
//#pragma comment(lib, "sqlite3_D.lib")
//#else
//#pragma comment(lib, "sqlite3_R.lib")
//#endif


//
#define COUNT_COMPOUNDS 191436
#define COMPOUNDS_SIZES 191437
#define MAX_COMPOUND_ID 191438


// 导出 DLL 类
//#define LIBMANAGER_EXPORTS
//#ifdef LIBMANAGER_EXPORTS
//#define LIBMANAGER_API __declspec(dllexport)
//#else
//#define LIBMANAGER_API __declspec(dllimport) 
//#endif

class /*LIBMANAGER_API*/ SqliteController {
private:
	std::wstring SqliteController::acsii2wideByte(std::string& strascii);
	std::string SqliteController::unicode2utf8(const std::wstring& wString);
	std::string SqliteController::ascii2utf8(std::string& strAsciiCode);

public:
	SqliteController(const std::string &file);
	~SqliteController(void);


	//dll接口
	void SqliteController::libSearch(const LibConfig& libConfig, Compound testCompound, std::vector<Compound> &libCompounds);


	// 预
	bool SqliteController::checkConnectionError();
	void SqliteController::pre_proccess();
	bool SqliteController::initTables();

	// 建
	void SqliteController::createTable(const char* table, int indexNumber, ...); // 可带多个 index

	// 查
	void SqliteController::getPeaksByCompoundIDs(int*, std::vector<Peak>& peaks);
	int SqliteController::totalCompoundCounts(); //化合物总数
	int SqliteController::maxCompoundID();
	int SqliteController::maxPeakCount(); //库内最大峰个数
	void SqliteController::queryCompoundData(std::vector<Compound> &selectedCompounds);
	void SqliteController::getPeakData(int compoundID, Peak &aPeak); //按ID获得对应化合物丰度数据
	std::vector<Peak> getPeakDatas(int startCompoundID, int limit);
	void SqliteController::dq_getPeakDatas_v3(int*, std::vector<Peak>& peaks);
	void SqliteController::getPeakPoints(int compoundID, unsigned int* x, float* y);
	void SqliteController::dq_getPeakPoints(std::vector<PeakPoint>& peakPoints);
	Compound SqliteController::getCompound(int compoundID); //按ID获得对应化合物
	std::vector<Compound> SqliteController::getCompounds(int startCompoundID, int limit); //按起始ID及limit 获取对应化合物
	std::vector<Compound> SqliteController::getCompounds(const SearchPara& searchPara);
	int SqliteController::countCompounds(const SearchPara& searchPara);
	// 滤
	void SqliteController::_filterPeakBy08(const std::vector<FilterPoint> &filterPoints, int* compoundIDs);
	void SqliteController::filterCompounds(const Compound& testCompound, int *compoundIDs);
	void SqliteController::filterCompounds_C(const Compound& testCompound, int *compoundIDs);
	void SqliteController::filterCompounds_D(const Compound& testCompound, int *compoundIDs);

	// 增/改
	void SqliteController::storePeakData(const PeakPoint& aPoint);
	void SqliteController::storeCompound(const Compound& aCompound); //插入或替代化合物
	void SqliteController::storeFiltePoint(const Compound& aCompound); //storeCompound后关联索引表（TODO: 考虑2者合并用事物）

	// 删
	void SqliteController::deleteCompound(const int compoundID);
	//void SqliteController::deleteFilters(const int compoundID);
	
	
	//Dirty & Quick
	std::vector<int> SqliteController::dq_getAllPeakCounts();
	void SqliteController::dq_pre_getCompounds(std::vector<Compound> &compounds);
	void SqliteController::dq_filterPeakByTwoMass(const Compound &aCompound, int* compoundIDs);
	void SqliteController::dq_filterPeakByMaxX(const int maxX, int* compoundIDs);
	void SqliteController::dq_filterPeakBy14(const std::vector<FilterPoint> &filterPoints, int* compoundIDs);
	
	void SqliteController::dq_filterCompounds(const Compound& unknownCompound, int *compoundIDs);
	void SqliteController::dq_pre_buildMassHash();
	void SqliteController::dq_pre_buildCompound(std::vector<Compound> &compounds);
	void SqliteController::dq_pre_buildFilter();
	bool SqliteController::init_openSQLite(const std::string &file);
	int  SqliteController::query_aSingleCount(sqlite3_stmt* pStatement);
	// Parse TODO： 合成一个函数
	void SqliteController::pre_parsePeakDate(); //把CompoundInfo表的PeakData字段数据解析存入PeakData表内
	void SqliteController::pre_parsePeakDataString(const std::string& strPeakData, int peakCount, int *x, int *y);
	void SqliteController::pre_parsePeakDataString(const std::string& strPeakData, int peakCount, std::vector<PeakPoint> &peakPoints);
	void SqliteController::pre_parsePeakDataString(const std::string& strPeakData, int peakCount, std::vector<FilterPoint> &filterPoints);
	//int  SqliteController::pre_parsePeakDataString(const std::string& strPeakData, int peakCount);
	void SqliteController::parseCompoundIDs(const std::string &strCompoundIDs, int* compoundIDs);
	void SqliteController::parseCompound(Compound& aCompound, unsigned int *x, float *y);
	void SqliteController::parsePeakData(const std::string& strPeakData, int peakCount, unsigned int *x, float *y);
	
	// 
	static bool peakCompare_MatchDegree(const Peak &p1, const Peak &p2) { return p1._matchDegree > p2._matchDegree; }
	static bool peakPointCompare_Y(const PeakPoint &p1, const PeakPoint &p2) {  return p1._y > p2._y; }  
	static bool filterPointCompare_X(const FilterPoint &p1, const FilterPoint &p2) {  return p1._peakPoint._x > p2._peakPoint._x; } 
	static bool filterPointCompare_Y(const FilterPoint &p1, const FilterPoint &p2) {  return p1._peakPoint._y > p2._peakPoint._y; } 
	static bool filterPointCompare_YrX(const FilterPoint &p1, const FilterPoint &p2) {  return p1._yrx > p2._yrx; } 
	 

	sqlite3* _ppDB;


	
};


/* 数据库结构

Compound 表 
	CompoundID		INTEGER		INTEGER	0	0	False				0	0
	CompoundName	CHAR		CHAR	0	0	False				1	1
	Formula			CHAR(255)	CHAR	255	0	False				2	2
	MassWeight		INTEGER		INTEGER	0	0	False				3	3
	CasNo			CHAR(255)	CHAR	255	0	False				4	4
	PeakCount		INTEGER		INTEGER	0	0	False				5	5
	MaxX			INTEGER		INTEGER	0	0	False				6	6
	PeakData		CHAR		CHAR	0	0	False				7	7

Filter 表 (Rank 存 1-9 即可)
	CompoundID	INTEGER	INTEGER	0	0	False				0	0
	X			INTEGER	INTEGER	0	0	False				1	1
	Y			INTEGER	INTEGER	0	0	False				2	2
	YrX			INTEGER	INTEGER	0	0	False				3	3
	Rank		INTEGER	INTEGER	0	0	False				4	4

FilterXY 表
	CompoundID	INTEGER	INTEGER	0	0	False				0	0
	X			INTEGER	INTEGER	0	0	False				1	1
	Y			INTEGER	INTEGER	0	0	False				2	2
	Rank		INTEGER	INTEGER	0	0	False				3	3

*/
