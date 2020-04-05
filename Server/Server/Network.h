#pragma once
#include "stdafx.h"
#include "PlayerProcess.h"

#include "PacketMgr.h"
#include "TimerMgr.h"
#include "DataBase.h"

class CNetwork
{
	SINGLE(CNetwork)

private:
	HANDLE			m_hIocp;
	SOCKET			m_ListenSock;
	volatile bool	m_bRunningServer;

private:
	CPlayerProcess*	m_pPlayerProcess;

private:
	volatile unsigned short m_usUserID;

private:
	char ipaddr[MAX_STR_LEN];

public:
	char* GetIpAddr() { return ipaddr; }

public:
	bool CreateSocket(SOCKET& _sClientSocket, unsigned short _usID);
	bool Associate(HANDLE _hDevice, unsigned short _usID);

public:
	bool InitWinSock();
	bool InitCompletionPort();
	bool InitSock();

	bool Initialize();
	void Disconnect();

	void WorkerThread();
	void AcceptThread();
	void UpdateThread();
	void DataBaseThread();

	void EndServer() { m_bRunningServer = false; }
	bool GetServerState() { return m_bRunningServer; }
	void GetServerIpAddress();

public:
	static void Err_quit(const char* msg, int err_no);
	static void Err_display(const char* msg, int err_no);
};

