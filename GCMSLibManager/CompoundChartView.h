#pragma once

#include "GCMSLibManager.h"

// CompoundChartView 对话框

class CompoundChartView : public CDialogEx
{
	DECLARE_DYNAMIC(CompoundChartView)

public:
	CompoundChartView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CompoundChartView();

// 对话框数据
	enum { IDD = IDD_COMPOUNDCHARTVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL CompoundChartView::OnInitDialog();
	VOID CompoundChartView::init();
	DECLARE_MESSAGE_MAP()

private:
	CSuperChartCtrl _compoundChart; 
};
