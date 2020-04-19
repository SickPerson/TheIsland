#include "Player.h"
#include "Network.h"


CPlayer::CPlayer()
{
	Init();
}


CPlayer::~CPlayer()
{
	m_cusViewList.clear();
}

void CPlayer::Init()
{
	SetPlayerSocket(INVALID_SOCKET);
	SetPlayerConnect(false);
	m_cusViewList.clear();
	m_iCursize = 0;
	m_iPrevsize = 0;

	ZeroMemory(&m_over, sizeof(OVER_EX));

	m_over.m_Event = EV_RECV;
	m_over.m_DataBuffer.buf = reinterpret_cast<char*>(m_over.m_DataBuffer.buf);
	m_over.m_DataBuffer.len = sizeof(m_over.m_DataBuffer.buf);
}

void CPlayer::SetRecvState()
{
	DWORD flag{ 0 };

	ZeroMemory(&m_over.m_Overlapped, sizeof(WSAOVERLAPPED));

	int retval = WSARecv(m_sSocket, &m_over.m_DataBuffer, 1, NULL, &flag, &m_over.m_Overlapped, NULL);

	if (0 != retval)
	{
		int err_no = WSAGetLastError();
		if (WSA_IO_PENDING != err_no)
			CNetwork::Err_display("Set Recv State - ", err_no);
	}
}

char* CPlayer::RecvEvent(DWORD data_size, char * _packet)
{

	int rest_size = data_size;
	char* packet = _packet;
	int curr = m_iCursize;
	int prev = m_iPrevsize;
	char* packetBuf = m_over.m_MessageBuffer;

	while (0 < rest_size)
	{
		int packet_size;
		if (0 != curr)
			packet_size = curr;
		else
		{
			packet_size = packet[0];
			curr = packet_size;
		}
		int need_size = packet_size - prev;
		if (need_size <= rest_size)
		{
			memcpy(packetBuf + prev, packet, need_size);
			prev = 0;
			curr = 0;
			rest_size -= need_size;
			packet += need_size;
			return packet;
		}
		else
		{
			memcpy(packetBuf + prev, packet, rest_size);
			prev += rest_size;
			rest_size = -rest_size;
			packet += rest_size;
		}
	}
	return nullptr;
}
