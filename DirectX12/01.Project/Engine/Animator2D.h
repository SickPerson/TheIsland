#pragma once
#include "Component.h"


class CAnimation2D;
class CTexture;

class CAnimator2D :
	public CComponent
{
public:
	CAnimator2D();
	CAnimator2D( const CAnimator2D& ani );
	virtual ~CAnimator2D();

	CLONE( CAnimator2D );

private:
	map<wstring, CAnimation2D*>	m_mapAni;
	CAnimation2D*				m_pCurAni;
	bool						m_bRepeat;
	bool						m_bPlay;

public:
	virtual void FinalUpdate();

public:
	bool AddAnimation( const wstring& strKey, Ptr<CTexture> pTex, Vec2 vLT, Vec2 vLen, int iFrameCount, int fDuration );
	void PlayAnimation( const wstring& strKey, bool bRepeat );
	void PlayAnimation();
	void PauseAniamtion();
	void StopAnimation();
	bool IsPlay();
	bool IsRepeat();

	CAnimation2D* FindAnimation( const wstring& strKey );
	CAnimation2D* GetCurAnimation();
	const map<wstring, CAnimation2D*>& GetAnimations();

public:
	void UpdateData();
	virtual void SaveToScene( FILE* pFile );
	virtual void LoadFromScene( FILE* pFile );


};

