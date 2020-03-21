#pragma once
#include "stdafx.h"

class CTimerMgr
{
	SINGLE(CTimerMgr);

public:
	float GetTotalTime() const; // in seconds
	float GetDeltaTime() const; // in seconds

	void Reset(); // call before message loop.
	void Start();// call when unpaused.
	void Stop(); // call when paused.
	void Tick(); // call every frame.

private:
	double m_SecondsPerCount = 0.f;
	double m_DeltaTime = -1.f;

	__int64 m_BaseTime = 0;
	__int64 m_PausedTime = 0;
	__int64 m_StopTime = 0;
	__int64 m_PrevTime = 0;
	__int64 m_CurrTime = 0;

	bool m_Stopped = false;
};

