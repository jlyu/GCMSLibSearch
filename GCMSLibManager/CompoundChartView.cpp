// CompoundChartView.cpp : 实现文件
//

#include "stdafx.h"
#include "CompoundChartView.h"
#include "afxdialogex.h"


// CompoundChartView 对话框

IMPLEMENT_DYNAMIC(CompoundChartView, CDialogEx)

CompoundChartView::CompoundChartView(CWnd* pParent /*=NULL*/)
	: CDialogEx(CompoundChartView::IDD, pParent)
{

}

CompoundChartView::~CompoundChartView()
{
}

void CompoundChartView::init() {
	CRect rcCompoundChart;
	GetDlgItem(IDC_EDIT_COMPOUND_CHART)->GetWindowRect(rcCompoundChart);
	ScreenToClient(rcCompoundChart);

	_compoundChart.Create(this, rcCompoundChart, 0, WS_CHILD|WS_VISIBLE);
	

	//_peakDrawView.setChartCtrl(&_peakChart, &_compareChart);
}

BOOL CompoundChartView::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;

}

void CompoundChartView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CompoundChartView, CDialogEx)
END_MESSAGE_MAP()


// CompoundChartView 消息处理程序
