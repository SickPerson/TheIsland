#pragma once
#include "stdafx.h"

class CCore
{
	DECLARE_SINGLE(CCore)
private:
	int		m_iCpuCore;
	int		m_iNumWorkerThread;

private:
	vector<thread*>		m_vWorkerThread;
	shared_ptr<thread>	m_pAcceptThread;
	shared_ptr<thread>	m_pUpdateThread;
	//shared_ptr<thread>	m_pDataBaseThread;

public:
	void Initalize();
	void StartServer();
	void CloseServer();

public:
	void CheckThisCpuCount();
};

