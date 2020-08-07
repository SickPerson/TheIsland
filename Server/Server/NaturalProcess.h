#pragma once
#include "Process.h"

class CNaturalProcess: public CProcess
{
public:
	explicit CNaturalProcess();
	virtual ~CNaturalProcess();

private:
	function<void(USHORT, USHORT)> m_fpNaturalUpdate[OBJ_STATE_END];

public:
	void BindNaturalUpdate() {

	}

public:
	void UpdateNatural(char state, USHORT natural_id, USHORT player_id)
	{
		//m_fpNaturalUpdate[]
	}
};

