#pragma once
#include "stdafx.h"
#include "Network.h"

class CPacketMgr
{
private:
	char	m_cSendBuf[256];
	char	m_cRecvBuf[256];

	WSABUF	m_wbSendWsaBuf;
	WSABUF	m_wbRecvWsaBuf;
public:
	void SendLoginPacket(wchar_t* _wcPlayerID, wchar_t* _wcPlayerPW);

public:
	bool GetLoginSend() {}
	void SetloginSend() {}
public:
	CPacketMgr();
	~CPacketMgr();
};

