#include "stdafx.h"
#include "Script.h"
#include "EventManager.h"

CScript::CScript( UINT iScriptType ) :
	CComponent(COMPONENT_TYPE::SCRIPT),
	m_iScriptType(iScriptType)
{
}


CScript::~CScript()
{
}

void CScript::CreateObject( CGameObject * pNewObject, int iLayerIdx )
{
	tEvent evt{};

}

void CScript::CreateObject( CGameObject * pNewObject, const wstring & strLayerName )
{
}

void CScript::DeleteObject( CGameObject * pDeleteObject )
{
}

void CScript::AddChild( CGameObject * pChildObject )
{
}

void CScript::AddChild( CGameObject * pParent, CGameObject * pChild )
{
}

void CScript::ClearParent()
{
}

void CScript::ClearParent( CGameObject * pTarget )
{
}

void CScript::TransferLayer( const wstring & strLayerName, bool bMoveAll )
{
}

void CScript::TransferLayer( int iLayerIdx, bool bMoveAll )
{
}

void CScript::TransferLayer( CGameObject * pTarget, const wstring & strLayerName, bool bMoveAll )
{
}

void CScript::TransferLayer( CGameObject * pTarget, int iLayerIdx, bool bMoveAll )
{
}

void CScript::EableObject( CGameObject * pTarget )
{
}

void CScript::Disable( CGameObject * pTarget )
{
}

void CScript::OnCollisionEnter( CCollider2D * pOther )
{
}

void CScript::OnCollision( CCollider2D * pOther )
{
}

void CScript::OnCollisionExit( CCollider2D * pOther )
{
}

void CScript::SaveToScene( FILE * pFile )
{
}

void CScript::LoadFromScene( FILE * pFile )
{
}
