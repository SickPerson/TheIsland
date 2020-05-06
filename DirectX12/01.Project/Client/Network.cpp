#include "stdafx.h"
#include "Network.h"
#include "LoginScene.h"
#include "IngameScene.h"

#include <Engine/Device.h>

#include <Engine/Ptr.h>

#include <Engine/Texture.h>

#include <Engine/ResMgr.h>
#include <Engine/CollisionMgr.h>
#include <Engine/FontMgr.h>

#include <Engine/Layer.h>
#include <Engine/Scene.h>

#include <Engine/GameObject.h>
#include <Engine/MeshRender.h>
#include <Engine/Transform.h>
#include <Engine/Camera.h>
#include <Engine/Light3D.h>
#include <Engine/LandScape.h>
#include <Engine/GridScript.h>

#include <Engine/PlayerScript.h>
#include <Engine/FPSCamScript.h>
#include <Engine/MonsterScript.h>
#include <Engine/StatusScript.h>
#include <Engine/QuickSlotScript.h>

unsigned short CNetwork::m_usID = 0;

CNetwork::CNetwork()
{
	Init();
	//ConnectServer();
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

void CNetwork::Init()
{
	for (auto i = 0; i < LT_END; ++i)
		m_bLoginState[i] = false;
	m_bPushKey = false;
	m_bClientClose = false;
	m_bCollision = false;

	m_cusViewList.clear();
}

bool CNetwork::ConnectServer(string ipAddr)
{
	char IP[15];
	strcpy_s(IP, ipAddr.c_str());

	WSADATA	wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	m_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	m_SendWsaBuf.buf = m_cRecvbuf;
	m_SendWsaBuf.len = BUF_SIZE;

	m_RecvWsaBuf.buf = m_cSendBuf;
	m_RecvWsaBuf.len = BUF_SIZE;

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(SOCKADDR_IN));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVER_PORT);
	serveraddr.sin_addr.s_addr = inet_addr(IP);
	int retval = WSAConnect(m_sock, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr), NULL, NULL, NULL, NULL);
	if (retval == SOCKET_ERROR) {
		int err_no = WSAGetLastError();
		// IP가 틀렸을 경우
		Err_display("socket err", err_no);
		return false;
	}
	
	// IP 주소로 연결되었을 경우
	if (CreateEventSelect()) {
		RunRecvThread();
		return true;
	}
	else
		return false;
}

void CNetwork::RunRecvThread()
{
	// Network Run
	m_tNetworkThread = make_shared<thread>([]() {CNetwork::GetInst()->RecvPacket(); });

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

			BYTE* packet = reinterpret_cast<BYTE*>(m_cRecvbuf);

			cout << "SIZE:" << size << endl;

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
	cout << "패킷 타입: "<< (int)_packet[1] << endl;
	switch (_packet[1])
	{
	case SC_LOGIN_OK: // 로그인이 성공한다면 서버에 연결을 시도합니다.
	{
		//cout << "ID 일치" << endl;
		cout << "여기 오니? " << endl;
		SetLogin(true);
		break;
	}
	case SC_LOGIN_FAIL: // 로그인이 실패한다면 서버에 연결이 되지 않습니다.
		//cout << "ID 불일치" << endl;
		SetLogin(false);
		break;
	case SC_LOGIN:
	{
		sc_first_status_packet* packet = reinterpret_cast<sc_first_status_packet*>(_packet);
		m_usID = packet->id;

		// 리스트에 넣기
		InsertList(m_usID);
		SetLoopStart(true);
		break;
	}
	case SC_CONNECT:
	{
		sc_accept_packet* packet = reinterpret_cast<sc_accept_packet*>(_packet);

		if (!CheckList(packet->id)) {
			InsertList(packet->id);
		}
		break;
	}
	case SC_POS:
	{
		sc_pos_packet* packet = reinterpret_cast<sc_pos_packet*>(_packet);

		break;
	}
	case SC_DISCONNECT:
	{
		sc_disconnect_packet* packet = reinterpret_cast<sc_disconnect_packet*>(_packet);
		DeleteList(packet->id);
		break;
	}
	}
}

void CNetwork::SetID(unsigned short _id)
{
	m_usID = _id;
}

unsigned short CNetwork::GetID()
{
	return m_usID;
}

// 로그인 패킷을 서버로 보냅니다. string에서 char*으로 바꿀예정입니다.
void CNetwork::SendLoginPacket(string _sPlayerID)
{
	DWORD size, flag = 0;

	cs_login_packet* packet = reinterpret_cast<cs_login_packet*>(m_cSendBuf);

	packet->size = sizeof(cs_login_packet);
	packet->type = CS_LOGIN;
	packet->id = m_usID;
	strcpy(packet->player_id, _sPlayerID.c_str());

	m_SendWsaBuf.len = sizeof(cs_login_packet);

	int retval = WSASend(m_sock, &m_SendWsaBuf, 1, &size, flag, NULL, NULL);

	if (retval != 0)
	{
		int err_no = WSAGetLastError();
		Err_display("Err while sending packet - ", err_no);
	}
}

void CNetwork::SendPosPacket()
{
}

void CNetwork::SendDirPacket()
{
}

void CNetwork::RecvIpPacket(char * _packet)
{
	sc_login_ip_packet* packet = reinterpret_cast<sc_login_ip_packet*>(_packet);
	m_usID = packet->id;
}

void CNetwork::RecvLoginPacket(char * _packet)
{

	sc_first_status_packet* packet = reinterpret_cast<sc_first_status_packet*>(_packet);
	m_usID = packet->id;
}
