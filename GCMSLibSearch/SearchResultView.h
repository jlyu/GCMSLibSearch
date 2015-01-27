#pragma once
#include "afxcmn.h"
#include "Compound.h"
#include <vector>


// CSearchResultView 对话框

class CSearchResultView : public CDialogEx
{
	DECLARE_DYNAMIC(CSearchResultView)
	enum { IDD = IDD_SEARCH_RESULT_VIEW };

protected:
	CSearchResultView(CWnd* pParent = NULL);
	virtual ~CSearchResultView();

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);
	
	DECLARE_MESSAGE_MAP()

private:
	void initListCtrl();

	CListCtrl _compoundList;
	std::vector<Compound> _compounds;


public:
	afx_msg void OnBnClickedCancel();
	void fillCompoundList(const std::vector<Compound> &compounds);



//单例模式
protected:
	static CSearchResultView* _pInstance;
public:
	static CSearchResultView* getInstance();

};



