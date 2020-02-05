#include "stdafx.h"
#include "Collider2D.h"
#include "Material.h"
#include "ResourceManager.h"
#include "Transform.h"
#include "Device.h"
#include "ConstantBuffer.h"
#include "Mesh.h"
#include "ResourceManager.h"
#include "Script.h"
#include "GameObject.h"

UINT	CCollider2D::g_iColID = 0;

CCollider2D::CCollider2D()	:
	CComponent(COMPONENT_TYPE::COLLIDER2D),
	m_vOffsetPos(Vec3::Zero),
	m_vOffsetScale(Vec3::One),
	m_pColMesh(NULL),
	m_pColMtrl(NULL),
	m_iColID(g_iColID++),
	m_iCollisionCount(0),
	m_eType(COLLIDER2D_TYPE::RECT)
{
	m_pColMtrl = GET_SINGLE( CResourceManager )->FindRes<CMaterial>( L"Collider2DMtrl_0" );
	SetCollider2DType( m_eType );
}

CCollider2D::CCollider2D( const CCollider2D & other ) :
	CComponent( COMPONENT_TYPE::COLLIDER2D ),
	m_vOffsetPos( other.m_vOffsetPos ),
	m_vOffsetScale( other.m_vOffsetScale ),
	m_pColMesh( other.m_pColMesh ),
	m_pColMtrl( other.m_pColMtrl ),
	m_iColID( g_iColID++ ),
	m_iCollisionCount( 0 ),
	m_eType( other.m_eType ),
	m_matColWorld( other.m_matColWorld )
{
}

CCollider2D::~CCollider2D()
{
}

void CCollider2D::Update()
{
}

void CCollider2D::FinalUpdate()
{
	if ( !IsActive() )
		return;

	Vec3 vFinalPos = m_vOffsetPos;
	vFinalPos = vFinalPos / Transform()->GetWorldScale();

	Matrix matTranslation = XMMatrixTranslation( vFinalPos.x, vFinalPos.y, vFinalPos.z );
	Matrix matScale = XMMatrixScaling( m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z );

	m_matColWorld = matScale * matTranslation;
	m_matColWorld *= Transform()->GetWorldMat();
}

void CCollider2D::Render()
{
	if ( !IsActive() )
		return;

	static CConstantBuffer* pCB = GET_SINGLE( CDevice )->GetConstBuffer( CONST_REGISTER::b0 );

	g_tTransform.matWorld = m_matColWorld;
	GET_SINGLE( CDevice )->SetConstBufferToRegister( pCB, pCB->AddData( &g_tTransform ) );

	m_pColMtrl->UpdateData();
	m_pColMesh->Render();

	memset( &m_matColWorld, 0, sizeof( Matrix ) );
}

void CCollider2D::OnCollisionEnter( CCollider2D * pOther )
{
	m_iCollisionCount += 1;

	const vector<CScript*>& vecScripts = GetGameObject()->GetScripts();

	for ( size_t i = 0; i < vecScripts.size(); ++i )
	{
		vecScripts[i]->OnCollisionEnter( pOther );
	}
}

void CCollider2D::OnCollision( CCollider2D * pOther )
{
	if ( 0 < m_iCollisionCount )
	{
		m_pColMtrl = GET_SINGLE( CResourceManager )->FindRes<CMaterial>( L"Collider2DMtrl_1" );
	}

	const vector<CScript*>& vecScripts = GetGameObject()->GetScripts();

	for ( size_t i = 0; i < vecScripts.size(); ++i )
	{
		vecScripts[i]->OnCollision( pOther );
	}
}

void CCollider2D::OnCollisionExit( CCollider2D * pOther )
{
	m_iCollisionCount -= 1;

	if ( m_iCollisionCount == 0 )
		m_pColMtrl = GET_SINGLE( CResourceManager )->FindRes<CMaterial>( L"Collider2DMtrl_0" );

	const vector<CScript*>& vecScripts = GetGameObject()->GetScripts();
	
	for ( size_t i = 0; i < vecScripts.size(); ++i )
	{
		vecScripts[i]->OnCollisionExit( pOther );
	}
}

void CCollider2D::SetOffsetPos( const Vec3 & vPos )
{
	m_vOffsetPos = vPos;
}

void CCollider2D::SetOffsetScale( const Vec3 & vScale )
{
	m_vOffsetScale = vScale;
}

void CCollider2D::SetCollider2DType( COLLIDER2D_TYPE eType )
{
	m_eType = eType;
}

const Vec3 & CCollider2D::GetOffsetPos()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vOffsetPos;
}

const Vec3 & CCollider2D::GetOffsetScale()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vOffsetScale;
}

COLLIDER2D_TYPE CCollider2D::GetColliderType()
{
	return m_eType;
}

UINT CCollider2D::GetColID()
{
	return m_iColID;
}

const Matrix & CCollider2D::GetColliderWorldMat()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_matColWorld;
}

void CCollider2D::SaveToScene( FILE * pFile )
{
	UINT iType = ( UINT )GetComponentType();
	fwrite( &iType, sizeof( UINT ), 1, pFile );

	fwrite( &m_vOffsetPos, sizeof( Vec3 ), 1, pFile );
	fwrite( &m_vOffsetScale, sizeof( Vec3 ), 1, pFile );
	fwrite( &m_eType, sizeof( UINT ), 1, pFile );
}

void CCollider2D::LoadFromScene( FILE * pFile )
{
	fread( &m_vOffsetPos, sizeof( Vec3 ), 1, pFile );
	fread( &m_vOffsetScale, sizeof( Vec3 ), 1, pFile );
	fread( &m_eType, sizeof( UINT ), 1, pFile );
	SetCollider2DType( m_eType );
}

void CCollider2D::operator=( const CCollider2D & other )
{
	UINT iColID = m_iColID;
	*this = other;
	m_iColID = iColID;
}
