#pragma once


// CSearchResultView 对话框

class CSearchResultView : public CDialog
{
	DECLARE_DYNAMIC(CSearchResultView)


// 对话框数据
	enum { IDD = IDD_SEARCH_RESULT_VIEW };

protected:
	CSearchResultView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSearchResultView();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	
	DECLARE_MESSAGE_MAP()

//单例模式
protected:
	static CSearchResultView* _pInstance;

public:
	static CSearchResultView* getInstance();
	
	afx_msg void OnBnClickedCancel();
};



