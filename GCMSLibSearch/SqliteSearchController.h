#pragma once
#include "sqlite3.h"
#include "Compound.h"

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "sqlite3_D.lib")
#else
#pragma comment(lib, "sqlite3_R.lib")
#endif



class SqliteSearchController
{
public:
	SqliteSearchController(void);
	~SqliteSearchController(void);

	bool connectSQLite();
	void closeSQLite();

	void queryCompoundData();
	Compound getCompound(int compoundID);


private:
	sqlite3* _ppDB;
};

