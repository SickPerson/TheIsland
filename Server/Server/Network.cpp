#include "stdafx.h"
#include "Network.h"
#include "PlayerProcess.h"
#include "MonsterProcess.h"

#include "PacketMgr.h"
#include "TimerMgr.h"
#include "DataBase.h"
#include "Player.h"

CNetwork::CNetwork()
{
	m_ListenSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	m_bRunningServer = true;
	m_pPlayerProcess = nullptr;
	m_pMonsterProcess = nullptr;
	m_UserID = 0;
	//------------------------------
	//Initialize();
	CheckThisCputCount();
}

CNetwork::~CNetwork()
{
	CloseServer();
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
	cout << "Server IP Address: " << ipaddr << endl;
}

<<<<<<< HEAD
HANDLE CNetwork::GetIocp()
{
	return m_hIocp;
}

void CNetwork::Initialize()
{
	CProcess::InitBeforeStart();

	m_pPlayerProcess = new CPlayerProcess();
	m_pMonsterProcess = new CMonsterProcess();
=======
void CNetwork::Initialize()
{
	CProcess::InitializeBeforeStart();

	m_pPlayerProcess = new CPlayerProcess();
>>>>>>> master

	CTimerMgr::GetInst()->Reset();

	// ==========================================
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		int err_no = WSAGetLastError();
		Err_display("InitWinSock() err", err_no);
	}

	int retval;

	m_ListenSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_ListenSock == INVALID_SOCKET) {
		int err_no = WSAGetLastError();
		Err_display("Socket Err", err_no);
	}

	SOCKADDR_IN	serveraddr;
	ZeroMemory(&serveraddr, sizeof(SOCKADDR_IN));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVER_PORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	retval = ::bind(m_ListenSock, (struct sockaddr*)&serveraddr, sizeof(SOCKADDR_IN));
	if (retval == SOCKET_ERROR) {
		int err_no = WSAGetLastError();
		Err_display("bind()", err_no);
	}
	retval = listen(m_ListenSock, 5);
	if (retval == SOCKET_ERROR) {
		int err_no = WSAGetLastError();
		Err_display("listen()", err_no);
	}
	//
	m_addrLen = sizeof(SOCKADDR_IN);
	memset(&m_clientAddr, 0, m_addrLen);

	//
	m_hIocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	if (m_hIocp == NULL) {
		int err_no = WSAGetLastError();
		Err_display("InitCompletionPort() err", err_no);
	}

	GetServerIpAddress();
}

void CNetwork::StartServer()
{
	for (int i = 0; i < m_iNumWorkerThread; ++i) {
		m_vWorkerThread.push_back(std::shared_ptr<std::thread>(new std::thread{ [&]() {CNetwork::GetInst()->WorkerThread(); } }));
<<<<<<< HEAD
	}
	cout << "WorkerThread Create" << endl;

	m_pAcceptThread = std::shared_ptr<std::thread>(new std::thread{ [&]() {CNetwork::GetInst()->AcceptThread(); } });
	cout << "AcceptThread Create" << endl;

	m_pUpdateThread = std::shared_ptr< std::thread >(new std::thread{ [&]() {CNetwork::GetInst()->UpdateThread(); } });
	cout << "UpdateThread Create" << endl;

	m_pDatabaseThread = std::shared_ptr<std::thread>(new std::thread{ [&]() {CNetwork::GetInst()->DataBaseThread(); } });
	cout << "DatabaseThread Create" << endl;

	cout << "==============================" << endl;
	cout << "∥       Server Start         ∥" << endl;
	cout << "==============================" << endl;
}

void CNetwork::CloseServer()
{
	m_pDatabaseThread->join();
	cout << "DatabaseThread Close" << endl;
	m_pUpdateThread->join();
	cout << "UpdateThread Close" << std::endl;
	m_pAcceptThread->join();
	cout << "AcceptThread Close" << std::endl;

	for (auto& thread : m_vWorkerThread)
		thread->join();
	cout << "WorkerThread Close" << std::endl;

	EndServer();
	Disconnect();

	cout << "==============================" << endl;
	cout << "∥       Server Exit          ∥" << endl;
	cout << "==============================" << endl;
=======
		//m_vWorkerThread.emplace_back(thread{ [&]() {CNetwork::GetInst()->WorkerThread(); } });
	}
	std::cout << "WorkerThread Create" << std::endl;

	m_pAcceptThread = std::shared_ptr<std::thread>(new std::thread{ [&]() {CNetwork::GetInst()->AcceptThread(); } });
	//m_tAcceptThread = thread{ [&]() {CNetwork::GetInst()->WorkerThread(); } };
	std::cout << "AcceptThread Create" << std::endl;

	m_pUpdateThread = std::shared_ptr< std::thread >(new std::thread{ [&]() {CNetwork::GetInst()->UpdateThread(); } });
	//std::cout << "UpdateThread Create" << std::endl;

	std::cout << "Server Start" << std::endl;
}

