// LibSettingView.cpp : 实现文件
//

#include "stdafx.h"
#include "LibSettingView.h"
#include "afxdialogex.h"


// LibSettingView 对话框

IMPLEMENT_DYNAMIC(LibSettingView, CDialogEx)

LibSettingView::LibSettingView(CWnd* pParent /*=NULL*/)
	: CDialogEx(LibSettingView::IDD, pParent)
{

}

LibSettingView::~LibSettingView()
{
}

void LibSettingView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(LibSettingView, CDialogEx)
END_MESSAGE_MAP()


// LibSettingView 消息处理程序
