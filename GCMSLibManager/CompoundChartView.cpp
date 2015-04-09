// CompoundChartView.cpp : 实现文件
//

#include "stdafx.h"
#include "CompoundChartView.h"
#include "afxdialogex.h"


// CompoundChartView 对话框

IMPLEMENT_DYNAMIC(CompoundChartView, CDialogEx)


CompoundChartView::CompoundChartView(const Compound& compound, CWnd* pParent /*=NULL*/)
	: CDialogEx(CompoundChartView::IDD, pParent) {
	_compound = compound;
}

CompoundChartView::~CompoundChartView()
{
}

void CompoundChartView::init() {
	CRect rcCompoundChart;
	GetDlgItem(IDC_EDIT_COMPOUND_CHART)->GetWindowRect(rcCompoundChart);
	ScreenToClient(rcCompoundChart);

	_compoundChart.Create(this, rcCompoundChart, 0, WS_CHILD|WS_VISIBLE);
	_superChartController.setChartCtrl(&_compoundChart);

	// 设置丰图
	const CString cstrPeakData = CString(_compound._peakData.c_str());
	_superChartController.drawCompoundChart(cstrPeakData);

	// 设置标题
	CString strID;
	strID.Format(_T("#%d : "), _compound._compoundID);
	CString strTitle = strID + CString(_compound._compoundName.c_str());
	SetWindowText(strTitle);
}

BOOL CompoundChartView::OnInitDialog() {
	CDialogEx::OnInitDialog();
	init();
	return TRUE;

}

void CompoundChartView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CompoundChartView, CDialogEx)
END_MESSAGE_MAP()


// CompoundChartView 消息处理程序
