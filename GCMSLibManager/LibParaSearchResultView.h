#pragma once

#include "GCMSLibManager.h"
#include <vector>
#include "Compound.h"
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
	BOOL LibParaSearchResultView::OnInitDialog();
	DECLARE_MESSAGE_MAP()

	VOID LibParaSearchResultView::initListCtrl();
	VOID LibParaSearchResultView::fillCompoundList();

public:
	CListCtrl _compoundsList;
	std::vector<Compound> _compounds;
};
