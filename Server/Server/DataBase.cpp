#include "DataBase.h"

CDataBase::CDataBase()
{
	if (!ConnectDataBase()) {
		std::cout << "DataBaseConnect Fail" << std::endl;
	}
	else {
		m_ccqDataBaseQueue.clear();
		m_ccqSaveStateQueue.clear();
		m_UserData.clear();
		BindDataBaseFP();
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
	while (SQLGetDiagRec(_hType, _hHandle, ++iRec, (SQLWCHAR*)wszState, &iError, (SQLWCHAR*)wszMessage,
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
				ret = SQLConnect(m_hDbc, (SQLWCHAR*)L"TheIsLand", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);
				if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
					ret = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);
					cout << "Connect DataBase" << endl;
					return true;
				}
				else {
					HandleDiagnosticRecord(m_hStmt, SQL_HANDLE_STMT, ret);
					return false;
				}
			}
			else {
				HandleDiagnosticRecord(m_hStmt, SQL_HANDLE_STMT, ret);
				return false;
			}
		}
		else {
			HandleDiagnosticRecord(m_hStmt, SQL_HANDLE_STMT, ret);
			return false;
		}
		HandleDiagnosticRecord(m_hStmt, SQL_HANDLE_STMT, ret);
		return false;
	}
	return false;
}

void CDataBase::DisConnectDataBase()
{
	SQLCancel(m_hStmt);
	SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
	SQLDisconnect(m_hDbc);
}

void CDataBase::RunDataBase(DataBase_Event & event)
{
	m_fpDataBaseProcess[event.state](event);
}

void CDataBase::LogInPorcess(DataBase_Event & event)
{
}

void CDataBase::LogOutPorcess(DataBase_Event & event)
{
}

void CDataBase::BindDataBaseFP()
{
	m_fpDataBaseProcess[DB_LOGIN] = [&](DataBase_Event& event) {LogInPorcess(event); };
	m_fpDataBaseProcess[DB_LOGOUT] = [&](DataBase_Event& event) {LogOutPorcess(event); };
}

bool CDataBase::DataBaseQueueIsEmpty()
{
	return m_ccqDataBaseQueue.empty();
}

void CDataBase::InsertToDataBaseQueue(DataBase_Event & event)
{
	m_ccqDataBaseQueue.push(event);
}

bool CDataBase::PopFromDataBaseQueue(DataBase_Event & event)
{
	return m_ccqDataBaseQueue.try_pop(event);
}

void CDataBase::InsertToStateQueue(DataBase_Event & event)
{
	m_ccqSaveStateQueue.push(event);
}

bool CDataBase::PopFromStateQueue(DataBase_Event & event)
{
	return m_ccqSaveStateQueue.try_pop(event);
}

bool CDataBase::GetIsLogin(std::string sID)
{
	for (auto& player : m_UserData) {
		if (player.m_sID == sID)
			return true;
	}
	return false;
}

bool CDataBase::GetIsLogin(unsigned int usID)
{
	for (auto& player : m_UserData) {
		if (player.m_usID == usID) {
			return true;
		}
	}
	return false;
}

void CDataBase::SetIsLogin(int iPlayerNum, unsigned int usID, std::string ID)
{
	m_UserData.emplace_back(User_Data{ ID, usID, iPlayerNum });
}

int CDataBase::FindIsLogin(unsigned int usID, wchar_t * user_id, bool bDelete)
{
	std::vector<User_Data>::iterator	iter;
	for (iter = m_UserData.begin(); iter != m_UserData.end();) {
		if (iter->m_usID == usID) {
			return iter->m_iPlayerNum;
		}
		else
			++iter;
	}
	return static_cast<int>(MAX_USER);
}
