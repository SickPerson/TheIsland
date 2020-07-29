#include "PacketMgr.h"
#include "Process.h"
#include "DataBase.h"
#include "Network.h"
#include "Player.h"

CPacketMgr::CPacketMgr()
{	
}

CPacketMgr::~CPacketMgr()
{
}

<<<<<<< HEAD
void CPacketMgr::Send_Packet(unsigned int playerId, void* packet)
{
	DWORD flags{ 0 };
	char* cPacket = reinterpret_cast<char*>(packet);
	int packet_size = cPacket[0];
=======
void CPacketMgr::Send_Packet(unsigned int _usID, void* _packet)
{
	DWORD flags{ 0 };
	char* packet = reinterpret_cast<char*>(_packet);
	int packet_size = packet[0];
	int packet_type = packet[1];

	cout << "패킷 사이즈:" << packet_size << "패킷 타입:" << packet_type << endl;
>>>>>>> master

	OVER_EX* send_over = new OVER_EX;
	memset(send_over, 0x00, sizeof(OVER_EX));
	send_over->m_Event = EV_SEND;
<<<<<<< HEAD
	memcpy(send_over->m_MessageBuffer, cPacket, packet_size);
	send_over->m_DataBuffer.buf = send_over->m_MessageBuffer;
	send_over->m_DataBuffer.len = packet_size;

	int retval = WSASend(CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->GetSocket(),
=======
	memcpy(send_over->m_MessageBuffer, packet, packet_size);
	send_over->m_DataBuffer.buf = send_over->m_MessageBuffer;
	send_over->m_DataBuffer.len = packet_size;

	int retval = WSASend(CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->GetPlayerSocket(), 
>>>>>>> master
		&send_over->m_DataBuffer, 1, NULL, flags, &send_over->m_Overlapped, NULL);

	if (retval == SOCKET_ERROR) {
		int err_no = WSAGetLastError();
		if (err_no != WSA_IO_PENDING) {
			CNetwork::Err_display("SEND EVENT SOCKET_ERR - ", err_no);
		}
	}
}

<<<<<<< HEAD
void CPacketMgr::Send_Login_OK_Packet(unsigned int playerId)
=======
void CPacketMgr::Send_Login_OK_Packet(unsigned int _usID)
>>>>>>> master
{
	sc_login_ok_packet packet;
	packet.size = sizeof(packet);
	packet.type = SC_LOGIN_OK;
<<<<<<< HEAD
	packet.id = playerId;
	Send_Packet(playerId, &packet);
}

void CPacketMgr::Send_Login_Fail_Packet(unsigned int playerId)
=======
	packet.id = _usID;
	cout << packet.size << endl;
	Send_Packet(_usID, &packet);
}

void CPacketMgr::Send_Login_Fail_Packet(unsigned int _usID)
>>>>>>> master
{
	sc_login_fail_packet packet;
	packet.size = sizeof(packet);
	packet.type = SC_LOGIN_FAIL;
	Send_Packet(playerId, &packet);
}

<<<<<<< HEAD
void CPacketMgr::Send_Put_Player_Packet(unsigned int playerId, unsigned int OtherId)
{
	Vec3 vPos = CProcess::m_pPlayerPool->m_cumPlayerPool[OtherId]->GetPos();
	Vec3 vRot = CProcess::m_pPlayerPool->m_cumPlayerPool[OtherId]->GetRot();

	sc_put_player_packet	packet;
	packet.id = OtherId;
	packet.size = sizeof(packet);
	packet.type = SC_PUT_PLAYER;

	packet.vPos = vPos;
	packet.vRot = vRot;

	CPacketMgr::Send_Packet(playerId, &packet);

	if (playerId == OtherId) return;

	if(!CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->ExistList(OtherId))
		CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->InsertList(OtherId);
}

void CPacketMgr::Send_Pos_Player_Packet(unsigned int playerId, unsigned int OtherId)
{
	Vec3 pos = CProcess::m_pPlayerPool->m_cumPlayerPool[OtherId]->GetPos();
	Vec3 rot = CProcess::m_pPlayerPool->m_cumPlayerPool[OtherId]->GetRot();

	sc_pos_player_packet packet;
	packet.size = sizeof(sc_pos_player_packet);
	packet.type = SC_POS;
	packet.id = OtherId;
	packet.vPos = pos;
	packet.vRot = rot;

	if (0 != CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->ExistList(OtherId))
		CPacketMgr::Send_Packet(playerId, &packet);
	else
		CPacketMgr::Send_Put_Player_Packet(playerId, OtherId);
}

void CPacketMgr::Send_Remove_Player_Packet(unsigned int playerId, unsigned int OtherId)
{
	sc_remove_player_packet packet;
	packet.size = sizeof(packet);
	packet.type = SC_REMOVE_PLAYER;
	packet.id = OtherId;
	Send_Packet(playerId, &packet);

	CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->DeleteList(OtherId);
}

void CPacketMgr::Send_Chat_Packet(unsigned int playerId, unsigned int OtherId, char message[])
{
	sc_chat_packet packet;
	packet.size = sizeof(sc_chat_packet);
	packet.type = SC_CHAT;
	wcscpy_s(packet.wcid, CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->GetWcID());
	strcpy_s(packet.meesage, message);
	CPacketMgr::Send_Packet(OtherId, &packet);
}

void CPacketMgr::Send_Put_Npc_Packet(unsigned int PlayerID, unsigned int NpcID)
=======
void CPacketMgr::Send_Message_Packet(unsigned int _usID, char* _message)
{
	sc_chat_packet packet;
	packet.size = sizeof(packet);
	packet.type = SC_CHAT;
	strcpy_s(packet.meesage, _message);
	Send_Packet(_usID, &packet);
}

void CPacketMgr::Send_First_Status_Packet(unsigned int _usID)
{
	sc_first_status_packet packet;
	packet.size = sizeof(sc_first_status_packet);
	packet.type = SC_LOGIN;
	packet.id = _usID;
	// No DataBase
	packet.HP = 100;
	packet.Hunger = 100;
	packet.Stamina = 100;
	packet.Thirst = 100;

	packet.fPosX = 0.f;
	packet.fPosY = 25.f;
	packet.fPosZ = 0.f;

	packet.fDirX = 0.f;
	packet.fDirY = 180.f;
	packet.fDirZ = 0.f;

	Send_Packet(_usID, &packet);

	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetHP(packet.HP);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetHunger(packet.Hunger);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetStamina(packet.Stamina);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetThirst(packet.Thirst);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetPos(packet.fPosX, packet.fPosY, packet.fPosZ);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetDir(packet.fDirX, packet.fDirY, packet.fDirZ);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetPlayerConnect(true);
}

void CPacketMgr::Send_Accept_Packet(unsigned int _usID, concurrent_unordered_set<unsigned int>& _ViewList)
{
	DirectX::XMFLOAT3 pos = CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->GetPos();
	DirectX::XMFLOAT3 dir = CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->GetDir();

	sc_accept_packet packet;
	packet.size = sizeof(sc_first_status_packet);
	packet.type = SC_CONNECT;
	packet.id = _usID;

	packet.fPosX = pos.x;
	packet.fPosY = pos.y;
	packet.fPosZ = pos.z;

	packet.fDirX = dir.x;
	packet.fDirY = dir.y;
	packet.fDirZ = dir.z;

	packet.HP = CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->GetHP();
	//Send_Packet(_usID, &packet);

	// 접속하여 있는 플레이어들에게 현재 플레이어가 들어왔음에 ViewList에 추가시킨다.
	for (auto& au : _ViewList)
	{
		if (au == _usID) 
			continue;
		Send_Packet(au, &packet);
	}
}

void CPacketMgr::Send_Status_Packet(unsigned int _usPlayer, unsigned int _usOther)
{
	DirectX::XMFLOAT3 pos = CProcess::m_pPlayerPool->m_cumPlayerPool[_usPlayer]->GetPos();
	DirectX::XMFLOAT3 dir = CProcess::m_pPlayerPool->m_cumPlayerPool[_usPlayer]->GetDir();

	sc_accept_packet packet;
	packet.size = sizeof(sc_accept_packet);
	packet.type = SC_CONNECT;

	packet.HP = CProcess::m_pPlayerPool->m_cumPlayerPool[_usPlayer]->GetHP();
	packet.fPosX = pos.x;
	packet.fPosY = pos.y;
	packet.fPosZ = pos.z;
	packet.fDirX = dir.x;
	packet.fDirY = dir.y;
	packet.fDirZ = dir.z;

	CPacketMgr::Send_Packet(_usOther, &packet);
}

void CPacketMgr::Send_Pos_Packet(unsigned int _usPlayer, unsigned int _usOther)
{
	concurrency::concurrent_unordered_set<unsigned int> viewlist;
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usPlayer]->CopyPlayerList(viewlist);
	
	DirectX::XMFLOAT3 pos = CProcess::m_pPlayerPool->m_cumPlayerPool[_usPlayer]->GetPos();
	DirectX::XMFLOAT3 dir = CProcess::m_pPlayerPool->m_cumPlayerPool[_usPlayer]->GetDir();

	sc_pos_packet packet;
	packet.size = sizeof(sc_pos_packet);
	packet.type = SC_POS;
	packet.id = _usPlayer;
	packet.fPosX = pos.x;
	packet.fPosY = pos.y;
	packet.fPosZ = pos.z;

	packet.fDirX = dir.x;
	packet.fDirY = dir.y;
	packet.fDirZ = dir.z;
	CPacketMgr::Send_Packet(_usOther, &packet);
}

