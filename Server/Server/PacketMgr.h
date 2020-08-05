#pragma once
#include "stdafx.h"

class CPacketMgr
{
	SINGLE(CPacketMgr)

public:
	static void Send_Packet(unsigned short playerId, void* packet);

public: // 로그인 관련 패킷
	static void Send_Login_OK_Packet(unsigned short playerId);
	static void Send_Login_Fail_Packet(unsigned short playerId);
	static void Send_Login_Status_Packet(unsigned short playerId);

public: // Player 관련 패킷
	static void Send_Put_Player_Packet(unsigned short playerId, unsigned short OtherId);
	static void Send_Pos_Player_Packet(unsigned short playerId, unsigned short OtherId);
	static void Send_Rot_Player_Packet(unsigned short playerId, unsigned short OtherId);
	static void Send_Remove_Player_Packet(unsigned short playerId, unsigned short OtherId);
	static void Send_Chat_Packet(unsigned short playerId, unsigned short OtherId, char message[]);
	static void Send_Death_Player_Packet(unsigned short playerId);
	static void Send_Animation_Player_Packet(unsigned short playerId, unsigned short OtherId, char AnimationType);

public:
	static void Send_Wakeup_Npc_Packet(unsigned short playerId, unsigned short NpcId);
	static void Send_Put_Npc_Packet(unsigned short playerId, unsigned short NpcId);
	static void Send_Pos_Npc_Packet(unsigned short playerId, unsigned short NpcId);
	static void Send_Remove_Npc_Packet(unsigned short playerId, unsigned short NpcId);
	static void Send_Animation_Npc_Packet(unsigned short playerId, unsigned short NpcId, char AnimationType);
};