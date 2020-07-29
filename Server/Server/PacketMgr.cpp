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

void CPacketMgr::Send_Packet(unsigned int playerId, void* packet)
{
	DWORD flags{ 0 };
	char* cPacket = reinterpret_cast<char*>(packet);
	int packet_size = cPacket[0];

	OVER_EX* send_over = new OVER_EX;
	memset(send_over, 0x00, sizeof(OVER_EX));
	send_over->m_Event = EV_SEND;
	memcpy(send_over->m_MessageBuffer, cPacket, packet_size);
	send_over->m_DataBuffer.buf = send_over->m_MessageBuffer;
	send_over->m_DataBuffer.len = packet_size;

	int retval = WSASend(CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->GetSocket(),
		&send_over->m_DataBuffer, 1, NULL, flags, &send_over->m_Overlapped, NULL);

	if (retval == SOCKET_ERROR) {
		int err_no = WSAGetLastError();
		if (err_no != WSA_IO_PENDING) {
			CNetwork::Err_display("SEND EVENT SOCKET_ERR - ", err_no);
		}
	}
}

void CPacketMgr::Send_Login_OK_Packet(unsigned int playerId)
{
	sc_login_ok_packet packet;
	packet.size = sizeof(packet);
	packet.type = SC_LOGIN_OK;
	packet.id = playerId;
	Send_Packet(playerId, &packet);
}

void CPacketMgr::Send_Login_Fail_Packet(unsigned int playerId)
{
	sc_login_fail_packet packet;
	packet.size = sizeof(packet);
	packet.type = SC_LOGIN_FAIL;
	Send_Packet(playerId, &packet);
}

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
}
