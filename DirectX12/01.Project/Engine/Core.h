#pragma once


class CCore
{
	SINGLE(CCore);
public:
	HWND	m_hMainHwnd;

public:
	int Init(HWND _hWnd, const tResolution& _resolution, bool _bWindow);
	void Progress();

private:
	std::shared_ptr<std::thread>	m_tNetworkThread = nullptr;
public:
	void InitNetwork();
	void RunNetwork();
	bool GetStartLoop();
private:
	void ChangeWindowSize(HWND _hWnd, const tResolution& _resolution);
};