void CPacketMgr::Send_Look_Packet(unsigned int _usPlayer)
{
	concurrent_unordered_set<unsigned int> viewlist;
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usPlayer]->CopyPlayerList(viewlist);
}

void CPacketMgr::Send_Remove_Packet(unsigned int _usPlayer, unsigned int _usOther)
>>>>>>> master
{
	Vec3 pos = CProcess::m_pMonsterPool->m_cumMonsterPool[NpcID]->GetPos();
	Vec3 rot = CProcess::m_pMonsterPool->m_cumMonsterPool[NpcID]->GetRot();

	sc_put_player_packet	packet;
	packet.id = NpcID;
	packet.size = sizeof(packet);
	packet.type = SC_PUT_NPC;

	packet.vPos = pos;
	packet.vRot = rot;

	CPacketMgr::Send_Packet(PlayerID, &packet);

	CProcess::m_pPlayerPool->m_cumPlayerPool[PlayerID]->InsertList(NpcID + MAX_USER);
}

<<<<<<< HEAD
void CPacketMgr::Send_Pos_Npc_Packet(unsigned int PlayerID, unsigned int NpcID)
{
	Vec3 pos = CProcess::m_pMonsterPool->m_cumMonsterPool[NpcID]->GetPos();
	Vec3 rot = CProcess::m_pMonsterPool->m_cumMonsterPool[NpcID]->GetRot();

	sc_pos_npc_packet packet;
	packet.size = sizeof(sc_pos_npc_packet);
	packet.type = SC_POS_NPC;
	packet.id = NpcID;
	packet.vPos = pos;
	packet.vRot = rot;

	if (0 != CProcess::m_pPlayerPool->m_cumPlayerPool[PlayerID]->ExistList(NpcID + MAX_USER))
		CPacketMgr::Send_Packet(PlayerID, &packet);
	else
		CPacketMgr::Send_Put_Npc_Packet(PlayerID, NpcID);
}

