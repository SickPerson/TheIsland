#include "DataBase.h"
#include "Network.h"

CDataBase::CDataBase()
{
	Init();
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
		BindDataBaseFP();
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
	SQLRETURN ret;
	ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &m_hEnv);
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
		ret = SQLSetEnvAttr(m_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0);
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
			ret = SQLAllocHandle(SQL_HANDLE_DBC, m_hEnv, &m_hDbc);
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
				ret = SQLSetConnectAttr(m_hDbc, SQL_LOGIN_TIMEOUT, (SQLPOINTER)(5), 0);
				ret = SQLConnect(m_hDbc, (SQLWCHAR*)L"game_db_odbc", SQL_NTS, (SQLWCHAR*)NULL, 0, NULL, 0);
				if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
					ret = SQLAllocHandle(SQL_HANDLE_STMT, m_hDbc, &m_hStmt);
					cout << "DataBase Connect" << endl;
					return true;
				}
				else {
					HandleDiagnosticRecord(m_hStmt, SQL_HANDLE_STMT, ret);
					return false;
				}
			}else {
				HandleDiagnosticRecord(m_hStmt, SQL_HANDLE_STMT, ret);
				return false;
			}
		}else {
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

void CDataBase::RunDataBase(DB_Event & _ev)
{
	m_fpDataBaseProcess[_ev.state](_ev);
}

void CDataBase::LogInProcess(DB_Event & _ev)
{
	//if (IsIDExist(_ev.sid)) {
	//	// ID 정보 존재 -> DB에서 불러옵니다.
	//}
	//else {
	//	// ID 정보 X -> 새로운 정보를 넣습니다.
	//}
	//char* id;
	//int for_str_len = WideCharToMultiByte(CP_ACP, 0, _ev., -1, NULL, 0, NULL, NULL);
	//id = new char[for_str_len];
	//WideCharToMultiByte(CP_ACP, 0, _ev.id, -1, id, for_str_len, 0, 0);
	//OVER_EX* lpover_ex = new OVER_EX;
	//ZeroMemory(&lpover_ex->m_Overlapped, sizeof(WSAOVERLAPPED));
	//lpover_ex->m_Event = EV_DB;

	//if (GetIsLogin(id)) {
	//	lpover_ex->m_Event = EV_DB;
	//	//lpover_ex->m_Status = DATA
	//	PostQueuedCompletionStatus(CNetwork::GetInst()->GetIocp(), 10, _ev.client_num, &lpover_ex->m_Overlapped);
	//	return;
	//}

	//wchar_t querry[MAX_BUF];

	//swprintf_s(querry, L"SELECT NUM, ID, HP, STAMINA, HUNGER, THIRST, POSX, POSY, POSZ, DIRX, DIRY, DIRZ FROM dbo.PlayerInfo WHERE ID = %s", event.id);

	//SQLWCHAR	name[MAX_STR_LEN];
	//SQLINTEGER	num, hp, stamina, hunger, thirst;
	//SQLFLOAT	posx, posy, posz, dirx, diry, dirz;
	//SQLLEN	b_num, b_name, b_hp, b_stamina, b_hunger, b_thirst, b_posx, b_posy, b_posz, b_dirx, b_diry, b_dirz;

	//SQLRETURN	ret;

	//ret = SQLExecDirect(m_hStmt, (SQLCHAR *)querry, SQL_NTS);
	//if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
	//	ret = SQLBindCol(m_hStmt, 1, SQL_C_LONG, &num, 4, &b_num);
	//	ret = SQLBindCol(m_hStmt, 2, SQL_C_CHAR, name, MAX_STR_LEN, &b_name);
	//	ret = SQLBindCol(m_hStmt, 3, SQL_C_LONG, &hp, 4, &b_hp);
	//	ret = SQLBindCol(m_hStmt, 4, SQL_C_LONG, &stamina, 4, &b_stamina);
	//	ret = SQLBindCol(m_hStmt, 5, SQL_C_LONG, &hunger, 4, &b_hunger);
	//	ret = SQLBindCol(m_hStmt, 6, SQL_C_LONG, &thirst, 4, &b_thirst);
	//	ret = SQLBindCol(m_hStmt, 7, SQL_C_FLOAT, &posx, 8, &b_posx);
	//	ret = SQLBindCol(m_hStmt, 8, SQL_C_FLOAT, &posy, 8, &b_posy);
	//	ret = SQLBindCol(m_hStmt, 9, SQL_C_FLOAT, &posz, 8, &b_posz);
	//	ret = SQLBindCol(m_hStmt, 10, SQL_C_FLOAT, &dirx, 8, &b_dirx);
	//	ret = SQLBindCol(m_hStmt, 11, SQL_C_FLOAT, &diry, 8, &b_diry);
	//	ret = SQLBindCol(m_hStmt, 12, SQL_C_FLOAT, &dirz, 8, &b_dirz);
	//	ret = SQLFetch(m_hStmt);
	//	if (ret == SQL_ERROR)
	//		printf("SQLFetch In UserLoginData error\n");
	//	if (ret == SQL_NO_DATA_FOUND || ret == SQL_NO_DATA) {
	//		DB_Event db_event;
	//		db_event.state = DBS_LOGIN_FAIL;
	//		db_event.client_num = event.client_num;
	//		InsertToStateQueue(db_event);
	//		PostQueuedCompletionStatus(CNetwork::GetInst()->GetIocp(), 10, event.client_num, &lpover_ex->m_Overlapped);
	//		SQLCancel(m_hStmt);
	//		return;
	//	}
	//	if (ret == SQL_SUCCESS || SQL_SUCCESS_WITH_INFO) {
	//		//SetIsLogin()
	//		SQLCancel(m_hStmt);
	//	}
	//}
	//else {
	//	SQLCancel(m_hStmt);
	//	HandleDiagnosticRecord(m_hStmt, SQL_HANDLE_STMT, ret);
	//	return;
	//}
}

void CDataBase::LogOutProcess(DB_Event & event)
{
}

void CDataBase::GetItemProcess(DB_Event & event)
{
}

void CDataBase::RemoveItemProcess(DB_Event & event)
{
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
	wstring execFunc = L"EXEC select_id " + login_id;

	SQLRETURN ret;
	ret = SQLExecDirect(m_hStmt, (SQLWCHAR*)execFunc.c_str(), SQL_NTS);
	ret = SQLBindCol(m_hStmt, 1, SQL_C_WCHAR, &m_dUserId, MAX_STR_LEN, &cbID);

	bool isExist = false;

	for (int i = 0;; ++i)
	{
		ret = SQLFetch(m_hStmt);
		if (ret == SQL_ERROR || ret == SQL_SUCCESS_WITH_INFO)
			HandleDiagnosticRecord(m_hStmt, SQL_HANDLE_STMT, ret);
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO);
		else
			break;
		isExist = true;
	}

	SQLCancel(m_hStmt);
	return isExist;
}

