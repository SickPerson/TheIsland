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

void CPacketMgr::Send_Packet(USHORT playerId, void* packet)
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

	int retval = WSASend(CProcess::m_pObjectPool->m_cumPlayerPool[playerId]->GetSocket(),
		&send_over->m_DataBuffer, 1, NULL, flags, &send_over->m_Overlapped, NULL);

	if (retval == SOCKET_ERROR) {
		int err_no = WSAGetLastError();
		if (err_no != WSA_IO_PENDING) {
			CNetwork::Err_display("SEND EVENT SOCKET_ERR - ", err_no);
		}
	}
}

void CPacketMgr::Send_Login_OK_Packet(USHORT playerId)
{
	sc_login_ok_packet packet;
	packet.size = sizeof(packet);
	packet.type = SC_LOGIN_OK;
	packet.id = playerId;
	Send_Packet(playerId, &packet);
}

void CPacketMgr::Send_Login_Fail_Packet(USHORT playerId)
{
	sc_login_fail_packet packet;
	packet.size = sizeof(packet);
	packet.type = SC_LOGIN_FAIL;
	Send_Packet(playerId, &packet);
}

void CPacketMgr::Send_Put_Player_Packet(USHORT playerId, USHORT OtherId)
{
	Vec3 vPos = CProcess::m_pObjectPool->m_cumPlayerPool[OtherId]->GetLocalPos();
	Vec3 vRot = CProcess::m_pObjectPool->m_cumPlayerPool[OtherId]->GetLocalRot();

	sc_put_player_packet	packet;
	packet.id = OtherId;
	packet.size = sizeof(packet);
	packet.type = SC_PUT_PLAYER;

	packet.vPos = vPos;
	packet.vRot = vRot;

	Send_Packet(playerId, &packet);

	if (playerId == OtherId) return;

	if(!CProcess::m_pObjectPool->m_cumPlayerPool[playerId]->ExistList(OtherId))
		CProcess::m_pObjectPool->m_cumPlayerPool[playerId]->InsertList(OtherId);
}

void CPacketMgr::Send_Pos_Player_Packet(USHORT playerId, USHORT OtherId)
{
	Vec3 pos = CProcess::m_pObjectPool->m_cumPlayerPool[OtherId]->GetLocalPos();
	Vec3 rot = CProcess::m_pObjectPool->m_cumPlayerPool[OtherId]->GetLocalRot();

	sc_pos_player_packet packet;
	packet.size = sizeof(sc_pos_player_packet);
	packet.type = SC_POS_PLAYER;
	packet.id = OtherId;
	packet.vPos = pos;
	packet.vRot = rot;
	if (0 != CProcess::m_pObjectPool->m_cumPlayerPool[playerId]->ExistList(OtherId) && playerId != OtherId)
		Send_Packet(playerId, &packet);
	else
		Send_Put_Player_Packet(playerId, OtherId);
}

void CPacketMgr::Send_Status_Player_Packet(USHORT playerId, USHORT OtherId)
{
	sc_status_player_packet packet;
	packet.size = sizeof(sc_status_player_packet);
	packet.type = SC_STATUS_PLAYER;

	packet.id = playerId;
	packet.fHealth = CProcess::m_pObjectPool->m_cumPlayerPool[playerId]->GetHealth();
	packet.fHungry = CProcess::m_pObjectPool->m_cumPlayerPool[playerId]->GetHungry();
	packet.fThrist = CProcess::m_pObjectPool->m_cumPlayerPool[playerId]->GetThirst();

	Send_Packet(playerId, &packet);
}

void CPacketMgr::Send_Rot_Player_Packet(USHORT playerId, USHORT OtherId)
{
	sc_rot_player_packet packet;
	packet.size = sizeof(sc_rot_player_packet);
	packet.type = SC_ROT_PLAYER;
	packet.id = OtherId;
	packet.vRot = CProcess::m_pObjectPool->m_cumPlayerPool[OtherId]->GetLocalRot();
	if(playerId != OtherId)
		Send_Packet(playerId, &packet);
}

