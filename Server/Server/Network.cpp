#include "stdafx.h"
#include "Network.h"
#include "PlayerProcess.h"
#include "PacketMgr.h"
#include "TimerMgr.h"
#include "DataBase.h"

CNetwork::CNetwork()
{
	m_ListenSock = INVALID_SOCKET;
	m_bRunningServer = true;
	m_pPlayerProcess = nullptr;
	m_usUserID = 0;
	//------------------------------
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
		strcpy_s(ip, ipaddr);
	}
	cout << "Server IP Address: " << ipaddr << endl;
}

bool CNetwork::CreateSocket(SOCKET & _sClientSocket, unsigned short _usID)
{
	if (_sClientSocket == INVALID_SOCKET)
	{
		closesocket(_sClientSocket);
		return false;
	}
	return Associate(reinterpret_cast<HANDLE>(_sClientSocket), _usID);
}

bool CNetwork::Associate(HANDLE _hDevice, unsigned short _usID)
{
	if (_hDevice == INVALID_HANDLE_VALUE)
		return false;
	CreateIoCompletionPort(_hDevice, m_hIocp, _usID, 0);
	if (NULL == m_hIocp)
		return false;
	else
		return true;
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
	if (m_ListenSock == INVALID_SOCKET) {
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
	CProcess::InitializeBeforeStart();

	m_pPlayerProcess = new CPlayerProcess();

	CTimerMgr::GetInst()->Reset();

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
		DWORD		io_byte;
		ULONGLONG	key;
		PULONG_PTR	p_key = &key;
		POVER_EX	lpover_ex;

		BOOL	is_error = GetQueuedCompletionStatus(m_hIocp, &io_byte, p_key,
			reinterpret_cast<LPWSAOVERLAPPED *>(&lpover_ex), INFINITE);

		unsigned short id = reinterpret_cast<unsigned short&>(key);

		// 비정상 종료 : FALSE, 수신 바이트 크기 = 0
		// 정상 종료 : TRUE, 수신 바이트 크기 = 0
		if (is_error == false)
		{
			int err_no = WSAGetLastError();
			if (err_no != WSA_IO_PENDING)
			{
				std::cout << "[ Player: " << key << " ] Disconnect" << std::endl;
				m_pPlayerProcess->PlayerDinconnect(id);
			}
			continue;
		}

		if (io_byte == 0)
		{
			std::cout << "[ Player: " << key << " ] Disconnect" << std::endl;
			m_pPlayerProcess->PlayerDinconnect(id);
			continue;
		}

		switch (lpover_ex.m_Event)
		{
		case EV_RECV:
		{
			m_pPlayerProcess->RecvPacket(id, io_byte, lpover_ex.m_MessageBuffer);
			break;
		}

		case EV_SEND:
		{
			if (io_byte != lpover_ex.m_DataBuffer.len)
			{
				int err_no = GetLastError();
				Err_display("WorkerThread]Send Error", err_no);
				m_pPlayerProcess->PlayerDinconnect(id); // 오류 발생시 소켓 종료
			}
			break;
		}
		case EV_UPDATE:
		{
			break;
		}
		case EV_PLAYER_UPDATE:
		{
			break;
		}
		case EV_DB:
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
		SOCKADDR_IN addr;
		ZeroMemory(&addr, sizeof(SOCKADDR_IN));
		addr.sin_family = AF_INET;
		addr.sin_port = htons(SERVER_PORT);
		addr.sin_addr.s_addr = INADDR_ANY;
		int addrsize = sizeof(SOCKADDR_IN);

		auto client_sock = WSAAccept(m_ListenSock, reinterpret_cast<sockaddr*>(&addr), &addrsize, NULL, NULL);

		if (client_sock == INVALID_SOCKET) {
			int err_no = WSAGetLastError();
			Err_display("ACCEPT INVALID_SOCKET!", err_no);
			break;
		}
		else {
			if (!CreateSocket(client_sock, m_usUserID)) {
				continue;
			}
			else {
				cout << "접속 : " << inet_ntoa(addr.sin_addr) << '\t' << ntohs(addr.sin_port) << endl;
				cout << "UserNum : " << m_usUserID + 1 << endl;
				++m_usUserID;
				continue;
			}
		}
	}
	return;
}

void CNetwork::UpdateThread()
{
	// 일정 시간이 지날때마다 업데이트를 해주라는 함수 부분
	if (CTimerMgr::GetInst())
		CTimerMgr::GetInst()->Start();
	while (m_bRunningServer)
	{
		this_thread::sleep_for(1ms);
		while (!CProcess::StateEventQueue())
		{
			Object_Event	object_event;
			if (CProcess::CheckEventStart(object_event))
			{
				OVER_EX*	over_ex = new OVER_EX;

				ZeroMemory(&over_ex->m_Overlapped, sizeof(WSAOVERLAPPED));
				over_ex->m_Event = object_event.m_EventType;
				PostQueuedCompletionStatus(m_hIocp, 10, object_event.m_usID, &over_ex->m_Overlapped);
			}
			else
				break;
		}
	}
	return;
}

void CNetwork::DataBaseThread()
{
	while (m_bRunningServer)
	{
		this_thread::sleep_for(10ms);
		while (!CDataBase::GetInst()->DataBaseQueueIsEmpty())
		{
			DataBase_Event	dbEvent;
			if (CDataBase::GetInst()->PopFromDataBaseQueue(dbEvent))
				CDataBase::GetInst()->RunDataBase(dbEvent);
			else
				break;
		}
	}
	return;
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