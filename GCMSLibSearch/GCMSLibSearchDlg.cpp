
// GCMSLibSearchDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "afxdialogex.h"
#include "GCMSLibSearch.h"
#include "GCMSLibSearchDlg.h"

//DLL
#include "..\GCMSLibManager\SqliteController.h"
#include "..\GCMSLibManager\GCMSLibManager.h"

#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGCMSLibSearchDlg 对话框




CGCMSLibSearchDlg::CGCMSLibSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGCMSLibSearchDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGCMSLibSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGCMSLibSearchDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CGCMSLibSearchDlg::OnBnLoadTestingPeakData)
	//ON_BN_CLICKED(IDCANCEL, &CGCMSLibSearchDlg::OnOpenSearchResult)
	ON_BN_CLICKED(IDC_BUTTON1, &CGCMSLibSearchDlg::OnBnShowSearchResult)
	ON_BN_CLICKED(IDC_BUTTON2, &CGCMSLibSearchDlg::OnBnClickedCallDLL)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_DB, &CGCMSLibSearchDlg::OnBnClickedLibSetting)
	ON_BN_CLICKED(IDC_BUTTON_PARA_SEARCH, &CGCMSLibSearchDlg::OnBnClickedParaSearch)
END_MESSAGE_MAP()


// CGCMSLibSearchDlg 消息处理程序

BOOL CGCMSLibSearchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	
	init();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CGCMSLibSearchDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CGCMSLibSearchDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CGCMSLibSearchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGCMSLibSearchDlg::OnCancel() {
	CDialogEx::OnCancel();
}

void CGCMSLibSearchDlg::init() {
	CRect rcPeak, rcCompare;
	GetDlgItem(IDC_PEAK)->GetWindowRect(rcPeak);
	ScreenToClient(rcPeak);
	GetDlgItem(IDC_PEAK_COMPARE)->GetWindowRect(rcCompare);
	ScreenToClient(rcCompare);

	_peakChart.Create(this, rcPeak, 0, WS_CHILD|WS_VISIBLE);
	_compareChart.Create(this, rcCompare, 0, WS_CHILD|WS_VISIBLE);

	_peakDrawView.setChartCtrl(&_peakChart, &_compareChart);
}

void CGCMSLibSearchDlg::OnBnLoadTestingPeakData() {

	CString peakData(_T("12 108;13 229;14 999;15 22;26 18;27 58;28 178;29 23;40 18;41 108;42 431;43 8;"));

	_peakDrawView.drawPeak(peakData);

}

void CGCMSLibSearchDlg::drawPeakCompare(const CString &strPeakData) {
	_peakDrawView.drawPeakCompare(strPeakData);
}


void CGCMSLibSearchDlg::OnBnShowSearchResult()
{
	std::vector<Compound> compounds;

	////开始谱库搜索逻辑
	//SqliteController sqliteController;
	//sqliteController.queryCompoundData(compounds);

	////

	CSearchResultView *pSearchResultDlg = CSearchResultView::getInstance();
	pSearchResultDlg->fillCompoundList(compounds);
	pSearchResultDlg->setNofityObject(this);
	pSearchResultDlg->ShowWindow(SW_SHOW);

}




void CGCMSLibSearchDlg::OnBnClickedCallDLL() {

	// 尝试在此调用 DLL 接口
	LibConfig libConfig;
	libConfig._dbPath1st = "wkdjfklajsdlfka";
	libConfig._dbPath2nd = "fakfklajsfklaj";
	libConfig._dbPath3rd = "dfa3";
	libConfig._minMass1st = 10;
	libConfig._minMass2nd = 20;
	libConfig._minMass3rd = 30;
	libConfig._lowerMass = 1;
	libConfig._upperMass = 999;
	libConfig._matchLimitNumber = 20;
	libConfig._isUnique = true;


	CString str(libConfig._dbPath1st.c_str());

	GCMSLibSearchStrategy(libConfig, this);
}


void CGCMSLibSearchDlg::OnBnClickedLibSetting() {
	
	const CString libPathName = CString(_T("E:\\GCMSLibSearch\\GCMSLibSearch\\nist.db"));
	GCMSLibSetting(libPathName, this);
}


void CGCMSLibSearchDlg::OnBnClickedParaSearch() {
	
	const CString libPathName = CString(_T("E:\\GCMSLibSearch\\GCMSLibSearch\\nist.db"));
	GCMSLibParaSearch(libPathName, this);
}
