#pragma once

#include "GCMSLibManager.h"
#include "SuperChartController.h"
#include "Compound.h"

// CompoundChartView 对话框

class CompoundChartView : public CDialogEx
{
	DECLARE_DYNAMIC(CompoundChartView)

public:
	CompoundChartView(const Compound& compound, CWnd* pParent = NULL);
	virtual ~CompoundChartView();

// 对话框数据
	enum { IDD = IDD_COMPOUNDCHARTVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL CompoundChartView::OnInitDialog();
	VOID CompoundChartView::init();
	DECLARE_MESSAGE_MAP()

private:
	Compound _compound;
	CSuperChartCtrl _compoundChart; 
	SuperChartController _superChartController;
	
};
