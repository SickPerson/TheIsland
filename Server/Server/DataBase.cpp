#include "DataBase.h"

DEFINITION_SINGLE(CDataBase)

CDataBase::CDataBase()
{
	if (!ConnectDataBase()) {
		std::cout << "DataBaseConnect Fail" << std::endl;
	}
	else {
		m_ccqDataBaseQueue.clear();
		m_ccqSaveStateQueue.clear();
	}
}

CDataBase::~CDataBase()
{
	DisConnectDataBase();
}

void CDataBase::HandleDiagnosticRecord(SQLHANDLE _hHandle, SQLSMALLINT _hType, RETCODE RetCode)
{
	SQLSMALLINT	iRec = 0;
	SQLINTEGER	iError;
	WCHAR	wszMessage[1000];
	WCHAR	wszState[SQL_SQLSTATE_SIZE + 1];

	setlocale(LC_ALL, "korean");
	if (RetCode == SQL_INVALID_HANDLE) {
		fwprintf(stderr, L"Invalid handle!\n");
		return;
	}
	while (SQLGetDiagRec(_hType, _hHandle, ++iRec, (SQLCHAR*)wszState, &iError, (SQLCHAR*)wszMessage,
		(SQLSMALLINT)(sizeof(wszMessage) / sizeof(WCHAR)), (SQLSMALLINT*)NULL) == SQL_SUCCESS) {
		// Hide data truncated..
		if (wcsncmp(wszState, L"01004", 5)) {
			fwprintf(stderr, L"[%5.5s] %s (%d)\n", wszState, wszMessage, iError);
		}
	}
}

bool CDataBase::ConnectDataBase()
{
	SQLRETURN ret;
	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
		ret = SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
			ret = SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc);
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
				ret = SQLSetConnectAttr(m_hDbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)(5), 0);
				ret = SQLConnect(m_hDbc, (SQLWCHAR*)L"20")
			}
		}
	}
	return false;
}

void CDataBase::DisConnectDataBase()
{
	SQLCancel(m_hStmt);
	SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
	SQLDisconnect(m_hDbc);
}

void CDataBase::RunDataBase(DATABASE_TYPE & event)
{
}

void CDataBase::LogInPorcess(DATABASE_TYPE & event)
{
}

void CDataBase::LogOutPorcess(DATABASE_TYPE & event)
{
}
