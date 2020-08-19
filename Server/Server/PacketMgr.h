#pragma once
#include "stdafx.h"

class CPacketMgr
{
	SINGLE(CPacketMgr)

public:
	static void Send_Packet(USHORT playerId, void* packet);

public: // 로그인 관련 패킷
	static void Send_Login_OK_Packet(USHORT playerId);
	static void Send_Login_Fail_Packet(USHORT playerId);
	static void Send_Login_Status_Packet(USHORT playerId);

public: // Player 관련 패킷
	static void Send_Put_Player_Packet(USHORT playerId, USHORT OtherId);
	static void Send_Pos_Player_Packet(USHORT playerId, USHORT OtherId);
	static void Send_Status_Player_Packet(USHORT playerId, USHORT OtherId);
	static void Send_Rot_Player_Packet(USHORT playerId, USHORT OtherId);
	static void Send_Remove_Player_Packet(USHORT playerId, USHORT OtherId);
	static void Send_Chat_Packet(USHORT playerId, USHORT OtherId, char message[]);
	static void Send_Death_Player_Packet(USHORT playerId);

	// Player Animation
	static void Send_Animation_Player_Packet(USHORT playerId, UINT AnimationType);

public: // Npc(Monster) 관련
	static void Send_Wakeup_Npc_Packet(USHORT playerId, USHORT NpcId);
	static void Send_Put_Npc_Packet(USHORT playerId, USHORT NpcId);
	static void Send_Pos_Npc_Packet(USHORT playerId, USHORT NpcId);
	static void Send_Remove_Npc_Packet(USHORT playerId, USHORT NpcId);
	static void Send_Animation_Npc_Packet(USHORT playerId, USHORT NpcId, UINT AnimationType);

public:
	static void Send_Install_Housing_Packet(USHORT player_Id, USHORT housing_Id);
	static void Send_Remove_Housing_Packet(USHORT player_Id, USHORT housing_Id);

public:
	static void Send_Put_Natural_Packet(USHORT PlayerId, USHORT NaturalId);
	static void Send_Natural_Destroy_Packet(USHORT PlayerId, USHORT NaturalId);

public: // etc
	static void Send_Weather_Packet(USHORT player_Id, bool bRain);
	static void Send_Time_Packet(USHORT player_Id, float fTime);
	static void Send_Disconnect_Server_Packet(USHORT player_Id);
};