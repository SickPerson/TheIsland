#include "stdafx.h"
#include "Network.h"

#include "PlayerProcess.h"
#include "MonsterProcess.h"
#include "NaturalProcess.h"
#include "HousingProcess.h"
#include "EtcProcess.h"

#include "PacketMgr.h"
#include "TimerMgr.h"
#include "DataBase.h"

CNetwork::CNetwork()
{
	m_ListenSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	m_bRunningServer = true;
	CheckThisCputCount();
}

CNetwork::~CNetwork()
{
	Release_Process();
	CProcess::Release();
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

void CNetwork::Init_Process()
{
	// [ Process Init ]
	m_pPlayerProcess = new CPlayerProcess();
	m_pAnimalProcess = new CMonsterProcess();
	m_pNaturalProcess = new CNaturalProcess();
	m_pHousingProcess = new CHousingProcess();
	m_pEtcProcess = new CEtcProcess();
}

void CNetwork::Release_Process()
{
	if (m_pAnimalProcess) {
		delete m_pAnimalProcess;
		m_pAnimalProcess = nullptr;
	}
	if (m_pAnimalProcess) {
		delete m_pAnimalProcess;
		m_pAnimalProcess = nullptr;
	}
	if (m_pHousingProcess) {
		delete m_pHousingProcess;
		m_pHousingProcess = nullptr;
	}
	if (m_pNaturalProcess) {
		delete m_pNaturalProcess;
		m_pNaturalProcess = nullptr;
	}
	if (m_pEtcProcess) {
		delete m_pEtcProcess;
		m_pEtcProcess = nullptr;
	}
}

HANDLE CNetwork::GetIocp()
{
	return m_hIocp;
}

USHORT CNetwork::GetUserID()
{
	for (USHORT i = 0; i < MAX_USER; ++i) {
		bool bConnect = CProcess::m_pObjectPool->m_cumPlayerPool[i]->GetConnect();
		if (bConnect)  continue;
		return i;
	}
	return MAX_USER;
}

void CNetwork::Initialize()
{
	//m_UserID = 0;
	CProcess::Initalize();
	Init_Process();
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
	for (int i = 0; i < m_iNumWorkerThread; ++i)
		m_vWorkerThread.push_back(std::shared_ptr<std::thread>(new std::thread{ [&]() {CNetwork::GetInst()->WorkerThread(); } }));
	cout << "Worker Thread Create" << endl;
	m_pAcceptThread = std::shared_ptr<std::thread>(new std::thread{ [&]() {CNetwork::GetInst()->AcceptThread(); } });
	cout << "Accept Thread Create" << endl;
	m_pUpdateThread = std::shared_ptr< std::thread >(new std::thread{ [&]() {CNetwork::GetInst()->UpdateThread(); } });
	cout << "Update Thread Create" << endl;
#ifdef DB_ON
	m_pDatabaseThread = std::shared_ptr<std::thread>(new std::thread{ [&]() {CNetwork::GetInst()->DataBaseThread(); } });
	cout << "DataBase Thread Create" << endl;
#endif // DB_ON
	cout << "==============================" << endl;
	cout << "∥      Server Start          ∥" << endl;
	cout << "==============================" << endl;
}

void CNetwork::CloseServer()
{
	// 서버 종료 패킷 보내기
	/*for (int i = 0; i < MAX_USER; ++i) {
		bool bConnect = m_pProcess->m_pObjectPool->m_cumPlayerPool[i]->GetConnect();
		if (!bConnect) continue;
		CPacketMgr::Send_Disconnect_Server_Packet(i);
	}*/

	// Thread Exit
#ifdef DB_ON
	m_pDatabaseThread->join();
	cout << "DatabaseThread Close" << endl;
#endif
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
	m_iNumWorkerThread = static_cast<int>(m_iCpuCore * 1.5f);
	cout << "CPU Core Count: " << m_iCpuCore << "\tThread: " << m_iNumWorkerThread << endl;
}

void CNetwork::WorkerThread()
{
	DWORD		num_byte;
	ULONGLONG	key64;
	PULONG_PTR	p_key = &key64;
	while (m_bRunningServer) {
		OVER_EX*	lpover_ex;

		bool	is_error = GetQueuedCompletionStatus(m_hIocp, &num_byte, p_key,
			reinterpret_cast<LPWSAOVERLAPPED *>(&lpover_ex), INFINITE);

		USHORT id = static_cast<unsigned>(key64);

		// 비정상 종료 : FALSE, 수신 바이트 크기 = 0
		// 정상 종료 : TRUE, 수신 바이트 크기 = 0
		if (is_error == false)
		{
			int err_no = WSAGetLastError();
			if (err_no != WSA_IO_PENDING){
				m_pPlayerProcess->PlayerLogout(id);
			}
			continue;
		}
		if (num_byte == 0)
		{
			/*auto strID = CProcess::m_pPlayerPool->m_cumPlayerPool[id]->GetWcID();
			std::cout << "[ Player: " << strID << " ] Disconnect" << std::endl;*/
			m_pPlayerProcess->PlayerLogout(id);
			continue;
		}
		switch (lpover_ex->m_Event)
		{
		case EV_RECV:
		{
			m_pPlayerProcess->RecvPacket(id, lpover_ex->m_MessageBuffer, num_byte);
			//delete lpover_ex;
			break;
		}
		case EV_SEND:
		{
			// Send 오류가 발생하면 플레이어를 종료시킨다.
			if (num_byte != lpover_ex->m_DataBuffer.len) {
				int err_no = WSAGetLastError();
				Err_display("[Worker Thread]Send Error: ",err_no);
				m_pPlayerProcess->PlayerLogout(id);
			}
			delete lpover_ex;
			break;
		}
		case EV_MONSTER_UPDATE:
		{
			m_pAnimalProcess->UpdateMonster(lpover_ex->m_Status, id, lpover_ex->m_usOtherID);
			delete lpover_ex;
			break;
		}
		case EV_NATURAL_UPDATE:
		{
			m_pNaturalProcess->UpdateNatural(lpover_ex->m_Status, id, lpover_ex->m_usOtherID);
			delete lpover_ex;
			break;
		}
		case EV_DB:
		{
			delete lpover_ex;
			break;
		}
		case EV_ETC:
		{
			switch (lpover_ex->m_Status)
			{
			case EUT_ROT:
				m_pEtcProcess->Rot_Event();
				break;
			case EUT_WEATHER:
				m_pEtcProcess->WeatherEvent();
				break;
			case EUT_TIMER:
				m_pEtcProcess->TimerEvent();
				break;
			}
			delete lpover_ex;
			break;
		}
		default:
		{
			cout << "Unknown Event Type :" << lpover_ex->m_Event << endl;
			while (true);
		}
		}
	}
}

void CNetwork::AcceptThread()
{
	while (m_bRunningServer) {
		auto client_sock = WSAAccept(m_ListenSock, 
			(struct sockaddr*)&m_clientAddr, &m_addrLen, NULL, NULL);

		if (client_sock == INVALID_SOCKET) {
			int err_no = WSAGetLastError();
			Err_display("ACCEPT INVALID_SOCKET - ", err_no);
			break;
		}

		USHORT id = CProcess::m_pObjectPool->GetLoginID();
		if (id == MAX_USER) {
			CPacketMgr::Send_Full_Server_Packet(client_sock);
		}

		CreateIoCompletionPort(reinterpret_cast<HANDLE>(client_sock), m_hIocp, id, 0);
		cout << "접속 : " << inet_ntoa(m_clientAddr.sin_addr) << '\t' 
			<< ntohs(m_clientAddr.sin_port) << endl;
		m_pPlayerProcess->AcceptClient(client_sock, id);
	}
}

void CNetwork::UpdateThread() // 일정 시간이 지날때마다 업데이트를 해주라는 함수 부분
{
	if (CTimerMgr::GetInst()) {
		CTimerMgr::GetInst()->Start();
	}
	while (m_bRunningServer)
	{
		while (CProcess::EmptyEventQueue()) {
			this_thread::sleep_for(10ms);
		}

		while (!CProcess::EmptyEventQueue())
		{
			Update_Event ev;
			if (CProcess::CheckEventStart(ev))
			{
				OVER_EX*	pOver_ex = new OVER_EX;
				ZeroMemory(&pOver_ex->m_Overlapped, sizeof(WSAOVERLAPPED));
				pOver_ex->m_Event = ev.m_EventType;
				pOver_ex->m_Status = ev.m_eObjUpdate;
				pOver_ex->m_usOtherID = ev.m_From_Object;
				PostQueuedCompletionStatus(m_hIocp, 1, ev.m_Do_Object, &pOver_ex->m_Overlapped);
			}
		}
	}
}

#ifdef DB_ON
void CNetwork::DataBaseThread()
{
	while (m_bRunningServer)
	{
		this_thread::sleep_for(10ms);
		while (!CDataBase::GetInst()->EmptyDatabaseEventQueue())
		{
			DB_Event db_ev;
			if (CDataBase::GetInst()->PopDatabaseEventQueue(db_ev))
				CDataBase::GetInst()->RunDataBase(db_ev);
			else
				break;
		}
	}
}
#endif // DB_ON

void CNetwork::Err_quit(const char* msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCWSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
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