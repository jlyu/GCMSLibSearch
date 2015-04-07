#pragma once

#include "GCMSLibManager.h"

#define CHECK_PASS					0
#define CHECK_MASS_LOWER_FAIL		1
#define CHECK_MASS_UPPER_FAIL		2
#define CHECK_ID_LOWER_FAIL			3
#define CHECK_ID_UPPER_FAIL			4

// LibParaSearchView 对话框

class LibParaSearchView : public CDialogEx
{
	DECLARE_DYNAMIC(LibParaSearchView)

public:
	LibParaSearchView(CWnd* pParent = NULL);   // 标准构造函数
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

	void LibParaSearchView::getSearchPara(SearchPara& searchPara);
	int LibParaSearchView::checkSearchPara(const SearchPara& searchPara);

public:
	CString _defaultDBPath; // 系统默认谱库
	CString _currentDBPath; // 当前使用谱库

	int _maxCompoundID;
};
