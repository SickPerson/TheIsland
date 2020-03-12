#pragma once
#include "stdafx.h"
#include "Process.h"

class CPlayerProcess:
	public CProcess
{
private:
	recursive_mutex	m_rmPlayerProcessLock;
	function<void(unsigned short)>	m_fpUpdateViewList;
	function<void(unsigned short, char*)> m_fppacketProcess[CS_END];
public:
	explicit CPlayerProcess();
	virtual ~CPlayerProcess();

public:
	void BindPacketProcess()
	{

	}
	void BindUpdateViewListFunction()
	{

	}
	void AcceptClient(const SOCKET& _sSocket, unsigned short _usID);
	void PacketProcess(unsigned short _usID, char* _packet);
	void Recv(unsigned short _usID, )
};

