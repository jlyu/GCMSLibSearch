#include "StdAfx.h"
#include "SuperChartController.h"


SuperChartController::SuperChartController(void) {
	_pChart = NULL;
}


SuperChartController::~SuperChartController(void)
{
}


VOID SuperChartController::setChartCtrl(CSuperChartCtrl* pSuperChartCtrl) {

	if ( (_pChart != NULL) && (_pChart != pSuperChartCtrl) ) {
		_pChart = pSuperChartCtrl;
	}
}

void SuperChartController::parsePeakData(const CString &strPeakData, std::vector<std::pair<int, int> > &peakData) {

	if (!peakData.empty()) { peakData.clear(); }

	int index = 0;
	while (true) {

		CString strXY;
		AfxExtractSubString(strXY, (LPCTSTR)strPeakData, index, ';');
		if (strXY == _T("")) {
			break;
		}

		CString strX, strY;
		AfxExtractSubString(strX, (LPCTSTR)strXY, 0, ' ');
		AfxExtractSubString(strY, (LPCTSTR)strXY, 1, ' ');

		peakData.push_back(std::make_pair(_ttoi(strX), _ttoi(strY)));

		index++;
	}
}

VOID SuperChartController::drawCompoundChart(const CString &strPeakData) {
	// 以传入的形如_T("12 108;13 229;14 999;15 22;26 18;27 58;28 178;29 23;40 18;41 108;42 431;43 8;") 字符串数据显示

	std::vector<std::pair<int, int> > peakData;
	parsePeakData(strPeakData, peakData);

	_pChart->EnableRefresh(false);

	double min_x = 0.0;
	double max_x = 0.0;
	double min_y = 0.0;
	double max_y = 0.0;

	// 绘制丰图
	for (int i=0; i != peakData.size(); i++) {

		CChartLineSerie* line;  
		line = _pChart->CreateLineSerie();
		line->SetColor(RGB(210, 0, 0));
		line->m_vPoints.InitPoints(4);

		double dx[] = { (double)peakData[i].first, (double)peakData[i].first };
		double dy[] = {  0.0f, (double)peakData[i].second };

		line->m_vPoints.AddPoints(dx, dy, 2);

		if (peakData[i].first > max_x) { max_x = peakData[i].first; }
		if (peakData[i].second > max_y) { max_y = peakData[i].second; }
	}

	CChartAxis* pAxisSelect;
	_pChart->SetBackColor(RGB(235, 235, 235)); //设置背景颜色

	pAxisSelect = _pChart->GetLeftAxis(); //设置Y坐标
	if (pAxisSelect == NULL) return;
	pAxisSelect->SetAutomatic(false);
	pAxisSelect->SetCoordinate(min_y, max_y * 1.1, COORDINATE_SET);

	pAxisSelect = _pChart->GetBottomAxis(); //设置X坐标
	if (pAxisSelect == NULL) return;
	pAxisSelect->SetAutomatic(false);
	pAxisSelect->SetCoordinate(min_x, max_x * 1.1, COORDINATE_SET);

	pAxisSelect->SetVisible(true);
	_pChart->EnableRefresh(true);
	_pChart->RefreshCtrl();	
}