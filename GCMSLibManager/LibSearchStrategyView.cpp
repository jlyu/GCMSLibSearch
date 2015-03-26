// LibSearchStrategyView.cpp : 实现文件
//

#include "stdafx.h"
#include "LibSearchStrategyView.h"
#include "afxdialogex.h"


// LibSearchStrategyView 对话框

IMPLEMENT_DYNAMIC(LibSearchStrategyView, CDialogEx)

LibSearchStrategyView::LibSearchStrategyView(CWnd* pParent /*=NULL*/, LibConfig& libConfig)
	: CDialogEx(LibSearchStrategyView::IDD, pParent) {

		_libConfig = libConfig;

}

LibSearchStrategyView::~LibSearchStrategyView()
{
}

void LibSearchStrategyView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LibSearchStrategyView, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_PATH1, &LibSearchStrategyView::OnBnClickedChoosePath1)
	ON_BN_CLICKED(IDC_BUTTON_PATH2, &LibSearchStrategyView::OnBnClickedChoosePath2)
	ON_BN_CLICKED(IDC_BUTTON_PATH3, &LibSearchStrategyView::OnBnClickedChoosePath3)
END_MESSAGE_MAP()

CString LibSearchStrategyView::getFilePath() {
	CString strPath = _T("");
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("谱库数据库 (*.db)|*.db|All Files (*.*)|*.*||"), NULL);

	if (fileDlg.DoModal()) {
		strPath = fileDlg.GetPathName();
	}
	return strPath;
}

// LibSearchStrategyView 消息处理程序


void LibSearchStrategyView::OnBnClickedChoosePath1() {
	CString strPath = getFilePath();
	GetDlgItem(IDC_EDIT_DB_PATH1)->SetWindowText(strPath);
}


void LibSearchStrategyView::OnBnClickedChoosePath2() {
	CString strPath = getFilePath();
	GetDlgItem(IDC_EDIT_DB_PATH2)->SetWindowText(strPath);
}


void LibSearchStrategyView::OnBnClickedChoosePath3() {
	CString strPath = getFilePath();
	GetDlgItem(IDC_EDIT_DB_PATH3)->SetWindowText(strPath);
}
