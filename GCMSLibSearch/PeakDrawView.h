#pragma once
#include "SuperChartLib.h"
#include <iostream>
#include <vector>

class CPeakDrawView {

public:
	CPeakDrawView(void);
	CPeakDrawView(CSuperChartCtrl* pSuperChartCtrl);
	virtual ~CPeakDrawView(void);

public:
	VOID setChartCtrl(CSuperChartCtrl* upChartCtrl, CSuperChartCtrl* downChartCtrl);
	VOID drawPeak(const CString &strPeakData);  //显示丰图 (上)

	VOID dropPeakCompare();
	VOID drawPeakCompare(const CString &strPeakData); //显示丰图 (下)

private:
	void parsePeakData(const CString &strPeakData, std::vector<std::pair<int, int> > &peakData); // TODO： 考虑作成缓存

private:
	CSuperChartCtrl* _pPeakChart;     //上：质谱图
	CSuperChartCtrl* _pCompareChart;  //下：对比图
};

