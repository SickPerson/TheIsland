#pragma once
#include "stdafx.h"

class CDataBase
{
	SINGLE(CDataBase)

private:
	SQLHENV		m_hEnv;
	SQLHDBC		m_hDbc;
	SQLHSTMT	m_hStmt;

private:
	SQLINTEGER	m_dUserNum;
	SQLWCHAR	m_dUserId[MAX_STR_LEN];
	SQLSMALLINT	m_dUserHp, m_dUserStamina, m_dUserHungry, m_dUserThirst;
	SQLFLOAT	m_dUserX, m_dUserY, m_dUserZ;
	SQLLEN	cbNum = 0, cbID = 0, cbHp = 0, cbStamina = 0, cbHungry = 0, cbThirst = 0;
	SQLLEN cbX = 0, cbY = 0, cbZ = 0;

private:
	concurrent_queue<DB_Event>m_DatabaseEventQueue;
	concurrent_queue<DB_Event>m_SaveStateQueue;

	std::function<void(DB_Event&)>m_fpDataBaseProcess[DB_UPDATE_TYPE::DUT_END];
	vector<User_Data> m_vUserData;

public:
	void Init();

public:
	void HandleDiagnosticRecord(SQLHANDLE _hHandle, SQLSMALLINT _hType, RETCODE RetCode);
	bool ConnectDataBase();
	void DisConnectDataBase();
	void RunDataBase(DB_Event& event);
public:
	void LogInProcess(DB_Event& event);
	void LogOutProcess(DB_Event& event);
	void GetItemProcess(DB_Event& event);
	void RemoveItemProcess(DB_Event& event);
	void ChnageStateProcess(DB_Event& event);
	void ChangeInvenProcess(DB_Event& event);

public:
	void BindDataBaseFP()
	{
		m_fpDataBaseProcess[DB_UPDATE_TYPE::DUT_LOGIN] = [&](DB_Event& event) {LogInProcess(event); };
		m_fpDataBaseProcess[DB_UPDATE_TYPE::DUT_LOGOUT] = [&](DB_Event& event) {LogOutProcess(event); };
		//m_fpDataBaseProcess[DB_UPDATE_TYPE::DUT_USER_SAVE] = [&](DB_Event& dbevent) {UserSaveProcess(dbevent); };
		m_fpDataBaseProcess[DB_UPDATE_TYPE::DUT_GET_ITEM] = [&](DB_Event& event) {GetItemProcess(event); };
		m_fpDataBaseProcess[DB_UPDATE_TYPE::DUT_REMOVE_ITEM] = [&](DB_Event& event) {RemoveItemProcess(event); };
		//m_fpDataBaseProcess[DB_UPDATE_TYPE::CHANGE_STATE] = [&](DB_Event& event) {ChnageStateProcess(event); };
		//m_fpDataBaseProcess[DB_UPDATE_TYPE::CHANGE_INVEN] = [&](DB_Event& event) {ChangeInvenProcess(event); };
	}
	bool EmptyDatabaseEventQueue() { return m_DatabaseEventQueue.empty(); }
	void PushDatabaseEventQueue(DB_Event& ev) { m_DatabaseEventQueue.push(ev); }
	bool PopDatabaseEventQueue(DB_Event& ev) { return m_DatabaseEventQueue.try_pop(ev); }

public:
	void PushEvent_Login();
	void PushEvent_LogOut();
	void PushEvent_DB_SaveProcess();
	void PushEvent_Get_Item();
	void PushEvent_Remove_Item();
	void PushEvent_Change_State(USHORT& player_Id);
	void PushEvent_Change_Inven();

public:
	bool	IsIDExist(wstring login_id);
	void	AddUserInfo(DB_Event& _ev);
	void	UpdateUserInfo(DB_Event& _ev);
	DB_Event& GetUserInfo(wstring& login_id);
};

