#include "stdafx.h"
#include "Scene.h"
#include "Layer.h"

CScene::CScene() : 
	m_arrLayer{}
{
	for ( UINT i = 0; i < MAX_LAYER; ++i )
	{
		AddLayer( L"", i );
	}
}


CScene::~CScene()
{
	Safe_Delete_Array( m_arrLayer );
}

void CScene::Awake()
{
	for ( UINT i = 0; i < MAX_LAYER; ++i )
	{
		if ( NULL != m_arrLayer[i] )
			m_arrLayer[i]->Awake();
	}
}

void CScene::Start()
{
	for ( UINT i = 0; i < MAX_LAYER; ++i )
	{
		if ( NULL != m_arrLayer[i] )
			m_arrLayer[i]->Start();
	}
}

void CScene::Update()
{
	for ( UINT i = 0; i < MAX_LAYER; ++i )
	{
		if ( NULL != m_arrLayer[i] )
			m_arrLayer[i]->Update();
	}
}

void CScene::LateUpdate()
{
	for ( UINT i = 0; i < MAX_LAYER; ++i )
	{
		if ( NULL != m_arrLayer[i] )
			m_arrLayer[i]->LateUpdate();
	}
}

void CScene::FinalUpdate()
{
	for ( UINT i = 0; i < MAX_LAYER; ++i )
	{
		if ( NULL != m_arrLayer[i] )
			m_arrLayer[i]->FinalUpdate();
	}
}

void CScene::SetLayerName( int iIdx, const wstring & strName )
{
	assert( -1 < iIdx && iIdx <= 31 );
	m_arrLayer[iIdx]->SetName( strName );
}

void CScene::AddGameObject( const wstring & strLayerName, CGameObject * pObject, bool bMoveAll )
{
	CLayer* pLayer = FindLayer( strLayerName );
	assert( pLayer );
	pLayer->AddGameObject( pObject, bMoveAll );
}

void CScene::AddGameObject( int iLayerIdx, CGameObject * pObject, bool bMoveAll )
{
	assert( -1 < iLayerIdx && iLayerIdx <= 31 );
	CLayer* pLayer = GetLayer( iLayerIdx );
	assert( pLayer );
	pLayer->AddGameObject( pObject, bMoveAll );
}

CLayer * CScene::FindLayer( const wstring & strLayerName )
{
	for ( UINT i = 0; i < MAX_LAYER; ++i )
	{
		if ( nullptr == m_arrLayer[i] )
			continue;

		if ( strLayerName == m_arrLayer[i]->GetName() )
			return m_arrLayer[i];
	}

	return nullptr;
}

CLayer * CScene::GetLayer( int iIdx )
{
	return m_arrLayer[iIdx];
}

void CScene::AddLayer( const wstring & strLayerName, int iIdx )
{
	// 비정상적인 LayerIndex 거르기
	assert( -1 < iIdx && iIdx <= 31 && ( nullptr == m_arrLayer[iIdx] ) );

	m_arrLayer[iIdx] = new CLayer;
	m_arrLayer[iIdx]->SetName( strLayerName );
	m_arrLayer[iIdx]->SetLayerIdx( iIdx );
}
