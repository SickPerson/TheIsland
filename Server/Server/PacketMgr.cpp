#include "stdafx.h"
#include "PacketMgr.h"
#include "DataBase.h"
#include "Network.h"

PacketMgr::PacketMgr()
{	
}

PacketMgr::~PacketMgr()
{
}

void PacketMgr::Send_Packet(const unsigned short& _usID, void* _packet)
{
	unsigned char* packet = reinterpret_cast<unsigned char*>(_packet);
	int packet_size = packet[0];

	OVER_EX* send_over = new OVER_EX;
	memset(send_over, 0x00, sizeof(OVER_EX));
	send_over->m_Event = EV_SEND;
	memcpy(send_over->m_MessageBuffer, packet, packet_size);
	send_over->m_DataBuffer.buf = reinterpret_cast<char*>(send_over->m_MessageBuffer);
	send_over->m_DataBuffer.len = packet_size;

	/*if (WSASend(client_s, &send_over->m_DataBuffer, 1, 0, 0, &send_over->m_Overlapped, 0) == SOCKET_ERROR) {
		if (WSAGetLastError() != WSA_IO_PENDING) {
			cout << "Error - Fail WSASend(error_code : " << WSAGetLastError << " ) " << endl;
		}
	}*/
}

void PacketMgr::Send_Login_Packet(unsigned short _usID)
{
	sc_packet_login_ok packet;
	packet.id = _usID;
	packet.size = sizeof(packet);
	packet.type = SC_LOGIN_OK;
	Send_Packet(_usID, &packet);
}

void PacketMgr::Send_Accept_Packet(concurrency::concurrent_unordered_set<unsigned short>& _ViewList, unsigned short _usID)
{
	sc_packet_accept	SendPacket;
	SendPacket.size = sizeof(sc_packet_accept);
	SendPacket.type = SC_CONNECT;
	SendPacket.usID = _usID;

	for (auto& au : _ViewList)
	{
		if (au == _usID) continue;
		Send_Packet(au, &SendPacket);
	}
}

void PacketMgr::Send_Status_Packet(unsigned short _usPlayer, unsigned short _usOther)
{
}

void PacketMgr::Send_Pos_Packet(unsigned short _usPlayer, unsigned short _usOther)
{
}

void PacketMgr::Send_Look_Packet(unsigned short _usPlayer)
{
}

void PacketMgr::Send_Remove_Packet(unsigned short _usPlayer, unsigned short _usOther)
{
}

void PacketMgr::Send_Disconnect_Packet(unsigned short _usPlayer)
{
}
