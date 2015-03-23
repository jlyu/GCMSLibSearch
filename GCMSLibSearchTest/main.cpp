#include <iostream>
#include <set>
#include <time.h>
#include <algorithm>
#include "../GCMSLibSearch/Compound.h"
#include "../GCMSLibSearch/SqliteController.h"
#include "../GCMSLibSearch/match.h"

void parseCompound(Compound& aCompound, unsigned int *x, float *y) {
	const std::string strPeakData = aCompound._peakData;
	const int peakCount = aCompound._peakCount;
	
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
	
	// 直接获取最大质量数
	aCompound._maxX = x[peakCount-1];

}
void parsePeak(Peak& aPeak, int aPeakMaxX, int* compoundIDs, int compoundIDindex,  unsigned int *x, float *y) {
	const std::string strPeakData = aPeak._peakData;
	const int peakCount = aPeak._peakCount;

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

	// 直接获取最大质量数
	int maxX = x[peakCount-1];
	if (maxX == aPeakMaxX) {
		//std::cout << compoundIDindex << " has MaxX = " << maxX << std::endl;
		if (compoundIDs[compoundIDindex + 1] == 2) { 
			compoundIDs[0]++;
		}
	}

	

}
void parsePeakData(const std::string& strPeakData, int peakCount, unsigned int *x, float *y) {
	
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
template <class T> void clearObject(T* obj) {
	T tmp;
	tmp.swap(*obj);
}

// Unit Test
bool test_totalCompoundCounts(SqliteController *pSqlController) {
	if (pSqlController) {
		std::cout << pSqlController->totalCompoundCounts() << std::endl;
		return true;
	} 
	return false;
}
bool test_maxPeakCount(SqliteController *pSqlController) {
	if (pSqlController) {
		std::cout << pSqlController->maxPeakCount() << std::endl;
		return true;
	} 
	return false;
}
bool test_getCompounds(SqliteController *pSqlController) {
	for (int page = 0; page < 300; page++) {
		int limit = 5;
		std::vector<Compound> compounds = pSqlController->getCompounds(123450 + page * limit, limit);

		for (size_t i = 0; i < compounds.size(); i++) {
			compounds[i].printBrief();
		}
		std::cout << "--- P" << page << "----------------" << std::endl; 
	}
	return true;
}
bool test_storeCompound(SqliteController *pSqlController) {
	Compound aCompound = pSqlController->getCompound(2);
	aCompound.print();
	std::cout << "---------------------" << std::endl; 
	aCompound._compoundID = 2;
	aCompound._compoundName = "CHain";
	aCompound._formula = "xxx";
	aCompound._massWeight = 233;
	aCompound._casNo = "7782-39-0";
	aCompound._peakCount = 3;
	aCompound._peakData = "2 55;4 222;5 111;";

	pSqlController->storeCompound(aCompound);
	aCompound = pSqlController->getCompound(2);
	aCompound.print();
	return true;
}
bool test_getPeakPoints(SqliteController *pSqlController) {

	double timeStart = (double)clock();
	
	std::vector<PeakPoint> peakPoints;
	pSqlController->dq_getPeakPoints(peakPoints);

	double timeFinish = (double)clock();
	std::cout << "1 times: dq_getPeakPoints() runtime: " << timeFinish - timeStart << " ms" << std::endl;
	return true;
}

void test_filterCompounds_v1(SqliteController *pSqlController, const Compound& testCompound, int *compoundIDs) {
	// -1 按 最大和次最大 Y 对应 X相等
	// -2 按 最大X相等
	static double filterTime = 0.0f;
	double timeStart = (double)clock();
	double timeFinish = (double)clock();

	timeStart = (double)clock();
	//int *compoundIDs = new int[COMPOUNDS_SIZES]();  // [0]存放个数
	pSqlController->dq_filterPeakByTwoMass(testCompound, compoundIDs);
	std::cout << "filterPeakByTwoMass:\t" << compoundIDs[0] << " to go" << std::endl;

	pSqlController->dq_filterPeakByMaxX(testCompound._maxX, compoundIDs);
	std::cout << "filterPeakByMaxX:\t" << compoundIDs[0] << " to go" << std::endl;

	timeFinish = (double)clock();
	filterTime += timeFinish - timeStart;
	std::cout << "FilterComps:\t"  << filterTime << std::endl;
}
void test_filterCompounds_v2(SqliteController *pSqlController, const Compound& testCompound, int *compoundIDs) {
	// 峰数 >=14 未知谱图中强度最大的14个峰（对应的X）要与谱库中谱图强度最大的14个峰（对应的X）相同
	// 
	//
	static double filterTime = 0.0f;
	double timeStart = (double)clock();
	double timeFinish = (double)clock();

	std::vector<FilterPoint> filterPoints;
	std::string strPeakData = testCompound._peakData;
	const int peakCount = testCompound._peakCount;
	filterPoints.resize(peakCount);
	pSqlController->pre_parsePeakDataString(strPeakData, peakCount, filterPoints);
	nth_element(filterPoints.begin(), filterPoints.begin() + peakCount, filterPoints.end(), SqliteController::filterPointCompare_Y);
	std::vector<FilterPoint> nthPoints;
	nthPoints.insert(nthPoints.end(), filterPoints.begin(), filterPoints.begin() + 8);
	
	//
	pSqlController->dq_filterPeakBy14(nthPoints, compoundIDs);


	timeFinish = (double)clock();
	filterTime += timeFinish - timeStart;
	std::cout << "FilterComps:\t"  << filterTime << std::endl;
}
void test_filterCompounds_v3(SqliteController *pSqlController, const Compound& unknownCompound, int *compoundIDs) {

	// 未知峰 >=8, 未知峰 1-8 的X 对应落在 YrX排序后 8~15 个谱库峰的 X 范围内，范围外则滤除

	std::string strPeakData = unknownCompound._peakData;
	const int peakCount = unknownCompound._peakCount;
	if (peakCount < 8) return;
	
	static double filterTime = 0.0f;
	double timeStart = (double)clock();
	double timeFinish = (double)clock();

	// 从未知峰依次（1-8个峰）取上下限
	const int filterPeakLimitNumbers = 8; //
	std::vector<FilterPoint> unknownPeakPoints; 
	unknownPeakPoints.resize(peakCount);
	pSqlController->pre_parsePeakDataString(strPeakData, peakCount, unknownPeakPoints);
	nth_element(unknownPeakPoints.begin(), 
		unknownPeakPoints.begin() + filterPeakLimitNumbers, 
		unknownPeakPoints.end(), 
		SqliteController::filterPointCompare_YrX);

	std::vector<FilterPoint> nthPoints;
	nthPoints.insert(nthPoints.end(), unknownPeakPoints.begin(), unknownPeakPoints.begin() + filterPeakLimitNumbers);
	//
	pSqlController->dq_filterPeakBy08(nthPoints, compoundIDs);
	
	timeFinish = (double)clock();
	filterTime += timeFinish - timeStart;
	//std::cout << "FilterComps:\t"  << filterTime << std::endl;
}
bool test_diffSpectrum_v1(SqliteController *pSqlController, int times) {
	//批量从CompoundInfo表内读数据，解析字符串

	//const int limit = 100000;
	//const int pages = times / limit;
	//for (int page = 0; page < pages; page++) {
	//	std::cout << "range: [" << 1 + page * limit <<" ~ " <<1 + page * limit + limit << "]" << std::endl;

	//	double timeStart = (double)clock();

	//	std::vector<Peak> peaks = pSqlController->getPeakDatas(1 + page * limit, limit);

	//	double timeFinish = (double)clock();
	//	sqliteTime += timeFinish - timeStart;

	//	const size_t peakSize = peaks.size();
	//	for (size_t i = 0; i < peakSize; i++) {

	//		const int libPeakCount = peaks[i]._peakCount;
	//		const std::string strLibPeakData = peaks[i]._peakData;

	//		timeStart = (double)clock();
	//		parsePeakData(strLibPeakData, libPeakCount, libMass, libAbundance);
	//		timeFinish = (double)clock();
	//		parseTime += timeFinish - timeStart;

	//		timeStart = (double)clock();
	//		unsigned int matchedDegree = DiffSpectrum(matchMass, matchAbundance, matchPeakCount,
	//			libMass, libAbundance, libPeakCount);
	//		timeFinish = (double)clock();
	//		diffSpectrumTime += timeFinish - timeStart;
	//	}
	//}

	return false;
}
bool test_diffSpectrum_v2(SqliteController *pSqlController, int times) {
	//// 不解析字符串，从PeakData中读取xy对应数据，单次读
	//timeStart = (double)clock();
	//const int libPeakCount = peakCounts[compoundID - 1]; //TODO: DQ
	//pSqlController->getPeakPoints(compoundID, libPeakCount ,libMass, libAbundance);
	//timeFinish = (double)clock();
	//sqliteTime += timeFinish - timeStart;

	//timeStart = (double)clock();
	//unsigned int matchedDegree = DiffSpectrum(matchMass, matchAbundance, matchPeakCount,
	//	libMass, libAbundance, libPeakCount);
	//timeFinish = (double)clock();
	//diffSpectrumTime += timeFinish - timeStart;
	return false;
}
void test_diffSpectrum_v3(SqliteController *pSqlController, int compoundID) {
	
	// 利用 x-ids 哈希表先过滤化合物 然后单条查询 解析 计算

	static double diffSpectrumTime = 0.0f;
	static double parseTime = 0.0f;
	static double sqliteTime = 0.0f;
	
	double timeStart = (double)clock();
	double timeFinish = (double)clock();

	// 【match compound】
	Compound testCompound = pSqlController->getCompound(compoundID); //190790
	const int matchPeakCount = testCompound._peakCount;
	unsigned int* matchX = new unsigned int[matchPeakCount];
	float* matchY = new float[matchPeakCount];
	parseCompound(testCompound, matchX, matchY);
	
	// 【filtered compounds】
	int *compoundIDs = new int[MAX_COMPOUND_ID + 1](); // [0]存放个数
	test_filterCompounds_v3(pSqlController, testCompound, compoundIDs);

	
	// lib compound
	const int maxPeakCount = 800; //pSqlController->maxPeakCount(); 
	unsigned int* libX = new unsigned int[maxPeakCount];
	float* libY = new float[maxPeakCount];

	// Search
	timeStart = (double)clock();
	std::vector<Peak> peaks;
	pSqlController->dq_getPeakDatas_v2(compoundIDs, peaks); 
	timeFinish = (double)clock();
	sqliteTime += timeFinish - timeStart;

	////typedef std::vector<Peak>::iterator ITER;
	////for(ITER it = peaks.begin(); it != peaks.end(); it++) {

	const size_t peakSize = peaks.size();
	for (size_t i = 0; i != peakSize; i++) {

		// Parse String
		timeStart = (double)clock();
		parsePeakData(peaks[i]._peakData, peaks[i]._peakCount, libX, libY);
		timeFinish = (double)clock();
		parseTime += timeFinish - timeStart;
	////	
		// Diff Algorithm
		timeStart = (double)clock();
		unsigned int matchedDegree = DiffSpectrum(matchX, matchY, matchPeakCount, libX, libY, peaks[i]._peakCount);
		timeFinish = (double)clock();
		diffSpectrumTime += timeFinish - timeStart;
	}

	
	std::cout << "CalMatchRate:\t" << diffSpectrumTime << std::endl;
	std::cout << "ParseStrings:\t" << parseTime << std::endl;
	std::cout << "SQLiteSearch:\t" << sqliteTime << std::endl;


	peaks.clear(); //TODO:

	delete [] compoundIDs;
	delete [] libY;
	delete [] libX;
	delete [] matchY;
	delete [] matchX;
}
void test_diffSpectrum_v4(SqliteController *pSqlController) {
	// 无过滤 利用PeakData的索引表直接对所有数据一一 计算

	static double diffSpectrumTime = 0.0f;
	static double parseTime = 0.0f;
	static double sqliteTime = 0.0f;
	static double filterTime = 0.0f;
	double timeStart = (double)clock();
	double timeFinish = (double)clock();

	// 【match compound】
	Compound testCompound = pSqlController->getCompound(4); //190790
	const int matchPeakCount = testCompound._peakCount;
	unsigned int* matchX = new unsigned int[matchPeakCount];
	float* matchY = new float[matchPeakCount];
	const std::string strPeakData = testCompound._peakData;
	parsePeakData(strPeakData, matchPeakCount, matchX, matchY);

	// lib compound
	const int maxPeakCount = 800; //pSqlController->maxPeakCount();
	unsigned int* libX = new unsigned int[maxPeakCount];
	float* libY = new float[maxPeakCount];

	
	//for(int it = 1; it != 10000; it++) { //191436

		//const int compoundID = it;

		// Search
		timeStart = (double)clock();

//		std::vector<PeakXY> peakXYs;
//		pSqlController->dq_getPeakPoints(4, peakXYs);
		//pSqlController->getPeakPoints(4, libX, libY);
		 
		timeFinish = (double)clock();
		sqliteTime += timeFinish - timeStart;

		// Parse String
		timeStart = (double)clock();
		//parsePeakData(aPeak._peakData, aPeak._peakCount, libX, libY);
		timeFinish = (double)clock();
		parseTime += timeFinish - timeStart;

		// Diff Algorithm
		timeStart = (double)clock();
		//unsigned int matchedDegree = DiffSpectrum(matchX, matchY, matchPeakCount, libX, libY, aPeak._peakCount);
		timeFinish = (double)clock();
		diffSpectrumTime += timeFinish - timeStart;
	//}

	delete [] libY;
	delete [] libX;
	delete [] matchY;
	delete [] matchX;

	std::cout << "FilterComps:\t"  << filterTime << std::endl;
	std::cout << "CalMatchRate:\t" << diffSpectrumTime << std::endl;
	std::cout << "ParseStrings:\t" << parseTime << std::endl;
	std::cout << "SQLiteSearch:\t" << sqliteTime << std::endl;
}



int main() {
	
	 
	SqliteController nistController("../nist.db");
	std::cout << "-" <<std::endl;

	int maxIndex = 0;
	double maxRunTime = 0.0f;

	for (int i = 11900; i < MAX_COMPOUND_ID; i++) {

		double timeStart = (double)clock();

		test_diffSpectrum_v3(&nistController, i); 

		double timeFinish = (double)clock(); //结束时间

		if (maxRunTime < (timeFinish - timeStart)) {
			maxRunTime = (timeFinish - timeStart);
			maxIndex = i;
		}
		std::cout << "CompoundID:\t" << i << "\t runtime: "<< (timeFinish - timeStart) << "\t max: "<< maxRunTime << "  id: "<< maxIndex << std::endl;

	}
	
	  

//	std::cout << "TotalRun:\t" << (timeFinish - timeStart) << std::endl;
	system("PAUSE");
	return 0;
}