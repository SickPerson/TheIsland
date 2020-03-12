#pragma once
#include "stdafx.h"

class CCore
{
	SINGLE(CCore)

private:
	int		m_iCpuCore;
	int		m_iNumWorkerThread;

private:
	vector<shared_ptr<thread>>		m_vWorkerThread;
	shared_ptr<thread>				m_pAcceptThread;
	shared_ptr<thread>				m_pUpdateThread;
	//shared_ptr<thread>	m_pDataBaseThread;

public:
	void CheckThisCpuCount();

public:
	void Initalize();
	void StartServer();
	void CloseServer();
};

