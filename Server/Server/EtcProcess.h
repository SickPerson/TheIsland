#pragma once
#include "Process.h"

class CEtcProcess : public CProcess
{
public:
	explicit CEtcProcess();
	virtual ~CEtcProcess();

private:
	function<void()> m_fpEtcUpdate[EUT_END];

private:
	void BindEtcUpdate() {
		m_fpEtcUpdate[EUT_WEATHER] = [&]() {WeatherEvent(); };
		m_fpEtcUpdate[EUT_TIMER] = [&]() {TimerEvent(); };
	}
public:
	void UpdateEtc(char eType) {
		m_fpEtcUpdate[eType];
	}

public:
	void WeatherEvent();
	void TimerEvent();
};

