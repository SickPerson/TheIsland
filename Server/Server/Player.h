#pragma once
#include "Object.h"

enum class PLAYER_LOCK_TYPE
{
	STATUS, HEALTH, HUNGRY, THIRST, SPEED, DAMAGE,
	SOCKET, CONNECT, NUMID, WCID, DBID, END
};

class CPlayer : public CObject
{
public:
	CPlayer();
	virtual ~CPlayer();

private:
	USHORT			m_usID;
	wchar_t			m_wcID[MAX_STR_LEN];
	int				m_db_ID;
	tPlayerStatus	m_tPlayerStatus;
	volatile bool	m_bConnect;
	SOCKET			m_sSocket;
	OVER_EX			m_over;
	int				m_iPrevsize;
	int				m_iCursize;

private:
	concurrent_unordered_set<USHORT> m_cusViewList;
	shared_mutex m_smPlayerStatusMutex[(UINT)PLAYER_LOCK_TYPE::END];
	recursive_mutex	m_rmPlayerListMutex;

public:
	void SetRecvState();
	char* RecvEvent(DWORD dataSize, char * packet);

public:
	void SetPlayerStatus(tPlayerStatus& status);
	void SetHealth(float& fHealth);
	void SetHungry(float& fHungry);
	void SetThirst(float& fThirst);
	void SetSpeed(float& fSpeed);
	void SetDamage(float& fDamage);

	void SetNumID(USHORT& numID);
	void SetWcID(wchar_t* wcID);
	void SetDbID(int& dbID);
	void SetConnect(bool bConnect);
	void SetSocket(const SOCKET& socket);

public:
	tPlayerStatus& GetPlayerStatus();
	float&	GetHealth();
	float&	GetHungry();
	float&	GetThirst();
	float&	GetSpeed();
	float&	GetDamage();

	USHORT&	GetNumID();
	wchar_t*	GetWcID();
	int		GetDbID();
	bool	GetConnect();
	SOCKET&	GetSocket();

public:
	void CopyBefore(concurrency::concurrent_unordered_set<USHORT>& _usCopyList)
	{
		lock_guard<recursive_mutex>lock(m_rmPlayerListMutex);
		_usCopyList = m_cusViewList;
	}
	void CopyPlayerList(concurrency::concurrent_unordered_set<USHORT>& _usCopyList) {
		unique_lock<recursive_mutex>lock(m_rmPlayerListMutex);
		_usCopyList = m_cusViewList;
		lock.unlock();

		for (auto au = _usCopyList.begin(); au != _usCopyList.end();)
		{
			if (*au >= MAX_USER)
				au = _usCopyList.unsafe_erase(au);
			else
				++au;
		}
	}
	void DeleteList(USHORT _usID)
	{
		concurrency::concurrent_unordered_set<USHORT> list;
		CopyBefore(list);
		for (auto au = list.begin(); au != list.end();)
		{
			if (*au == _usID)
			{
				au = list.unsafe_erase(au);
				break;
			}
			else
				++au;
		}
		lock_guard<recursive_mutex>lock(m_rmPlayerListMutex);
		m_cusViewList = list;
	}
	void InsertList(USHORT _usID)
	{
		lock_guard<recursive_mutex>lock(m_rmPlayerListMutex);
		m_cusViewList.insert(_usID);
	}
	bool ExistList(USHORT _usID)
	{
		if (m_cusViewList.count(_usID) != 0)
			return true;
		else
			return false;
	}
	void ClearList()
	{
		lock_guard<recursive_mutex>lock(m_rmPlayerListMutex);
		m_cusViewList.clear();
	}
};

