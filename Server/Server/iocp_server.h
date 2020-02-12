#pragma once
#include "stdafx.h"
#include "Network.h"

class PacketMgr;
class iocp_server
{
	DECLARE_SINGLE(iocp_server)

private:
	HANDLE	m_hIocp;
	int		m_iCpuCore;
	vector<thread*>		m_vWorkerThread;
	shared_ptr<thread>	m_pAcceptThread;
	shared_ptr<thread>	m_pUpdateThread;
	shared_ptr<thread>	m_pDataBaseThread;
	bool				m_ServerShutdown{ false };

public:
	void GetServerIpAddress();
	bool Init();
	void	CheckThisCpuCount();

	void MakeThreads();

	void Do_WokerThread();
	void Do_AcceptThread();
	void Do_TimerThread();

	/*int ConnectDB(int id);
	int CreateDB(int id);
	int SavePosDB(int id);*/

	void ProcessPacket(const unsigned int& id, const char* buf);

private:
	PacketMgr*	m_pPacketMgr;

private:
	map<int, PLAYER_INFO*>	m_clients;
	unsigned int							new_user_id;
};

