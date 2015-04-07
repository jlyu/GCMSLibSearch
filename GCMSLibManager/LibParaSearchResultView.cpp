// LibParaSearchResultView.cpp : 实现文件
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "LibParaSearchResultView.h"



// LibParaSearchResultView 对话框

IMPLEMENT_DYNAMIC(LibParaSearchResultView, CDialogEx)

LibParaSearchResultView::LibParaSearchResultView(CWnd* pParent /*=NULL*/)
	: CDialogEx(LibParaSearchResultView::IDD, pParent)
{
	//initListCtrl();
}

LibParaSearchResultView::~LibParaSearchResultView()
{
}

void LibParaSearchResultView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SEARCH_RESULT, _compoundsList);
}

BOOL LibParaSearchResultView::OnInitDialog(){
	CDialogEx::OnInitDialog();
	initListCtrl();
	return TRUE;
}


BEGIN_MESSAGE_MAP(LibParaSearchResultView, CDialogEx)
END_MESSAGE_MAP()

VOID LibParaSearchResultView::initListCtrl() {
	_compoundsList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	_compoundsList.SetTextColor(RGB(45,45,45));

	_compoundsList.InsertColumn(0, _T("ID."), LVCFMT_LEFT,50);
	_compoundsList.InsertColumn(1, _T("化合物名称"), LVCFMT_LEFT,510);
	_compoundsList.InsertColumn(2, _T("分子式"), LVCFMT_LEFT,110);
	_compoundsList.InsertColumn(3, _T("分子量"), LVCFMT_LEFT,50);
	_compoundsList.InsertColumn(4, _T("CAS号"), LVCFMT_LEFT,100); 
}

void LibParaSearchResultView::fillCompoundList(const std::vector<Compound> &compounds) {

	//clearList();
	//_compounds = compounds;

	const int listRows = compounds.size();
	const int listCols = 5;
	for (int row = 0; row < listRows; row++) {


		CString strCompound[listCols];
		strCompound[0].Format(_T("%d"), compounds[row]._compoundID);
		strCompound[1] = compounds[row]._compoundName.c_str();
		strCompound[2] = compounds[row]._formula.c_str();
		strCompound[3].Format(_T("%d"), compounds[row]._massWeight);
		strCompound[4] = compounds[row]._casNo.c_str();

		_compoundsList.InsertItem(row, strCompound[0], row);
		for (int col = 1; col < listCols; col++) {
			_compoundsList.SetItemText(row, col, strCompound[col]);
		}
	}
}
// LibParaSearchResultView 消息处理程序
