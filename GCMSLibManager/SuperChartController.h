#pragma once
#include "SuperChartLib.h"
#include <vector>


class SuperChartController
{
public:
	SuperChartController(void);
	SuperChartController(CSuperChartCtrl* pSuperChartCtrl);
	~SuperChartController(void);

	VOID setChartCtrl(CSuperChartCtrl* pSuperChartCtrl);
	VOID drawCompoundChart(const CString &strPeakData);  //显示丰图 
	VOID drawCompoundString(std::vector<CPoint> &peakPoints); //标记区间最大Y值对应X值

	VOID dropCompoundChart();

private:
	void parsePeakData(const CString &strPeakData, std::vector<CPoint> &peakPoints);

private:
	CSuperChartCtrl* _pChart;
};




	