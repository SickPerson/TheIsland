#pragma once
#include "Object.h"

//float fHP;
//float fHungry;
//float fStamina;
//float fThirst;
enum PLAYER_LOCK_TYPE {
	PLAYER_LOCK_STATUS,
	PLAYER_LOCK_HP,
	PLAYER_LOCK_HUNGRY,
	PLAYER_LOCK_STAMINA,
	PLAYER_LOCK_THIRST,
	PLAYER_LOCK_SOCKET,
	PLAYER_LOCK_CONNECT,
	PLAYER_LOCK_NUMID,
	PLAYER_LOCK_WCID,
	PLAYER_LOCK_END
};

class CPlayer : public CObject
{
public:
	CPlayer();
	virtual ~CPlayer();

private:
	unsigned int	m_uiID;
	wchar_t			m_wcID[MAX_STR_LEN];
	tPlayerStatus	m_tPlayerStatus;
	volatile bool	m_bConnect;
	SOCKET			m_sSocket;
	OVER_EX			m_over;
	int				m_iPrevsize;
	int				m_iCursize;

private:
	concurrent_unordered_set<unsigned int> m_cusViewList;
	shared_mutex m_smPlayerStatusMutex[PLAYER_LOCK_END];
	recursive_mutex	m_rmPlayerListMutex;

public:
	void SetRecvState();
	char* RecvEvent(DWORD dataSize, char * packet);

public:
	void SetPlayerStatus( tPlayerStatus& status);
	void SetHP( float& fHP);
	void SetHungry( float& fHungry);
	void SetStamina( float& fStamina);
	void SetThirst( float& fThirst);
	void SetNumID( unsigned int& numID);
	void SetWcID(wchar_t* wcID);
	void SetConnect(bool bConnect);
	void SetSocket(const SOCKET& socket);

public:
	tPlayerStatus& GetPlayerStatus();
	float&	GetHP();
	float&	GetHungry();
	float&	GetStamina();
	float&	GetThirst();
	unsigned int&	GetNumID();
	wchar_t*	GetWcID();
	bool	GetConnect();
	SOCKET&	GetSocket();

public:
	void CopyBefore(concurrency::concurrent_unordered_set<unsigned int>& _usCopyList)
	{
		lock_guard<recursive_mutex>lock(m_rmPlayerListMutex);
		_usCopyList = m_cusViewList;
	}
	void CopyPlayerList(concurrency::concurrent_unordered_set<unsigned int>& _usCopyList) {
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
	void DeleteList(unsigned int _usID)
	{
		concurrency::concurrent_unordered_set<unsigned int> list;
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
	void InsertList(unsigned int _usID)
	{
		lock_guard<recursive_mutex>lock(m_rmPlayerListMutex);
		m_cusViewList.insert(_usID);
	}
	bool ExistList(unsigned int _usID)
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

