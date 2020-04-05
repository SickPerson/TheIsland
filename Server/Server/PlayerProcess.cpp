#include "stdafx.h"
#include "Process.h"
#include "PlayerProcess.h"
#include "Network.h"
#include "PacketMgr.h"


CPlayerProcess::CPlayerProcess()
{
	if (!CProcess::m_pPlayerPool)
		CProcess::m_pPlayerPool = new class CPlayerpool();
}


CPlayerProcess::~CPlayerProcess()
{
}

void CPlayerProcess::AcceptClient(const SOCKET & _sSocket, unsigned short _usID)
{
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetPlayerSocket(_sSocket);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetPlayerID(_usID);

	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetRecvState();
}

void CPlayerProcess::PacketProcess(unsigned short _usID, char * _packet)
{
	m_fppacketProcess[_packet[1]](_usID, _packet);
}

void CPlayerProcess::RecvPacket(unsigned short _usID, DWORD _dwSize, char * _Packet)
{
	char* packet = CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->RecvEvent(_dwSize, _Packet);

	if (packet != nullptr)
	{
		m_fppacketProcess[packet[1]](_usID, packet);
		CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetRecvState();
	}
	return;
}

void CPlayerProcess::PlayerLogin(DataBase_Event& _event)
{
	sc_login_state_packet packet;
	packet.size = sizeof(sc_login_state_packet);
	packet.type = SC_LOGIN_OK;
	PacketMgr::Send_Packet(_event.client_num, &packet);

	concurrency::concurrent_unordered_set<unsigned short>viewlist;

	InsertList(_event.client_num);
}

void CPlayerProcess::PlayerLogout(unsigned short _usID)
{
}

void CPlayerProcess::PlayerDinconnect(unsigned short _usID)
{
}

void CPlayerProcess::PlayerPos(unsigned short _usID, char * _Packet)
{
}

void CPlayerProcess::PlayerLook(unsigned short _usID, char * _Packet)
{
}
