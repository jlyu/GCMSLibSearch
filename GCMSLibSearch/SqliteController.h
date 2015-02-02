#pragma once

#include "sqlite3.h"
#include "Compound.h"

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "sqlite3_D.lib")
#else
#pragma comment(lib, "sqlite3_R.lib")
#endif


#define DEFAULT_SQLITE_DB  "ms.db"



class SqliteController
{
public:
	SqliteController(void);
	~SqliteController(void);

//½Ó¿Ú
	bool openSQLiteDB(const std::string &dbName = DEFAULT_SQLITE_DB);


	bool connectSQLite();
	void closeSQLite();

	void queryCompoundData(std::vector<Compound> &selectedCompounds);
	Compound getCompound(int compoundID);


private:
	sqlite3* _ppDB;
};

