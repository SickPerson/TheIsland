#pragma once


class CCore
{
	SINGLE(CCore);
public:
	HWND	m_hMainHwnd;

	HINSTANCE m_hInstance;

public:
	int Init(HWND _hWnd, const tResolution& _resolution, bool _bWindow);
	int ToolInit( HINSTANCE _hInstance, HWND _hWnd, const tResolution& _start, const tResolution& _resolution, bool _bWindow );
	void Progress();

private:
	void ChangeWindowSize(HWND _hWnd, const tResolution& _resolution);

	void ChangeWindowSize( HWND _hWnd, const tResolution& _start, const tResolution& _resolution );
};

