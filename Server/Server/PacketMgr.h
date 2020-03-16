#pragma once

#include "stdafx.h"

class PacketMgr
{
	SINGLE(PacketMgr)

public:
	static void Send_Packet(const unsigned short& _usID, void* _packet);

public:
	void Send_Login_Packet(unsigned short _usID);
	void Send_Accept_Packet(concurrent_unordered_set<unsigned short>& _ViewList, unsigned short _usID);
	void Send_Status_Packet(unsigned short _usPlayer, unsigned short _usOther);
	void Send_Pos_Packet(unsigned short _usPlayer, unsigned short _usOther);
	void Send_Look_Packet(unsigned short _usPlayer);
	void Send_Remove_Packet(unsigned short _usPlayer, unsigned short _usOther);
	void Send_Disconnect_Packet(unsigned short _usPlayer);
};