#pragma once
#include "Component.h"

#include "Ptr.h"
#include "Material.h"
#include "Mesh.h"

class CStructuredBuffer;

class CParticleSystem :
	public CComponent
{
public:
	CParticleSystem();
	~CParticleSystem();
	CLONE( CParticleSystem );

private:
	CStructuredBuffer*	m_pParticleBuffer;			// 파이클 데이터 저장
	CStructuredBuffer*	m_pSharedBuffer;			// 파티클 공유 메모리
	UINT				m_iMaxParticle;				// 최대 파티클 개수

	Ptr<CMaterial>		m_pUpdateMtrl;				// 컴퓨트 셰이더
	Ptr<CMaterial>		m_pMtrl;					// 파티클 렌더링용
	Ptr<CMesh>			m_pMesh;					// 사용 메쉬 (Point Mesh)

	float				m_fFrequency;				// 파티클 생성 주기
	float				m_fAccTime;					// 파티클 누적시간 체크
	
	float				m_fMinLifeTime;				// 파티클 최소 생존 시간
	float				m_fMaxLifeTime;				// 파티클 최대 생존 시간

	float				m_fMinSpeed;				// 파티클 시작 속도
	float				m_fMaxSpeed;				// 파티클 최종 속도

	float				m_fStartScale;				// 파티클 시작 크기
	float				m_fEndScale;				// 파티클 최종 크기

	Vec4				m_vStartColor;				// 파티클 초기 색상
	Vec4				m_vEndColor;				// 파티클 최종 색상

	int					m_iKind;					// 파티클 종류


public:
	virtual void FinalUpdate();
	virtual void Render();

public:
	void SetStartColor( const Vec4& vColor );
	void SetEndColor( const Vec4& vColor );

	void SetStartSpeed(float fSpeed);
	void SetEndSpeed(float fSpeed);

	void SetKind( int iKind );

public:
	virtual void SaveToScene( FILE* _pFile );
	virtual void LoadFromScene( FILE* _pFile );
};

