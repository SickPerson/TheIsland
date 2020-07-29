#include "stdafx.h"
#include "Animator3D.h"

#include "TimeMgr.h"
#include "ResMgr.h"
#include "MeshRender.h"

#include "StructuredBuffer.h"

CAnimator3D::CAnimator3D() :
	m_iCurClip(0),
	m_dCurTime(0.f),
	m_iFrameCount(30),
	CComponent(COMPONENT_TYPE::ANIMATOR3D),
	m_pBoneFinalMat(NULL),
	m_bFinalMatUpdate(false)
{
	m_pBoneMtrl = CResMgr::GetInst()->FindRes<CMaterial>( L"Animation3DUpdateMtrl" );
	m_pBoneFinalMat = new CStructuredBuffer;
}	


CAnimator3D::~CAnimator3D()
{
	SAFE_DELETE( m_pBoneFinalMat );
}

CAnimator3D * CAnimator3D::Clone()
{
	return new CAnimator3D(*this);
}

void CAnimator3D::SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip)
{
	m_pVecClip = _vecAnimClip;
	m_vecClipUpdateTime.resize(m_pVecClip->size());


	static float fTime = 0.f;
	fTime += 1.f;
	m_vecClipUpdateTime[0] = fTime;
}

void CAnimator3D::UpdateData()
{
	if (!m_bFinalMatUpdate)
	{
		// Bone Data Update	
		Ptr<CMesh> pMesh = MeshRender()->GetMesh();
		pMesh->GetBoneFrameDataBuffer()->UpdateData_CS(TEXTURE_REGISTER::t10);
		pMesh->GetBoneOffsetBuffer()->UpdateData_CS(TEXTURE_REGISTER::t11);

		Check_Mesh(pMesh);
		m_pBoneFinalMat->UpdateRWData(UAV_REGISTER::u0);

		UINT iBoneCount = (UINT)m_pVecBones->size();
		m_pBoneMtrl->SetData(SHADER_PARAM::INT_0, &iBoneCount);
		m_pBoneMtrl->SetData(SHADER_PARAM::INT_1, &m_iFrameIdx);

		UINT iGrounX = (iBoneCount / 256) + 1;
		m_pBoneMtrl->Dispatch(iGrounX, 1, 1);

		m_bFinalMatUpdate = true;
	}
}

void CAnimator3D::UpdateData_Inst(CStructuredBuffer * _pBoneBuffer, UINT _iRow)
{
	if (!m_bFinalMatUpdate)
	{
		// Bone Data Update	
		Ptr<CMesh> pMesh = MeshRender()->GetMesh();
		pMesh->GetBoneFrameDataBuffer()->UpdateData_CS(TEXTURE_REGISTER::t10);
		pMesh->GetBoneOffsetBuffer()->UpdateData_CS(TEXTURE_REGISTER::t11);

		Check_Mesh(pMesh);
		_pBoneBuffer->UpdateRWData(UAV_REGISTER::u0);

		UINT iBoneCount = (UINT)m_pVecBones->size();
		m_pBoneMtrl->SetData(SHADER_PARAM::INT_0, &iBoneCount);
		m_pBoneMtrl->SetData(SHADER_PARAM::INT_1, &m_iFrameIdx);
		m_pBoneMtrl->SetData(SHADER_PARAM::INT_3, &_iRow);

		UINT iGrounX = (iBoneCount / 256) + 1;
		m_pBoneMtrl->Dispatch(iGrounX, 1, 1);

		m_bFinalMatUpdate = true;
	}
}

void CAnimator3D::Check_Mesh( Ptr<CMesh> pMesh )
{
	UINT iBoneCount = pMesh->GetBoneCount();
	if ( m_pBoneFinalMat->GetElementCount() < iBoneCount )
	{
		m_pBoneFinalMat->Create( sizeof( Matrix ), iBoneCount, nullptr );
	}
}

void CAnimator3D::Update()
{
}

void CAnimator3D::LateUpdate()
{
}

void CAnimator3D::FinalUpdate()
{
	m_dCurTime = 0.f;

	// 현재 재생중인 Clip 의 시간을 진행한다.
	m_vecClipUpdateTime[m_iCurClip] += DT;

	if ( m_vecClipUpdateTime[m_iCurClip] >= m_pVecClip->at( m_iCurClip ).dTimeLength )
	{
		m_vecClipUpdateTime[m_iCurClip] = 0.f;
	}

	m_dCurTime = m_pVecClip->at( m_iCurClip ).dStartTime + m_vecClipUpdateTime[m_iCurClip];

	m_iFrameIdx = ( int )( m_dCurTime * ( float )m_iFrameCount );

	m_bFinalMatUpdate = false;
}

void CAnimator3D::SaveToScene( FILE * pFile )
{
}

void CAnimator3D::LoadFromScene( FILE * pFile )
{
}
