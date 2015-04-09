// GCMSLibManager.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "GCMSLibManager.h"
#include "LibSearchStrategyView.h"
#include "LibSettingView.h"
#include "LibParaSearchView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// - DLL 导出

VOID GCMSLibSearchStrategy(LibConfig& libConfig, CWnd* pParent /* = NULL */ ) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	LibSearchStrategyView libManagerView(pParent, libConfig);
	if (libManagerView.DoModal()) {
		libConfig = libManagerView._libConfig;
	}
}

VOID GCMSLibSetting(const CString defaultLibPathName, CWnd* pParent /* = NULL */) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	LibSettingView libSettingView(defaultLibPathName, pParent);
	libSettingView.DoModal();
}

LIBMANAGERDLL_API VOID GCMSLibParaSearch(const CString defaultLibPathName, CWnd* pParent /* = NULL */) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	LibParaSearchView libParaSearchView(defaultLibPathName, pParent);
	libParaSearchView.DoModal();
}


// CGCMSLibManagerApp
BEGIN_MESSAGE_MAP(CGCMSLibManagerApp, CWinApp)
END_MESSAGE_MAP()


CGCMSLibManagerApp theApp;


