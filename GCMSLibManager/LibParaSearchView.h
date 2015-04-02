#pragma once

#include "GCMSLibManager.h"
// LibParaSearchView 对话框

class LibParaSearchView : public CDialogEx
{
	DECLARE_DYNAMIC(LibParaSearchView)

public:
	LibParaSearchView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~LibParaSearchView();

// 对话框数据
	enum { IDD = IDD_LIBPARASEARCHVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()



public:
	CString _defaultDBPath; // 系统默认谱库
	CString _currentDBPath; // 当前使用谱库
	afx_msg void OnBnClickedCheckCompoundName();
};
