#pragma once
#include "Component.h"

#include "Ptr.h"
#include "Texture.h"

class CAnimator3D :
	public CComponent
{
public:
	CAnimator3D();
	~CAnimator3D();

	CAnimator3D* Clone();

private:
	const vector<tMTBone>*			m_vecBones;
	const vector<tMTAnimClip>*		m_vecClip;
	
	vector<float>					m_vecClipUpdateTime;
	vector<Matrix>					m_vecFinalBoneMat;
	int								m_iFrameCount;
	double							m_dCurTime;
	int								m_iCurClip;
	Ptr<CTexture>					m_pBoneTex;

public:
	void SetBones( const vector<tMTBone>* vecBones );
	void SetAnimClip( const vector<tMTAnimClip>* vecAnimClip );
	void SetBoneTex( Ptr<CTexture> pBoneTex );
	Ptr<CTexture> GetBornTex();

	void UpdateData();

	void SetClipTime( int iClipIdx, float fTime );

	vector<Matrix>& GetFinalBoneMat();

public:
	virtual void Awake() {};
	virtual void Start() {};
	virtual void Update();
	virtual void LateUpdate();
	virtual void FinalUpdate();

public:
	virtual void SaveToScene( FILE* pFile );
	virtual void LoadFromScene( FILE* pFile );
};

