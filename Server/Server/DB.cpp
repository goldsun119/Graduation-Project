#include "server.h"

#define Default_LEN 10
SQLHENV henv;
SQLHDBC hdbc;
SQLHSTMT hstmt = 0;
SQLRETURN retcode;
SQLWCHAR sz_id[Default_LEN];
int db_x, db_y, db_level, db_hp, db_maxhp, db_exp, db_skill[3], db_connect;
SQLLEN cb_id = 0, cb_x = 0, cb_y = 0, cb_level = 0, cb_hp = 0, cb_maxhp = 0, cb_exp = 0, cb_skill[3]{ 0 }, cb_connect = 0;

char  buf[255];
wchar_t sql_data[255];
SQLWCHAR sqldata[255] = { 0 };

void init_DB()
{
	// Allocate environment handle  
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
	// Set the ODBC version environment attribute  
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
		// Allocate connection handle  
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);

			// Set login timeout to 5 seconds  
			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)
			{
				SQLSetConnectAttr(hdbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)5, 0);
			}
		}
	}

	std::cout << std::endl << "DB Load Complete!" << std::endl;
}