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

BOOL LibSearchStrategyView::OnInitDialog() {
	CDialogEx::OnInitDialog();
	initCtrls();
	return TRUE;
}


BEGIN_MESSAGE_MAP(LibSearchStrategyView, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_PATH1, &LibSearchStrategyView::OnBnClickedChoosePath1)
	ON_BN_CLICKED(IDC_BUTTON_PATH2, &LibSearchStrategyView::OnBnClickedChoosePath2)
	ON_BN_CLICKED(IDC_BUTTON_PATH3, &LibSearchStrategyView::OnBnClickedChoosePath3)
	ON_BN_CLICKED(IDOK, &LibSearchStrategyView::OnBnClickedOk)
END_MESSAGE_MAP()

void LibSearchStrategyView::initCtrls() {
	CString cstrMinMass1, cstrMinMass2, cstrMinMass3;
	CString cstrUpperMass, cstrLowerMass, cstrMatchLimit;
	cstrMinMass1.Format(_T("%d"), _libConfig._minMass1st);
	cstrMinMass2.Format(_T("%d"), _libConfig._minMass2nd);
	cstrMinMass3.Format(_T("%d"), _libConfig._minMass3rd);
	cstrUpperMass.Format(_T("%d"), _libConfig._upperMass);
	cstrLowerMass.Format(_T("%d"), _libConfig._lowerMass);
	cstrMatchLimit.Format(_T("%d"), _libConfig._matchLimitNumber);
	
	GetDlgItem(IDC_EDIT_DB_PATH1)->SetWindowText(CString(_libConfig._dbPath1st.c_str()));
	GetDlgItem(IDC_EDIT_DB_PATH2)->SetWindowText(CString(_libConfig._dbPath2nd.c_str()));
	GetDlgItem(IDC_EDIT_DB_PATH3)->SetWindowText(CString(_libConfig._dbPath3rd.c_str()));
	GetDlgItem(IDC_EDIT_MIN_MASS1)->SetWindowText(cstrMinMass1);
	GetDlgItem(IDC_EDIT_MIN_MASS2)->SetWindowText(cstrMinMass2);
	GetDlgItem(IDC_EDIT_MIN_MASS3)->SetWindowText(cstrMinMass3);
	GetDlgItem(IDC_EDIT_UPPER_MASS)->SetWindowText(cstrUpperMass);
	GetDlgItem(IDC_EDIT_LOWER_MASS)->SetWindowText(cstrLowerMass);
	GetDlgItem(IDC_EDIT_MAX_MATCH)->SetWindowText(cstrMatchLimit);

	( (CButton*)GetDlgItem(IDC_CHECK_UNIQUE) )->SetCheck(_libConfig._isUnique);
}

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


void LibSearchStrategyView::OnBnClickedOk() {
	
	CString cstrPath1, cstrPath2, cstrPath3;
	CString cstrMinMass1, cstrMinMass2, cstrMinMass3;
	CString cstrUpperMass, cstrLowerMass, cstrMatchLimit;

	GetDlgItem(IDC_EDIT_DB_PATH1)->GetWindowText(cstrPath1);
	GetDlgItem(IDC_EDIT_DB_PATH2)->GetWindowText(cstrPath2);
	GetDlgItem(IDC_EDIT_DB_PATH3)->GetWindowText(cstrPath3);
	GetDlgItem(IDC_EDIT_MIN_MASS1)->GetWindowText(cstrMinMass1);
	GetDlgItem(IDC_EDIT_MIN_MASS2)->GetWindowText(cstrMinMass2);
	GetDlgItem(IDC_EDIT_MIN_MASS3)->GetWindowText(cstrMinMass3);
	GetDlgItem(IDC_EDIT_UPPER_MASS)->GetWindowText(cstrUpperMass);
	GetDlgItem(IDC_EDIT_LOWER_MASS)->GetWindowText(cstrLowerMass);
	GetDlgItem(IDC_EDIT_MAX_MATCH)->GetWindowText(cstrMatchLimit);

	_libConfig._dbPath1st = CT2A(cstrPath1);
	_libConfig._dbPath2nd = CT2A(cstrPath2);
	_libConfig._dbPath3rd = CT2A(cstrPath3);
	_libConfig._minMass1st = _ttoi(cstrMinMass1);
	_libConfig._minMass2nd = _ttoi(cstrMinMass2);
	_libConfig._minMass3rd = _ttoi(cstrMinMass3);
	_libConfig._upperMass = _ttoi(cstrUpperMass);
	_libConfig._lowerMass = _ttoi(cstrLowerMass);
	_libConfig._matchLimitNumber = _ttoi(cstrMatchLimit);

	if ( BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_UNIQUE)) {
		_libConfig._isUnique = true;
	} else {
		_libConfig._isUnique = false;
	}

	CDialogEx::OnOK();
}
