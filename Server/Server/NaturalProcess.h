#pragma once
#include "Process.h"

enum class NATURAL_STATE_TYPE
{
	DAMAGE,
	DEAD,
	RESPAWN,
	END
};
class CNaturalProcess:
	public CProcess
{
public:
	explicit CNaturalProcess();
	virtual ~CNaturalProcess();

private:
	function<void(USHORT, USHORT)> m_fpNaturalUpdate[(UINT)NATURAL_STATE_TYPE::END];

public:
	void BindNaturalUpdate() {
		m_fpNaturalUpdate[(UINT)NATURAL_STATE_TYPE::DAMAGE] = [&](USHORT natural_id, USHORT target_id) 
		{
			DamageEvent(natural_id, target_id); 
		};
		m_fpNaturalUpdate[(UINT)NATURAL_STATE_TYPE::DEAD] = [&](USHORT natural_id, USHORT target_id) 
		{
			DeadEvent(natural_id); 
		};
		m_fpNaturalUpdate[(UINT)NATURAL_STATE_TYPE::RESPAWN] = [&](USHORT natural_id, USHORT target_id)
		{
			RespawnEvent(natural_id);
		};
	}
	void UpdateNatural(UINT state, USHORT natural_id, USHORT target_id) {
		m_fpNaturalUpdate[state](natural_id, target_id);
	}

public:
	void DamageEvent(USHORT natural_id, USHORT target_id);
	void DeadEvent(USHORT natural_id);
	void RespawnEvent(USHORT natural_id);

public:
	void PushEvent_Respawn(USHORT natural_id);
};

