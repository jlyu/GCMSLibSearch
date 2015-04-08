// GCMSLibSearchDlg.h : 头文件
#pragma once

#include "PeakDrawView.h"
#include "..\GCMSLibManager\Compound.h"
#include "SearchResultView.h"


// CGCMSLibSearchDlg 对话框
class CGCMSLibSearchDlg : public CDialogEx
{
// 构造
public:
	CGCMSLibSearchDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GCMSLIBSEARCH_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	void init();


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnCancel();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


private:
	CSuperChartCtrl _peakChart; 
	CSuperChartCtrl _compareChart;
	CPeakDrawView _peakDrawView;
	

public:
	afx_msg void OnBnLoadTestingPeakData();
	afx_msg void OnBnShowSearchResult();

	void drawPeakCompare(const CString &strPeakData);
	afx_msg void OnBnClickedCallDLL();
	afx_msg void OnBnClickedLibSetting();
	afx_msg void OnBnClickedParaSearch();
};
