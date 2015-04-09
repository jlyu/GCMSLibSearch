#include "StdAfx.h"
#include "SuperChartController.h"


SuperChartController::SuperChartController(void) {
	_pChart = NULL;
}


SuperChartController::~SuperChartController(void)
{
}


VOID SuperChartController::setChartCtrl(CSuperChartCtrl* pSuperChartCtrl) {

	if ( (NULL != pSuperChartCtrl) && (_pChart != pSuperChartCtrl) ) {
		_pChart = pSuperChartCtrl;
	}
}

void SuperChartController::parsePeakData(const CString &strPeakData, std::vector<CPoint> &peakPoints) {
	// 保证 xn > .. > x2 > x1 不做检查

	if (!peakPoints.empty()) { peakPoints.clear(); }

	int index = 0;
	while (true) {

		CString strXY;
		AfxExtractSubString(strXY, (LPCTSTR)strPeakData, index, ';');
		if (strXY == _T("")) { break; }

		CString strX, strY;
		AfxExtractSubString(strX, (LPCTSTR)strXY, 0, ' ');
		AfxExtractSubString(strY, (LPCTSTR)strXY, 1, ' ');

		peakPoints.push_back(CPoint(_ttoi(strX), _ttoi(strY)));
		index++;
	}
}

VOID SuperChartController::drawCompoundChart(const CString &strPeakData) {
	// 以传入的形如_T("12 108;13 229;14 999;15 22;26 18;27 58;28 178;29 23;40 18;41 108;42 431;43 8;") 字符串数据显示

	std::vector<CPoint> peakPoints;
	parsePeakData(strPeakData, peakPoints);

	_pChart->EnableRefresh(false);

	double min_x = 0.0;
	double max_x = 0.0;
	double min_y = 0.0;
	double max_y = 0.0;

	// 绘制丰图
	for (int i=0; i != peakPoints.size(); i++) {

		CChartLineSerie* line;  
		line = _pChart->CreateLineSerie();
		line->SetColor(RGB(210, 0, 0));
		line->m_vPoints.InitPoints(4);

		double dx[] = { (double)peakPoints[i].x, (double)peakPoints[i].y };
		double dy[] = {  0.0f, (double)peakPoints[i].y };

		line->m_vPoints.AddPoints(dx, dy, 2);

		if (peakPoints[i].x > max_x) { max_x = peakPoints[i].x; }
		if (peakPoints[i].y > max_y) { max_y = peakPoints[i].y; }
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

	//
	drawCompoundString(peakPoints);
}

VOID SuperChartController::drawCompoundString(std::vector<CPoint> &peakPoints) {
	// 拿到最大X值后，分割成10个区间，每个区间选出最大的Y值加入，但是Y存在下限就是MaxY的10%
	const int peakPointsSize = peakPoints.size();
	int maxX = peakPoints[peakPointsSize - 1].x;
	int maxY = 0;
	for (int i = 0; i != peakPointsSize; i++) {
		if (peakPoints[i].y > maxY) {
			maxY = peakPoints[i].y;
		}
	}

	int interval = maxX / 10;
	int intervalY = 0;
	int intervalIndex = 0;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j != interval; j++) {
			if (peakPoints[i*interval+j].y > intervalY) {
				intervalY = peakPoints[i*interval+j].y;
				intervalIndex = i*interval+j;
			}

			if (intervalY > (maxY*0.1)) {
				CString strMark;
				strMark.Format(_T("%d"), peakPoints[intervalIndex].x);
				_pChart->AddChartString(peakPoints[intervalIndex].x * 0.05, 
										peakPoints[intervalIndex].y * 0.05, 
										_T("20"));
			}
		}
	}
	_pChart->DisplayAllChartStrings();

}