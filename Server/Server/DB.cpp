#include "server.h"

#define MAX_STR_LEN 10
SQLHENV henv;
SQLHDBC hdbc;
SQLHSTMT hstmt = 0;
SQLRETURN retcode;
SQLWCHAR sz_id[MAX_STR_LEN], sz_password[MAX_STR_LEN], sz_nickname[MAX_STR_LEN];
float db_x, db_y, db_z;
int db_hp, db_maxhp, db_item[4], db_connect;
SQLLEN cb_id = 0, cb_password = 0, cb_nickname = 0, cb_x = 0, cb_y = 0, cb_z = 0, cb_hp = 0, cb_maxhp = 0,  cb_item[4]{ 0 }, cb_connect = 0;

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


int get_DB_Info(int ci) {
	// Connect to data source  
	retcode = SQLConnect(hdbc, (SQLWCHAR*)L"FineDust", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);

	// Allocate statement handle  
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

		sprintf(buf, "EXEC dbo.user_get_info %s, %s", clients[ci].GetGameId(), clients[ci].GetGamePassword());
		MultiByteToWideChar(CP_UTF8, 0, buf, strlen(buf), sql_data, sizeof sql_data / sizeof *sql_data);
		sql_data[strlen(buf)] = '\0';

		retcode = SQLExecDirect(hstmt, sql_data, SQL_NTS);
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

			// Bind columns 1, 2, and 3  
			retcode = SQLBindCol(hstmt, 1, SQL_WCHAR, sz_id, MAX_STR_LEN, &cb_id);
			retcode = SQLBindCol(hstmt, 2, SQL_WCHAR, sz_password, MAX_STR_LEN, &cb_password);
			retcode = SQLBindCol(hstmt, 3, SQL_WCHAR, sz_nickname, MAX_STR_LEN, &cb_nickname);
			retcode = SQLBindCol(hstmt, 4, SQL_FLOAT, &db_x, MAX_STR_LEN, &cb_x);
			retcode = SQLBindCol(hstmt, 5, SQL_FLOAT, &db_y, MAX_STR_LEN, &cb_y);
			retcode = SQLBindCol(hstmt, 6, SQL_FLOAT, &db_y, MAX_STR_LEN, &cb_z);
			retcode = SQLBindCol(hstmt, 7, SQL_INTEGER, &db_hp, MAX_STR_LEN, &cb_hp);
			retcode = SQLBindCol(hstmt, 8, SQL_INTEGER, &db_maxhp, MAX_STR_LEN, &cb_maxhp);
			retcode = SQLBindCol(hstmt, 9, SQL_INTEGER, &db_item[0], MAX_STR_LEN, &cb_item[0]);
			retcode = SQLBindCol(hstmt, 10, SQL_INTEGER, &db_item[1], MAX_STR_LEN, &cb_item[1]);
			retcode = SQLBindCol(hstmt, 11, SQL_INTEGER, &db_item[2], MAX_STR_LEN, &cb_item[2]);
			retcode = SQLBindCol(hstmt, 12, SQL_INTEGER, &db_item[3], MAX_STR_LEN, &cb_item[3]);
			retcode = SQLBindCol(hstmt, 13, SQL_INTEGER, &db_connect, MAX_STR_LEN, &cb_connect);

			// Fetch and print each row of data. On an error, display a message and exit.  

			retcode = SQLFetch(hstmt);

			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
#if (DebugMod == TRUE )
				printf("ID : %s\tX : %d\tY : %d\n", sz_id, db_x, db_y);
#endif
				SQLDisconnect(hdbc);
				return DB_LOGIN_SUCCESS;
			}
		}

		if (retcode == SQL_NO_DATA) {
			SQLDisconnect(hdbc);
			return DB_NO_DATA;
		}

		if (retcode == SQL_ERROR) {
			SQLDisconnect(hdbc);
			return DB_LOGIN_FAIL;
		}
	}
	SQLDisconnect(hdbc);
}

