// LibSettingView.cpp : 实现文件
//

#include "stdafx.h"
#include "LibSettingView.h"
#include "afxdialogex.h"
#include "SqliteController.h"


// LibSettingView 对话框

IMPLEMENT_DYNAMIC(LibSettingView, CDialogEx)

LibSettingView::LibSettingView(CWnd* pParent /*=NULL*/)
	: CDialogEx(LibSettingView::IDD, pParent)
{
	_defaultDBPath = CString(_T("E:\\GCMSLibSearch\\GCMSLibSearch\\nist.db"));
}

LibSettingView::~LibSettingView()
{
}

BOOL LibSettingView::OnInitDialog() {
	CDialogEx::OnInitDialog();

	GetDlgItem(IDC_EDIT_DB_PATH)->SetWindowText(_defaultDBPath);
	return TRUE;
}

void LibSettingView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

void LibSettingView::setCompoundsOnEditCtrls(const Compound &aCompound) {
	CString cstrMassWeight;
	cstrMassWeight.Format(_T("%d"), aCompound._massWeight);

	GetDlgItem(IDC_EDIT_COMPOUND_NAME)->SetWindowText(CString(aCompound._compoundName.c_str()));
	GetDlgItem(IDC_EDIT_FORMULA)->SetWindowText(CString(aCompound._formula.c_str()));
	GetDlgItem(IDC_EDIT_MASSWEIGHT)->SetWindowText(cstrMassWeight);
	GetDlgItem(IDC_EDIT_CASNO)->SetWindowText(CString(aCompound._casNo.c_str()));
	GetDlgItem(IDC_EDIT_PEAKDATA)->SetWindowText(CString(aCompound._peakData.c_str()));
}
void LibSettingView::getCompoundsOnEditCtrls(Compound &aCompound) {
	CString cstrName, cstrFormula, cstrMass, cstrCas, cstrPeakData;

	GetDlgItem(IDC_EDIT_COMPOUND_NAME)->GetWindowText(cstrName);
	GetDlgItem(IDC_EDIT_FORMULA)->GetWindowText(cstrFormula);
	GetDlgItem(IDC_EDIT_MASSWEIGHT)->GetWindowText(cstrMass);
	GetDlgItem(IDC_EDIT_CASNO)->GetWindowText(cstrCas);
	GetDlgItem(IDC_EDIT_PEAKDATA)->GetWindowText(cstrPeakData);

	aCompound._compoundName = CT2A(cstrName);
	aCompound._formula = CT2A(cstrFormula);
	aCompound._massWeight = _ttoi(cstrMass);
	aCompound._casNo = CT2A(cstrCas);
	aCompound._peakData = CT2A(cstrPeakData);
}


BEGIN_MESSAGE_MAP(LibSettingView, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CHOOSE_DB, &LibSettingView::OnBnClickedChooseDB)
	ON_BN_CLICKED(IDC_BUTTON_QUERY_COMPOUND, &LibSettingView::OnBnClickedQueryCompound)
	ON_BN_CLICKED(IDC_BUTTON_CREATE_DB, &LibSettingView::OnBnClickedCreateDB)
END_MESSAGE_MAP()


// LibSettingView 消息处理程序


void LibSettingView::OnBnClickedChooseDB() {
	CString strPath = _T("");
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("谱库数据库 (*.db)|*.db|All Files (*.*)|*.*||"), NULL);

	if (fileDlg.DoModal()) {
		strPath = fileDlg.GetPathName();
	} else { return; }
	
	GetDlgItem(IDC_EDIT_DB_PATH)->SetWindowText(strPath);
	_cstrDBPath = strPath;
}


void LibSettingView::OnBnClickedQueryCompound() {
	// 先拿到 CompoundID
	CString cstrCompoundID;
	GetDlgItem(IDC_EDIT_COMPOUND_ID)->GetWindowText(cstrCompoundID);
	int compoundID = _ttoi(cstrCompoundID);
	if (compoundID <= 0) {
		::MessageBox(NULL, _T("指定的化合物ID范围必须大于0"), _T("警告"), MB_OK | MB_ICONWARNING);
	}
	// 从当前谱库搜索
	std::string sqlitePath = CT2A(_cstrDBPath);
	SqliteController sqliteController(sqlitePath);
	Compound compound = sqliteController.getCompound(compoundID);

	// 显示结果
	setCompoundsOnEditCtrls(compound);
}


void LibSettingView::OnBnClickedCreateDB() {
	CString cstrPath = _T("");
	CFileDialog fileDlg(FALSE, _T("db"), NULL, OFN_HIDEREADONLY, _T("谱库数据库 (*.db)|*.db||"), NULL);

	if (fileDlg.DoModal()) {
		cstrPath = fileDlg.GetPathName();
	} else { return; }

	// TODO: 检测是否命名冲突
	const std::string strPath = CT2A(cstrPath);
	SqliteController sqliteController(strPath);
	GetDlgItem(IDC_EDIT_DB_PATH)->SetWindowText(cstrPath);
	_cstrDBPath = cstrPath;
}