void CDataBase::AddUserInfo(DB_Event & _ev)
{
	wstring execFunc = L"EXEC insert_info";
	wstring var = to_wstring(_ev.inum) + L", " + _ev.sid + L", " + to_wstring(_ev.fHealth) + L", " +
		L", " + to_wstring(_ev.fHungry) + L", " + to_wstring(_ev.fThirst) + L", " +
		to_wstring(_ev.fX) + L", " + to_wstring(_ev.fY) + L", " + to_wstring(_ev.fZ);
	execFunc += var;

	SQLRETURN ret;
	ret = SQLExecDirect(m_hStmt, (SQLWCHAR*)execFunc.c_str(), SQL_NTS);
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
	{
		ret = SQLBindCol(m_hStmt, 2, SQL_C_WCHAR, &m_dUserId, 100, &cbID);
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
			ret = SQLFetch(m_hStmt);
		else
			HandleDiagnosticRecord(m_hStmt, SQL_HANDLE_STMT, ret);
	}
	else
		HandleDiagnosticRecord(m_hStmt, SQL_HANDLE_STMT, ret);

	SQLCancel(m_hStmt);
}

void CDataBase::UpdateUserInfo(DB_Event & _ev)
{
	wstring execFunc = L"EXEC update_info ";
	wstring var = to_wstring(_ev.inum) + L", " + _ev.sid + L", " + to_wstring(_ev.fHealth) + L", " +
		L", " + to_wstring(_ev.fHungry) + L", " + to_wstring(_ev.fThirst) + L", " +
		to_wstring(_ev.fX) + L", " + to_wstring(_ev.fY) + L", " + to_wstring(_ev.fZ);
	execFunc += var;

	SQLRETURN ret;
	ret = SQLExecDirect(m_hStmt, (SQLWCHAR*)execFunc.c_str(), SQL_NTS);
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
		ret = SQLFetch(m_hStmt);
	else
		HandleDiagnosticRecord(m_hStmt, SQL_HANDLE_STMT, ret);

	SQLCancel(m_hStmt);
}

DB_Event & CDataBase::GetUserInfo(wstring & login_id)
{
	wstring execFunc = L"EXEC select_info " + login_id;

	SQLRETURN ret = SQLExecDirect(m_hStmt, (SQLWCHAR*)execFunc.c_str(), SQL_NTS);

	ret = SQLBindCol(m_hStmt, 1, SQL_C_LONG, &m_dUserNum, 100, &cbNum);
	ret = SQLBindCol(m_hStmt, 2, SQL_C_WCHAR, &m_dUserId, MAX_STR_LEN + 1, &cbID);
	ret = SQLBindCol(m_hStmt, 3, SQL_C_SHORT, &m_dUserHp, 100, &cbHp);
	ret = SQLBindCol(m_hStmt, 5, SQL_C_SHORT, &m_dUserHungry, 100, &cbHungry);
	ret = SQLBindCol(m_hStmt, 6, SQL_C_SHORT, &m_dUserThirst, 100, &cbThirst);
	ret = SQLBindCol(m_hStmt, 7, SQL_C_DOUBLE, &m_dUserX, 100, &cbX);
	ret = SQLBindCol(m_hStmt, 8, SQL_C_DOUBLE, &m_dUserY, 100, &cbY);
	ret = SQLBindCol(m_hStmt, 9, SQL_C_DOUBLE, &m_dUserZ, 100, &cbZ);

	DB_Event ev{};
	ret = SQLFetch(m_hStmt);
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
		cout << "Error" << endl;
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
		ev.inum = m_dUserNum;
		ev.sid = m_dUserId;
		ev.fHealth = m_dUserHp;
		ev.fHungry = m_dUserHungry;
		ev.fThirst = m_dUserThirst;
		ev.fX = m_dUserX;
		ev.fY = m_dUserY;
		ev.fZ = m_dUserZ;
	}
	SQLCancel(m_hStmt);
	return ev;
}
