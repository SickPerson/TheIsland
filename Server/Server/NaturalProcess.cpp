#include "NaturalProcess.h"
#include "PacketMgr.h"


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

void CNaturalProcess::RespawnEvent(USHORT natural_id)
{
	bool bDestroy = m_pNaturalPool->m_cumNaturalPool[natural_id]->GetDestroy();
	if (!bDestroy) return;

	m_pNaturalPool->m_cumNaturalPool[natural_id]->SetDestroy(false);
	
	NATURAL_TYPE eType = m_pNaturalPool->m_cumNaturalPool[natural_id]->GetType();
	
	switch (eType)
	{
	case N_TREE:
		m_pNaturalPool->m_cumNaturalPool[natural_id]->SetHealth(150.f);
		break;
	case N_STONE:
		m_pNaturalPool->m_cumNaturalPool[natural_id]->SetHealth(150.f);
		break;
	case N_BUSH:
	case N_NONE:
		m_pNaturalPool->m_cumNaturalPool[natural_id]->SetHealth(1.f);
		break;
	default:
		break;
	}
}

void CNaturalProcess::DamageEvent(USHORT NaturalId, USHORT PlayerId)
{
	bool bDestroy = m_pNaturalPool->m_cumNaturalPool[NaturalId]->GetDestroy();
	if (bDestroy) return;

	NATURAL_TYPE eType = m_pNaturalPool->m_cumNaturalPool[NaturalId]->GetType();

	if (eType == N_NONE)
		return;

	float fHealth = m_pNaturalPool->m_cumNaturalPool[NaturalId]->GetHealth();
	float fDamage = m_pPlayerPool->m_cumPlayerPool[PlayerId]->GetDamage();

	fHealth -= fDamage;

	m_pNaturalPool->m_cumNaturalPool[NaturalId]->SetHealth(fHealth);

	if (fHealth <= 0.f)
	{
		concurrent_unordered_set<USHORT> list;
		CProcess::CopyBeforeLoginList(list);
		for (auto& au : list)
		{
			bool bConnect = m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
			if (!bConnect) continue;
			CPacketMgr::GetInst()->Send_Natural_Destroy_Packet(au, NaturalId);
		}

		PushEvent_Natural_Respawn(NaturalId);
	}
}
