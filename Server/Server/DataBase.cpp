#include "DataBase.h"
#include "Network.h"

CDataBase::CDataBase()
{
}

CDataBase::~CDataBase()
{
	DisConnectDataBase();
}

void CDataBase::Init()
{
	if (!ConnectDataBase()) {
		cout << "Database Coonect Fail" << endl;
	}
	else {
		m_DatabaseEventQueue.clear();
		m_SaveStateQueue.clear();
		m_vUserData.clear();
	}
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
	m_ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);

	if (m_ret == SQL_SUCCESS || m_ret == SQL_SUCCESS_WITH_INFO) {
		m_ret = SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
		if (m_ret == SQL_SUCCESS || m_ret == SQL_SUCCESS_WITH_INFO) {
			m_ret = SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc);
			if (m_ret == SQL_SUCCESS || m_ret == SQL_SUCCESS_WITH_INFO) {
				m_ret = SQLSetConnectAttr(m_hDbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)(5), 0);
				m_ret = SQLConnect(m_hDbc, (SQLWCHAR*)L"game_db_odbc", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);
				cout << "DataBase Connect" << endl;
				return true;
			}
			else {
				HandleDiagnosticRecord(m_hStmt, SQL_HANDLE_STMT, m_ret);
				return false;
			}
		}
		else {
			HandleDiagnosticRecord(m_hStmt, SQL_HANDLE_STMT, m_ret);
			return false;
		}
	}
	else{
		HandleDiagnosticRecord(m_hStmt, SQL_HANDLE_STMT, m_ret);
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

void CDataBase::ChnageStateProcess(DB_Event & event)
{
}

void CDataBase::ChangeInvenProcess(DB_Event & event)
{
}

void CDataBase::PushEvent_Change_State(USHORT & player_Id)
{
	/*DB_Event ev;
	ev.state = (UINT)DB_STATE_TYPE::CHANGE_STATE;*/

}

bool CDataBase::IsIDExist(wstring login_id)
{
	shared_lock<shared_mutex>	lock(m_smDBMutex);
	wstring execFunc = L"EXEC select_id " + login_id;

	m_ret = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);
	m_ret = SQLExecDirect(m_hStmt, (SQLWCHAR*)execFunc.c_str(), SQL_NTS);
	m_ret = SQLBindCol(m_hStmt, 1, SQL_C_WCHAR, &m_dUserId, MAX_STR_LEN, &cbID);

	bool isExist = false;

	for (int i = 0;; ++i)
	{
		m_ret = SQLFetch(m_hStmt); // 왜 여기서 문제 생기지
		if (m_ret == SQL_ERROR || m_ret == SQL_SUCCESS_WITH_INFO)
			HandleDiagnosticRecord(m_hStmt, SQL_HANDLE_STMT, m_ret);
		if (m_ret == SQL_SUCCESS || m_ret == SQL_SUCCESS_WITH_INFO);
		else
			break;
		isExist = true;
	}

	SQLCancel(m_hStmt);
	SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
	return isExist;
}

void CDataBase::AddUserInfo(DB_Event & _ev)
{
	wstring execFunc = L"EXEC insert_info ";
	wstring var = to_wstring(_ev.inum) + L", " + _ev.strID + L", " + to_wstring(_ev.fHealth)
		+ L", " + to_wstring(_ev.fHungry) + L", " + to_wstring(_ev.fThirst) + L", "
		+ to_wstring(_ev.fX) + L", " + to_wstring(_ev.fY) + L", " + to_wstring(_ev.fZ);
	execFunc += var;

	m_ret = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);
	m_ret = SQLExecDirect(m_hStmt, (SQLWCHAR*)execFunc.c_str(), SQL_NTS);

	if (m_ret == SQL_SUCCESS || m_ret == SQL_SUCCESS_WITH_INFO)
	{
		m_ret = SQLBindCol(m_hStmt, 2, SQL_C_WCHAR, &m_dUserId, 100, &cbID);
		if (m_ret == SQL_SUCCESS || m_ret == SQL_SUCCESS_WITH_INFO)
			m_ret = SQLFetch(m_hStmt);
		else
			HandleDiagnosticRecord(m_hStmt, SQL_HANDLE_STMT, m_ret);
	}
	else
		HandleDiagnosticRecord(m_hStmt, SQL_HANDLE_STMT, m_ret);

	SQLCancel(m_hStmt);
	SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
}

