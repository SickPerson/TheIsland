#pragma once

class CCore
{
public:
	HWND	m_hMainHwnd;

public:
	int Init( HWND hWnd, const RESOLUTION& resolution, bool bWindow );
	void Run();

private:
	void ChangeWindowSize( HWND hWnd, const RESOLUTION& resolution );

	void Update();
	void LateUpdate();
	void FinalUpdate();
	void Render();

	DECLARE_SINGLE(CCore)
};

