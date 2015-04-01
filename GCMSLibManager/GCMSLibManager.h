// GCMSLibManager.h : GCMSLibManager DLL 的主头文件
//

#pragma once
#include "StdAfx.h" 

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "Resource.h"		
#include "Compound.h"
//#include "../GCMSLibSearch/Compound.h"


// CGCMSLibManagerApp
// 有关此类实现的信息，请参阅 GCMSLibManager.cpp
//

#define LIBMANAGERDLL_EXPORTS
#ifdef LIBMANAGERDLL_EXPORTS
#define LIBMANAGERDLL_API __declspec(dllexport)
#else
#define LIBMANAGERDLL_API __declspec(dllimport) 
#endif

// - 导出 DLL - //

// 【谱库检索策略 对话框界面】
// 输入：配置参数 libConfig
// 输出：修改后的配置参数 Ref
LIBMANAGERDLL_API VOID GCMSLibSearchStrategy(CWnd* pParent, LibConfig& libConfig);

// 【谱库的增删改 对话框界面】
// 输入： 1.默认数据库路径 defaultDB
//		  2. 当前MS峰数据 std::string peakData
LIBMANAGERDLL_API VOID GCMSLibSetting(CWnd* pParent);


class CGCMSLibManagerApp : public CWinApp {
public:
	CGCMSLibManagerApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
