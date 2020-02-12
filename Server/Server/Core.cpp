#include "Core.h"
#include "Network.h"

DEFINITION_SINGLE(CCore)

CCore::CCore()
{
	m_iNumWorkerThread = 0;
	m_vWorkerThread.clear();
}


CCore::~CCore()
{

}

void CCore::Initalize()
{
	CheckThisCpuCount();
}

void CCore::CheckThisCpuCount()
{
	// CPU , Thread 개수 확인
	SYSTEM_INFO	si; // CPU 개수 확인용
	GetSystemInfo(&si); // 시스템 정보를 받아온다.
	m_iCpuCore = static_cast<int>(si.dwNumberOfProcessors);
	m_iNumWorkerThread = static_cast<int>(m_iCpuCore * 2 - 2);
	cout << "CPU Core Count: " << m_iCpuCore << "Thread: " << m_iNumWorkerThread << endl;
}

void CCore::StartServer()
{
	/*m_pAcceptThread = std::shared_ptr<std::thread>(new std::thread{ [&]() {CNetwork::GetInstance()->AcceptThread(); } });
	std::cout << "AcceptThreadCreate" << std::endl;

	m_pUpdateThread = std::shared_ptr<std::thread>(new std::thread{ [&]() {CNetwork::GetInstance()->UpdateThread(); } });
	std::cout << "UpdateThreadCreate" << std::endl;

	for (int i = 0; i < m_iNumWorkerThread; ++i) {
		m_vWorkerThread.push_back(std::shared_ptr<std::thread>(new thread{ [&]() {CNetwork::GetInstance()->WorkerThread(); } }));
	}
	std::cout << "WorkerThreadCreate" << std::endl;
	std::cout << "Start!" << std::endl;*/
}

void CCore::CloseServer()
{
	m_pAcceptThread->join();
	m_pUpdateThread->join();

	for (auto& thread : m_vWorkerThread)
		thread->join();
}
