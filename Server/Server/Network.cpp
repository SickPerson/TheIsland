#include "stdafx.h"
#include "Network.h"

CNetwork::CNetwork()
{
	m_ListenSock = INVALID_SOCKET;
	m_bRunningServer = true;
	m_pPlayerProcess = nullptr;
	m_usUserID = 0;
	Initialize();
	GetServerIpAddress();
}

CNetwork::~CNetwork()
{
}

void CNetwork::GetServerIpAddress()
{
	PHOSTENT	hostinfo;
	char		hostname[50];
	char		ipaddr[50];
	memset(hostname, 0, sizeof(hostname));
	memset(ipaddr, 0, sizeof(ipaddr));

	int err_no = gethostname(hostname, sizeof(hostname));
	if (err_no == 0) {
		hostinfo = gethostbyname(hostname);
		strcpy_s(ipaddr, inet_ntoa(*reinterpret_cast<struct in_addr*>(hostinfo->h_addr_list[0])));
	}
	WSACleanup();
	cout << "Server IP Address: " << ipaddr << endl;
}

bool CNetwork::InitWinSock()
{
	// WinSock Initalize
	WSADATA	wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;
	else
		return true;
}

bool CNetwork::InitCompletionPort()
{
	m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	if (m_hIocp == NULL)
		return false;
	else
		return true;
}

bool CNetwork::InitSock()
{
	int retval;

	m_ListenSock = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (!m_ListenSock == INVALID_SOCKET) {
		int err_no = WSAGetLastError();
		Err_display("Socket Err", err_no);
		return false;
	}

	SOCKADDR_IN	serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVER_PORT);
	retval = ::bind(m_ListenSock, reinterpret_cast<SOCKADDR*>(&serveraddr), sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		int err_no = WSAGetLastError();
		Err_display("bind()", err_no);
		return false;
	}

	retval = listen(m_ListenSock, SOMAXCONN);
	if (retval == SOCKET_ERROR) {
		int err_no = WSAGetLastError();
		Err_display("listen()", err_no);
		return false;
	}
	return true;
}

bool CNetwork::Initialize()
{
	m_pPlayerProcess = new CPlayerProcess();

	if (!InitWinSock()) {
		int err_no = WSAGetLastError();
		Err_display("InitWinSock() err", err_no);
		return false;
	}

	if (!InitCompletionPort()) {
		int err_no = WSAGetLastError();
		Err_display("InitCompletionPort() err", err_no);
		return false;
	}

	if (!InitSock()) {
		int err_no = WSAGetLastError();
		Err_display("InitSock() err", err_no);
		return false;
	}
	return true;
}

void CNetwork::Disconnect()
{
	closesocket(m_ListenSock);
	CloseHandle(m_hIocp);
	WSACleanup();
	m_ListenSock = INVALID_SOCKET;
}

void CNetwork::WorkerThread()
{
	while (m_bRunningServer) {
		DWORD io_byte;
		ULONGLONG key;
		POVER_EX	lpover_ex;

		BOOL	is_error = GetQueuedCompletionStatus(m_hIocp, &io_byte, &key, reinterpret_cast<LPWSAOVERLAPPED *>(&lpover_ex), INFINITE);

		unsigned short id = reinterpret_cast<unsigned short&>(key);

		if (FALSE == is_error || 0 == io_byte) {
			if (FALSE == is_error) {
				int err_no = WSAGetLastError();
				Err_display("Woker_Thread Start - GetQueuedCompletionStatus", err_no);
			}

			//closesocket(/*클라이언트 소켓*/);
		// 클라이언트 연결 끊기
			std::cout << "[ Player: " << key << " ] Disconnect" << std::endl;
			continue;
		}

		switch (lpover_ex.m_Event)
		{
		case EV_RECV:
		{
			break;
		}

		case EV_SEND:
		{
			break;
		}
		}
	}
	return;
}

void CNetwork::AcceptThread()
{
	while (m_bRunningServer) {
		/*SOCKADDR_IN addr;
		ZeroMemory(&addr, sizeof(SOCKADDR_IN));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(SERVER_PORT);
		addr.sin_addr.s_addr = INADDR_ANY;
		int addrsize = sizeof(SOCKADDR_IN);*/

		/*auto client_sock = WSAAccept(m_ListenSock, reinterpret_cast<sockaddr*>(&addr), &addrsize, NULL, NULL);
	
		if (client_sock == INVALID_SOCKET) {
			int err_no = WSAGetLastError();
			Err_display("ACCEPT INVALID_SOCKET!", err_no);
			break;
		}
		else {
			if () {

				continue;
			}
			else {
				continue;
			}
		}*/
	}
}

void CNetwork::UpdateThread()
{
}

void CNetwork::DataBaseThread()
{
}

void CNetwork::Err_quit(const char* msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

void CNetwork::Err_display(const char* msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	cout << "[ " << msg << " ]";
	wcout << L"에러 " << lpMsgBuf << endl;
	while (true);
	LocalFree(lpMsgBuf);
}