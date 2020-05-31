#pragma once
#include "Component.h"

#include "Frustum.h"

enum class PROJ_TYPE
{
	ORTHGRAPHIC,
	PERSPECTIVE,
};

enum class CAM_TYPE
{
	BASIC,
	INVENTORY,
	END
};

class CCamera :
	public CComponent
{
public:
	CCamera();
	virtual ~CCamera();

	CLONE( CCamera );

private:	
	CFrustum	m_frustum;

	float		m_fNear; // View Space 에서 시야 범위
	float		m_fFar;  // View Space 에서 시야 범위

	float		m_fFOV;  // 원근 투영 시, 시야각
	float		m_fScale; // 직교투영 시, 투영 범위
	float		m_fWidth;	// 투영 해상도
	float		m_fHeight;	// 투영 해상도
	   
	Matrix		m_matView;		// 뷰행렬
	Matrix		m_matViewInv;	// 뷰역행렬
	Matrix		m_matProj;		// 투영행렬
	Matrix		m_matProjInv;	// 투영역행렬

	PROJ_TYPE	m_eProjType;
	UINT		m_iLayerCheck; // 카메라가 찍는 Layer 비트값

	vector<CGameObject*>			m_vecDeferred;
	vector<CGameObject*>			m_vecForward;
	vector<CGameObject*>			m_vecParticle;
	vector<CGameObject*>			m_vecPostEffect;
	vector<CGameObject*>			m_vecFont;

	vector<CGameObject*>	m_vecShadowObj;

	bool		m_bModule;

	CAM_TYPE	m_eCamType;	// 일반 카메라인지 플레이어만 보는 특수한 카메라인지
public:	
	virtual void FinalUpdate();

	void SortGameObject();
	void SortShadowObject();

	void Render_Deferred();
	void Render_Forward();
	void Render_PostEffect();
	void Render_Shadowmap();
	void Render();

public:
	void SetProjType(PROJ_TYPE _eType) { m_eProjType = _eType; Changed(); }
	PROJ_TYPE GetProjType() { return m_eProjType; }

	void SetFar(float _far) { m_fFar = _far; Changed();}
	void SetScale(float _fScale) { m_fScale = _fScale; if (m_fScale < 0.01f) m_fScale = 0.01f; Changed(); }
	void SetFOV(float _fAngle) { m_fFOV = _fAngle; Changed();}
	void SetWidth(float _fWidth) { m_fWidth = _fWidth; }
	void SetHeight(float _fHeight) { m_fHeight = _fHeight; }

	float GetFar() { return m_fFar; }
	float GetScale() { return m_fScale; }
	float GetFOV() { return m_fFOV; }

	void SetLayerCheck(int _iLayerIdx, bool _bAdd)
	{
		assert(_iLayerIdx != -1);
		if (_bAdd)
			m_iLayerCheck |= (1 << _iLayerIdx);
		else
			m_iLayerCheck &= ~(1 << _iLayerIdx);
	}
	void SetLayerAllCheck(){m_iLayerCheck = 0xffffffff;}
	void SetLayerAllRelease() { m_iLayerCheck = 0; }

	void SetModule(bool _bModule) { m_bModule = _bModule; }

	const Matrix& GetViewMat() { return m_matView; }
	const Matrix& GetViewMatInv() { return m_matViewInv; }
	const Matrix& GetProjMat() { return m_matProj; }
	const Matrix& GetProjMatInv() { return m_matProjInv; }

	void SetCamType(CAM_TYPE _eType) { m_eCamType = _eType; }
	CAM_TYPE GetCamType() { return m_eCamType; }

public:
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);
};

