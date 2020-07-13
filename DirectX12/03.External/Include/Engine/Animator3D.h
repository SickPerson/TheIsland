#pragma once
#include "Component.h"

#include "Ptr.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"

class CStructuredBuffer;

class CAnimator3D :
	public CComponent
{
public:
	CAnimator3D();
	~CAnimator3D();

	CAnimator3D* Clone();

private:
	const vector<tMTBone>*		m_pVecBones;
	const vector<tMTAnimClip>*	m_pVecClip;

	vector<float>					m_vecClipUpdateTime;
	vector<Matrix>				m_vecFinalBoneMat; // 텍스쳐에 전달할 최종 행렬정보
	int										m_iFrameCount; // 30
	double								m_dCurTime;
	int										m_iCurClip; // 클립 인덱스	
	int										m_iFrameIdx; // 클립의 현재 프레임

	Ptr<CMaterial>				m_pBoneMtrl; // BoneData Update Compute Shader
	CStructuredBuffer*		m_pBoneFinalMat;  // 특정 프레임의 최종 행렬
	bool									m_bFinalMatUpdate; // 최종행렬 연산 수행여부

public:
	void SetBones( const vector<tMTBone>* vecBones );
	void SetAnimClip( const vector<tMTAnimClip>* vecAnimClip );
	//void SetBoneTex( Ptr<CTexture> pBoneTex );
	//Ptr<CTexture> GetBornTex();

	void UpdateData();

	void SetClipTime( int iClipIdx, float fTime );

	vector<Matrix>& GetFinalBoneMat();

private:
	void Check_Mesh( Ptr<CMesh> pMesh );

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