void CNetwork::CloseServer()
{
	m_pUpdateThread->join();
	cout << "나와도 돼" << endl;
	m_pAcceptThread->join();
	//m_tAcceptThread.join();
	cout << "나오면 안돼" << endl;

	for (auto& thread : m_vWorkerThread)
		thread->join();

	EndServer();
	Disconnect();
>>>>>>> master
}

void CNetwork::Disconnect()
{
	closesocket(m_ListenSock);
	CloseHandle(m_hIocp);
	WSACleanup();
}

void CNetwork::CheckThisCputCount()
{
	// CPU , Thread 개수 확인
	SYSTEM_INFO	si; // CPU 개수 확인용
	GetSystemInfo(&si); // 시스템 정보를 받아온다.
	m_iCpuCore = static_cast<int>(si.dwNumberOfProcessors);
	m_iNumWorkerThread = static_cast<int>(m_iCpuCore * 2 - 2);
	cout << "CPU Core Count: " << m_iCpuCore << "\tThread: " << m_iNumWorkerThread << endl;
}

void CNetwork::CheckThisCputCount()
{
	// CPU , Thread 개수 확인
	SYSTEM_INFO	si; // CPU 개수 확인용
	GetSystemInfo(&si); // 시스템 정보를 받아온다.
	m_iCpuCore = static_cast<int>(si.dwNumberOfProcessors);
	m_iNumWorkerThread = static_cast<int>(m_iCpuCore * 2 - 2);
	cout << "CPU Core Count: " << m_iCpuCore << "\tThread: " << m_iNumWorkerThread << endl;
}

void CNetwork::WorkerThread()
{
<<<<<<< HEAD
=======
	cout << "워커 스레드 들어와" << endl;
>>>>>>> master
	DWORD		num_byte;
	ULONGLONG	key64;
	PULONG_PTR	p_key = &key64;
	while (m_bRunningServer) {
		OVER_EX*	lpover_ex;

		BOOL	is_error = GetQueuedCompletionStatus(m_hIocp, &num_byte, p_key,
			reinterpret_cast<LPWSAOVERLAPPED *>(&lpover_ex), INFINITE);

		unsigned int id = static_cast<unsigned>(key64);

		// 비정상 종료 : FALSE, 수신 바이트 크기 = 0
		// 정상 종료 : TRUE, 수신 바이트 크기 = 0
		if (is_error == false)
		{
			int err_no = WSAGetLastError();
			if (err_no != WSA_IO_PENDING){
				std::cout << "[ Player: " << id << " ] Disconnect" << std::endl;
				m_pPlayerProcess->PlayerDinconnect(id);
			}
			continue;
		}

		if (num_byte == 0)
		{
			std::cout << "[ Player: " << id << " ] Disconnect" << std::endl;
			m_pPlayerProcess->PlayerDinconnect(id);
			continue;
		}
		switch (lpover_ex->m_Event)
		{
		case EV_RECV:
		{
<<<<<<< HEAD
			m_pPlayerProcess->RecvPacket(id, lpover_ex->m_MessageBuffer, num_byte);
=======
			cout << "받자받자" << endl;
			m_pPlayerProcess->RecvPacket(id, lpover_ex->m_MessageBuffer, num_byte);
			//m_pPlayerProcess->RecvPacket(id, num_byte, lpover_ex.m_MessageBuffer);
>>>>>>> master
			break;
		}

		case EV_SEND:
		{
<<<<<<< HEAD
			// Send 오류가 발생하면 플레이어를 종료시킨다.
			if (num_byte != lpover_ex->m_DataBuffer.len) {
				int err_no = WSAGetLastError();
				Err_display("[Worker Thread]Send Error: ",err_no);
				m_pPlayerProcess->PlayerDinconnect(id);
			}
=======
>>>>>>> master
			delete lpover_ex;
			break;
		}
		case EV_MONSTER_UPDATE:
		{
<<<<<<< HEAD
			m_pMonsterProcess->UpdateMonster(lpover_ex->m_Status, id, lpover_ex->m_uiOtherID);
=======
>>>>>>> master
			delete lpover_ex;
			break;
		}
		case EV_DB:
		{
<<<<<<< HEAD
			//DB_Event db_event;
			//if (CDataBase::GetInst()->PopFromStateQueue(db_event)) {
			//	switch (db_event.state) {
			//	case DATABASE_SAVE_TYPE::DBS_LOGIN_SUCCESS:
			//	{
			//		m_pPlayerProcess->PlayerLogin(db_event);
			//		break;
			//	}
			//	case DATABASE_SAVE_TYPE::DBS_LOGIN_FAIL:
			//	{
			//		//m_pPlayerProcess->PlayerLoginFail(db_event.client_num);
			//		break;
			//	}
			//	}
			//}
=======
>>>>>>> master
			delete lpover_ex;
			break;
		}
		default:
		{
<<<<<<< HEAD
			cout << "Unknown Event Type :" << lpover_ex->m_Event << endl;
			while (true);
=======
			delete lpover_ex;
			break;
>>>>>>> master
		}
		default:
		{
			cout << "Unknown Event Type :" << lpover_ex->m_Event << endl;
			while (true);
		}
		}
	}
<<<<<<< HEAD
=======
	cout << "워커 나오면 안돼" << endl;
>>>>>>> master
}

