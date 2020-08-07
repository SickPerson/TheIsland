#pragma once
#include "Component.h"

#include "Ptr.h"
#include "Texture.h"
#include "Material.h"
#include "Mesh.h"

enum class ANIMATION_MODE
{
	LOOP,
	ONCE_RETURN,
	ONCE_STOP,
	END,
};

struct tAnimation
{
	int			iStartFrame;
	int			iEndFrame;
	int			iFrameLength;

	double		dStartTime;
	double		dEndTime;
	double		dTimeLength;

	ANIMATION_MODE	eMode;
};

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

	vector<float>				m_vecClipUpdateTime;
	vector<Matrix>				m_vecFinalBoneMat; // 텍스쳐에 전달할 최종 행렬정보
	int							m_iFrameCount; // 30
	double						m_dCurTime;
	int							m_iCurClip; // 클립 인덱스	
	int							m_iFrameIdx; // 클립의 현제 프레임

	Ptr<CMaterial>				m_pBoneMtrl; // BoneData Update Compute Shader
	CStructuredBuffer*			m_pBoneFinalMat;  // 특정 프레임의 최종 행렬
	bool						m_bFinalMatUpdate; // 최종행렬 연산 수행여부

	wstring						m_strCurAniKey;
	wstring						m_strDefaultKey;
	map<wstring, tAnimation*>	m_mapClip;
	map<wstring, float>			m_mapClipUpdateTime;

	bool						m_bStop;			// 애니메이션을 그만 돌릴건지

public:
	void SetBones(const vector<tMTBone>* _vecBones) { m_pVecBones = _vecBones; m_vecFinalBoneMat.resize(m_pVecBones->size()); }
	void SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip);
	void UpdateData();
	void UpdateData_Inst(CStructuredBuffer* _pBoneBuffer, UINT _iRow);
	void SetClipTime(int _iClipIdx, float _fTime) { m_vecClipUpdateTime[_iClipIdx] = _fTime; }

	void AddClip( const wstring& strKey, int iStartFrame, int iEndFrame, ANIMATION_MODE eMode );

	CStructuredBuffer* GetFinalBoneMat() { return m_pBoneFinalMat; }
	UINT GetBoneCount() { return (UINT)m_pVecBones->size(); }

	void ChangeAnimation( const wstring& strKey );
	void SetDefaultKey( const wstring& strKey );			// 이걸 설정해줘야만 처음 클립이 뭔지 암.
	void Restart(const wstring& strKey = L"");				// 파라미터에 아무것도 안넣으면 기본적으로 디폴트 애니메이션으로 재시작함

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

