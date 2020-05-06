#pragma once
#include "stdafx.h"

class CPacketMgr
{
	SINGLE(CPacketMgr)

public:
	static void Send_Packet(unsigned int _usID, void* _packet);

public:
	static void Send_Login_IP_Packet(unsigned int _usID);
	static void Send_Login_OK_Packet(unsigned int _usID);
	static void Send_Login_Fail_Packet(unsigned int _usID);
	static void Send_Message_Packet(unsigned int _usID, char* _message);

	static void Send_First_Status_Packet(unsigned int _usID);

	static void Send_Accept_Packet(unsigned int _usID, concurrent_unordered_set<unsigned int>& _ViewList);
	static void Send_Status_Packet(unsigned int _usID, unsigned int _usOther);
	static void Send_Pos_Packet(unsigned int _usID, unsigned int _usOther);
	static void Send_Look_Packet(unsigned int _usID);
	static void Send_Remove_Packet(unsigned int _usID, unsigned int _usOther);
	static void Send_Disconnect_Packet(unsigned int _usID);
	static void Send_Chat_Packet(unsigned int _usID, unsigned int _usOther, char message[]);
};