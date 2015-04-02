#pragma once

#include "GCMSLibManager.h"

#define CHECK_OK			1
#define CHECK_FIND_NULL		2
#define CHECK_FIND_MINUS	3
#define CHECK_PEAK_FAIL		4
#define CHECK_MASS_FAIL		5



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
	afx_msg void OnBnClickedCreateDB();
	afx_msg void OnBnClickedQueryCompound();
	afx_msg void OnBnClickedSaveCompound();
	afx_msg void OnBnClickedDelCompound();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	void setCompoundsOnEditCtrls(const Compound &aCompound);
	void getCompoundsOnEditCtrls(Compound &aCompound);
	void clearCompoundsOnEditCtrls();
	int  checkCompound(Compound &aCompound); // 在检查后要把maxX写入compound3
	bool checkPeakDataString(const std::string peakData, int& maxX, int& peakCount);

public:
	CString _defaultDBPath; // 系统默认谱库
	CString _currentDBPath; // 当前使用谱库
	
};
