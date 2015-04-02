// LibParaSearchView.cpp : 实现文件
//

#include "stdafx.h"
#include "LibParaSearchView.h"
#include "afxdialogex.h"


// LibParaSearchView 对话框

IMPLEMENT_DYNAMIC(LibParaSearchView, CDialogEx)

LibParaSearchView::LibParaSearchView(CWnd* pParent /*=NULL*/)
	: CDialogEx(LibParaSearchView::IDD, pParent)
{
	_defaultDBPath = CString(_T("E:\\GCMSLibSearch\\GCMSLibSearch\\nist.db")); // TODO: 用MD5绑定文件
	_currentDBPath = _defaultDBPath;
}

LibParaSearchView::~LibParaSearchView()
{
}

BOOL LibParaSearchView::OnInitDialog() {
	CDialogEx::OnInitDialog();

	GetDlgItem(IDC_EDIT_SEARCH_DB_PATH)->SetWindowText(_defaultDBPath);
	
	GetDlgItem(IDC_EDIT_COMPOUND_NAME)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_CAS_NO)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_COMPOUND_FORMULA)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MASS_LOWER)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MASS_UPPER)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_ID_LOWER)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_ID_UPPER)->EnableWindow(FALSE);

	return TRUE;
}

void LibParaSearchView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LibParaSearchView, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_COMPOUND_NAME, &LibParaSearchView::OnBnClickedCheckCompoundName)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH_DB_PATH, &LibParaSearchView::OnBnClickedButtonSearchDBPath)
	ON_BN_CLICKED(IDC_CHECK_CAS_NO, &LibParaSearchView::OnBnClickedCheckCasNo)
	ON_BN_CLICKED(IDC_CHECK_FORMULA, &LibParaSearchView::OnBnClickedCheckFormula)
	ON_BN_CLICKED(IDC_CHECK_MASS_RANGE, &LibParaSearchView::OnBnClickedCheckMassRange)
	ON_BN_CLICKED(IDC_CHECK_COMPOUND_ID_RANGE, &LibParaSearchView::OnBnClickedCheckCompoundIdRange)
	ON_BN_CLICKED(IDC_BUTTON_LIB_PARA_SEARCH, &LibParaSearchView::OnBnClickedButtonLibParaSearch)
END_MESSAGE_MAP()


// LibParaSearchView 消息处理程序
void LibParaSearchView::OnBnClickedButtonSearchDBPath() {
	CString strPath = _T("");
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("谱库数据库 (*.db)|*.db|All Files (*.*)|*.*||"), NULL);

	if (fileDlg.DoModal() == IDOK) {

		strPath = fileDlg.GetPathName();
		GetDlgItem(IDC_EDIT_SEARCH_DB_PATH)->SetWindowText(strPath);
		_currentDBPath = strPath;
	} else { 

		GetDlgItem(IDC_EDIT_SEARCH_DB_PATH)->SetWindowText(_currentDBPath);
		return; 
	}
}

// - CheckBox 勾选框
void LibParaSearchView::OnBnClickedCheckCompoundName() {
	BOOL isReadOnly = ( BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_COMPOUND_NAME) );
	GetDlgItem(IDC_EDIT_COMPOUND_NAME)->EnableWindow(isReadOnly);
}
void LibParaSearchView::OnBnClickedCheckCasNo() {
	BOOL isReadOnly = ( BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_CAS_NO) );
	GetDlgItem(IDC_EDIT_CAS_NO)->EnableWindow(isReadOnly);
}
void LibParaSearchView::OnBnClickedCheckFormula() {
	BOOL isReadOnly = ( BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_FORMULA) );
	GetDlgItem(IDC_EDIT_COMPOUND_FORMULA)->EnableWindow(isReadOnly);
}
void LibParaSearchView::OnBnClickedCheckMassRange() {
	BOOL isReadOnly = ( BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_MASS_RANGE) );
	GetDlgItem(IDC_EDIT_MASS_LOWER)->EnableWindow(isReadOnly);
	GetDlgItem(IDC_EDIT_MASS_UPPER)->EnableWindow(isReadOnly);
}
void LibParaSearchView::OnBnClickedCheckCompoundIdRange() {
	BOOL isReadOnly = ( BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_COMPOUND_ID_RANGE) );
	GetDlgItem(IDC_EDIT_ID_LOWER)->EnableWindow(isReadOnly);
	GetDlgItem(IDC_EDIT_ID_UPPER)->EnableWindow(isReadOnly);
}
// - 检索
void LibParaSearchView::OnBnClickedButtonLibParaSearch() {
	
}
