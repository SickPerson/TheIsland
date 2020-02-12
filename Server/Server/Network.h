#pragma once
#include "stdafx.h"

class CNetwork
{
	DECLARE_SINGLE(CNetwork);

private:
	SOCKET	m_ListenSock = INVALID_SOCKET;
	volatile bool	m_bRunningServer = true;
	volatile unsigned short m_usUserID = 0;

public:
	bool	InitWinSock();
	bool	InitCompletionPort();
	bool	InitSock();

	void Initialize();
	void Disconnect();

	void WorkerThread();
	void AcceptThread();
	void UpdateThread();
	void DataBaseThread();

	void EndServer() { m_bRunningServer = false; }
	bool GetServerState() { return m_bRunningServer; }

public:
	static void Err_quit(const char* msg, int err_no);
	static void Err_display(const char* msg, int err_no);
};

