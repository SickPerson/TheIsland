#include "stdafx.h"
#include "MeshRender.h"

#include "Transform.h"
#include "ResMgr.h"
#include "Animator3D.h"

CMeshRender::CMeshRender()
	: CComponent(COMPONENT_TYPE::MESHRender)
	, m_bDynamicShadow(false)
{
	m_vecMtrl.resize( 1 );
}

CMeshRender::~CMeshRender()
{
}


void CMeshRender::Render()
{
	if (IsActive() == false || nullptr == m_pMesh )
		return;

	for (size_t i = 0; i < m_vecMtrl.size(); ++i)
	{
		if (NULL == m_vecMtrl[i] || NULL == m_vecMtrl[i]->GetShader())
			continue;

		Transform()->UpdateData();

		// Animator3D 컴포넌트가 있는 경우...
		if (Animator3D())
		{
			Animator3D()->UpdateData();
			int a = 1;
			m_vecMtrl[i]->SetData(SHADER_PARAM::INT_0, &a); // Animation Mesh 알리기
		}

		m_vecMtrl[i]->UpdateData();
		m_pMesh->Render((UINT)i);

		// 정리
		if (Animator3D())
		{
			int a = 0;
			m_vecMtrl[i]->SetData(SHADER_PARAM::INT_0, &a);
		}
	}
}

void CMeshRender::Render(UINT _iMtrlIdx)
{
	if (IsActive() == false || nullptr == m_pMesh)
		return;

	int a = 1;

	if (nullptr == m_vecMtrl[_iMtrlIdx] || nullptr == m_vecMtrl[_iMtrlIdx]->GetShader())
		return;

	// Transform 정보 업데이트
	Transform()->UpdateData();

	// Animator3D 컴포넌트가 있는 경우...
	if (Animator3D())
	{
		Animator3D()->UpdateData();
		Animator3D()->GetFinalBoneMat()->UpdateData(TEXTURE_REGISTER::t7); // t7 레지스터에 최종행렬 데이터(구조버퍼) 바인딩

		a = 1;
		m_vecMtrl[_iMtrlIdx]->SetData(SHADER_PARAM::INT_0, &a); // Animation Mesh 알리기
	}

	m_vecMtrl[_iMtrlIdx]->UpdateData();
	m_pMesh->Render((UINT)_iMtrlIdx);

	// 정리
	if (Animator3D())
	{
		a = 0;
		m_vecMtrl[_iMtrlIdx]->SetData(SHADER_PARAM::INT_0, &a);
	}
}

void CMeshRender::Render_Shadowmap()
{
	int a = 1;
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ShadowMapMtrl");
	// pMtrl->SetData(SHADER_PARAM::TEX_0, m_vecMtrl[0]->GetTexture().GetPointer());

	for (UINT i = 0; i < m_pMesh->GetSubsetCount(); ++i)
	{
		if (Animator3D())
		{
			Animator3D()->UpdateData();
			pMtrl->SetData(SHADER_PARAM::INT_0, &a); // Animation Mesh 알리기
		}

		Transform()->UpdateData();
		pMtrl->UpdateData();
		m_pMesh->Render(i);

		// 정리
		if (Animator3D())
		{
			a = 0;
			pMtrl->SetData(SHADER_PARAM::INT_0, &a);
		}
	}
}

Ptr<CMaterial> CMeshRender::GetCloneMaterial( UINT iSubset )
{
	if ( nullptr == m_vecMtrl[iSubset] )
		return nullptr;

	m_vecMtrl[iSubset] = m_vecMtrl[iSubset]->Clone();

	return m_vecMtrl[iSubset];
}

void CMeshRender::SetMaterial( Ptr<CMaterial> _pMtrl, UINT iSubset )
{
	if ( m_vecMtrl.size() <= ( size_t )iSubset )
		m_vecMtrl.resize( iSubset + 1 );

	m_vecMtrl[iSubset] = _pMtrl;
}

ULONG64 CMeshRender::GetInstID(UINT _iMtrlIdx)
{
	if (m_pMesh == NULL || m_vecMtrl[_iMtrlIdx] == NULL)
		return 0;

	uInstID id{ m_pMesh->GetID(), (WORD)m_vecMtrl[_iMtrlIdx]->GetID(), (WORD)_iMtrlIdx };
	return id.llID;
}

void CMeshRender::SaveToScene(FILE * _pFile)
{
}

void CMeshRender::LoadFromScene(FILE * _pFile)
{
}