#pragma once
#include "Object.h"

enum class PLAYER_LOCK_TYPE
{
	STATUS,
	HP,
	HUNGRY,
	STAMINA,
	THIRST,
	SPEED,
	SOCKET,
	CONNECT,
	NUMID,
	WCID,
	END
};

class CPlayer : public CObject
{
public:
	CPlayer();
	virtual ~CPlayer();

private:
	unsigned short	m_uiID;
	wchar_t			m_wcID[MAX_STR_LEN];
	tPlayerStatus	m_tPlayerStatus;
	volatile bool	m_bConnect;
	SOCKET			m_sSocket;
	OVER_EX			m_over;
	int				m_iPrevsize;
	int				m_iCursize;

private:
	concurrent_unordered_set<unsigned short> m_cusViewList;
	shared_mutex m_smPlayerStatusMutex[(UINT)PLAYER_LOCK_TYPE::END];
	recursive_mutex	m_rmPlayerListMutex;

public:
	void SetRecvState();
	char* RecvEvent(DWORD dataSize, char * packet);

public:
	void SetPlayerStatus(tPlayerStatus& status);
	void SetHP(float& fHP);
	void SetHungry(float& fHungry);
	void SetStamina(float& fStamina);
	void SetThirst(float& fThirst);
	void SetSpeed(float& fSpeed);

	void SetNumID(unsigned short& numID);
	void SetWcID(wchar_t* wcID);
	void SetConnect(bool bConnect);
	void SetSocket(const SOCKET& socket);

public:
	tPlayerStatus& GetPlayerStatus();
	float&	GetHP();
	float&	GetHungry();
	float&	GetStamina();
	float&	GetThirst();
	float&	GetSpeed();

	unsigned short&	GetNumID();
	wchar_t*	GetWcID();
	bool	GetConnect();
	SOCKET&	GetSocket();

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
	bool ExistList(unsigned short _usID)
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

