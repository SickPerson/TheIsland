#include "stdafx.h"
#include "Network.h"


CNetwork::CNetwork()
{
	if (!Init())
		printf("Network Init Fail\n");
	else
		printf("Network Init Success\n");
}


CNetwork::~CNetwork()
{
}

bool CNetwork::Init()
{
	WSADATA	wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	m_bLoginSuccess = false;

	m_sGameSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);
	
	m_wbSendWsaBuf.buf = m_cSendBuf;
	m_wbSendWsaBuf.len = 256;
	
	m_wbRecvWsaBuf.buf = m_cRecvBuf;
	m_wbSendWsaBuf.len = 256;

	return ConnectServer();
}

bool CNetwork::ConnectServer()
{
	printf("IP :");
	std::cin >> SERVERIP;
	SOCKADDR_IN	serveraddr;
	ZeroMemory(&serveraddr, sizeof(SOCKADDR_IN));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVER_PORT);
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	
	int retval = WSAConnect(m_sGameSocket, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr), NULL, NULL, NULL, NULL);
	if (retval == SOCKET_ERROR) {
		printf("socket err");
		while (true)
		{
			volatile bool reconnect = ConnectServer();
			if (reconnect)
				break;
		}
	}
	else 
	{
		printf("¿¬°á\n");
		if (CreateEventSelect())
			return true;
		else
			return false;
	}
	return true;
}

bool CNetwork::CreateEventSelect()
{
	int retval;
	m_hEvent = WSACreateEvent();

	retval = WSAEventSelect(m_sGameSocket, m_hEvent, FD_READ || FD_CLOSE);
	if (retval != SOCKET_ERROR)
		return true;
	else {
		retval = WSAGetLastError();
		return false;
	}
	return false;
}

void CNetwork::DisConnect()
{
	closesocket(m_sGameSocket);
	WSACloseEvent(m_hEvent);
	WSACleanup();
	printf("DisConnect Server\n");
}

void CNetwork::RecvPacket()
{
	printf("Network Run\n");
	while (!m_bClientClose)
	{
		m_iIndex = WSAWaitForMultipleEvents(1, &m_hEvent, FALSE, WSA_INFINITE, FALSE);
		if ((m_iIndex == WSA_WAIT_FAILED) || (m_iIndex == WSA_WAIT_TIMEOUT))
		{
			break;
		}
		int retval = WSAEnumNetworkEvents(m_sGameSocket, m_hEvent, &m_weEvent);
		if (retval == SOCKET_ERROR)
			break;
		
		if (m_weEvent.lNetworkEvents & FD_READ)
		{
			if (m_weEvent.iErrorCode[FD_READ_BIT] != 0)
			{
				break;
			}

			DWORD	size, flag = 0;

			retval = WSARecv(m_sGameSocket, &m_wbRecvWsaBuf, 1, &size, &flag, NULL, NULL);
			if (retval == SOCKET_ERROR)
				break;

			char* packet = reinterpret_cast<char*>(m_cRecvBuf);

			while (size != 0)
			{
				if (m_dwPacketSize == 0) m_dwPacketSize = packet[0];
				if (size + m_dwSavePacketSize >= m_dwPacketSize)
				{
					memcpy(m_cPacketBuf + m_dwSavePacketSize, packet, m_dwPacketSize - m_dwSavePacketSize);
					ProcessPacket(m_cPacketBuf);
					packet += m_dwPacketSize - m_dwSavePacketSize;
					size -= m_dwPacketSize - m_dwSavePacketSize;
					m_dwPacketSize = 0;
					m_dwSavePacketSize = 0;
				}
				else
				{
					memcpy(m_cPacketBuf + m_dwSavePacketSize, packet, size);
					m_dwSavePacketSize += size;
					size = 0;
				}
			}
		}
		if (m_weEvent.lNetworkEvents & FD_CLOSE)
		{
			m_bClientClose = true;
			break;
		}
	}
}

void CNetwork::ProcessPacket(char * _packet)
{
	switch (_packet[1])
	{
	case SC_LOGIN_OK:
	{
		break;
	}
	case SC_LOGIN_FAIL:
	{
		break;
	}
	}
}
