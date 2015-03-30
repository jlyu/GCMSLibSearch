#pragma once

#include "GCMSLibManager.h"
// LibSettingView 对话框

class LibSettingView : public CDialogEx
{
	DECLARE_DYNAMIC(LibSettingView)

public:
	LibSettingView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~LibSettingView();

// 对话框数据
	enum { IDD = IDD_LIBSETTINGVIEW };

	afx_msg void OnBnClickedChooseDB();
	afx_msg void OnBnClickedQueryCompound();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	void setCompoundsOnEditCtrls(const Compound &aCompound);
	void getCompoundsOnEditCtrls(Compound &aCompound);

public:
	CString _cstrDBPath;


	
};
