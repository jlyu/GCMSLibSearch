#pragma once

#include "GCMSLibManager.h"

// LibSearchStrategyView 对话框

class LibSearchStrategyView : public CDialogEx
{
	DECLARE_DYNAMIC(LibSearchStrategyView)
	enum { IDD = IDD_LIBSEARCHSTRATEGYVIEW };  

	afx_msg void OnBnClickedChoosePath1();
	afx_msg void OnBnClickedChoosePath2();
	afx_msg void OnBnClickedChoosePath3();
	afx_msg void OnBnClickedOk();

public:
	LibSearchStrategyView(CWnd* pParent, LibConfig& libConfig);   // 标准构造函数
	virtual ~LibSearchStrategyView();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CString getFilePath();
	void initCtrls();

public:
	LibConfig _libConfig;

};
