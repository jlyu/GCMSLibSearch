#include <iostream>
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
bool test_diffSpectrum(SqliteController *pSqlController, int times) {

	static double diffSpectrumTime = 0.0f;
	static double parseTime = 0.0f;
	static double sqliteTime = 0.0f;

	// match compound
	Compound testCompound = pSqlController->getCompound(4); //190790
	const int matchPeakCount = testCompound._peakCount;
	unsigned int* matchMass = new unsigned int[matchPeakCount];
	float* matchAbundance = new float[matchPeakCount];
	const std::string strPeakData = testCompound._peakData;
	parsePeakData(strPeakData, matchPeakCount, matchMass, matchAbundance);

	// lib compound
	const int maxPeakCount = pSqlController->maxPeakCount();
	unsigned int* libMass = new unsigned int[maxPeakCount];
	float* libAbundance = new float[maxPeakCount];
	
	const int limit = 10000;
	const int page = times / limit;
	for (int i = 1; i < page; i++) {
		
		double timeStart = (double)clock();

		std::vector<Peak> peaks = pSqlController->getPeakDatas(1 + page * limit, limit);
		Peak libPeak = pSqlController->getPeakData(i);
		const std::string strLibPeakData = libPeak._peakData;
		const int libPeakCount = libPeak._peakCount;

		double timeFinish = (double)clock();
		sqliteTime += timeFinish - timeStart;

		const size_t peakSize = peaks.size();
		for (size_t i = 0; i < peakSize; i++) {

			const int libPeakCount = peaks[i]._peakCount;
			const std::string strLibPeakData = peaks[i]._peakData;

			timeStart = (double)clock();
			parsePeakData(strLibPeakData, libPeakCount, libMass, libAbundance);
			timeFinish = (double)clock();
			parseTime += timeFinish - timeStart;
			
			timeStart = (double)clock();
			unsigned int matchedDegree = DiffSpectrum(matchMass, matchAbundance, matchPeakCount,
														libMass, libAbundance, libPeakCount);
			timeFinish = (double)clock();
			diffSpectrumTime += timeFinish - timeStart;


		}
	}

	delete [] libAbundance;
	delete [] libMass;
	delete [] matchAbundance;
	delete [] matchMass;

	std::cout << "diffSpectrumTime: " << diffSpectrumTime << std::endl;
	std::cout << "parseTime: " << parseTime << std::endl;
	std::cout << "sqliteTime: " << sqliteTime << std::endl;
	
	return true;
}

int main() {
	
	double timeStart = (double)clock(); 

	SqliteController sqlController("../ms.db");
	

	//test_diffSpectrum(&sqlController, 10000); // 4200ms
	test_diffSpectrum(&sqlController, 100000); // 293-297s -> 279-325s -> 297-323s
	//test_maxPeakCount(&sqlController); //1233


	double timeFinish = (double)clock(); //Ω· ¯ ±º‰
	std::cout << "run time: " << (timeFinish - timeStart) << std::endl;
	system("PAUSE");
	return 0;
}