#include "stdafx.h"
#include "Camera.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Scene.h"
#include "Layer.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "MeshRender.h"
#include "Collider2D.h"


CCamera::CCamera() : 
	CComponent(COMPONENT_TYPE::CAMERA),
	m_Frustum(this),
	m_fFar(1000.f),
	m_fNear(1.f),
	m_fFOV(XM_PI/ 4.f),
	m_fScale(1.f),
	m_eProjType(PROJ_TYPE::ORTHGRAPHIC),
	m_iLayerCheck(0)
{
}


CCamera::~CCamera()
{
}

void CCamera::FinalUpdate()
{
	// 뷰행렬
	Vec3 vPos = Transform()->GetWorldPos();
	Matrix matViewTrans = XMMatrixTranslation( -vPos.x, -vPos.y, -vPos.z );

	Matrix matViewRot = XMMatrixIdentity();
	Vec3 vRight = Transform()->GetWorldDir( DIR_TYPE::RIGHT );
	Vec3 vUp = Transform()->GetWorldDir( DIR_TYPE::UP );
	Vec3 vFront = Transform()->GetWorldDir( DIR_TYPE::FRONT);

	matViewRot._11 = vRight.x;	matViewRot._12 = vUp.x;	matViewRot._13 = vFront.x;
	matViewRot._21 = vRight.y;	matViewRot._22 = vUp.y;	matViewRot._23 = vFront.y;
	matViewRot._31 = vRight.z;	matViewRot._32 = vUp.z;	matViewRot._33 = vFront.z;

	m_matView = matViewTrans * matViewRot;

	// 투영행렬
	m_matProj = XMMatrixIdentity();

	tResolution res = GET_SINGLE( CRenderManager )->GetResolution();

	if ( m_eProjType == PROJ_TYPE::PERSPECTIVE )
	{
		m_matProj = XMMatrixPerspectiveFovLH( m_fFOV, res.fWidth / res.fHeight, m_fNear, m_fFar );
	}

	else
	{
		m_matProj = XMMatrixOrthographicLH( res.fWidth * m_fScale, res.fHeight *m_fScale, m_fNear, m_fFar );
	}

	m_matViewInv = XMMatrixInverse( NULL, m_matView );
	m_matProjInv = XMMatrixInverse( NULL, m_matProj );

	m_Frustum.FinalUpdate();

	GET_SINGLE( CRenderManager )->RegisterCamera( this );
}

void CCamera::Render()
{
	g_tTransform.matView = GetViewMat();
	g_tTransform.matProj = GetProjMat();

	CScene* pCurScene = GET_SINGLE( CSceneManager )->GetCurScene();

	for ( UINT i = 0; i < MAX_LAYER; ++i )
	{
		if ( m_iLayerCheck & ( 1 << i ) )
		{
			const vector<CGameObject*>& vecObj = pCurScene->GetLayer( i )->GetObjects();

			for ( UINT i = 0; i < vecObj.size(); ++i )
			{
				if(!vecObj[i]->GetFrustumCheck()||
					m_Frustum.CheckFrustumSphere( vecObj[i]->Transform()->GetWorldPos(), vecObj[i]->Transform()->GetMaxScale() ))
				{
					if ( vecObj[i]->MeshRender() )
					{
						vecObj[i]->MeshRender()->Render();
					}

					if ( vecObj[i]->Collider2D() )
					{
						vecObj[i]->Collider2D()->Render();
					}
				}
			}
		}
	}
}

void CCamera::SetProjType( PROJ_TYPE eType )
{
	m_eProjType = eType;
	Changed();
}

void CCamera::SetFar( float fFar )
{
	m_fFar = fFar;
	Changed();
}

void CCamera::SetScale( float fScale )
{
	m_fScale = fScale;
	if ( m_fScale < 0.01f )
		m_fScale = 0.01f;

	Changed();
}

void CCamera::SetFOV( float fFOV )
{
	m_fFOV = fFOV;
	Changed();
}

void CCamera::SetLayerCheck( int iLayerIdx, bool bAdd )
{
	assert( iLayerIdx != -1 );
	if ( bAdd )
		m_iLayerCheck |= ( 1 << iLayerIdx );
	else
		m_iLayerCheck &= ~( 1 << iLayerIdx );
}

void CCamera::SetLayerAllCheck()
{
	m_iLayerCheck = 0xfffffff;
}

void CCamera::SetLayerAllRelease()
{
	m_iLayerCheck = 0;
}

PROJ_TYPE CCamera::GetProjType()
{
	return m_eProjType;
}

float CCamera::GetFar()
{
	return m_fFar;
}

float CCamera::GetScale()
{
	return m_fScale;
}

float CCamera::GetFOV()
{
	return m_fFOV;
}

const Matrix & CCamera::GetViewMat()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_matView;
}

const Matrix & CCamera::GetViewMatInv()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_matViewInv;
}

const Matrix & CCamera::GetProjMat()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_matProj;
}

const Matrix & CCamera::GetProjMatInv()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return m_matProjInv;
}

void CCamera::SaveToScene( FILE * pFile )
{
	UINT iType = ( UINT )GetComponentType();
	fwrite( &iType, sizeof( UINT ), 1, pFile );

	fwrite( &m_fNear, sizeof( float ), 1, pFile );
	fwrite( &m_fFar, sizeof( float ), 1, pFile );

	fwrite( &m_fFOV, sizeof( float ), 1, pFile );
	fwrite( &m_fScale, sizeof( float ), 1, pFile );

	fwrite( &m_eProjType, sizeof( UINT ), 1, pFile );
	fwrite( &m_iLayerCheck, 4, 1, pFile );
}

void CCamera::LoadFromScene( FILE * pFile )
{
	fread( &m_fNear, sizeof( float ), 1, pFile );
	fread( &m_fFar, sizeof( float ), 1, pFile );

	fread( &m_fFOV, sizeof( float ), 1, pFile );
	fread( &m_fScale, sizeof( float ), 1, pFile );

	fread( &m_eProjType, sizeof( UINT ), 1, pFile );
	fread( &m_iLayerCheck, 4, 1, pFile );
}
