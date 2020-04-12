#pragma once
#include "Object.h"

enum LOCK_TYPE {
	LOCK_SOCKET,
	LOCK_CONNECT,
	LOCK_ID,
	LOCK_END
};

class CPlayer : public CObject
{
public:
	CPlayer();
	virtual ~CPlayer();

private:
	unsigned short	m_usID;
	volatile bool	m_bConnect;
	SOCKET			m_sSocket;
	OVER_EX			m_over;
	int				m_iPrevsize;
	int				m_iCursize;

	concurrent_unordered_set<unsigned short> m_cusViewList;
	shared_mutex m_rmPlayerStatusMutex[LOCK_END];
	recursive_mutex	m_rmPlayerListMutex;

public:
	void Init();
public:
	void SetRecvState();
	char* RecvEvent(DWORD data_size, char * _packet);
public:
	void SetPlayerSocket(const SOCKET& _sSocket) {
		unique_lock<shared_mutex>lock(m_rmPlayerStatusMutex[LOCK_SOCKET]); // Write Lock(1개의 스레드만 접근할 수 있는 락)
		m_sSocket = _sSocket;
	}
	void SetPlayerConnect(bool _bState) {
		unique_lock<shared_mutex>lock(m_rmPlayerStatusMutex[LOCK_CONNECT]); // Write Lock(1개의 스레드만 접근할 수 있는 락)
		m_bConnect = _bState;
	}
	void SetPlayerID(const unsigned short& _usID) {
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
	const unsigned short GetPlayerID() {
		shared_lock<shared_mutex>lock(m_rmPlayerStatusMutex[LOCK_ID]);
		return m_usID;
	}

public:
	void CopyBefore(concurrency::concurrent_unordered_set<unsigned short>& _usCopyList)
	{
		lock_guard<recursive_mutex>lock(m_rmPlayerListMutex);
		_usCopyList = m_cusViewList;
	}
	void CopyPlayerList(concurrency::concurrent_unordered_set<unsigned short>& _usCopyList) {
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
	void DeleteList(unsigned short _usID)
	{
		concurrency::concurrent_unordered_set<unsigned short> list;
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
	void InsertList(unsigned short _usID)
	{
		lock_guard<recursive_mutex>lock(m_rmPlayerListMutex);
		m_cusViewList.insert(_usID);
	}
	bool CheckList(unsigned short _usID)
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

