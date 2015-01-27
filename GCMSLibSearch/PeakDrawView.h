#pragma once
#include "SuperChartLib.h"
#include <vector>

class CPeakDrawView {

public:
	CPeakDrawView(void);
	CPeakDrawView(CSuperChartCtrl* pSuperChartCtrl);
	virtual ~CPeakDrawView(void);

public:
	VOID setChartCtrl(CSuperChartCtrl* pSuperChartCtrl);
	VOID drawPeak(const CString &strPeakData);  //ÏÔÊ¾·áÍ¼

private:
	void parsePeakData(const CString &strPeakData, std::vector<std::pair<int, int> > &peakData); // TODO£º ¿¼ÂÇ×÷³É»º´æ

private:
	CSuperChartCtrl* _pPeakChart; 
	CChartLineSerie* _pPeakLine; //·áÍ¼
	
	


};

