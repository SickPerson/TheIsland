#pragma once
#include "Process.h"

class CEtcProcess : 
	public CProcess
{
public:
	explicit CEtcProcess();
	virtual ~CEtcProcess();

public:
	void Animal_Collision_Event();
	void Rot_Event();
	void WeatherEvent();
	void TimerEvent();
	void UserInfo_Save_Event();
};

