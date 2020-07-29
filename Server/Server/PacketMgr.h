#pragma once
#include "stdafx.h"

class CPacketMgr
{
	SINGLE(CPacketMgr)

public:
	static void Send_Packet(unsigned int playerId, void* packet);

public: // 로그인 관련 패킷
	static void Send_Login_OK_Packet(unsigned int playerId);
	static void Send_Login_Fail_Packet(unsigned int playerId);

public: // Player 관련 패킷
	static void Send_Put_Player_Packet(unsigned int playerId, unsigned int OtherId);
	static void Send_Pos_Player_Packet(unsigned int playerId, unsigned int OtherId);
	static void Send_Remove_Player_Packet(unsigned int playerId, unsigned int OtherId);
	static void Send_Chat_Packet(unsigned int playerId, unsigned int OtherId, char message[]);

public:
	static void Send_Put_Npc_Packet(unsigned int playerId, unsigned int NpcId);
	static void Send_Pos_Npc_Packet(unsigned int playerId, unsigned int NpcId);
	static void Send_Remove_Npc_Packet(unsigned int playerId, unsigned int NpcId);
};