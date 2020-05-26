#pragma once
#include "header.h"

#include <Engine/SceneScript.h>

class CGameObject;
class CPlayer;

class CNetwork
{
	SINGLE(CNetwork)

private:
	SOCKET	m_sock;

	WSAEVENT	m_hEvent;
	WSANETWORKEVENTS m_weEvent;

	WSABUF	m_SendWsaBuf;
	WSABUF	m_RecvWsaBuf;

	char	m_cSendBuf[BUF_SIZE];
	char	m_cRecvbuf[BUF_SIZE];

	DWORD	m_in_packet_size;
	DWORD	m_saved_packet_size;
	char	m_cPacketBuf[255];

	int		m_iIndex;

	recursive_mutex m_rmListMutex;
	recursive_mutex m_rmLoginStateLock[LT_END];

	shared_ptr<thread>m_tNetworkThread;

	volatile bool m_bLoginState[LT_END];
	bool m_bPushKey;
	bool m_bClientClose;
	bool m_bCollision;

	CScene* pScene;

public:
	static unsigned short m_usID;
	static concurrent_unordered_map<unsigned int, CGameObject*> m_cumPlayer;
	static concurrent_unordered_set<unsigned int> m_cusViewList;

public:
	static void Err_quit(const char* msg, int err_no);
	static void Err_display(const char* msg, int err_no);
public:
	void Init();
	bool ConnectServer(string ipAddr);
	void RunRecvThread();
	bool CreateEventSelect();

	void DisConnect();
	void RecvPacket();
	void ProcessPacket(char* _packet);

public:
	void SetID(unsigned short _id);
	unsigned short GetID();

public:
	void SetLogin(bool _bLogin)
	{
		lock_guard<recursive_mutex> lock(m_rmLoginStateLock[LT_LOGIN_SUCCESS]);
		m_bLoginState[LT_LOGIN_SUCCESS] = _bLogin;
	}
	void SetLoopStart(bool _bStart)
	{
		lock_guard<recursive_mutex> lock(m_rmLoginStateLock[LT_LOOP_START]);
		m_bLoginState[LT_LOOP_START] = _bStart;
	}
	void SetLoginSend(bool _bSend)
	{
		lock_guard<recursive_mutex> lock(m_rmLoginStateLock[LT_LOGIN_PACKET_SEND]);
		m_bLoginState[LT_LOGIN_PACKET_SEND] = _bSend;
	}
	void SetPushKey(bool _bPush) { m_bPushKey = _bPush; }
	void SetClientClose(bool _bClientClose) { m_bClientClose = _bClientClose; }
	void SetCollision(bool _bCollision) { m_bCollision = _bCollision; }

public:
	bool GetLogin() { lock_guard<recursive_mutex>lock(m_rmLoginStateLock[LT_LOGIN_SUCCESS]); return m_bLoginState[LT_LOGIN_SUCCESS]; }
	bool GetLoopStart() { lock_guard<recursive_mutex>lock(m_rmLoginStateLock[LT_LOOP_START]); return m_bLoginState[LT_LOOP_START]; }
	bool GetLoginSend() { lock_guard<recursive_mutex>lock(m_rmLoginStateLock[LT_LOGIN_PACKET_SEND]); return m_bLoginState[LT_LOGIN_PACKET_SEND]; }
	bool GetPushKey() { return m_bPushKey; }
	bool GetClientClose() { return m_bClientClose; }
	bool GetCollision() { return m_bCollision; }
public:
	void CopyBefore(concurrent_unordered_set<unsigned int>& _usCopyList)
	{
		lock_guard<recursive_mutex>	lock(m_rmListMutex);
		_usCopyList = m_cusViewList;

	}
	void InsertList(unsigned int _usID)
	{
		m_cusViewList.insert(_usID);
	}
	void DeleteList(unsigned int _usID)
	{
		concurrent_unordered_set<unsigned int> List;
		CopyBefore(List);
		for (auto iter = List.begin(); iter != List.end();)
		{
			if (*iter == _usID)
			{
				iter = List.unsafe_erase(iter);
				break;
			}
			else
				++iter;
		}
		lock_guard<recursive_mutex> lock(m_rmListMutex);
		m_cusViewList = List;
	}
	void ClearList()
	{
		lock_guard<recursive_mutex> lock(m_rmListMutex);
		m_cusViewList.clear();
	}
	bool CheckList(unsigned int _usID)
	{
		if (m_cusViewList.count(_usID) != 0)
			return true;
		else
			return false;
	}

public:
	void SendLoginPacket(string _sPlayerID);
	void SendPosPacket();
	void SendDirPacket();
	void SendChatPacket(string _message);

public:
	void RecvLoginPacket(char* _packet);
	void RecvConnectPacket(char* _packet);
	void RecvPosPacket(char* _packet);
	void RecvChatPacket(char* _packet);
};