#pragma once
#include "stdafx.h"
#include "Process.h"

class CPlayerProcess:
	public CProcess
{
private:
	recursive_mutex	m_rmPlayerProcessLock;
	function<void(unsigned int)>	m_fpUpdateViewList;
	function<void(unsigned int, char*)> m_fpPacketProcess[CS_END];
public:
	explicit CPlayerProcess();
	virtual ~CPlayerProcess();

public:
	void BindPacketProcess()
	{
		m_fpPacketProcess[CS_LOGIN] = [&](unsigned int _usID, char* _packet)
		{
			PlayerLogin(_usID, _packet);
		};
		m_fpPacketProcess[CS_LOOK] = [&](unsigned int _usID, char* _packet)
		{
			PlayerLook(_usID, _packet);
		};
		m_fpPacketProcess[CS_POS] = [&](unsigned int _usID, char* _packet)
		{
			PlayerPos(_usID, _packet);
		};
	}
	void BindUpdateViewListFunction()
	{
		m_fpUpdateViewList = [&](unsigned int usID) {UpdateViewList(usID); };
	}
	void AcceptClient(const SOCKET& _sSocket, unsigned int _usID);
	void ProcessPacket(unsigned int _usID, char* _packet);
	void RecvPacket(unsigned int _usID, char* _Packet, DWORD bytesize);

	void PlayerLogin(unsigned int _usID, char* _packet);
	void PlayerLogin(DataBase_Event& _event);
	void PlayerPos(unsigned int _usID, char* _Packet);
	void PlayerLook(unsigned int _usID, char* _Packet);
	void PlayerDinconnect(unsigned int _usID);

public:
	void UpdateViewList(unsigned int _usID);
};

