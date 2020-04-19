#pragma once
#include "stdafx.h"

class CPacketMgr
{
	SINGLE(CPacketMgr)

public:
	static void Send_Packet(const unsigned short& _usID, void* _packet);

public:
	static void Send_Login_OK_Packet(unsigned short _usID);
	static void Send_Login_Fail_Packet(unsigned short _usID);
	static void Send_Message_Packet(unsigned short _usID, wchar_t* _message);
	static void Send_First_Status_Packet(unsigned short _usID);
	static void Send_Accept_Packet(unsigned short _usID, concurrent_unordered_set<unsigned short>& _ViewList);
	static void Send_Status_Packet(unsigned short _usID, unsigned short _usOther);
	static void Send_Pos_Packet(unsigned short _usID, unsigned short _usOther);
	static void Send_Look_Packet(unsigned short _usID);
	static void Send_Remove_Packet(unsigned short _usID, unsigned short _usOther);
	static void Send_Disconnect_Packet(unsigned short _usID);
};