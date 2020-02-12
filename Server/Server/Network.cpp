#include "stdafx.h"
#include "Network.h"



CNetwork::CNetwork()
{
	m_bRunningServer = true;
	this->Initialize();
}


CNetwork::~CNetwork()
{
}

bool CNetwork::InitWinSock()
{
	// 윈속 초기화
	WSADATA	wsa;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return false;
	else return true;
}

bool CNetwork::InitCompletionPort()
{
	return false;
}

bool CNetwork::InitSock()
{
	int retval;

	m_ListenSock = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (!m_ListenSock == INVALID_SOCKET)
		return false;

	SOCKADDR_IN	serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVER_PORT);
	retval = bind(m_ListenSock, reinterpret_cast<SOCKADDR*>(&serveraddr), sizeof(serveraddr));
	if (retval == SOCKET_ERROR) {
		Err_quit("bind()");
		return false;
	}

	retval = listen(m_ListenSock, SOMAXCONN);
	if (retval == SOCKET_ERROR) {
		Err_display("listen()");
		return false;
	}
	return true;
}

void CNetwork::Initialize()
{
	if (!InitWinSock()) {
		Err_display("InitWinSock() err");
	}

	if (!InitCompletionPort()) {
		Err_display("InitCompletionPort() err");
	}

	if (!InitSock()) {
		Err_display("InitSock() err");
	}
}

void CNetwork::Disconnect()
{
	closesocket(m_ListenSock);
	WSACleanup();
	m_ListenSock = INVALID_SOCKET;
}

void CNetwork::WorkerThread()
{
	DWORD size;
	DWORD flag = 0;
	ULONGLONG key;
	int err_code;

	while (m_bRunningServer) {
		err_code = 0;
		s_pContext scontext = nullptr;
		BOOL retval = iocp_server::GetInst()->Ge
	}
}

void CNetwork::AcceptThread()
{
	while (m_bRunningServer) {
		SOCKADDR_IN addr;
		ZeroMemory(&addr, sizeof(SOCKADDR_IN));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(SERVER_PORT);
		addr.sin_addr.s_addr = INADDR_ANY;
		int addrsize = sizeof(SOCKADDR_IN);

		auto client_sock = WSAAccept(m_ListenSock, reinterpret_cast<sockaddr*>(&addr), &addrsize, NULL, NULL);
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
