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

void CPacketMgr::Send_Packet(unsigned int _usID, void* _packet)
{
	DWORD flags{ 0 };
	char* packet = reinterpret_cast<char*>(_packet);
	int packet_size = packet[0];
	int packet_type = packet[1];

	cout << "패킷 사이즈:" << packet_size << "패킷 타입:" << packet_type << endl;

	OVER_EX* send_over = new OVER_EX;
	memset(send_over, 0x00, sizeof(OVER_EX));
	send_over->m_Event = EV_SEND;
	memcpy(send_over->m_MessageBuffer, packet, packet_size);
	send_over->m_DataBuffer.buf = send_over->m_MessageBuffer;
	send_over->m_DataBuffer.len = packet_size;

	int retval = WSASend(CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->GetPlayerSocket(), 
		&send_over->m_DataBuffer, 1, NULL, flags, &send_over->m_Overlapped, NULL);

	if (retval == SOCKET_ERROR) {
		int err_no = WSAGetLastError();
		if (err_no != WSA_IO_PENDING) {
			CNetwork::Err_display("SEND EVENT SOCKET_ERR - ", err_no);
		}
	}
}

void CPacketMgr::Send_Login_IP_Packet(unsigned int _usID)
{
	sc_login_ip_packet packet;
	packet.size = sizeof(packet);
	packet.type = SC_LOGIN_IP;
	packet.id = _usID;	
	Send_Packet(_usID, &packet);
}

void CPacketMgr::Send_Login_OK_Packet(unsigned int _usID)
{
	sc_login_ok_packet packet;
	packet.size = sizeof(packet);
	packet.type = SC_LOGIN_OK;
	packet.id = _usID;
	Send_Packet(_usID, &packet);
}

void CPacketMgr::Send_Login_Fail_Packet(unsigned int _usID)
{
	sc_login_fail_packet packet;
	packet.size = sizeof(packet);
	packet.type = SC_LOGIN_FAIL;
	Send_Packet(_usID, &packet);
}

void CPacketMgr::Send_Message_Packet(unsigned int _usID, char* _message)
{
	sc_chat_packet packet;
	packet.size = sizeof(packet);
	packet.type = SC_CHAT;
	strcpy_s(packet.meesage, _message);
	Send_Packet(_usID, &packet);
}

void CPacketMgr::Send_First_Status_Packet(unsigned int _usID)
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

	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetHP(packet.HP);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetHunger(packet.Hunger);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetStamina(packet.Stamina);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetThirst(packet.Thirst);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetPos(packet.fPosX, packet.fPosY, packet.fPosZ);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetDir(packet.fDirX, packet.fDirY, packet.fDirZ);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetPlayerConnect(true);
}

void CPacketMgr::Send_Accept_Packet(unsigned int _usID, concurrent_unordered_set<unsigned int>& _ViewList)
{
	DirectX::XMFLOAT3 pos = CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->GetPos();
	DirectX::XMFLOAT3 dir = CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->GetDir();

	sc_accept_packet packet;
	packet.size = sizeof(sc_first_status_packet);
	packet.type = SC_CONNECT;
	packet.id = _usID;

	packet.fPosX = pos.x;
	packet.fPosY = pos.y;
	packet.fPosZ = pos.z;

	packet.fDirX = dir.x;
	packet.fDirY = dir.y;
	packet.fDirZ = dir.z;

	packet.HP = CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->GetHP();
	//Send_Packet(_usID, &packet);

	// 접속하여 있는 플레이어들에게 현재 플레이어가 들어왔음에 ViewList에 추가시킨다.
	for (auto& au : _ViewList)
	{
		if (au == _usID) 
			continue;
		Send_Packet(au, &packet);
	}
}

void CPacketMgr::Send_Status_Packet(unsigned int _usPlayer, unsigned int _usOther)
{
	DirectX::XMFLOAT3 pos = CProcess::m_pPlayerPool->m_cumPlayerPool[_usPlayer]->GetPos();
	DirectX::XMFLOAT3 dir = CProcess::m_pPlayerPool->m_cumPlayerPool[_usPlayer]->GetDir();

	sc_accept_packet packet;
	packet.size = sizeof(sc_accept_packet);
	packet.type = SC_CONNECT;

	packet.HP = CProcess::m_pPlayerPool->m_cumPlayerPool[_usPlayer]->GetHP();
	packet.fPosX = pos.x;
	packet.fPosY = pos.y;
	packet.fPosZ = pos.z;
	packet.fDirX = dir.x;
	packet.fDirY = dir.y;
	packet.fDirZ = dir.z;

	CPacketMgr::Send_Packet(_usOther, &packet);
}

void CPacketMgr::Send_Pos_Packet(unsigned int _usPlayer, unsigned int _usOther)
{
	concurrency::concurrent_unordered_set<unsigned int> viewlist;
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usPlayer]->CopyPlayerList(viewlist);
	
	DirectX::XMFLOAT3 pos = CProcess::m_pPlayerPool->m_cumPlayerPool[_usPlayer]->GetPos();
	DirectX::XMFLOAT3 dir = CProcess::m_pPlayerPool->m_cumPlayerPool[_usPlayer]->GetDir();

	sc_pos_packet packet;
	packet.size = sizeof(sc_pos_packet);
	packet.type = SC_POS;
	packet.id = _usPlayer;
	packet.fPosX = pos.x;
	packet.fPosY = pos.y;
	packet.fPosZ = pos.z;

	packet.fDirX = dir.x;
	packet.fDirY = dir.y;
	packet.fDirZ = dir.z;
	CPacketMgr::Send_Packet(_usOther, &packet);
}

void CPacketMgr::Send_Look_Packet(unsigned int _usPlayer)
{
	concurrent_unordered_set<unsigned int> viewlist;
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usPlayer]->CopyPlayerList(viewlist);
}

void CPacketMgr::Send_Remove_Packet(unsigned int _usPlayer, unsigned int _usOther)
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

void CPacketMgr::Send_Disconnect_Packet(unsigned int _usID)
{
	concurrent_unordered_set<unsigned int> list;
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->CopyPlayerList(list);

	// 패킷 보내기
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

void CPacketMgr::Send_Chat_Packet(unsigned int _usID, unsigned int _usOther, char message[])
{
	concurrent_unordered_set<unsigned int> viewlist;
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->CopyPlayerList(viewlist);

	sc_chat_packet packet;

	packet.size = sizeof(sc_chat_packet);
	packet.type = SC_CHAT;
	strcpy_s(packet.meesage, message);

	for (auto& au : viewlist) {
		if (au == _usID) continue;
		if (CProcess::m_pPlayerPool->m_cumPlayerPool[au]->CheckList(_usID)) {
				CPacketMgr::Send_Packet(au, &packet);
		}
	}

}
