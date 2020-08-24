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

private:
	function<void(char*)> m_fpPacketProcess[SC_END];
	bool m_bClientClose;

private:
	void BindfpPacket();
public:
	CScene* pScene;
	static unsigned int m_usID;
	CGameObject*	m_pPlayer;
	CGameObject*	m_pChat;

	static concurrent_unordered_map<unsigned int, CGameObject*> m_cumPlayer;
	static concurrent_unordered_map<unsigned int, CGameObject*> m_cumAnimal;

public:
	static void Err_quit(const char* msg, int err_no);
	static void Err_display(const char* msg, int err_no);
public:
	bool ConnectServer(string ipAddr);
	void RunRecvThread();
	bool CreateEventSelect();

	void DisConnect();
	void RecvPacket();
	void ProcessPacket(char* _packet);

public:
	void Send_Login_Packet(wstring playerID);
	void Send_Move_Packet();
	//void Send_Move_Packet(Vec3 vWorldDir, bool bRun);
	//void Send_Rot_Packet();
	//void Send_Rot_Packet();
	void Send_Chat_Packet(string message);
	void Send_Collision_Player_Packet(UINT Collision_type, USHORT Collision_Id, bool bRun);
	void Send_Attack_Player_Packet(UINT attack_type, USHORT attack_Id);
	void Send_Animation_Player_Packet(UINT eAnimationType);
	/*void Send_Collision_Natural_Packet(unsigned short naturalId, bool bRun);
	void Send_Collision_House_Packet(USHORT houseId, bool bRun);
	void Send_Collision_Animal_Packet(USHORT animalId, bool bRun);*/
public: // Natural

public: // Housing
	void Send_Install_Housing_Packet(UINT uiType, Vec3 vLocalPos, Vec3 vLocalRot, Vec3 vLocalScale, Vec3 vOffsetPos, Vec3 vOffsetScale);
	void Send_Remove_Housing_Packet(USHORT house_id);

public: // Item
	void Send_Get_Item_Packet(UINT uiItemType, UINT uiInvenNum);
	void Send_Remove_Item_Packet(UINT uiItemType, UINT uiInvenNum);

	//void Send_Install_Housing_Packet(USHORT )
public:
	// - Login
	void Recv_Login_OK_Packet(char* packet);
	void Recv_Login_Fail_Packet(char* packet);
	void Recv_Disconnect_Server_Packet(char* packet);

	// - Player
	void Recv_Status_Player_Packet(char* packet);
	void Recv_Put_Player_Packet(char* packet);
	void Recv_Remove_Player_Packet(char* packet);
	void Recv_Pos_Player_Packet(char* packet);
	void Recv_Chat_Packet(char* packet);
	void Recv_Animation_Player_Packet(char* packet);

public:
	void Recv_WakeUp_Npc_Packet(char* packet);
	void Recv_Put_Animal_Packet(char* packet);
	void Recv_Remove_Animal_Packet(char* packet);
	void Recv_Pos_Animal_Packet(char* packet);
	void Recv_Animation_Animal_Packet(char* packet);

public: // Natural
	void Recv_Put_Natural_Packet(char* packet);
	void Recv_Destroy_Natural_Packet(char* packet);
	
public: // Housing
	void Recv_Install_Housing_Packet(char* packet);
	void Recv_Remove_Housing_Packet(char* packet);
public:
	void Recv_Add_Item_Packet(char* packet);

public: //etc
	void Recv_Weather_Packet(char* packet);
	void Recv_Time_Packet(char* packet);

public:
	void SetChatObj(CGameObject* pObj) { m_pChat = pObj; }
	void SetPlayerObj(CGameObject* pObj) { m_pPlayer = pObj; }
	void SetOtherPlayerObj(concurrent_unordered_map<unsigned int, CGameObject*> pObjs) { m_cumPlayer = pObjs; }
	void SetAnimalObj(concurrent_unordered_map<unsigned int, CGameObject*> pObjs) { m_cumPlayer = pObjs; }
};