#pragma once
#include "stdafx.h"

class CCore
{
private:
	int		m_iCpuCore;
	int		m_iNumWorkerThread;
	vector<thread*>		m_vWorkerThread;
	shared_ptr<thread>	m_pAcceptThread;
	shared_ptr<thread>	m_pUpdateThread;
	shared_ptr<thread>	m_pDataBaseThread;

public:
	void Initalize();
	void CheckThisCpuCount();
	void StartServer();
	void CloseServer();

	DECLARE_SINGLE(CCore)
};

