#pragma once
#include "Process.h"

class CEtcProcess : public CProcess
{
public:
	explicit CEtcProcess();
	virtual ~CEtcProcess();

private:
	function<void()> m_fpEtcUpdate[EUT_END];

public:
	void BindEtcUpdate() {
		m_fpEtcUpdate[EUT_PLAYER_COLLISION] = [&]() {Player_Collision_Event(); };
		m_fpEtcUpdate[EUT_ANIMAL_COLLISION] = [&]() {Animal_Collision_Event(); };
		m_fpEtcUpdate[EUT_ROT] = [&]() {Player_Rot_Event(); };
		m_fpEtcUpdate[EUT_WEATHER] = [&]() {WeatherEvent(); };
		m_fpEtcUpdate[EUT_TIMER] = [&]() {TimerEvent(); };
	}
public:
	void UpdateEtc(char eType) {
		m_fpEtcUpdate[eType];
	}

public:
	void Player_Collision_Event();
	void Animal_Collision_Event();
	void Player_Rot_Event();
	void WeatherEvent();
	void TimerEvent();
};

