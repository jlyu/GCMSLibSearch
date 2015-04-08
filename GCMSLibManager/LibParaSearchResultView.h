#pragma once

#include "GCMSLibManager.h"
#include "Compound.h"
#include "CompoundChartView.h"

#include <vector>
// LibParaSearchResultView 对话框

class LibParaSearchResultView : public CDialogEx
{
	DECLARE_DYNAMIC(LibParaSearchResultView)

public:
	LibParaSearchResultView(const std::vector<Compound> &compounds, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~LibParaSearchResultView();

	

// 对话框数据
	enum { IDD = IDD_LIBPARASEARCHRESULTVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()
	afx_msg void OnHdnItemdblclickListSearchResult(NMHDR *pNMHDR, LRESULT *pResult);

	BOOL LibParaSearchResultView::OnInitDialog();
	VOID LibParaSearchResultView::initListCtrl();
	VOID LibParaSearchResultView::fillCompoundList();

public:
	CListCtrl _compoundsList;
	std::vector<Compound> _compounds;
	afx_msg void OnNMDblclkListSearchResult(NMHDR *pNMHDR, LRESULT *pResult);
};
