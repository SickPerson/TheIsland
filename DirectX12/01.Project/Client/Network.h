#pragma once
#include "header.h"

class CGameObject;
class CScene;

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

public:
	CScene* pScene;
	static unsigned int m_usID;
	CGameObject* m_pPlayer;
	CGameObject*	m_pChat;

	static concurrent_unordered_map<unsigned int, CGameObject*> m_cumPlayer;
	static concurrent_unordered_map<unsigned int, CGameObject*> m_cumAnimal;

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
	void Send_Login_Packet(wstring playerID);
	void Send_Move_Packet();
	//void Send_Move_Packet(Vec3 vWorldDir, bool bRun);
	//void Send_Rot_Packet();
	//void Send_Rot_Packet();
	void Send_Chat_Packet(string message);
	void Send_Collision_Animal_Packet(unsigned short animalId, bool bRun);
	void Send_Install_Housing_Packet(UINT uiType, Vec3 vLocalPos, Vec3 vLocalRot, Vec3 vLocalScale);

	//void Send_Install_Housing_Packet(USHORT )
public:
	void Recv_Login_OK_Packet(char* packet);
	void Recv_Login_Fail_Packet(char* packet);
	void Recv_Status_Player_Packet(char* packet);
	void Recv_Put_Player_Packet(char* packet);
	void Recv_Remove_Player_Packet(char* packet);
	void Recv_Pos_Player_Packet(char* packet);
	void Recv_Chat_Packet(char* packet);
	void Recv_Animation_Player_Packet(char* packet);

public:
	void Recv_WakeUp_Npc_Packet(char* packet);
	void Recv_Put_Npc_Packet(char* packet);
	void Recv_Remove_Npc_Packet(char* packet);
	void Recv_Pos_Npc_Packet(char* packet);
	void Recv_Animation_Npc_Packet(char* packet);

public:
	
public: // Housing
	void Recv_Install_Housing_Packet(char* packet);

public: //etc
	void Recv_Weather_Packet(char* packet);

public:
	void SetChatObj(CGameObject* pObj) { m_pChat = pObj; }
	void SetPlayerObj(CGameObject* pObj) { m_pPlayer = pObj; }
	void SetOtherPlayerObj(concurrent_unordered_map<unsigned int, CGameObject*> pObjs) { m_cumPlayer = pObjs; }
	void SetAnimalObj(concurrent_unordered_map<unsigned int, CGameObject*> pObjs) { m_cumPlayer = pObjs; }
};