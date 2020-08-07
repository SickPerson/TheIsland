#include "NaturalProcess.h"



CNaturalProcess::CNaturalProcess()
{
	if (!CProcess::m_pNaturalPool)
		CProcess::m_pNaturalPool = new class CNaturalpool();
	BindNaturalUpdate();
}


CNaturalProcess::~CNaturalProcess()
{
	if (m_pNaturalPool)
	{
		delete m_pNaturalPool;
		m_pNaturalPool = nullptr;
	}
}

void CNaturalProcess::DamageEvent(USHORT natural_id, USHORT target_id)
{
	UINT eType = m_pNaturalPool->m_cumNaturalPool[natural_id]->GetType();
	if (eType == NATURAL_NONE)
		return;

	float fHealth = m_pNaturalPool->m_cumNaturalPool[natural_id]->GetHealth();
	float fDamage = m_pPlayerPool->m_cumPlayerPool[target_id]->GetDamage();

	fHealth -= fDamage;

	if (fHealth <= 0.f)
	{
		m_pNaturalPool->m_cumNaturalPool[natural_id]->SetDestroy(true);
		PushEvent_Respawn(natural_id);
	}
}

void CNaturalProcess::DeadEvent(USHORT natural_id)
{
}

void CNaturalProcess::RespawnEvent(USHORT natural_id)
{
}

void CNaturalProcess::PushEvent_Respawn(USHORT natural_id)
{
	Update_Event ev;
	ev.m_Do_Object = natural_id;
	ev.m_EventType = EV_NATURAL_UPDATE;
	ev.m_From_Object = NO_TARGET;
	ev.m_ObjState = (UINT)NATURAL_STATE_TYPE::RESPAWN;
	ev.wakeup_time = high_resolution_clock::now() + 10s;
	PushEventQueue(ev);
}
