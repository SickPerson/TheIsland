#include "stdafx.h"
#include "Process.h"
#include "PacketMgr.h"



CProcess::CProcess()
{
}


CProcess::~CProcess()
{
	InitProcessData();
}

void CProcess::InitProcessData()
{
	m_cusLoginList.clear();
	m_cpqEventQueue.clear();
}
