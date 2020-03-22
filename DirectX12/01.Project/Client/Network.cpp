#include "stdafx.h"
#include "Network.h"

CNetwork::CNetwork()
{
	if (!Init())
		cout << "Network Init Fail" << endl;
	else
		cout << "Network Init Success" << endl;
}

CNetwork::~CNetwork()
{
}

void CNetwork::Err_quit(const char * msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

void CNetwork::Err_display(const char * msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	cout << "[ " << msg << " ]";
	wcout << L"에러 " << lpMsgBuf << endl;
	while (true);
	LocalFree(lpMsgBuf);
}

bool CNetwork::Init()
{
	for (auto i = 0; i < LT_END; ++i)
		m_bLoginState[i] = false;
	m_bPushKey = false;
	m_bClientClose = false;
	m_bCollision = false;

	m_cusViewList.clear();

	WSADATA	wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	m_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	m_SendWsaBuf.buf = m_cRecvbuf;
	m_SendWsaBuf.len = BUF_SIZE;

	m_RecvWsaBuf.buf = m_cSendBuf;
	m_RecvWsaBuf.len = BUF_SIZE;

	return ConnectServer();
}

bool CNetwork::ConnectServer()
{
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(SOCKADDR_IN));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVER_PORT);
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	int retval = WSAConnect(m_sock, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr), NULL, NULL, NULL, NULL);
	if (retval == SOCKET_ERROR) {
		int err_no = WSAGetLastError();
		Err_display("socket err", err_no);
		while (1) {
			cout << "IP :";
			cin >> SERVERIP;
			volatile bool reconnect = ConnectServer();
			if (reconnect)
				break;
		}
	}
	else {
		cout << "연결" << endl;
		if (CreateEventSelect()) {
			return true;
		}
		else {
			return false;
		}
	}

	return true;
}

bool CNetwork::CreateEventSelect()
{
	m_hEvent = WSACreateEvent();

	int retval = WSAEventSelect(m_sock, m_hEvent, FD_READ || FD_CLOSE);
	if (retval != SOCKET_ERROR)
	{
		cout << "CreateSelectEvent Success" << endl;
		return true;
	}
	else
	{
		int err_no = WSAGetLastError();
		Err_display("CreateSelectEvent err", err_no);
		return false;
	}
}

void CNetwork::DisConnect()
{
	closesocket(m_sock);
	WSACloseEvent(m_hEvent);
	WSACleanup();
	cout << "DisConnect" << endl;
}

void CNetwork::RecvPacket()
{
	cout << "Network Run" << endl;

	while (!m_bClientClose)
	{
		m_iIndex = WSAWaitForMultipleEvents(1, &m_hEvent, FALSE, WSA_INFINITE, FALSE);

		if ((m_iIndex == WSA_WAIT_FAILED) || (m_iIndex == WSA_WAIT_TIMEOUT))
		{
			int err_no = WSAGetLastError();
			Err_display("WSAWaitForMultipleEvents Err", err_no);
			break;
		}
		int retval = WSAEnumNetworkEvents(m_sock, m_hEvent, &m_weEvent);
		if (retval == SOCKET_ERROR)
		{
			int err_no = WSAGetLastError();
			Err_display("WSAEnumNetworkEvents Err", err_no);
			break;
		}

		if (m_weEvent.lNetworkEvents & FD_READ)
		{
			if (m_weEvent.iErrorCode[FD_READ_BIT] != 0)
			{
				int err_no = GetLastError();
				Err_display("m_weEvent.iErrorCode Err", err_no);
				break;
			}
			DWORD	size, flag = 0;
			int ret = WSARecv(m_sock, &m_RecvWsaBuf, 1, &size, &flag, NULL, NULL);
			if (ret == SOCKET_ERROR)
			{
				int err_no = GetLastError();
				Err_display("WSARecv SOCKET_ERROR", err_no);
				break;
			}

			char* packet = reinterpret_cast<char*>(m_cRecvbuf);

			while (size != 0)
			{
				if (0 == m_in_packet_size) m_in_packet_size = packet[0];
				if (size + m_saved_packet_size >= m_in_packet_size)
				{
					memcpy(m_cpacket_buffer + m_saved_packet_size, packet, m_in_packet_size - m_saved_packet_size);
					ProcessPacket(m_cpacket_buffer);
					packet += m_in_packet_size - m_saved_packet_size;
					size -= m_in_packet_size - m_saved_packet_size;
					m_in_packet_size = 0;
					m_saved_packet_size = 0;
				}
				else
				{
					memcpy(m_cpacket_buffer + m_saved_packet_size, packet, size);
					m_saved_packet_size += size;
					size = 0;
				}
			}
		}
		if (m_weEvent.lNetworkEvents & FD_CLOSE)
		{
			cout << "FD_CLOSE" << endl;
			m_bClientClose = true;
			break;
		}
	}
	cout << "Network Close" << endl;
}

void CNetwork::ProcessPacket(char* _packet)
{
	switch (_packet[1])
	{
	case SC_LOGIN_OK:
		break;
	case SC_LOGIN_FAIL:
		break;
	}
}

void CNetwork::SendLoginPacket(wchar_t * _wcPlayerID, wchar_t * _wcPlayerPW)
{
	DWORD size, flag = 0;

	cs_login_packet* packet = reinterpret_cast<cs_login_packet*>(m_cSendBuf);

	packet->size = sizeof(cs_login_packet);
	packet->type = CS_LOGIN;

	wcsncpy_s(packet->player_id, wcslen(packet->player_id), _wcPlayerID, wcslen(_wcPlayerID));
	wcsncpy_s(packet->player_pw, wcslen(packet->player_pw), _wcPlayerPW, wcslen(_wcPlayerPW));

	m_SendWsaBuf.len = sizeof(cs_login_packet);

	int retval = WSASend(m_sock, &m_SendWsaBuf, 1, &size, flag, NULL, NULL);

	if (retval != 0)
	{
		int err_no = WSAGetLastError();
		Err_display("Err while sending packet - ", err_no);
	}
}
