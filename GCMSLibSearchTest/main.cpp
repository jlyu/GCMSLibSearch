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
/************************************************************************/
/* 功能：谱图对比
   参数：
   Mass_Match：待匹配质量数[数组]
   AjustedAbundance_Match：待匹配质量数丰度[数组]
   MassCount_Match：待匹配峰个数

   Mass_Lib：库谱图质量数[数组]
   AjustedAbundance_Lib：库谱图质量数丰度[数组]
   MassCount_Lib：库谱图峰个数
   返回值：
   -1：匹配失败
   说明：输入的Mass_Match和Mass_Lib需要是质量数从小到大排列。
*/
/************************************************************************/
	Compound testCompound = pSqlController->getCompound(4); //190790

	const int matchPeakCount = testCompound._peakCount;
	unsigned int* matchMass = new unsigned int[matchPeakCount];
	float* matchAbundance = new float[matchPeakCount];
	const std::string strPeakData = testCompound._peakData;
	parsePeakData(strPeakData, matchPeakCount, matchMass, matchAbundance);

	// test -10000 times
	const int maxPeakCount = pSqlController->maxPeakCount();
	unsigned int* libMass = new unsigned int[maxPeakCount];
	float* libAbundance = new float[maxPeakCount];

	const int limit = 100;
	const int pages = times / limit;
	for (int page = 0; page < pages; page++) {
		
		std::vector<Compound> compounds = pSqlController->getCompounds(1 + page * limit, limit);

		for (size_t i = 0; i < compounds.size(); i++) {
			const int libPeakCount = compounds[i]._peakCount;
			
			const std::string strLibPeakData = compounds[i]._peakData;
			parsePeakData(strLibPeakData, libPeakCount, libMass, libAbundance);

			unsigned int matchedDegree = DiffSpectrum(matchMass, matchAbundance, matchPeakCount,
														libMass, libAbundance, libPeakCount);

			//std::cout << "[" << compounds[i]._compoundID << "]: " << matchedDegree << std::endl;


			//delete [] libMass;
			//delete [] libAbundance;
		}
	}

	delete [] libAbundance;
	delete [] libMass;
	delete [] matchAbundance;
	delete [] matchMass;
	
	return true;
}

int main() {
	
	SqliteController sqlController("../ms.db");

	double timeStart = (double)clock(); 

	//test_diffSpectrum(&sqlController, 10000); // 4200ms
	test_diffSpectrum(&sqlController, 100000); // 293-297s -> 279-325s -> 323s
	//test_maxPeakCount(&sqlController); //1233


	double timeFinish = (double)clock(); //结束时间
	std::cout << "run time: " << (timeFinish - timeStart) << std::endl;
	system("PAUSE");
	return 0;
}