// GCMSLibManager.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "GCMSLibManager.h"
#include "LibSearchStrategyView.h"
#include "LibSettingView.h"
#include "LibParaSearchView.h"
#include "SqliteController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// - DLL 导出
LIBMANAGERDLL_API VOID GCMSLibSearch(const LibConfig& libConfig, Compound &testCompound, std::vector<Compound> &libCompounds) {
	
	if (!libCompounds.empty()) { libCompounds.clear(); }
	
	CString libPathName[] = { libConfig._dbPath1st, libConfig._dbPath2nd, libConfig._dbPath3rd };
	int matchDegree[] = { libConfig._minMass1st, libConfig._minMass2nd, libConfig._minMass3rd };

	for (int t = 0; t != 3; t++) {

		const std::string strLibPathName = CT2A(libPathName[t]);
		SqliteController sqliteController(strLibPathName);
		testCompound = sqliteController.getCompound(100); // del
		sqliteController.libSearch(libConfig, testCompound, libCompounds);

		const int libCompoundsSize = libCompounds.size();
		for (int i = 0; i != libCompoundsSize; i++) {
			if (libCompounds[i]._matchDegree > matchDegree[t]) {
				return;
			}
		}
	}

	libCompounds.clear();
}

bool GCMSLibSearchStrategy(LibConfig& libConfig, CWnd* pParent /* = NULL */ ) {
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	LibSearchStrategyView libManagerView(pParent, libConfig);
	INT_PTR ret = libManagerView.DoModal();
	libConfig = libManagerView._libConfig;

	if (ret == IDOK) { return true; }
	return false;
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


//// CGCMSLibManagerApp
BEGIN_MESSAGE_MAP(CGCMSLibManagerApp, CWinApp)
END_MESSAGE_MAP()


CGCMSLibManagerApp theApp;


