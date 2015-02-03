#### 编译静态sqlite3.lib  http://www.cnblogs.com/imoon/archive/2012/11/30/2796726.html
VC++使用 sqlite 数据库模板，官网下载到动态dll，需要自己编译成静态sqlite3.lib文件
从VS2010的安装目录下Microsoft Visual Studio 10.0\VC\bin找到lib.exe和link.exe
从VS2010的安装目录下\Microsoft Visual Studio 10.0\Common7\IDE找到mspdb100.dll。
将lib.exe link.exe mspdb100.dll放到的sqlite-dll文件夹下。
动态的sqlite3.lib（47KB）     运行.exe 时需要加入对应dll【输入cmd命令：LIB /DEF:sqlite3.def /machine:IX86】
静态的【Debug】sqlite3.lib（2.32MB） 运行.exe 时不再需要对应dll支持，因为编译的时候已经把声明实现加入【lib sqlite3.obj】

#### 数据库文件转换
Access 数据库文件 ms.accdb 另存为 ms.mdb 
利用 SQLite Expert 转换为 ms.db 
Provider=Microsoft.Jet.OLEDB.4.0;Data Source=E:\XKTV_2012_0515.mdb;Persist Security Info=False

#### 使用SuperChart控件
```
#include "SuperChartLib.h"
//#include "SHP_Public.h"
//#include "ViewXML\PAInformation.h"
#if defined(DEBUG) || defined(_DEBUG)
#pragma comment (lib, "SuperChartCtrl_D.lib")
#else
#pragma comment (lib, "SuperChartCtrl_R.lib")
#endif
```
系统配置要把 v100 -> v90

#### Debug -> Release 版
#include "SuperChartLib.h" //导致无法打开包括文件:“SuperChartLib.h”

#### sqlite3集成
编译错误：fatal error C1900: “P1”(第“20081201”版)和“P2”(第“20080116”版)之间 Il 不匹配
要编译Debug版本的 sqlite3.lib 使用

#### sqlite3 API 操作 http://www.sqlite.org/c3ref/funclist.html

用sqlite3_prepare、sqlite3_step、sqlite3_column_xxxx、sqlite3_finalize 这一套函数
