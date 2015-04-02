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

}

LibParaSearchView::~LibParaSearchView()
{
}

void LibParaSearchView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LibParaSearchView, CDialogEx)
END_MESSAGE_MAP()


// LibParaSearchView 消息处理程序
