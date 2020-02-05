#include "stdafx.h"
#include "Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}


CLayer::~CLayer()
{
	Safe_Delete_Vector( m_vecParentObj );
}

void CLayer::AddGameObject( CGameObject * pObject, bool bMoveAll )
{
	if ( !pObject->GetParent() )
	{

	}
}

int CLayer::GetLayerIdx() const
{
	return 0;
}

const vector<CGameObject*>& CLayer::GetParentObj() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vecParentObj;
}

const vector<CGameObject*>& CLayer::GetObjects() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vecLayerObj;
}
	
void CLayer::RegisterObject( CGameObject * pObj )
{
}

void CLayer::CheckParentObject()
{
}

void CLayer::RemoveParentObject( CGameObject * pObject )
{
}

void CLayer::Awake()
{
}

void CLayer::Start()
{
}

void CLayer::Update()
{
}

void CLayer::LateUpdate()
{
}

void CLayer::FinalUpdate()
{
}

void CLayer::Render()
{
}

void CLayer::SetLayerIdx( int iIdx )
{
}
