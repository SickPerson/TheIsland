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
	m_bFinalMatUpdate(false),
	m_bStop(false)
{
	m_pBoneMtrl = CResMgr::GetInst()->FindRes<CMaterial>( L"Animation3DUpdateMtrl" );
	m_pBoneFinalMat = new CStructuredBuffer;
}	


CAnimator3D::~CAnimator3D()
{
	SAFE_DELETE( m_pBoneFinalMat );

	Safe_Delete_Map( m_mapClip );
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
		UINT iRow = 0;

		m_pBoneMtrl->SetData(SHADER_PARAM::INT_0, &iBoneCount);
		m_pBoneMtrl->SetData(SHADER_PARAM::INT_1, &m_iFrameIdx);
		m_pBoneMtrl->SetData(SHADER_PARAM::INT_2, &m_iNextFrameIdx);
		m_pBoneMtrl->SetData(SHADER_PARAM::INT_3, &iRow);
		m_pBoneMtrl->SetData(SHADER_PARAM::FLOAT_0, &m_fRatio);

		UINT iGrounX = (iBoneCount / 256) + 1;
		m_pBoneMtrl->Dispatch(iGrounX, 1, 1);

		m_bFinalMatUpdate = true;
	}

	// t7 레지스터에 최종행렬 데이터(구조버퍼) 바인딩
	m_pBoneFinalMat->UpdateData(TEXTURE_REGISTER::t7);
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
		m_pBoneMtrl->SetData(SHADER_PARAM::FLOAT_0, &m_fRatio);

		UINT iGrounX = (iBoneCount / 256) + 1;
		m_pBoneMtrl->Dispatch(iGrounX, 1, 1);

		m_bFinalMatUpdate = true;
	}
}

void CAnimator3D::AddClip( const wstring & strKey, int iStartFrame, int iEndFrame, ANIMATION_MODE eMode )
{
	tAnimation* pAnimClip = new tAnimation;

	pAnimClip->iStartFrame = iStartFrame;
	pAnimClip->iEndFrame = iEndFrame;
	pAnimClip->iFrameLength = iEndFrame - iStartFrame + 1;

	pAnimClip->dStartTime = ( double )iStartFrame / m_iFrameCount;
	pAnimClip->dEndTime = ( double )iEndFrame / m_iFrameCount;
	pAnimClip->dTimeLength = pAnimClip->dEndTime - pAnimClip->dStartTime;

	pAnimClip->eMode = eMode;

	m_mapClip[strKey] = pAnimClip;

	m_mapClipUpdateTime[strKey] = 0.f;
}

void CAnimator3D::ChangeAnimation( const wstring & strKey )
{
	if ( m_strCurAniKey == strKey )
		return;
	m_mapClipUpdateTime[m_strCurAniKey] = 0.f;
	m_strCurAniKey = strKey;
}

void CAnimator3D::SetDefaultKey( const wstring & strKey )
{
	m_strDefaultKey = strKey;

	m_strCurAniKey = strKey;
}

void CAnimator3D::Restart( const wstring& strKey )
{
	m_bStop = false;

	if ( strKey == L"" )
		m_strCurAniKey = m_strDefaultKey;

	else
		m_strCurAniKey = strKey;
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
	m_mapClipUpdateTime[m_strCurAniKey] += DT;

	if ( m_mapClipUpdateTime[m_strCurAniKey] >= m_mapClip[m_strCurAniKey]->dTimeLength )
	{
		if ( m_mapClip[m_strCurAniKey]->eMode == ANIMATION_MODE::LOOP )
		{
			m_mapClipUpdateTime[m_strCurAniKey] = 0.f;
		}

		else if ( m_mapClip[m_strCurAniKey]->eMode == ANIMATION_MODE::ONCE_RETURN )
		{
			m_mapClipUpdateTime[m_strCurAniKey] = 0.f;
			m_strCurAniKey = m_strDefaultKey;
		}

		else if ( m_mapClip[m_strCurAniKey]->eMode == ANIMATION_MODE::ONCE_STOP )
		{
			m_bStop = true;
		}
	}

	if ( !m_bStop )
	{
		m_dCurTime = m_mapClip[m_strCurAniKey]->dStartTime + m_mapClipUpdateTime[m_strCurAniKey];
	}
	else
	{
		m_dCurTime = m_mapClip[m_strCurAniKey]->dEndTime;
	}

	// 현재 프레임 인덱스 구하기
	double dFrameIdx = m_dCurTime * ( double )m_iFrameCount;
	m_iFrameIdx = ( int )( dFrameIdx );

	// 다음 프레임 인덱스
	if ( m_iFrameIdx >= m_mapClip[m_strCurAniKey]->iFrameLength - 1 )
		m_iNextFrameIdx = m_iFrameIdx;	// 끝이면 현재 인덱스를 유지
	else
		m_iNextFrameIdx = m_iFrameIdx + 1;

	// 프레임간의 시간에 따른 비율을 구해준다.
	m_fRatio = ( float )( dFrameIdx - ( double )m_iFrameIdx );

	// 컴퓨트 쉐이더 연산여부
	m_bFinalMatUpdate = false;
}

void CAnimator3D::SaveToScene( FILE * pFile )
{
}

void CAnimator3D::LoadFromScene( FILE * pFile )
{
}
