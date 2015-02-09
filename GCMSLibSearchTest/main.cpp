#include <iostream>
#include <set>
#include <time.h>
#include "../GCMSLibSearch/Compound.h"
#include "../GCMSLibSearch/SqliteController.h"
#include "../GCMSLibSearch/match.h"

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
void test_diffSpectrum_v3(SqliteController *pSqlController) {
	
	// 利用 x-ids 哈希表先过滤化合物 然后单条查询 解析 计算

	static double diffSpectrumTime = 0.0f;
	static double parseTime = 0.0f;
	static double sqliteTime = 0.0f;
	static double filterTime = 0.0f;
	double timeStart = (double)clock();
	double timeFinish = (double)clock();

	// 【match compound】
	Compound testCompound = pSqlController->getCompound(6); //190790
	const int matchPeakCount = testCompound._peakCount;
	unsigned int* matchX = new unsigned int[matchPeakCount];
	float* matchY = new float[matchPeakCount];
	const std::string strPeakData = testCompound._peakData;
	parsePeakData(strPeakData, matchPeakCount, matchX, matchY);

	// 【filtered compounds】
	timeStart = (double)clock();
	std::set<int> compoundIDsSet;
	
	pSqlController->dq_filterPeakByTwoMass(testCompound, compoundIDsSet);
	std::cout << compoundIDsSet.size() << " Found. Searching .." << std::endl;
	timeFinish = (double)clock();
	filterTime += timeFinish - timeStart;
	

	// lib compound
	const int maxPeakCount = 800; //pSqlController->maxPeakCount();
	unsigned int* libX = new unsigned int[maxPeakCount];
	float* libY = new float[maxPeakCount];

	typedef std::set<int>::iterator ITER;
	for(ITER it = compoundIDsSet.begin(); it != compoundIDsSet.end(); it++) {
		const int compoundID = *it;

		// Search
		timeStart = (double)clock();
		Peak aPeak;
		pSqlController->getPeakData(compoundID, aPeak); 
		timeFinish = (double)clock();
		sqliteTime += timeFinish - timeStart;

		// Parse String
		timeStart = (double)clock();
		parsePeakData(aPeak._peakData, aPeak._peakCount, libX, libY);
		timeFinish = (double)clock();
		parseTime += timeFinish - timeStart;

		// Diff Algorithm
		timeStart = (double)clock();
		//unsigned int matchedDegree = DiffSpectrum(matchX, matchY, matchPeakCount, libX, libY, aPeak._peakCount);
		timeFinish = (double)clock();
		diffSpectrumTime += timeFinish - timeStart;
	}

	delete [] libY;
	delete [] libX;
	delete [] matchY;
	delete [] matchX;

	std::cout << "FilterComps:\t"  << filterTime << std::endl;
	std::cout << "CalMatchRate:\t" << diffSpectrumTime << std::endl;
	std::cout << "ParseStrings:\t" << parseTime << std::endl;
	std::cout << "SQLiteSearch:\t" << sqliteTime << std::endl;

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
	Compound testCompound = pSqlController->getCompound(6); //190790
	const int matchPeakCount = testCompound._peakCount;
	unsigned int* matchX = new unsigned int[matchPeakCount];
	float* matchY = new float[matchPeakCount];
	const std::string strPeakData = testCompound._peakData;
	parsePeakData(strPeakData, matchPeakCount, matchX, matchY);

	// lib compound
	const int maxPeakCount = 800; //pSqlController->maxPeakCount();
	unsigned int* libX = new unsigned int[maxPeakCount];
	float* libY = new float[maxPeakCount];

	
	for(int it = 1; it != 191436; it++) {

		const int compoundID = it;

		// Search
		timeStart = (double)clock();

		pSqlController->getPeakPoints(compoundID, libX, libY);
		 
		timeFinish = (double)clock();
		sqliteTime += timeFinish - timeStart;

		//// Parse String
		//timeStart = (double)clock();
		//parsePeakData(aPeak._peakData, aPeak._peakCount, libX, libY);
		//timeFinish = (double)clock();
		//parseTime += timeFinish - timeStart;

		// Diff Algorithm
		timeStart = (double)clock();
		//unsigned int matchedDegree = DiffSpectrum(matchX, matchY, matchPeakCount, libX, libY, aPeak._peakCount);
		timeFinish = (double)clock();
		diffSpectrumTime += timeFinish - timeStart;
	}

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
	
	double timeStart = (double)clock(); 

	SqliteController sqlController("../ms.db");
	std::cout << "-" <<std::endl;

	test_diffSpectrum_v4(&sqlController); // 293-297s -> 279-325s -> 297-323s

	double timeFinish = (double)clock(); //结束时间
	std::cout << "TotalRun:\t" << (timeFinish - timeStart) << std::endl;
	system("PAUSE");
	return 0;
}