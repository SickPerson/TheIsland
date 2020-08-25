#pragma once
#include "header.h"

class CGameObject;
class CScene;

class CNetwork
{
	SINGLE(CNetwork)

private:
	SOCKET	m_sock;
	WSABUF	m_RecvWsaBuf;
	char	m_cRecvbuf[BUF_SIZE];

	DWORD	m_in_packet_size;
	DWORD	m_saved_packet_size;
	char	m_cPacketBuf[255];

	bool m_bClientClose;
private:
	function<void(char*)> m_fpPacketProcess[SC_END];

private:
	void BindfpPacket();
public:
	CScene* pScene;
	static unsigned int m_usID;
	static CGameObject*	m_pPlayer;
	static CGameObject*	m_pChat;

	static concurrent_unordered_map<unsigned int, CGameObject*> m_cumPlayer;
	static concurrent_unordered_map<unsigned int, CGameObject*> m_cumAnimal;

public:
	SOCKET&	GetSocket() { return m_sock; }
public:
	static void Err_quit(const char* msg, int err_no);
	static void Err_display(const char* msg, int err_no);
public:
	bool ConnectServer(string ipAddr);
	void DisConnect();
	void RecvPacket();
	void ProcessPacket(char* _packet);

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