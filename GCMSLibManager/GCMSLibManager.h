// GCMSLibManager.h : GCMSLibManager DLL 的主头文件
#pragma once
#include "StdAfx.h" 
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "Resource.h"		
#include "Compound.h"
#include <vector>


#define LIBMANAGERDLL_EXPORTS
#ifdef  LIBMANAGERDLL_EXPORTS
#define LIBMANAGERDLL_API __declspec(dllexport)
#else
#define LIBMANAGERDLL_API __declspec(dllimport) 
#endif

// - 导出 DLL - //

// 【谱库检索算法】
// 输入：检索策略配置参数 libConfig
//		 单个样品化合物
// 输出：谱库化合物组
LIBMANAGERDLL_API VOID GCMSLibSearch(const LibConfig& libConfig, Compound &testCompound, std::vector<Compound> &libCompounds);

// 【谱库检索策略 对话框界面】
// 输入：配置参数 libConfig
// 输出：修改后的配置参数 Ref
// 返回：策略是否改变
LIBMANAGERDLL_API bool GCMSLibSearchStrategy(LibConfig& libConfig, CWnd* pParent = NULL);

// 【谱库的增删改 对话框界面】
// 输入： 1.  默认数据库路径 defaultDB
//		  2. 当前MS峰数据 std::string peakData
LIBMANAGERDLL_API VOID GCMSLibSetting(const CString defaultLibPathName, CWnd* pParent = NULL);

// 【参数检索 对话框界面】
// 输入： 1.默认数据库路径 defaultDB
LIBMANAGERDLL_API VOID GCMSLibParaSearch(const CString defaultLibPathName, CWnd* pParent = NULL);



// 调用编译前可删
class CGCMSLibManagerApp : public CWinApp {
public:
	CGCMSLibManagerApp() { }

// 重写
public:
	virtual BOOL InitInstance() { CWinApp::InitInstance(); return TRUE; }

	DECLARE_MESSAGE_MAP()
};


