#pragma once

class CBlendState;
class CDepthStencilState;
class CCamera;
class CLight3D;

class CRenderTarget24;
class CMRT;

#include "Ptr.h"
#include "Texture.h"

class CRenderMgr
{
	SINGLE(CRenderMgr);
	friend class CSceneMgr;

private:	
	Ptr<CTexture>			m_pRenderTargetTex;
	Ptr<CTexture>			m_pDepthStencilTex;
		
	tResolution				m_tResolution;

	//CRenderTarget24*		m_arrRT[(UINT)RT_TYPE::END];
	CMRT*					m_arrMRT[(UINT)MRT_TYPE::END];	
	UINT					m_iRTVHeapSize;

	tLight2DInfo			m_tLight2DInfo;

	vector<CLight3D*>		m_vecLight3D;
	vector<CCamera*>		m_vecCam;
	
	HWND					m_hWnd;
	bool					m_bWindowed;

public:
	void Init(HWND _hWnd, const tResolution & _res, bool _bWindow);
	void Render();
	void Render_tool();			

	void Render_Lights();
	void Merge_Light();

private:
	void CreateMRT();
	
	void UpdateLight2D();
	void UpdateLight3D();

public:
	void RegisterLight2D( const tLight2D& _Light2D );

	int RegisterLight3D( CLight3D* _pLight3D );

	CCamera* GetCamera(int _iIdx) { return m_vecCam[_iIdx]; }
	void RegisterCamera(CCamera* _pCam) { m_vecCam.push_back(_pCam); }
	void ClearCamera() { m_vecCam.clear(); }

	tResolution GetResolution() { return m_tResolution; }
	HWND GetHwnd() { return m_hWnd; }

	CMRT*	GetMRT( MRT_TYPE eType );

	void CopySwapToPosteffect();

	UINT	GetRTVHeapSize();
};


