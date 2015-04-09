#pragma once

#include "GCMSLibManager.h"
#include "LibParaSearchResultView.h"

#include <sstream>
#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
	( std::ostringstream() << std::dec << x ) ).str()

#define CHECK_PASS					0
#define CHECK_EMPTY					1
#define CHECK_MASS_RANGE_FAIL		2
#define CHECK_ID_RANGE_FAIL			3


// LibParaSearchView 对话框

class LibParaSearchView : public CDialogEx
{
	DECLARE_DYNAMIC(LibParaSearchView)

public:
	LibParaSearchView(const CString defaultLibPathName, CWnd* pParent = NULL);    
	virtual ~LibParaSearchView();

// 对话框数据
	enum { IDD = IDD_LIBPARASEARCHVIEW };

	afx_msg void OnBnClickedCheckCompoundName();
	afx_msg void OnBnClickedButtonSearchDBPath();
	afx_msg void OnBnClickedCheckCasNo();
	afx_msg void OnBnClickedCheckFormula();
	afx_msg void OnBnClickedCheckMassRange();
	afx_msg void OnBnClickedCheckCompoundIdRange();
	afx_msg void OnBnClickedButtonLibParaSearch();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	void LibParaSearchView::setSearchPara();
	void LibParaSearchView::getSearchPara();
	int LibParaSearchView::checkSearchPara();

public:
	CString _defaultDBPath; // 系统默认谱库
	CString _currentDBPath; // 当前使用谱库

	SearchPara _searchPara;
	int _maxCompoundID;
	
};
