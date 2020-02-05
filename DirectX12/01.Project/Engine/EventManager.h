#pragma once

#include "Engine.h"

struct tEvent
{
	EVENT_TYPE	eType;
	DWORD_PTR	wParam;
	DWORD_PTR	lParam;
};

class CGameObject;
class CEventManager
{
private:
	vector<tEvent>			m_vecEvent;
	vector<tEvent>			m_vecDeadEvent;
	vector<CGameObject*>	m_vecDead;

public:
	void Update();
	void Clear();
	void AddEvent( tEvent& e );

	const vector<tEvent>& GetEventList();

private:
	void Execute( tEvent& e );

	DECLARE_SINGLE(CEventManager)
};

