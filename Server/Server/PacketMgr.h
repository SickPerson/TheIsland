#pragma once

#include "stdafx.h"

class PacketMgr
{
	DECLARE_SINGLE(PacketMgr)

public:
	static void Send_Packet(const unsigned short& id, void* buf);

public:
	void Send_Login_Ok_Packet(const int& id);
};