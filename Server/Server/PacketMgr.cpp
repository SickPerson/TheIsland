#include "PacketMgr.h"
#include "Process.h"
#include "DataBase.h"
#include "Network.h"
#include "Player.h"

CPacketMgr::CPacketMgr()
{	
}

CPacketMgr::~CPacketMgr()
{
}

void CPacketMgr::Send_Packet(const unsigned short& _usID, void* _packet)
{
	char* packet = reinterpret_cast<char*>(_packet);
	int packet_size = packet[0];

	OVER_EX* send_over = new OVER_EX;
	memset(send_over, 0x00, sizeof(OVER_EX));
	send_over->m_Event = EV_SEND;
	memcpy(send_over->m_MessageBuffer, packet, packet_size);
	send_over->m_DataBuffer.buf = reinterpret_cast<char*>(send_over->m_MessageBuffer);
	send_over->m_DataBuffer.len = packet_size;

	int retval = WSASend(CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->GetPlayerSocket(), 
		&send_over->m_DataBuffer, 1, NULL, 0, &send_over->m_Overlapped, NULL);

	if (retval == SOCKET_ERROR) {
		int err_no = WSAGetLastError();
		if (err_no != WSA_IO_PENDING) {
			CNetwork::Err_display("SEND EVENT SOCKET_ERR - ", err_no);
		}
	}
}

void CPacketMgr::Send_Login_OK_Packet(unsigned short _usID)
{
	sc_login_ok_packet packet;
	packet.size = sizeof(packet);
	packet.type = SC_LOGIN_OK;
	Send_Packet(_usID, &packet);
}

void CPacketMgr::Send_Login_Fail_Packet(unsigned short _usID)
{
	sc_login_fail_packet packet;
	packet.size = sizeof(packet);
	packet.type = SC_LOGIN_FAIL;
	Send_Packet(_usID, &packet);
}

void CPacketMgr::Send_Message_Packet(unsigned short _usID, wchar_t * _message)
{
	sc_chat_packet packet;
	packet.size = sizeof(packet);
	packet.type = SC_CHAT;
	lstrcpyW(packet.meesage, _message);
	Send_Packet(_usID, &packet);
}

void CPacketMgr::Send_First_Status_Packet(unsigned short _usID)
{
	sc_first_status_packet packet;
	packet.size = sizeof(sc_first_status_packet);
	packet.type = SC_LOGIN;
	packet.id = _usID;
	// No DataBase
	packet.HP = 100;
	packet.Hunger = 100;
	packet.Stamina = 100;
	packet.Thirst = 100;

	packet.fPosX = 0.f;
	packet.fPosY = 25.f;
	packet.fPosZ = 0.f;

	packet.fDirX = 0.f;
	packet.fDirY = 0.f;
	packet.fDirZ = 0.f;

	Send_Packet(_usID, &packet);
}

void CPacketMgr::Send_Accept_Packet(unsigned short _usID, concurrent_unordered_set<unsigned short>& _ViewList)
{
	sc_first_status_packet	packet;
	packet.size = sizeof(sc_first_status_packet);
	packet.type = SC_CONNECT;
	packet.id = _usID;
	//Send_Packet(_usID, &packet);

	// 접속하여 있는 플레이어들에게 현재 플레이어가 들어왔음에 ViewList에 추가시킨다.
	for (auto& au : _ViewList)
	{
		if (au == _usID) 
			continue;
		Send_Packet(au, &packet);
	}
}

void CPacketMgr::Send_Status_Packet(unsigned short _usPlayer, unsigned short _usOther)
{
	DirectX::XMFLOAT3 pos = CProcess::m_pPlayerPool->m_cumPlayerPool[_usPlayer]->GetPos();
	DirectX::XMFLOAT3 dir = CProcess::m_pPlayerPool->m_cumPlayerPool[_usPlayer]->GetDir();
}

void CPacketMgr::Send_Pos_Packet(unsigned short _usPlayer, unsigned short _usOther)
{
}

void CPacketMgr::Send_Look_Packet(unsigned short _usPlayer)
{
}

void CPacketMgr::Send_Remove_Packet(unsigned short _usPlayer, unsigned short _usOther)
{
	if (_usPlayer != _usOther)
	{
		sc_disconnect_packet packet;
		ZeroMemory(&packet, sizeof(sc_disconnect_packet));
		packet.size = sizeof(sc_disconnect_packet);
		packet.type = SC_DISCONNECT;
		packet.id = _usPlayer;
		Send_Packet(_usOther, &packet);
	}
}

void CPacketMgr::Send_Disconnect_Packet(unsigned short _usID)
{
	concurrent_unordered_set<unsigned short> list;
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->CopyPlayerList(list);

	sc_disconnect_packet	packet;
	packet.size = sizeof(sc_disconnect_packet);
	packet.type = SC_DISCONNECT;
	packet.id = _usID;
	//Send_Packet(_usID, &packet);

	for (auto& au : list)
	{
		if (au == _usID)
			continue;
		if (CProcess::m_pPlayerPool->m_cumPlayerPool[au]->CheckList(_usID)) {
			CProcess::m_pPlayerPool->m_cumPlayerPool[au]->DeleteList(_usID);
			if (CProcess::m_pPlayerPool->m_cumPlayerPool[au]->GetPlayerConnect())
				CPacketMgr::Send_Packet(au, &packet);
		}
	}
	//closesocket(CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->GetPlayerSocket());
	//CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->Init();
}
