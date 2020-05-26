#include "stdafx.h"
#include "StoneScript.h"

#include <Engine/GameObject.h>
#include <Engine/Ptr.h>
#include <Engine/MeshData.h>
#include <Engine/Transform.h>

CStoneScript::CStoneScript(): 
	CScript( ( UINT )SCRIPT_TYPE::WORLDSCRIPT ),
	m_pGameObject(NULL)
{
}


CStoneScript::~CStoneScript()
{
}

void CStoneScript::OnCollisionEnter( CCollider2D * _pOther )
{
}

void CStoneScript::OnCollision( CCollider2D * _pOther )
{
}

void CStoneScript::OnCollisionExit( CCollider2D * _pOther )
{
}

void CStoneScript::Update()
{
}

void CStoneScript::SetGameObject( CGameObject * pGameObject )
{
	m_pGameObject = pGameObject;
}

void CStoneScript::SetFBXName( const wstring & strFBXName )
{
	m_strFBXName = strFBXName;

	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>( strFBXName, strFBXName );

	m_pGameObject = pMeshData->Instantiate();
	m_pGameObject->SetName( m_strFBXName );
}

void CStoneScript::SetTransform( CTransform * pTransform )
{
	Vec3 vPos = pTransform->GetLocalPos();
	Vec3 vScale = pTransform->GetLocalScale();
	Vec3 vRotate = pTransform->GetLocalRot();

	m_pGameObject->Transform()->SetLocalPos( vPos );
	m_pGameObject->Transform()->SetLocalScale( vScale );
	m_pGameObject->Transform()->SetLocalRot( vRotate );
}

void CStoneScript::Save( FILE * pFile )
{
}

void CStoneScript::Load( FILE * pFile )
{
}