void set_DB_Info(int ci) {
	// Connect to data source  
	retcode = SQLConnect(hdbc, (SQLWCHAR*)L"FineDust", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);

	// Allocate statement handle  
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

		sprintf(buf, "EXEC dbo.set_user %s, %s, %s, %f, %f, %f, %d, %d, %d, %d, %d, %d, %d",clients[ci].GetGameId(), clients[ci].GetGamePassword(), clients[ci].GetNickname(), clients[ci].GetXPos(), clients[ci].GetYPos(), clients[ci].GetZPos(), clients[ci].GetHp(), clients[ci].GetMaxhp(), clients[ci].GetItem(0), clients[ci].GetItem(1), clients[ci].GetItem(2), clients[ci].GetItem(3), 1);
		MultiByteToWideChar(CP_UTF8, 0, buf, strlen(buf), sql_data, sizeof sql_data / sizeof *sql_data);
		sql_data[strlen(buf)] = '\0';

		retcode = SQLExecDirect(hstmt, sql_data, SQL_NTS);
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {


			// Bind columns 1, 2, and 3  
			retcode = SQLBindCol(hstmt, 1, SQL_WCHAR, sz_id, MAX_STR_LEN, &cb_id);
			retcode = SQLBindCol(hstmt, 2, SQL_WCHAR, sz_password, MAX_STR_LEN, &cb_password);
			retcode = SQLBindCol(hstmt, 3, SQL_WCHAR, sz_nickname, MAX_STR_LEN, &cb_nickname);
			retcode = SQLBindCol(hstmt, 4, SQL_FLOAT, &db_x, MAX_STR_LEN, &cb_x);
			retcode = SQLBindCol(hstmt, 5, SQL_FLOAT, &db_y, MAX_STR_LEN, &cb_y);
			retcode = SQLBindCol(hstmt, 6, SQL_FLOAT, &db_y, MAX_STR_LEN, &cb_z);
			retcode = SQLBindCol(hstmt, 7, SQL_INTEGER, &db_hp, MAX_STR_LEN, &cb_hp);
			retcode = SQLBindCol(hstmt, 8, SQL_INTEGER, &db_maxhp, MAX_STR_LEN, &cb_maxhp);
			retcode = SQLBindCol(hstmt, 9, SQL_INTEGER, &db_item[0], MAX_STR_LEN, &cb_item[0]);
			retcode = SQLBindCol(hstmt, 10, SQL_INTEGER, &db_item[1], MAX_STR_LEN, &cb_item[1]);
			retcode = SQLBindCol(hstmt, 11, SQL_INTEGER, &db_item[2], MAX_STR_LEN, &cb_item[2]);
			retcode = SQLBindCol(hstmt, 12, SQL_INTEGER, &db_item[3], MAX_STR_LEN, &cb_item[3]);
			retcode = SQLBindCol(hstmt, 13, SQL_INTEGER, &db_connect, MAX_STR_LEN, &cb_connect);

			// Fetch and print each row of data. On an error, display a message and exit.  

			retcode = SQLFetch(hstmt);
			if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO) {
			}

			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
#if (DebugMod == TRUE )
				printf("ID : %s\tX : %d\tY : %d\n", sz_id, db_x, db_y);
#endif
			}

		}
	}
	SQLDisconnect(hdbc);
}

void new_DB_Id(int ci) {
	// Connect to data source  
	retcode = SQLConnect(hdbc, (SQLWCHAR*)L"FineDust", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);

	// Allocate statement handle  
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

		sprintf(buf, "EXEC dbo.insert_user %s, %s, %s, %f, %f, %f, %d", clients[ci].GetGameId(), clients[ci].GetGamePassword(), clients[ci].GetNickname(), clients[ci].GetXPos(), clients[ci].GetYPos(), clients[ci].GetZPos(), clients[ci].GetHp());
		MultiByteToWideChar(CP_UTF8, 0, buf, strlen(buf), sql_data, sizeof sql_data / sizeof *sql_data);
		sql_data[strlen(buf)] = '\0';

		retcode = SQLExecDirect(hstmt, sql_data, SQL_NTS);
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

			retcode = SQLFetch(hstmt);
			if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO) {
			}

			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
#if (DebugMod == TRUE )
				printf("ID : %s\tX : %d\tY : %d\n", sz_id, db_x, db_y);
#endif
			}

		}
	}
	SQLDisconnect(hdbc);
}

void set_DB_Shutdown(int ci) {
	// Connect to data source  
	retcode = SQLConnect(hdbc, (SQLWCHAR*)L"FineDust", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);

	// Allocate statement handle  
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);

		//sprintf(buf, "EXEC dbo.shutdown_id %s", g_clients[ci].game_id);
		MultiByteToWideChar(CP_UTF8, 0, buf, strlen(buf), sql_data, sizeof sql_data / sizeof *sql_data);
		sql_data[strlen(buf)] = '\0';

		retcode = SQLExecDirect(hstmt, sql_data, SQL_NTS);
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			retcode = SQLFetch(hstmt);
			if (retcode == SQL_ERROR || retcode == SQL_SUCCESS_WITH_INFO) {
			}

			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
#if (DebugMod == TRUE )
				printf("ID : %s\tX : %d\tY : %d\n", sz_id, db_x, db_y);
#endif
			}

		}
	}
	SQLDisconnect(hdbc);
}