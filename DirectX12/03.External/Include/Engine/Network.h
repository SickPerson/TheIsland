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

	WSABUF	m_wbSendWsaBuf;
	WSABUF	m_wbRecvWsaBuf;

	char	m_cSendBuf[256];
	char	m_cRecvBuf[256];
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

	SOCKET GetSocket() { return m_sGameSocket; }

	bool Init();
	bool ConnectServer();
	bool CreateEventSelect();

	void DisConnect();
	void RecvPacket();
	void ProcessPacket(char* _packet);

	void SetIsLogin(bool _bLogin) { std::lock_guard<std::recursive_mutex>lock(m_rmLoginStateLock[LT_LOGIN_SUCCESS]); m_bLoginSuccess = _bLogin; }
	bool GetIsLogin() { std::lock_guard<std::recursive_mutex>lock(m_rmLoginStateLock[LT_LOGIN_SUCCESS]); return m_bLoginSuccess; }
	bool GetLoginSend() { std::lock_guard<std::recursive_mutex>lock(m_rmLoginStateLock[LT_LOGIN_PACKET_SEND]); return m_bLoginPacketSend; }
	void SetLoginSend(bool bStart) { std::lock_guard<std::recursive_mutex>lock(m_rmLoginStateLock[LT_LOGIN_PACKET_SEND]); m_bLoginPacketSend = bStart; }

	void SetLoopStart(bool bStart) { std::lock_guard<std::recursive_mutex>lock(m_rmLoginStateLock[LT_LOOP_START]); m_bLoopStart = bStart; }
	bool GetLoopStart() { std::lock_guard<std::recursive_mutex>lock(m_rmLoginStateLock[LT_LOOP_START]); return m_bLoopStart; }
	void SetClient(bool bStart) { m_bClientClose = bStart; }
public:
	static void Err_quit(const char* msg, int err_no);
	static void Err_display(const char* msg, int err_no);
};