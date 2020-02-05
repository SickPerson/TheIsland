#include "stdafx.h"
#include "Transform.h"
#include "GameObject.h"
#include "ConstantBuffer.h"
#include "Device.h"

tTransform g_tTransform;

CTransform::CTransform() :
	CComponent( COMPONENT_TYPE::TRANSFORM ),
	m_vLocalPos( Vec3::Zero ),
	m_vLocalRot( Vec3::Zero ),
	m_vLocalScale( Vec3::One )
{
}

CTransform::~CTransform()
{
}

const Vec3 & CTransform::GetLocalPos()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vLocalPos;
}

Vec3 CTransform::GetWorldPos()
{
	return m_matWorld.Translation();
}

const Vec3 & CTransform::GetLocalScale()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vLocalScale;
}

Vec3 CTransform::GetWorldScale()
{
	CGameObject* pParent = GetGameObject()->GetParent();
	Vec3 vWorldScale = m_vLocalScale;

	while ( pParent )
	{
		vWorldScale *= pParent->Transform()->GetLocalScale();

		pParent = pParent->GetParent();
	}

	return vWorldScale;
}

const Vec3 & CTransform::GetLocalRot()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vLocalRot;
}

const Matrix & CTransform::GetWorldMat()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_matWorld;
}

const Vec3 & CTransform::GetLocalDir( DIR_TYPE eType )
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vLocalDir[( UINT )eType];
}

const Vec3 & CTransform::GetWorldDir( DIR_TYPE eType )
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_vWorldDir[( UINT )eType];
}

void CTransform::SetLocalPos( const Vec3 & vPos )
{
	m_vLocalPos = vPos;
}

void CTransform::SetLocalScale( const Vec3 & vScale )
{
	m_vLocalScale = vScale;
}

void CTransform::SetLocalRot( const Vec3 & vRot )
{
	m_vLocalRot = vRot;
}

bool CTransform::IsCasting( const Vec3 & vPos )
{
	Vec3 vWorldPos = GetWorldPos();
	Vec3 vWorldScale = GetWorldScale();

	if ( vWorldPos.x - vWorldScale.x / 2.f < vPos.x &&
		vPos.x < vWorldPos.x + vWorldScale.x / 2.f &&
		vWorldPos.y - vWorldScale.y / 2.f < vPos.y &&
		vPos.y < vWorldPos.y + vWorldScale.y / 2.f )
		return true;

	return false;
}

float CTransform::GetMaxScale()
{
	Vec3 vWorldScale = GetWorldScale();
	float fMax = max( vWorldScale.x, vWorldScale.y );
	fMax = max( fMax, vWorldScale.z );

	return fMax;
}

void CTransform::UpdateDate()
{
	static CConstantBuffer* pCB = GET_SINGLE( CDevice )->GetConstBuffer( CONST_REGISTER::b0 );

	g_tTransform.matWorld = m_matWorld;
	g_tTransform.matWV = g_tTransform.matWorld * g_tTransform.matView;
	g_tTransform.matWVP = g_tTransform.matWV * g_tTransform.matProj;

	UINT iOffsetPos = pCB->AddData( &g_tTransform );
	GET_SINGLE( CDevice )->SetConstBufferToRegister( pCB, iOffsetPos );
}

void CTransform::FinalUpdate()
{
	// 렌더링하기 직전에 Transform이 업데이트 되는데 업데이트 전에 월드행렬이 계산되어야함. 
	// 그래서 FianlUpdate에서 월드행렬 계산

	Matrix matTranslation = XMMatrixTranslation( m_vLocalPos.x, m_vLocalPos.y, m_vLocalPos.z );
	Matrix matScale = XMMatrixScaling( m_vLocalScale.x, m_vLocalScale.y, m_vLocalScale.z );
	Matrix matRot = XMMatrixRotationX( m_vLocalRot.x );
	matRot *= XMMatrixRotationY( m_vLocalRot.y );
	matRot *= XMMatrixRotationZ( m_vLocalRot.z );

	static Vec3 arrDefault[( UINT )DIR_TYPE::END] = { Vec3::Right, Vec3::Up, Vec3::Front };

	for ( UINT i = 0; i < ( UINT )DIR_TYPE::END; ++i )
	{
		m_vLocalDir[i] = XMVector3TransformNormal( arrDefault[i], matRot );
	}

	// 월드행렬 = 로컬 x (크기 x 회전 x 이동)
	m_matWorld = matScale * matRot * matTranslation;

	if ( GetGameObject()->GetParent() )
	{
		m_matWorld *= GetGameObject()->GetParent()->Transform()->GetWorldMat();

		for ( UINT i = 0; i < ( UINT )DIR_TYPE::END; ++i )
		{
			m_vWorldDir[i] = XMVector3TransformNormal( arrDefault[i], m_matWorld );
			m_vWorldDir[i].Normalize();
		}
	}

	else
	{
		memcpy( m_vWorldDir, m_vLocalDir, sizeof( Vec3 ) * 3 );
	}
}

void CTransform::SaveToScene( FILE * pFile )
{
	UINT iType = ( UINT )GetComponentType();
	fwrite( &iType, sizeof( UINT ), 1, pFile );

	fwrite( &m_vLocalPos, sizeof( Vec3 ), 1, pFile );
	fwrite( &m_vLocalScale, sizeof( Vec3 ), 1, pFile );
	fwrite( &m_vLocalRot, sizeof( Vec3 ), 1, pFile );
}

void CTransform::LoadFromScene( FILE * pFile )
{
	fread( &m_vLocalPos, sizeof( Vec3 ), 1, pFile );
	fread( &m_vLocalScale, sizeof( Vec3 ), 1, pFile );
	fread( &m_vLocalRot, sizeof( Vec3 ), 1, pFile );
}
