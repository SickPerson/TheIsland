#pragma once

#include "Engine.h"

class CRenderTarget24;
class CMRT;
class CLight3D;
class CTexture;
class CCamera;

class CRenderManager
{
private:
	friend class CSceneManager;

private:
	Ptr<CTexture>			m_pRenderTargetTex;
	Ptr<CTexture>			m_pDepthStencilTex;

	tResolution				m_tResolution;

	CRenderTarget24*		m_arrRT[( UINT )RT_TYPE::END];
	CMRT*					m_arrMRT[( UINT )MRT_TYPE::END];

	tLight2DInfo			m_tLight2DInfo;

	vector<CLight3D*>		m_vecLight3D;
	vector<CCamera*>		m_vecCamera;

	bool					m_bWindow;

public:
	void Init( HWND hWnd, const tResolution& res, bool bWindow );
	void Render();

private:
	void CreateSamplerState();
	void CreateBlendState();
	void CreateDepthStencilState();

	void UpdateLight2D();
	void UpdateLight3D();

public:
	void RegisterLight2D( const tLight2D& Light2D );
	void RegisterLight3D( CLight3D* pLight3D );

public:
	CCamera* GetCamera( int iIdx );
	void RegisterCamera( CCamera* pCamera );
	void ClearCamera();
	tResolution GetResolution();

	DECLARE_SINGLE(CRenderManager)
};

