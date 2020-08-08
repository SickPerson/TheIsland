#include "HousingProcess.h"



CHousingProcess::CHousingProcess()
{
	if (!CProcess::m_pHousingPool)
		CProcess::m_pHousingPool = new class CHousingpool();
}


CHousingProcess::~CHousingProcess()
{
	if (m_pHousingPool)
	{
		delete m_pHousingPool;
		m_pHousingPool = nullptr;
	}
}
