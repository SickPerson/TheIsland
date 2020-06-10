#include "stdafx.h"
#include "MeshRender.h"

#include "Transform.h"
#include "ResMgr.h"
#include "Animator3D.h"
//#include <iostream>

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

		//std::cout << GetObj()->GetName().c_str() << std::endl;
		m_vecMtrl[i]->UpdateData();
		m_pMesh->Render((UINT)i);
	}
}

void CMeshRender::Render_Shadowmap()
{
	int a = 1;
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ShadowMapMtrl");
	pMtrl->SetData(SHADER_PARAM::TEX_0, m_vecMtrl[0]->GetTexture().GetPointer());

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
	}

	// 정리
	if (Animator3D())
	{
		a = 0;
		pMtrl->SetData(SHADER_PARAM::INT_0, &a);
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

void CMeshRender::SaveToScene(FILE * _pFile)
{
}

void CMeshRender::LoadFromScene(FILE * _pFile)
{
}