void CPacketMgr::Send_Remove_Player_Packet(USHORT playerId, USHORT OtherId)
{
	sc_remove_player_packet packet;
	packet.size = sizeof(packet);
	packet.type = SC_REMOVE_PLAYER;
	packet.id = OtherId;
	Send_Packet(playerId, &packet);

	if(CProcess::m_pObjectPool->m_cumPlayerPool[playerId]->ExistList(OtherId))
		CProcess::m_pObjectPool->m_cumPlayerPool[playerId]->DeleteList(OtherId);
}

void CPacketMgr::Send_Chat_Packet(USHORT playerId, USHORT OtherId, char message[])
{
	sc_chat_packet packet;
	packet.size = sizeof(sc_chat_packet);
	packet.type = SC_CHAT;
	wcscpy_s(packet.wcid, CProcess::m_pObjectPool->m_cumPlayerPool[playerId]->GetWcID());
	strcpy_s(packet.meesage, message);
	Send_Packet(OtherId, &packet);
}

void CPacketMgr::Send_Death_Player_Packet(USHORT playerId)
{

}

void CPacketMgr::Send_Animation_Player_Packet(USHORT playerId, USHORT actId, UINT AnimationType)
{
	sc_animation_player_packet packet;
	packet.size = sizeof(sc_animation_player_packet);
	packet.type = SC_ANIMATION_PLAYER;
	packet.animation_uiType = (UINT)AnimationType;
	packet.id = actId;
	cout << packet.animation_uiType << endl;
	Send_Packet(playerId, &packet);
}

void CPacketMgr::Send_Put_Npc_Packet(USHORT PlayerID, USHORT NpcID)
{
	Vec3 pos = CProcess::m_pObjectPool->m_cumAnimalPool[NpcID]->GetLocalPos();
	Vec3 rot = CProcess::m_pObjectPool->m_cumAnimalPool[NpcID]->GetLocalRot();
	char eType = CProcess::m_pObjectPool->m_cumAnimalPool[NpcID]->GetKind();

	sc_put_npc_packet	packet;
	packet.id = NpcID;
	packet.size = sizeof(packet);
	packet.type = SC_PUT_ANIMAL;

	packet.eType = eType;
	packet.vPos = pos;
	packet.vRot = rot;
	

	Send_Packet(PlayerID, &packet);

	if(CProcess::m_pObjectPool->m_cumPlayerPool[PlayerID]->ExistList(NpcID + BEGIN_ANIMAL))
		CProcess::m_pObjectPool->m_cumPlayerPool[PlayerID]->InsertList(NpcID + BEGIN_ANIMAL);
}

void CPacketMgr::Send_Pos_Npc_Packet(USHORT PlayerID, USHORT NpcID)
{
	Vec3 pos = CProcess::m_pObjectPool->m_cumAnimalPool[NpcID]->GetLocalPos();
	Vec3 rot = CProcess::m_pObjectPool->m_cumAnimalPool[NpcID]->GetLocalRot();
	char eType = CProcess::m_pObjectPool->m_cumAnimalPool[NpcID]->GetKind();

	sc_pos_npc_packet packet;
	packet.size = sizeof(sc_pos_npc_packet);
	packet.type = SC_POS_ANIMAL;
	packet.id = NpcID;

	packet.eType = eType;
	packet.vPos = pos;
	packet.vRot = rot;

	if (0 != CProcess::m_pObjectPool->m_cumPlayerPool[PlayerID]->ExistList(NpcID + BEGIN_ANIMAL))
		CPacketMgr::Send_Packet(PlayerID, &packet);
	else
		CPacketMgr::Send_Put_Npc_Packet(PlayerID, NpcID);
}

void CPacketMgr::Send_Remove_Npc_Packet(USHORT PlayerID, USHORT NpcID)
{
	sc_remove_player_packet packet;
	packet.size = sizeof(packet);
	packet.type = SC_REMOVE_ANIMAL;
	packet.id = NpcID;
	Send_Packet(PlayerID, &packet);

	if (CProcess::m_pObjectPool->m_cumPlayerPool[PlayerID]->ExistList(NpcID + BEGIN_ANIMAL))
		CProcess::m_pObjectPool->m_cumPlayerPool[PlayerID]->DeleteList(NpcID + BEGIN_ANIMAL);
}

