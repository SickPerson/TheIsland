#include "stdafx.h"
#include "Network.h"
#include "LoginScene.h"
#include "IngameScene.h"

unsigned short CNetwork::m_usID = 0;

CNetwork::CNetwork()
{
	Init();
	ConnectServer();
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
	wcout << L"���� " << lpMsgBuf << endl;
	while (true);
	LocalFree(lpMsgBuf);
}

void CNetwork::Init()
{
	for (auto i = 0; i < LT_END; ++i)
		m_bLoginState[i] = false;
	m_bPushKey = false;
	m_bClientClose = false;
	m_bCollision = false;

	m_cusViewList.clear();
	//---------------------------------------------------
	WSADATA	wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	m_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	m_SendWsaBuf.buf = m_cRecvbuf;
	m_SendWsaBuf.len = BUF_SIZE;

	m_RecvWsaBuf.buf = m_cSendBuf;
	m_RecvWsaBuf.len = BUF_SIZE;
}

bool CNetwork::ConnectServer()
{
	//sc_login_ok_packet* packet = reinterpret_cast<sc_login_ok_packet*>(_packet);

	char IP[15] = "127.0.0.1";
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(SOCKADDR_IN));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVER_PORT);
	serveraddr.sin_addr.s_addr = inet_addr(IP);
	int retval = WSAConnect(m_sock, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr), NULL, NULL, NULL, NULL);
	if (retval == SOCKET_ERROR) {
		int err_no = WSAGetLastError();
		Err_display("socket err", err_no);
		return false;
	}
	// IP �ּҷ� ����Ǿ��� ���
	if (CreateEventSelect())
		return true;
	else
		return false;
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
	while (!m_bClientClose)
	{
		cout << "Network Run" << endl;
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
	//cout << "Network Close" << endl;
}

void CNetwork::ProcessPacket(char* _packet)
{
	switch (_packet[1])
	{
	case SC_LOGIN_OK: // �α����� �����Ѵٸ� ������ ������ �õ��մϴ�.
		cout << "IP ��ġ" << endl;
		SetLogin(true);

		break;
	case SC_LOGIN_FAIL: // �α����� �����Ѵٸ� ������ ������ ���� �ʽ��ϴ�.
		cout << "IP ����ġ" << endl;
		SetLogin(false);
		break;
	case SC_LOGIN:
		break;
	}
}

// �α��� ��Ŷ�� ������ �����ϴ�. string���� char*���� �ٲܿ����Դϴ�.
void CNetwork::SendLoginPacket(string _sPlayerID, string _sIP)
{
	DWORD size, flag = 0;

	cs_login_packet* packet = reinterpret_cast<cs_login_packet*>(m_cSendBuf);

	packet->size = sizeof(cs_login_packet);
	packet->type = CS_LOGIN;
	strcpy(packet->player_id, _sPlayerID.c_str());
	strcpy(packet->player_ip, _sIP.c_str());

	m_SendWsaBuf.len = sizeof(cs_login_packet);

	int retval = WSASend(m_sock, &m_SendWsaBuf, 1, &size, flag, NULL, NULL);

	if (retval != 0)
	{
		int err_no = WSAGetLastError();
		Err_display("Err while sending packet - ", err_no);
	}
}

void CNetwork::RecvLoginPacket(char * _packet)
{
	sc_first_status_packet* packet = reinterpret_cast<sc_first_status_packet*>(_packet);
	m_usID = packet->id;

	CIngameScene::m_cumPlayer[m_usID];
}
