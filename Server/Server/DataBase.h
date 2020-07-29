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

	concurrency::concurrent_queue<DB_Event>m_ccqDBQueue;

	std::function<void(DB_Event&)>m_fpDataBaseProcess[DB_END];
	//std::vector<User_Data> m_UserData;

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
public:
	void BindDataBaseFP()
	{
		m_fpDataBaseProcess[DB_LOGIN] = [&](DB_Event& event) {LogInProcess(event); };
		m_fpDataBaseProcess[DB_LOGOUT] = [&](DB_Event& event) {LogOutProcess(event); };
		m_fpDataBaseProcess[DB_GET_ITEM] = [&](DB_Event& event) {GetItemProcess(event); };
		m_fpDataBaseProcess[DB_REMOVE_ITEM] = [&](DB_Event& event) {RemoveItemProcess(event); };
	}
	bool IsEmptyDBQueue() {
		return m_ccqDBQueue.empty();
	}
	void PushDBQueue(DB_Event& ev) {
		m_ccqDBQueue.push(ev);
	}
	bool TryPopDBQueue(DB_Event& ev) {
		return m_ccqDBQueue.try_pop(ev);
	}

public:
<<<<<<< HEAD
	bool	IsIDExist(wstring login_id);
	void	AddUserInfo(DB_Event& _ev);
	void	UpdateUserInfo(DB_Event& _ev);
	DB_Event& GetUserInfo(wstring& login_id);
=======
	void BindDataBaseFP();
	bool DataBaseQueueIsEmpty();
	void InsertToDataBaseQueue(DataBase_Event& event);
	bool PopFromDataBaseQueue(DataBase_Event& event);
	void InsertToStateQueue(DataBase_Event& event);
	bool PopFromStateQueue(DataBase_Event& event);

	bool	GetIsLogin(std::string sID);
	bool	GetIsLogin(unsigned int usID);
	void	SetIsLogin(int iPlayerNum, unsigned int usID, std::string ID);
	int		FindIsLogin(unsigned int usID, wchar_t* user_id, bool bDelete = false);
>>>>>>> master
};