void CNetwork::AcceptThread()
{
	while (m_bRunningServer) {
		auto client_sock = WSAAccept(m_ListenSock, (struct sockaddr*)&m_clientAddr, &m_addrLen, NULL, NULL);

		if (client_sock == INVALID_SOCKET) {
			int err_no = WSAGetLastError();
			Err_display("ACCEPT INVALID_SOCKET!", err_no);
			break;
		}
<<<<<<< HEAD

		CreateIoCompletionPort(reinterpret_cast<HANDLE>(client_sock), m_hIocp, m_UserID, 0);


		//cout << "현재 인원: " << m_usUserID + 1 << endl;
		//cout << "접속 : " << inet_ntoa(m_clientAddr.sin_addr) << '\t' << ntohs(m_clientAddr.sin_port) << endl;
		m_pPlayerProcess->AcceptClient(client_sock, m_UserID);
		++m_UserID;
	}
=======
		//m_usUserID = CProcess::GetNewID();
		CreateIoCompletionPort(reinterpret_cast<HANDLE>(client_sock), m_hIocp, m_usUserID, 0);

		cout << "접속 : " << inet_ntoa(m_clientAddr.sin_addr) << '\t' << ntohs(m_clientAddr.sin_port) << endl;
		m_pPlayerProcess->AcceptClient(client_sock, m_usUserID);
		cout << "UserNum : " << m_usUserID << endl;
		++m_usUserID;
	}
	cout << "어셉 스레드 나오면 안돼" << endl;
>>>>>>> master
}

void CNetwork::UpdateThread()
{
	// 일정 시간이 지날때마다 업데이트를 해주라는 함수 부분
	if (CTimerMgr::GetInst()) {

		CTimerMgr::GetInst()->Start();
	}
	while (m_bRunningServer)
	{
		while (CProcess::EmptyEventQueue()) {
			this_thread::sleep_for(10ms);
		}
		Update_Event ev;
		if (CProcess::CheckEventStart(ev))
		{
			OVER_EX*	pOver_ex = new OVER_EX;
			ZeroMemory(&pOver_ex->m_Overlapped, sizeof(WSAOVERLAPPED));
			pOver_ex->m_Event = ev.m_EventType;
			pOver_ex->m_Status = ev.m_ObjState;

			PostQueuedCompletionStatus(m_hIocp, 1, ev.m_Do_Object, &pOver_ex->m_Overlapped);
		}
		else
			break;
	}
<<<<<<< HEAD
=======
	cout << "업데이트 나오면 안돼" << endl;
>>>>>>> master
}

void CNetwork::DataBaseThread()
{
	while (m_bRunningServer)
	{
		this_thread::sleep_for(10ms);
		while (!CDataBase::GetInst()->IsEmptyDBQueue())
		{
			DB_Event db_ev;
			if (CDataBase::GetInst()->TryPopDBQueue(db_ev))
				CDataBase::GetInst()->RunDataBase(db_ev);
			else
				break;
		}
	}
}

void CNetwork::Err_quit(const char* msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
<<<<<<< HEAD
	MessageBox(NULL, (LPCSTR)lpMsgBuf, (LPCSTR)msg, MB_ICONERROR);
=======
	MessageBox(NULL, (LPCWSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
>>>>>>> master
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
	LocalFree(lpMsgBuf);
	while (true);
}