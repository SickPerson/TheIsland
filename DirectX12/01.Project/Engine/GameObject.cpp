#include "stdafx.h"
#include "GameObject.h"
#include "Component.h"
#include "Layer.h"
#include "Scene.h"
#include "SceneManager.h"

CGameObject::CGameObject() : 
	m_arrCom{ },
	m_pParentObj(NULL),
	m_iLayerIdx(-1),
	m_bDead(false),
	m_bActive(true),
	m_bFrustumCheck(true)
{
}

CGameObject::CGameObject( const CGameObject & obj ) : 
	CEntity(obj),
	m_arrCom{ },
	m_pParentObj( NULL ),
	m_iLayerIdx( -1 ),
	m_bDead( false ),
	m_bActive( true ),
	m_bFrustumCheck( obj.m_bFrustumCheck )
{
	for ( UINT i = 0; i < ( UINT )COMPONENT_TYPE::END; ++i )
	{
		if ( NULL != obj.m_arrCom[i] )
			AddComponent( obj.m_arrCom[i]->Clone() );
	}
}

CGameObject::~CGameObject()
{
	Safe_Delete_Array( m_arrCom );
	Safe_Delete_Vector( m_vecChild );
	Safe_Delete_Vector( m_vecScript );
}

void CGameObject::SetLayerIdx( int iLayerIdx )
{
}

void CGameObject::Awake()
{
}

void CGameObject::Start()
{
}

void CGameObject::Update()
{
}

void CGameObject::LateUpdate()
{
}

void CGameObject::FinalUpdate()
{
}

void CGameObject::Enable()
{
}

void CGameObject::Disable()
{
}

void CGameObject::SetActive( bool bActive )
{
	m_bActive = bActive;
}

bool CGameObject::IsActive() const
{
	return m_bActive;
}

void CGameObject::FrustumCheck( bool bFrustumCheck )
{
	m_bFrustumCheck = bFrustumCheck;
}

bool CGameObject::GetFrustumCheck() const
{
	return m_bFrustumCheck;
}

bool CGameObject::IsDead()
{
	return false;
}

void CGameObject::SetDead()
{
}

int CGameObject::GetLayerIdx()
{
	return m_iLayerIdx;
}

void CGameObject::AddComponent( CComponent * pCom )
{
}

CComponent * CGameObject::GetComponent( COMPONENT_TYPE eType )
{
	return nullptr;
}

CTransform * CGameObject::Transform()
{
	return nullptr;
}

CMeshRender * CGameObject::MeshRender()
{
	return nullptr;
}

CCollider2D * CGameObject::Collider2D()
{
	return nullptr;
}

CCollider3D * CGameObject::Collider3D()
{
	return nullptr;
}

CAnimation2D * CGameObject::Animation2D()
{
	return nullptr;
}

CCamera * CGameObject::Camera()
{
	return nullptr;
}

CLight2D * CGameObject::Light2D()
{
	return nullptr;
}

CLight3D * CGameObject::Light3D()
{
	return nullptr;
}

const vector<CScript*>& CGameObject::GetScripts() const
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vecScript;
}

void CGameObject::AddChild( CGameObject * pChildObj )
{// 예외 1
	// 자기자신이 자식이 되는 경우
	assert( this != pChildObj );

	// 예외 2.
	// 자식으로 들어오는 오브젝트가, 부모계층 오브젝트인 경우
	assert( !IsAncestor( pChildObj ) );

	// 예외 3.
	// 자식 오브젝트가 이전에 다른 부모 오브젝트가 있었다면
	pChildObj->ClearParent( this );

	// 자식 오브젝트가 Layer 에 포함되어이지 않는 경우
	// 부모 오브젝트의 Layer로 지정한다.
	if ( -1 == pChildObj->m_iLayerIdx )
	{
		pChildObj->m_iLayerIdx = m_iLayerIdx;
	}
}

bool CGameObject::IsAncestor( CGameObject * pObj )
{
	CGameObject* pParent = m_pParentObj;

	while ( NULL != pParent )
	{
		if ( pParent == pObj )
			return true;

		pParent = pParent->m_pParentObj;
	}

	return false;
}

void CGameObject::ClearParent( CGameObject * pNextParent )
{
	// 부모가 없는 경우
	if ( NULL == m_pParentObj )
	{
		// 다음 부모가 지정된 경우
		if ( pNextParent )
		{
			pNextParent->m_vecChild.push_back( this );
			m_pParentObj = pNextParent;
		}

		else // 부모도 없고, 다음 부모도 없고
			return;

		// 최상위 부모 오브젝트, 다음 부모가 지정됨 (Layer ParentList에서 빠짐)
		if ( -1 != m_iLayerIdx )
		{
			CLayer* pCurLayer = GET_SINGLE( CSceneManager )->GetCurScene()->GetLayer( m_iLayerIdx );
		}
	}
}

CGameObject * CGameObject::GetParent()
{
	return nullptr;
}

const vector<CGameObject*>& CGameObject::GetChild()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vecChild;
}

void CGameObject::RegisterToLayer()
{
}
