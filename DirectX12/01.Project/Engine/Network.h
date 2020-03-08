#pragma once
#include "stdafx.h"
#include "../../../Server/Server/protocol.h"

enum LOCK_TYPE {
	LT_LOGIN_SUCCESS,
	LT_LOOP_START,
	LT_LOGIN_PACKET_SEND,
	LT_END
};
class CNetwork
{
	SINGLE(CNetwork);

private:
	SOCKET	m_sGameSocket;

	WSAEVENT			m_hEvent;
	WSANETWORKEVENTS	m_weEvent;

	char	m_cPacketBuf[256];

	DWORD	m_dwPacketSize = 0;
	DWORD	m_dwSavePacketSize = 0;

	int m_iIndex;

	std::recursive_mutex	m_rmLoginStateLock[LT_END];

	volatile bool	m_bLoginPacketSend = false;
	volatile bool	m_bLoginSuccess = false;
	volatile bool	m_bLoopStart = false;

	bool m_bClientClose = false;

public:
	static unsigned short m_usID;

	bool Init();
	bool ConnectServer();
	bool CreateEventSelect();

	void DisConnect();
	void RecvPacket();
	void ProcessPacket(char* _packet);

	SOCKET GetSocket() { return m_sGameSocket; }
};

