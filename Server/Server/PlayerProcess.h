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
		/*CS_LOGIN = 0;
		CS_LOGOUT = 1;
		CS_LOOK = 2;
		CS_POS = 3;
		CS_ATTACK = 4;
		CS_IDLE = 5;
		CS_END = 6;*/
		m_fppacketProcess[CS_LOGIN] = [&](unsigned short _usID, char* _packet)
		{
			PlayerLogin(_usID, _packet);
		};
		m_fppacketProcess[CS_LOGOUT] = [&](unsigned short _usID, char* _packet)
		{

		};
		m_fppacketProcess[CS_LOOK] = [&](unsigned short _usID, char* _packet)
		{

		};
		m_fppacketProcess[CS_POS] = [&](unsigned short _usID, char* _packet)
		{

		};
		/*m_fppacketProcess[CS_LOGIN] = [&](unsigned short _usID, char* _packet)
		{
			if(CProcess::m_pPlayerPool)
		}*/
	}
	void BindUpdateViewListFunction()
	{

	}
	void AcceptClient(const SOCKET& _sSocket, unsigned short _usID);
	void PacketProcess(unsigned short _usID, char* _packet);
	void RecvPacket(unsigned short _usID, DWORD _dwSize, char* _Packet);

	void PlayerLogin(unsigned short _usID, char* _packet);
	void PlayerLoginCheck(unsigned short _usID, char* _packet);
	void PlayerLogin(DataBase_Event& _event);
	void PlayerDinconnect(unsigned short _usID);
	void PlayerPos(unsigned short _usID, char* _Packet);
	void PlayerLook(unsigned short _usID, char* _Packet);
};

