#include "NaturalProcess.h"
#include "PacketMgr.h"


CNaturalProcess::CNaturalProcess()
{
	if (!CProcess::m_pNaturalPool)
		CProcess::m_pNaturalPool = new class CNaturalpool();
}


CNaturalProcess::~CNaturalProcess()
{
	if (m_pNaturalPool)
	{
		delete m_pNaturalPool;
		m_pNaturalPool = nullptr;
	}
}

void CNaturalProcess::RespawnEvent(USHORT natural_id)
{
}

void CNaturalProcess::DamageEvent(USHORT NaturalId, USHORT PlayerId)
{
	NATURAL_TYPE eType = m_pNaturalPool->m_cumNaturalPool[NaturalId]->GetType();

	if (eType == NATURAL_NONE)
		return;

	float fHealth = m_pNaturalPool->m_cumNaturalPool[NaturalId]->GetHealth();

	if (fHealth <= 0.f)
	{
		PushEvent_Respawn(NaturalId);
		//CPacketMgr::GetInst()->Send_Natural_Destroy_Packet(NaturalId);
	}
}

void CNaturalProcess::PushEvent_Respawn(USHORT natural_id)
{
	m_pNaturalPool->m_cumNaturalPool[natural_id]->SetDestroy(true);
	m_pNaturalPool->m_cumNaturalPool[natural_id]->SetAngle(0.f);
	//m_pNaturalPool->m_cumNaturalPool[natural_id]->SetTargetRot();
	Update_Event ev;
	ev.m_Do_Object = natural_id;
	ev.m_EventType = EV_NATURAL_UPDATE;
	ev.m_From_Object = NO_TARGET;
	ev.m_ObjState = (UINT)NATURAL_UPDATE_TYPE::RESPAWN;
	ev.wakeup_time = high_resolution_clock::now() + 10s;
	PushEventQueue(ev);
}

void CNaturalProcess::PushEvent_Damage(USHORT NaturalId, USHORT PlayerId)
{
	Update_Event ev;
	ev.m_Do_Object = NaturalId;
	ev.m_EventType = EV_NATURAL_UPDATE;
	ev.m_From_Object = PlayerId;
	ev.m_ObjState = (UINT)NATURAL_UPDATE_TYPE::DAMAGE;
	ev.wakeup_time = high_resolution_clock::now();
	PushEventQueue(ev);
}
