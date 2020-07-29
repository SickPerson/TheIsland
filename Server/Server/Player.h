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
<<<<<<< HEAD
	unsigned int	m_uiID;
	wchar_t			m_wcID[MAX_STR_LEN];
	tPlayerStatus	m_tPlayerStatus;
=======
	unsigned int	m_usID;
>>>>>>> master
	volatile bool	m_bConnect;
	SOCKET			m_sSocket;
	OVER_EX			m_over;
	int				m_iPrevsize;
	int				m_iCursize;

<<<<<<< HEAD
private:
	concurrent_unordered_set<unsigned int> m_cusViewList;
	shared_mutex m_smPlayerStatusMutex[PLAYER_LOCK_END];
=======
	concurrent_unordered_set<unsigned int> m_cusViewList;
	shared_mutex m_rmPlayerStatusMutex[LOCK_END];
>>>>>>> master
	recursive_mutex	m_rmPlayerListMutex;

public:
	void SetRecvState();
	char* RecvEvent(DWORD dataSize, char * packet);

public:
<<<<<<< HEAD
	void SetPlayerStatus(const tPlayerStatus& status);
	void SetHP(const float& fHP);
	void SetHungry(const float& fHungry);
	void SetStamina(const float& fStamina);
	void SetThirst(const float& fThirst);
	void SetNumID(const unsigned int& numID);
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
=======
	void SetPlayerSocket(const SOCKET& _sSocket) {
		unique_lock<shared_mutex>lock(m_rmPlayerStatusMutex[LOCK_SOCKET]); // Write Lock(1개의 스레드만 접근할 수 있는 락)
		m_sSocket = _sSocket;
	}
	void SetPlayerConnect(bool _bState) {
		unique_lock<shared_mutex>lock(m_rmPlayerStatusMutex[LOCK_CONNECT]); // Write Lock(1개의 스레드만 접근할 수 있는 락)
		m_bConnect = _bState;
	}
	void SetPlayerID(const unsigned int& _usID) {
		unique_lock<shared_mutex>lock(m_rmPlayerStatusMutex[LOCK_ID]); // Write Lock(1개의 스레드만 접근할 수 있는 락)
		m_usID = _usID;
	}

	const SOCKET GetPlayerSocket() {
		shared_lock<shared_mutex>lock(m_rmPlayerStatusMutex[LOCK_SOCKET]);
		return m_sSocket;
	}
	const bool GetPlayerConnect() {
		shared_lock<shared_mutex>lock(m_rmPlayerStatusMutex[LOCK_CONNECT]);
		return m_bConnect;
	}
	const unsigned int GetPlayerID() {
		shared_lock<shared_mutex>lock(m_rmPlayerStatusMutex[LOCK_ID]);
		return m_usID;
	}
>>>>>>> master

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
<<<<<<< HEAD
	bool ExistList(unsigned int _usID)
=======
	bool CheckList(unsigned int _usID)
>>>>>>> master
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

