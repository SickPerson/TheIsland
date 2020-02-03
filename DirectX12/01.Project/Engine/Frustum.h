#pragma once
#include "Entity.h"

class CCamera;

class CFrustum :
	public CEntity
{
public:
	CFrustum(CCamera* pCamera);
	virtual ~CFrustum();

private:
	CCamera*	m_pCamera;
	Vec3		m_arrProj[8];
	Vec4		m_arrFace[( UINT )FACE_TYPE::END];

public:
	void Init();
	void FinalUpdate();

	bool CheckFrustum( const Vec3& vPos );
	bool CheckFrustumSphere( const Vec3& vPos, float fRadius );
};