void CPacketMgr::Send_Animation_Npc_Packet(USHORT playerId, USHORT NpcId, UINT AnimationType)
{
	sc_animation_player_packet packet;
	packet.size = sizeof(sc_animation_npc_packet);
	packet.type = SC_ANIMATION_ANIMAL;
	packet.animation_uiType = AnimationType;
	packet.id = NpcId;
	Send_Packet(playerId, &packet);
}

void CPacketMgr::Send_Install_Housing_Packet(USHORT player_Id, USHORT housing_Id)
{
	sc_install_housing_packet packet;

	packet.size = sizeof(sc_install_housing_packet);
	packet.type = SC_INSTALL_HOUSE;
	packet.house_id = housing_Id;

	packet.housing_type =	CProcess::m_pObjectPool->m_cumHousingPool[housing_Id]->GetType();
	packet.vLocalPos =		CProcess::m_pObjectPool->m_cumHousingPool[housing_Id]->GetLocalPos();
	packet.vLocalRot =		CProcess::m_pObjectPool->m_cumHousingPool[housing_Id]->GetLocalRot();
	packet.vLocalScale =	CProcess::m_pObjectPool->m_cumHousingPool[housing_Id]->GetLocalScale();
	Send_Packet(player_Id, &packet);
}

void CPacketMgr::Send_Remove_Housing_Packet(USHORT player_Id, USHORT housing_Id)
{
	sc_remove_housing_packet packet;

	packet.size = sizeof(sc_remove_housing_packet);
	packet.type = SC_REMOVE_HOUSE;
	packet.house_id = housing_Id;
	Send_Packet(player_Id, &packet);
}

void CPacketMgr::Send_Put_Natural_Packet(USHORT PlayerId, USHORT NaturalId)
{
	sc_put_natural_packet packet;
	packet.size = sizeof(sc_put_natural_packet);
	packet.type = SC_PUT_NATURAL;
	packet.natural_id = NaturalId;
	packet.fHealth = CProcess::m_pObjectPool->m_cumNaturalPool[NaturalId]->GetHealth();
	packet.bDestroy = CProcess::m_pObjectPool->m_cumNaturalPool[NaturalId]->GetDestroy();
	packet.vLocalPos = CProcess::m_pObjectPool->m_cumNaturalPool[NaturalId]->GetLocalPos();
	packet.vLocalScale = CProcess::m_pObjectPool->m_cumNaturalPool[NaturalId]->GetLocalScale();
	packet.vOffsetPos = CProcess::m_pObjectPool->m_cumNaturalPool[NaturalId]->GetOffsetPos();
	packet.vOffsetScale = CProcess::m_pObjectPool->m_cumNaturalPool[NaturalId]->GetOffsetScale();
	Send_Packet(PlayerId, &packet);
}

void CPacketMgr::Send_Natural_Destroy_Packet(USHORT PlayerId, USHORT NaturalId)
{
	sc_destroy_natural_packet packet;
	packet.size = sizeof(sc_destroy_natural_packet);
	packet.type = SC_DESTROY_NATURAL;
	packet.natural_id = NaturalId;
	Send_Packet(PlayerId, &packet);

}

void CPacketMgr::Send_Weather_Packet(USHORT player_Id, bool bRain)
{
	sc_weather_packet packet;
	packet.size = sizeof(sc_weather_packet);
	packet.type = SC_WEATHER;
	packet.bRain = bRain;
	Send_Packet(player_Id, &packet);
}

void CPacketMgr::Send_Time_Packet(USHORT player_Id, float fTime)
{
	sc_time_packet packet;
	packet.size = sizeof(sc_time_packet);
	packet.type = SC_TIME;
	packet.fTime = fTime;
	Send_Packet(player_Id, &packet);
}

void CPacketMgr::Send_Disconnect_Server_Packet(USHORT player_Id)
{
	sc_disconnect_server_packet packet;
	packet.size = sizeof(sc_disconnect_server_packet);
	packet.tpye = SC_DISCONNECT_SERVER;
	Send_Packet(player_Id, &packet);
}
