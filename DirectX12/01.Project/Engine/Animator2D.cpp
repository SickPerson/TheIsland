#include "stdafx.h"
#include "Animator2D.h"
#include "Animation2D.h"
#include "PathManager.h"

CAnimator2D::CAnimator2D() : 
	CComponent(COMPONENT_TYPE::ANIMATOR2D),
	m_pCurAni(NULL)
{
}

CAnimator2D::CAnimator2D( const CAnimator2D & ani ) :
	CComponent( ani ),
	m_bRepeat( ani.m_bRepeat )
{
	for ( auto& pair : ani.m_mapAni )
	{
		CAnimation2D* pCopyAni = new CAnimation2D( *pair.second );
		pCopyAni->SetAnimatior2D( this );
		m_mapAni[pair.first] = pCopyAni;
	}

	if ( ani.m_pCurAni )
	{
		PlayAnimation( ani.m_pCurAni->GetName, m_bRepeat );
	}
}


CAnimator2D::~CAnimator2D()
{
	Safe_Delete_Map( m_mapAni );
}

bool CAnimator2D::AddAnimation( const wstring & strKey, Ptr<CTexture> pTex, Vec2 vLT, Vec2 vLen, int iFrameCount, int fDuration )
{
	CAnimation2D* pAni = FindAnimation( strKey );

	if ( pAni )
		return false;

	pAni = new CAnimation2D;

	pAni->Create( pTex, vLT, vLen, iFrameCount, fDuration );

	m_mapAni[strKey] = pAni;

	pAni->SetAnimatior2D( this );
	pAni->SetName( strKey );

	return true;
}

void CAnimator2D::PlayAnimation( const wstring & strKey, bool bRepeat )
{
	m_pCurAni = FindAnimation( strKey );

	if ( !m_pCurAni )
		return;

	m_bRepeat = bRepeat;
	m_bPlay = true;
}

void CAnimator2D::PlayAnimation()
{
	if ( m_pCurAni )
		m_bPlay = true;
}

void CAnimator2D::PauseAniamtion()
{
	m_bPlay = false;
}

void CAnimator2D::StopAnimation()
{
	m_bPlay = false;
	if ( !m_pCurAni )
		return;

	m_pCurAni->Play();
}

bool CAnimator2D::IsPlay()
{
	return m_bPlay;
}

bool CAnimator2D::IsRepeat()
{
	return m_bRepeat;
}

CAnimation2D * CAnimator2D::FindAnimation( const wstring & strKey )
{
	map<wstring, CAnimation2D*>::iterator iter = m_mapAni.find( strKey );

	if(iter == m_mapAni.end() )
		return nullptr;

	return iter->second;
}

CAnimation2D * CAnimator2D::GetCurAnimation()
{
	return m_pCurAni;
}

const map<wstring, CAnimation2D*>& CAnimator2D::GetAnimations()
{
	// TODO: 여기에 반환 구문을 삽입합니다.

	return m_mapAni;
}

void CAnimator2D::UpdateData()
{
	if ( m_pCurAni )
		m_pCurAni->UpdateData();
}

void CAnimator2D::SaveToScene( FILE * pFile )
{
	UINT iType = ( UINT )GetComponentType();
	fwrite( &iType, sizeof( UINT ), 1, pFile );

	fwrite( &m_bRepeat, 1, 1, pFile );

	size_t iAnimCount = m_mapAni.size();
	fwrite( &iAnimCount, 4, 1, pFile );

	for ( auto& pair : m_mapAni )
	{
		GET_SINGLE(CPathManager)->SaveWString( pFile, pair.first );
		pair.second->SaveToScene( pFile );
	}

	fwrite( &m_pCurAni, sizeof( void* ), 1, pFile );
	if ( nullptr != m_pCurAni )
	{
		GET_SINGLE( CPathManager )->SaveWString( pFile, m_pCurAni->GetName() );
	}
}

void CAnimator2D::LoadFromScene( FILE * pFile )
{
	fread( &m_bRepeat, 1, 1, pFile );

	UINT iAnimCount = 0;
	fread( &iAnimCount, 4, 1, pFile );

	for ( UINT i = 0; i < iAnimCount; ++i )
	{
		wstring strAnimName = GET_SINGLE( CPathManager )->LoadWString( pFile );
		CAnimation2D* pAnim = new CAnimation2D;
		pAnim->SetName( strAnimName );
		pAnim->LoadFromScene( pFile );
		m_mapAni.insert( make_pair( strAnimName, pAnim ) );
	}

	void* pData = nullptr;
	fread( &pData, sizeof( void* ), 1, pFile );

	if ( pData )
	{
		wstring strCurAni = GET_SINGLE( CPathManager )->LoadWString( pFile );
		m_pCurAni = FindAnimation( strCurAni );
		assert( m_pCurAni );
	}
}
