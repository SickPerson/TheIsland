#include "PacketMgr.h"

DEFINITION_SINGLE(PacketMgr)

PacketMgr::PacketMgr()
{	
}


PacketMgr::~PacketMgr()
{
}

void PacketMgr::Send_Packet(const unsigned short & id, void * buf)
{
	unsigned char* packet = reinterpret_cast<unsigned char*>(buf);
	int packet_size = packet[0];

	OVER_EX* send_over = new OVER_EX;
	memset(send_over, 0x00, sizeof(OVER_EX));
	send_over->m_Event = EV_SEND;
	memcpy(send_over->m_MessageBuffer, packet, packet_size);
	send_over->m_DataBuffer.buf = reinterpret_cast<char*>(send_over->m_MessageBuffer);
	send_over->m_DataBuffer.len = packet_size;

	if (WSASend(client_s, &send_over->m_DataBuffer, 1, 0, 0, &send_over->m_Overlapped, 0) == SOCKET_ERROR) {
		if (WSAGetLastError() != WSA_IO_PENDING) {
			cout << "Error - Fail WSASend(error_code : " << WSAGetLastError << " ) " << endl;
		}
	}
}

void PacketMgr::Send_Login_Ok_Packet(const int & id)
{
	sc_packet_login_ok packet;
	packet.id = id;
	packet.size = sizeof(packet);
	packet.type = SC_LOGIN_OK;
	Send_Packet(id, &packet);
}
