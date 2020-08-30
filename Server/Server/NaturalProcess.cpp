#include "NaturalProcess.h"
#include "PacketMgr.h"


CNaturalProcess::CNaturalProcess()
{
	BindNaturalUpdate();
}


CNaturalProcess::~CNaturalProcess()
{
}

void CNaturalProcess::Damage(USHORT Natural_Id, float fDamage)
{
	bool bDestroy = m_pObjectPool->m_cumNaturalPool[Natural_Id]->GetDestroy();
	if (bDestroy) return;

	NATURAL_TYPE eType = m_pObjectPool->m_cumNaturalPool[Natural_Id]->GetType();

	if (eType == N_NONE)
		return;

	float fHealth = m_pObjectPool->m_cumNaturalPool[Natural_Id]->GetHealth();

	fHealth -= fDamage;

	if (fHealth <= 0.f)
	{
		m_pObjectPool->m_cumNaturalPool[Natural_Id]->SetDestroy(true);
		m_pObjectPool->m_cumNaturalPool[Natural_Id]->SetHealth(0.f);

		concurrent_unordered_set<USHORT> list;
		CProcess::CopyBeforeLoginList(list);
		for (auto& au : list)
		{
			bool bConnect = m_pObjectPool->m_cumPlayerPool[au]->GetConnect();
			if (!bConnect) continue;
			CPacketMgr::GetInst()->Send_Natural_Destroy_Packet(au, Natural_Id);
		}
		PushEvent_Natural_Respawn(Natural_Id);
	}
	else {
		m_pObjectPool->m_cumNaturalPool[Natural_Id]->SetHealth(fHealth);
	}
}

void CNaturalProcess::RespawnEvent(USHORT natural_id)
{
	bool bDestroy = m_pObjectPool->m_cumNaturalPool[natural_id]->GetDestroy();
	if (!bDestroy) return;

	m_pObjectPool->m_cumNaturalPool[natural_id]->SetDestroy(false);
	
	NATURAL_TYPE eType = m_pObjectPool->m_cumNaturalPool[natural_id]->GetType();
	
	switch (eType)
	{
	case N_TREE:
		m_pObjectPool->m_cumNaturalPool[natural_id]->SetHealth(150.f);
		break;
	case N_STONE:
		m_pObjectPool->m_cumNaturalPool[natural_id]->SetHealth(150.f);
		break;
	case N_BUSH:
	case N_NONE:
		m_pObjectPool->m_cumNaturalPool[natural_id]->SetHealth(1.f);
		break;
	default:
		break;
	}
}

void CNaturalProcess::DamageEvent(USHORT NaturalId, USHORT PlayerId)
{
	bool bDestroy = m_pObjectPool->m_cumNaturalPool[NaturalId]->GetDestroy();
	if (bDestroy) return;

	NATURAL_TYPE eType = m_pObjectPool->m_cumNaturalPool[NaturalId]->GetType();

	if (eType == N_NONE)
		return;

	float fHealth = m_pObjectPool->m_cumNaturalPool[NaturalId]->GetHealth();
	//float fDamage = m_pObjectPool->m_cumPlayerPool[PlayerId]->GetDamage();

	//fHealth -= fDamage;

	if (fHealth <= 0.f)
	{
		m_pObjectPool->m_cumNaturalPool[NaturalId]->SetDestroy(true);
		m_pObjectPool->m_cumNaturalPool[NaturalId]->SetHealth(0.f);

		concurrent_unordered_set<USHORT> list;
		CProcess::CopyBeforeLoginList(list);
		for (auto& au : list)
		{
			bool bConnect = m_pObjectPool->m_cumPlayerPool[au]->GetConnect();
			if (!bConnect) continue;
			CPacketMgr::GetInst()->Send_Natural_Destroy_Packet(au, NaturalId);
		}

		PushEvent_Natural_Respawn(NaturalId);
	}
	else {
		m_pObjectPool->m_cumNaturalPool[NaturalId]->SetHealth(fHealth);
	}
}
