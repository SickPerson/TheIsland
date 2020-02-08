#pragma once
#include "Entity.h"

class CTexture;
class CAnimator2D;

// 2D Animation 1 Frame Info
struct tAnimation2DFrame
{
	Ptr<CTexture>		pTex;		// Texture
	Vec2				vLT;		// Left Top UV
	Vec2				vLen;		// UV Legth
	Vec2				vOffsetPos;	// 출력위치 상대 이동값
	float				fDuration;	// 프레임 지속시간	
};

class CAnimation2D :
	public CEntity
{
public:
	CAnimation2D();
	virtual ~CAnimation2D();

	CLONE( CAnimation2D );

private:
	CAnimator2D*				m_pAnimator;
	vector<tAnimation2DFrame>	m_vecFrame;
	int							m_iCurFrame;
	float						m_fAccTime;
	bool						m_bFinish;

public:
	void FinalUpdate();
	void UpdateData();

	void Play();
	bool IsFinish();	

public:
	void Create( Ptr<CTexture> pTex, Vec2& vLT, Vec2& vLen, int iFrameCount, float fDuration );
	
	void SaveToScene( FILE* pFile );
	void LoadFromScene( FILE* pFile );

public:
	void SetAnimatior2D( CAnimator2D* pAnimator2D );
};