void CDataBase::UpdateUserInfo(DB_Event & _ev)
{
	unique_lock<shared_mutex>	lock(m_smDBMutex);

	wstring execFunc = L"EXEC update_info ";
	wstring var = to_wstring(_ev.inum) + L", " + to_wstring(_ev.fHealth) 
		+ L", " + to_wstring(_ev.fHungry) + L", " + to_wstring(_ev.fThirst) 
		+ L", " + to_wstring(_ev.fX) + L", " + to_wstring(_ev.fY) + L", " + to_wstring(_ev.fZ);
	execFunc += var;

	m_ret = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);
	m_ret = SQLExecDirect(m_hStmt, (SQLWCHAR*)execFunc.c_str(), SQL_NTS);
	if (m_ret == SQL_SUCCESS || m_ret == SQL_SUCCESS_WITH_INFO)
		m_ret = SQLFetch(m_hStmt);
	else
		HandleDiagnosticRecord(m_hStmt, SQL_HANDLE_STMT, m_ret);
	SQLCancel(m_hStmt);
	SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
}

DB_Event & CDataBase::GetUserInfo(wstring & login_id)
{
	wstring execFunc = L"EXEC select_info " + login_id;

	m_ret = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);
	m_ret = SQLExecDirect(m_hStmt, (SQLWCHAR*)execFunc.c_str(), SQL_NTS);

	m_ret = SQLBindCol(m_hStmt, 1, SQL_C_LONG, &m_dUserNum, 100, &cbNum);
	m_ret = SQLBindCol(m_hStmt, 2, SQL_C_WCHAR, &m_dUserId, MAX_STR_LEN + 1, &cbID);
	m_ret = SQLBindCol(m_hStmt, 3, SQL_C_SHORT, &m_dUserHp, 100, &cbHp);
	m_ret = SQLBindCol(m_hStmt, 4, SQL_C_SHORT, &m_dUserHungry, 100, &cbHungry);
	m_ret = SQLBindCol(m_hStmt, 5, SQL_C_SHORT, &m_dUserThirst, 100, &cbThirst);
	m_ret = SQLBindCol(m_hStmt, 6, SQL_C_DOUBLE, &m_dUserX, 100, &cbX);
	m_ret = SQLBindCol(m_hStmt, 7, SQL_C_DOUBLE, &m_dUserY, 100, &cbY);
	m_ret = SQLBindCol(m_hStmt, 8, SQL_C_DOUBLE, &m_dUserZ, 100, &cbZ);

	DB_Event ev{};
	for (int i = 0;; i++) {
		m_ret = SQLFetch(m_hStmt);
		if (m_ret == SQL_ERROR || m_ret == SQL_SUCCESS_WITH_INFO)
			HandleDiagnosticRecord(m_hStmt, SQL_HANDLE_STMT, m_ret);
		if (m_ret == SQL_SUCCESS || m_ret == SQL_SUCCESS_WITH_INFO) {
			ev.inum = m_dUserNum;
			ev.strID = m_dUserId;
			ev.fHealth = m_dUserHp;
			ev.fHungry = m_dUserHungry;
			ev.fThirst = m_dUserThirst;
			ev.fX = m_dUserX;
			ev.fY = m_dUserY;
			ev.fZ = m_dUserZ;
		}
		else
			break;
	}
	SQLCancel(m_hStmt);
	SQLFreeHandle(SQL_HANDLE_STMT, m_hStmt);
	return ev;
}