void CPacketMgr::Send_Remove_Npc_Packet(unsigned int PlayerID, unsigned int NpcID)
{
	sc_remove_player_packet packet;
	packet.size = sizeof(packet);
	packet.type = SC_REMOVE_NPC;
	packet.id = NpcID;
	Send_Packet(PlayerID, &packet);

	CProcess::m_pPlayerPool->m_cumPlayerPool[PlayerID]->DeleteList(NpcID + MAX_USER);
=======
void CPacketMgr::Send_Disconnect_Packet(unsigned int _usID)
{
	concurrent_unordered_set<unsigned int> list;
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->CopyPlayerList(list);

	// 패킷 보내기
	sc_disconnect_packet	packet;
	packet.size = sizeof(sc_disconnect_packet);
	packet.type = SC_DISCONNECT;
	packet.id = _usID;
	//Send_Packet(_usID, &packet);

	for (auto& au : list)
	{
		if (au == _usID)
			continue;
		if (CProcess::m_pPlayerPool->m_cumPlayerPool[au]->CheckList(_usID)) {
			CProcess::m_pPlayerPool->m_cumPlayerPool[au]->DeleteList(_usID);
			if (CProcess::m_pPlayerPool->m_cumPlayerPool[au]->GetPlayerConnect())
				CPacketMgr::Send_Packet(au, &packet);
		}
	}
	//closesocket(CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->GetPlayerSocket());
	//CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->Init();
}

void CPacketMgr::Send_Chat_Packet(unsigned int _usID, unsigned int _usOther, char message[])
{
	concurrent_unordered_set<unsigned int> viewlist;
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->CopyPlayerList(viewlist);

	sc_chat_packet packet;

	packet.size = sizeof(sc_chat_packet);
	packet.type = SC_CHAT;
	strcpy_s(packet.meesage, message);

	for (auto& au : viewlist) {
		if (au == _usID) continue;
		if (CProcess::m_pPlayerPool->m_cumPlayerPool[au]->CheckList(_usID)) {
				CPacketMgr::Send_Packet(au, &packet);
		}
	}

>>>>>>> master
}
