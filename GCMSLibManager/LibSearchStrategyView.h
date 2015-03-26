#pragma once
//#include "stdafx.h"
//#include "afxdialogex.h"
//#include "afxcmn.h"
//#include "Resource.h"

#include "GCMSLibManager.h"

// LibSearchStrategyView 对话框

class LibSearchStrategyView : public CDialogEx
{
	DECLARE_DYNAMIC(LibSearchStrategyView)
	enum { IDD = IDD_LIBSEARCHSTRATEGYVIEW };  

public:
	LibSearchStrategyView(CWnd* pParent, LibConfig& libConfig);   // 标准构造函数
	virtual ~LibSearchStrategyView();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CString getFilePath();

public:
	LibConfig _libConfig;
	afx_msg void OnBnClickedChoosePath1();
	afx_msg void OnBnClickedChoosePath2();
	afx_msg void OnBnClickedChoosePath3();
};
