#pragma once
#include "Object.h"

class CPlayer : public CObject
{
public:
	explicit CPlayer();
	virtual ~CPlayer();

private:
	SOCKET			m_sSocket;
	OVER_EX			m_over;
	bool			m_bConnect;
	int				m_iPrevsize;
	int				m_iCursize;

	int				m_dbNum;
	USHORT			m_usID;
	char			m_ID[MAX_STR_LEN];

	tPlayerStatus	m_tPlayerStatus;
	float			m_fArmor;

private:
	concurrent_unordered_set<USHORT> m_cusViewList;
	shared_mutex m_smPlayerStatusMutex;
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
	void SetArmor(float& fArmor);

	void SetIncreaseHealth(float& fAmount);
	void SetIncreaseHungry(float& fAmount);
	void SetIncreaseThirst(float& fAmount);

	void SetNumID(USHORT& numID);
	void SetWcID(char* wcID);
	void SetDbNum(int& dbID);
	void SetConnect(bool bConnect);
	void SetSocket(SOCKET& socket);

public:
	tPlayerStatus& GetPlayerStatus();
	float&	GetHealth();
	float&	GetHungry();
	float&	GetThirst();
	float&	GetSpeed();
	float&	GetArmor();

	const USHORT&	GetNumID();
	const char*	GetWcID();
	const int&		GetDbNum();
	const bool		GetConnect();
	const SOCKET&	GetSocket();

public:
	void CopyViewList(concurrent_unordered_set<USHORT>& cusCopyViewList){
		lock_guard<recursive_mutex>lock(m_rmPlayerListMutex);
		cusCopyViewList = m_cusViewList;
	}
	void CopyUserViewList(concurrent_unordered_set<USHORT>& cusCopyUserViewList) {
		unique_lock<recursive_mutex>lock(m_rmPlayerListMutex);
		cusCopyUserViewList = m_cusViewList;
		lock.unlock();

		for (auto au = cusCopyUserViewList.begin(); au != cusCopyUserViewList.end();)
		{
			if (*au >= MAX_USER)
				au = cusCopyUserViewList.unsafe_erase(au);
			else
				++au;
		}
	}
	void CopyAnimalViewList(concurrent_unordered_set<USHORT>& cusCopyAnimalViewList) {
		unique_lock<recursive_mutex> lock(m_rmPlayerListMutex);
		cusCopyAnimalViewList = m_cusViewList;
		lock.unlock();

		for (auto au = cusCopyAnimalViewList.begin(); au != cusCopyAnimalViewList.end();) {
			if (*au < MAX_USER)
				au = cusCopyAnimalViewList.unsafe_erase(au);
			else
				++au;
		}
	}
	void InsertList(USHORT usId)
	{
		if (m_usID == usId)
			return;
		m_cusViewList.insert(usId);
	}
	bool ExistList(USHORT usId)
	{
		if (m_cusViewList.count(usId))
			return true;
		return false;
	}
	void DeleteList(USHORT usId)
	{
		concurrency::concurrent_unordered_set<USHORT> list;
		CopyViewList(list);

		for (auto au = list.begin(); au != list.end();)
		{
			if (*au == usId)
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
	void ClearList()
	{
		lock_guard<recursive_mutex>lock(m_rmPlayerListMutex);
		m_cusViewList.clear();
	}
};

