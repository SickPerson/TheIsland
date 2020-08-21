#pragma once
#include "Process.h"

class CNaturalProcess: public CProcess
{
public:
	explicit CNaturalProcess();
	virtual ~CNaturalProcess();

private:
	function<void(USHORT, USHORT)> m_fpNaturalUpdate[NUT_END];

public:
	void BindNaturalUpdate() {
		m_fpNaturalUpdate[NUT_RESPAWN] = [&](USHORT NaturalId, USHORT PlayerId) {
			RespawnEvent(NaturalId);
		};
		m_fpNaturalUpdate[NUT_DAMAGE] = [&](USHORT NaturalId, USHORT PlayerId) {
			DamageEvent(NaturalId, PlayerId);
		};
	}

public:
	void UpdateNatural(UINT state, USHORT natural_id, USHORT player_id)
	{
		m_fpNaturalUpdate[state](natural_id, player_id);
	}

public:
	void RespawnEvent(USHORT natural_id);
	void DamageEvent(USHORT NaturalId, USHORT PlayerId);
};

