#include "Core.h"
#include "Network.h"
#include "PacketMgr.h"

CCore::CCore()
{
	m_iNumWorkerThread = 0;
	m_vWorkerThread.clear();

	Initalize();
}

CCore::~CCore()
{
}

void CCore::Initalize()
{
	CheckThisCpuCount();
	CNetwork::GetInst();
}

void CCore::CheckThisCpuCount()
{
	// CPU , Thread 개수 확인
	SYSTEM_INFO	si; // CPU 개수 확인용
	GetSystemInfo(&si); // 시스템 정보를 받아온다.
	m_iCpuCore = static_cast<int>(si.dwNumberOfProcessors);
	m_iNumWorkerThread = static_cast<int>(m_iCpuCore * 2 - 2);
	cout << "CPU Core Count: " << m_iCpuCore << "\tThread: " << m_iNumWorkerThread << endl;
}

void CCore::StartServer()
{
	for (int i = 0; i < m_iNumWorkerThread; ++i) {
		m_vWorkerThread.emplace_back(std::shared_ptr<std::thread>(new std::thread{ [&]() {CNetwork::GetInst()->WorkerThread(); } }));
		//m_vWorkerThread.emplace_back(thread{ [&]() {CNetwork::GetInst()->WorkerThread(); } });
	}
	std::cout << "WorkerThread Create" << std::endl;

	m_pAcceptThread = std::shared_ptr<std::thread>(new std::thread{ [&]() {CNetwork::GetInst()->AcceptThread(); } });
	//m_tAcceptThread = thread{ [&]() {CNetwork::GetInst()->WorkerThread(); } };
	std::cout << "AcceptThread Create" << std::endl;

	m_pUpdateThread = std::shared_ptr< std::thread > (new std::thread{ [&]() {CNetwork::GetInst()->UpdateThread(); } });
	//std::cout << "UpdateThread Create" << std::endl;

	std::cout << "Server Start" << std::endl;
}

void CCore::CloseServer()
{
	
	m_pUpdateThread->join();
	cout << "나와도 돼" << endl;
	m_pAcceptThread->join();
	//m_tAcceptThread.join();
	cout << "나오면 안돼" << endl;

	for (auto& thread : m_vWorkerThread)
		thread->join();
	/*for (auto& thread : m_vWorkerThread)
		thread.join();*/

	CNetwork::GetInst()->EndServer();
	CNetwork::GetInst()->Disconnect();
}
