#pragma once
#include "Component.h"

enum class PROJ_TYPE
{
	ORTHGRAPHIC,
	PERSPECTIVE,
};

class CFrustum;

class CCamera :
	public CComponent
{
public:
	CCamera();
	virtual ~CCamera();

private:
	CFrustum		m_Frustum;
	
	float			m_fNear;			// View Space 시야 범위 (near ~ far)
	float			m_fFar;

	float			m_fFOV;				// 원근 투영 시, 시야각
	float			m_fScale;			// 직교 투영 시, 투영 범위

	Matrix			m_matView;			// 뷰행렬
	Matrix			m_matViewInv;		// 뷰역행렬
	Matrix			m_matProj;			// 투영행렬
	Matrix			m_matProjInv;		// 투영역행렬

	PROJ_TYPE		m_eProType;			// 카메라 유형 
	UINT			m_iLayerCheck;		// 카메라가 찍는 Layer 비트값

public:
	virtual void FinalUpdate();
	void Render();

public:
	void SetProjType( PROJ_TYPE eType );
	void SetFar( float fFar );
	void SetScale( float fScale );
	void SetFOV( float fFOV );

	void SetLayerCheck( int iLayerIdx, bool bAdd );
	void SetLayerAllCheck();
	void SetLayerAllRelease();

public:
	PROJ_TYPE GetProjType();
	float GetFar();
	float GetScale();
	float GetFOV();

	const Matrix& GetViewMat();
	const Matrix& GetViewMatInv();
	const Matrix& GetProjMat();
	const Matrix& GetProjMatInv();

public:
	CLONE( CCamera );
	virtual void SaveToScene( FILE* pFile );
	virtual void LoadFromScene( FILE* pFile );
};

