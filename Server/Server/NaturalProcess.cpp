#include "NaturalProcess.h"
#include "PacketMgr.h"


CNaturalProcess::CNaturalProcess()
{
	BindNaturalUpdate();
}


CNaturalProcess::~CNaturalProcess()
{
}

void CNaturalProcess::DieEvent(USHORT Natural_Id)
{
	bool bDestroy = m_pObjectPool->m_cumNaturalPool[Natural_Id]->GetDestroy();
	if (!bDestroy) return;

	for (auto& user : m_pObjectPool->m_cumPlayerPool) {
		bool bConnect = user.second->GetConnect();
		if (!bConnect) continue;
		CPacketMgr::Send_Natural_Destroy_Packet(user.first, Natural_Id);
	}
	PushEvent_Natural_Respawn(Natural_Id);
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

	for (auto& user : m_pObjectPool->m_cumPlayerPool) {
		bool bConnect = user.second->GetConnect();
		if (!bConnect) continue;
		CPacketMgr::Send_Put_Natural_Packet(user.first, natural_id);
	}
}
