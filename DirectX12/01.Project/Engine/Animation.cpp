#include "stdafx.h"
#include "Animation.h"
#include "FBXLoader.h"
#include "AnimationClip.h"
#include "TimeMgr.h"

CAnimation::CAnimation() :
	CComponent(COMPONENT_TYPE::SCRIPT)
{
}


CAnimation::~CAnimation()
{
}

void CAnimation::SaveToScene( FILE * _pFile )
{
}

void CAnimation::LoadFromScene( FILE * _pFile )
{
}

bool CAnimation::IsAnimationEnd() const
{
	return m_bEnd;
}

CAnimationClip * CAnimation::FindClip( const wstring & strKey )
{
	unordered_map<wstring, class CAnimationClip*>::iterator iter = m_mapClip.find( strKey );

	if ( iter == m_mapClip.end() )
		return NULL;

	return iter->second;
}

CAnimationClip * CAnimation::GetCurrentClip() const
{
	return m_vecChannel[m_iCurChannel].pClip;
}

CAnimationClip * CAnimation::GetLastAddClip() const
{
	return m_pLastAddClip;
}

const unordered_map<wstring, class CAnimationClip*>* CAnimation::GetAllClip() const
{
	return &m_mapClip;
}

void CAnimation::AddClipCallback( const wstring & strName, int iFrame, void( *pFunc )( float ) )
{
	CAnimationClip* pClip = FindClip( strName );

	if ( !pClip )
		return;

	pClip->AddCallback( iFrame, pFunc );
}

void CAnimation::AddClipCallback( const wstring & strName, float fProgress, void( *pFunc )( float ) )
{
	CAnimationClip* pClip = FindClip( strName );

	if ( !pClip )
		return;

	pClip->AddCallback( fProgress, pFunc );
}

void CAnimation::AddClip( ANIMATION_OPTION eOption, tFbxAnimClip * pClip )
{
	CAnimationClip* pAniClip = FindClip( pClip->strName );

	if ( pAniClip )
	{
		assert( false );
		return;
	}

	pAniClip = new CAnimationClip;
	pAniClip->SetClipInfo( eOption, pClip );

	switch ( pClip->eMode )
	{
	case FbxTime::eFrames24:
		m_iFrameMode = 24;
		break;

	case FbxTime::eFrames30:
		m_iFrameMode = 30;
		break;

	case FbxTime::eFrames60:
		m_iFrameMode = 60;
		break;
	}

	pAniClip->SetAnimationLimitFrame( m_iFrameMode );
	m_fFrameTime = 1.f / m_iFrameMode;

	if ( m_mapClip.empty() )
	{
		SetDefaultClip( pClip->strName );
		SetCurrentClip( pClip->strName );

		m_vecChannel[m_iCurChannel].pClip = pAniClip;
	}

	m_mapClip[pClip->strName] = pAniClip;
	
	m_pLastAddClip = pAniClip;
}

void CAnimation::AddClip( const wstring & strName, ANIMATION_OPTION eOption, int iAnimationLimitFrame, int iStartFrame, int iEndFrame, double dStartTime, double dEndTime )
{
	CAnimationClip* pAniClip = FindClip( strName );

	if ( pAniClip )
	{
		assert( false );
		return;
	}

	m_iFrameMode = iAnimationLimitFrame;

	pAniClip = new CAnimationClip;

	pAniClip->SetClipInfo( strName, eOption, iAnimationLimitFrame, iStartFrame, iEndFrame, dStartTime, dEndTime );

	pAniClip->SetAnimationLimitFrame( m_iFrameMode );
	m_fFrameTime = 1.f / m_iFrameMode;

	if ( m_mapClip.empty() )
	{
		SetDefaultClip( strName );
		SetCurrentClip( strName );

		m_vecChannel[m_iCurChannel].pClip = pAniClip;
	}

	m_mapClip[strName] = pAniClip;

	m_pLastAddClip = pAniClip;
}

void CAnimation::AddClip( const wstring & strName, ANIMATION_OPTION eOption, int iStartFrame, int iEndFrame, double dStartTime, double dEndTime )
{
	CAnimationClip* pAniClip = FindClip( strName );

	if ( pAniClip )
	{
		assert( false );
		return;
	}

	pAniClip = new CAnimationClip;
	
	pAniClip->SetAnimationLimitFrame( m_iFrameMode );
	m_fFrameTime = 1.f / m_iFrameMode;

	pAniClip->SetClipInfo( strName, eOption, m_iFrameMode, iStartFrame, iEndFrame, dStartTime, dEndTime );

	if ( m_mapClip.empty() )
	{
		SetDefaultClip( strName );
		SetCurrentClip( strName );

		m_vecChannel[m_iCurChannel].pClip = pAniClip;
	}

	m_mapClip[strName] = pAniClip;

	m_pLastAddClip = pAniClip;
}

void CAnimation::AddClip( const wchar_t * pFullPath )
{
}

void CAnimation::AddClip( const char * pFullPath )
{
}

void CAnimation::SetDefaultClip( const wstring & strClip )
{
	m_strDefualtClip = strClip;
}

void CAnimation::SetCurrentClip( const wstring & strClip )
{
	m_iCurChannel = 0;
	m_strCurrentClip = strClip;
}

bool CAnimation::ChangeClip( const wstring & strClip )
{
	if(m_strCurrentClip == strClip )
		return false;

	CAnimationClip* pClip = FindClip( strClip );

	if ( pClip == m_vecChannel[m_iCurChannel].pClip )
	{
		m_iNextChannel = -1;
		return false;
	}

	else if ( !pClip )
	{
		m_iNextChannel = -1;
		return false;
	}

	m_iNextChannel = m_iCurChannel == 0 ? 1 : 0;
	
	m_vecChannel[m_iNextChannel].pClip = pClip;
	m_vecChannel[m_iNextChannel].fWeight = 1.f;
	m_strCurrentClip = strClip;

	return true;
}

bool CAnimation::ModifyClip( const wstring & strKey, const wstring & strChangeKey, ANIMATION_OPTION eOption, int iStartFrame, int iEndFrame )
{
	CAnimationClip* pClip = FindClip( strKey );

	if ( !pClip )
		return false;

	m_mapClip.erase( strKey );

	int iLength = iEndFrame - iStartFrame;
	pClip->SetClipInfo( strChangeKey, eOption, m_iFrameMode, iStartFrame, iEndFrame, 0.f, iLength / ( float )m_iFrameMode );

	m_mapClip[strChangeKey] = pClip;

	if ( m_strDefualtClip == strKey )
		m_strDefualtClip = strChangeKey;

	if ( m_strCurrentClip == strKey )
		m_strCurrentClip = strChangeKey;

	return true;
}

bool CAnimation::DeleteClip( const wstring & strKey )
{
	auto iter = m_mapClip.find( strKey );
	
	if ( iter == m_mapClip.end() )
		return false;

	if ( strKey == m_strDefualtClip )
	{
		if ( !m_mapClip.empty() )
			SetDefaultClip( m_mapClip.begin()->first );
	}

	if ( strKey == m_strCurrentClip )
	{
		m_vecChannel[m_iCurChannel].pClip = NULL;
		m_vecChannel[m_iCurChannel].fWeight = 1.f;
		SetCurrentClip( m_strDefualtClip );

		m_vecChannel[m_iCurChannel].pClip = FindClip( m_strDefualtClip );
		m_vecChannel[m_iCurChannel].fWeight = 1.f;
	}

	m_mapClip.erase( iter );

	return true;
}

bool CAnimation::ReturnDefaultClip()
{
	ChangeClip( m_strDefualtClip );

	return true;
}

void CAnimation::LateUpdate()
{
	if ( m_mapClip.empty() )
		return;

	m_bEnd = false;

	// 본 정보를 업데이트 한다.
	vector<Matrix> vecBones;

	// 모션이 변할 때
	if ( m_iNextChannel != -1 )
	{
		m_fChangeTime += DT;

		bool bChange = false;

		if ( m_fChangeTime >= m_fChangeLimitTime )
		{
			m_fChangeTime = m_fChangeLimitTime;
			bChange = true;
		}

		float fAnimationTime = m_fAnimationGlobalTime + m_vecChannel[m_iCurChannel].pClip->GetClipInfo().dStartTime;

		vecBones.reserve( m_vecBones.size() );

		// 본 수 만큼 반복
		for ( size_t i = 0; i < m_vecBones.size(); ++i )
		{
			// 키프레임이 없을 경우
			if ( m_vecChannel[m_iCurChannel].pClip->IsEmptyFrame( i ) )
			{
				vecBones.push_back( *m_vecBones[i]->matBone );
				continue;
			}

			int iFrameIndex = m_vecChannel[m_iCurChannel].iChangeFrame;
			int iNextFrameIndex = m_vecChannel[m_iNextChannel].pClip->GetClipInfo().iStartFrame;

			const tKeyFrame* pCurKey = m_vecChannel[m_iCurChannel].pClip->GetKeyFrame( i, iFrameIndex );
			const tKeyFrame* pNextKey = m_vecChannel[m_iNextChannel].pClip->GetKeyFrame( i, iNextFrameIndex );

			float fPercent = m_fChangeTime / m_fChangeLimitTime;

			XMVECTOR vS = XMVectorLerp( XMLoadFloat3( &pCurKey->vScale ), XMLoadFloat3( &pNextKey->vScale ), fPercent );
			XMVECTOR vT = XMVectorLerp( XMLoadFloat3( &pCurKey->vPos ), XMLoadFloat3( &pNextKey->vPos ), fPercent );
			XMVECTOR vR = XMQuaternionSlerp( XMLoadFloat4( &pCurKey->vRot ), XMLoadFloat4( &pNextKey->vRot ), fPercent );

			XMVECTOR vZero = XMVectorSet( 0.f, 0.f, 0.f, 1.f );

			Matrix matBone = XMMatrixAffineTransformation( vS, vZero, vR, vT );

			*m_vecBones[i]->matBone = matBone;

			matBone = *m_vecBones[i]->matOffset * matBone;

			vecBones.push_back( matBone );
		}

		if ( bChange )
		{
			m_iCurChannel = m_iNextChannel;
			m_iNextChannel = -1;
			m_fAnimationGlobalTime = 0.f;
			m_fChangeTime = 0.f;
		}
	}

	// 기존 모션이 계속 동작될 때
	else
	{
		m_fAnimationGlobalTime += DT;
		m_fClipProgress = m_fAnimationGlobalTime / m_vecChannel[m_iCurChannel].pClip->GetClipInfo().dTimeLength;

		while ( m_fAnimationGlobalTime >= m_vecChannel[m_iCurChannel].pClip->GetClipInfo().dTimeLength )
		{
			m_fAnimationGlobalTime -= m_vecChannel[m_iCurChannel].pClip->GetClipInfo().dTimeLength;
			m_bEnd = true;

			for ( size_t i = 0; i < m_vecChannel[m_iCurChannel].pClip->GetClipInfo().vecCallback.size(); ++i )
			{
				m_vecChannel[m_iCurChannel].pClip->GetClipInfo().vecCallback[i]->bCall = false;
			}
		}

		float fAnimationTime = m_fAnimationGlobalTime + m_vecChannel[m_iCurChannel].pClip->GetClipInfo().dStartTime;

		int iStartFrame = m_vecChannel[m_iCurChannel].pClip->GetClipInfo().iStartFrame;
		int iEndFrame = m_vecChannel[m_iCurChannel].pClip->GetClipInfo().iEndFrame;

		vecBones.reserve( m_vecBones.size() );

		int iFrameIndex = ( int )( fAnimationTime * m_iFrameMode );

		if ( m_bEnd )
		{
			switch ( m_vecChannel[m_iCurChannel].pClip->GetClipInfo().eOption)
			{
			case ANIMATION_OPTION::LOOP:
				iFrameIndex = iStartFrame;
				break;
			case ANIMATION_OPTION::ONCE_RETURN:
				ChangeClip( m_strDefualtClip );
				break;
			case ANIMATION_OPTION::ONCE_DESTROY:
				GetObj()->SetDead();
				break;
			default:
				break;
			}
		}

		else
		{
			int iNextFrameIndex = iFrameIndex + 1;

			m_vecChannel[m_iCurChannel].iChangeFrame = iFrameIndex;

			if ( iNextFrameIndex > iEndFrame )
				iNextFrameIndex = iStartFrame;

			// 본 수만큼 반복한다.
			for ( size_t i = 0; i < m_vecBones.size(); ++i )
			{
				// 키프레임이 없을 경우
				if ( m_vecChannel[m_iCurChannel].pClip->IsEmptyFrame( i ) )
				{
					vecBones.push_back( *m_vecBones[i]->matBone );
					continue;
				}
				
				const tKeyFrame* pCurKey = m_vecChannel[m_iCurChannel].pClip->GetKeyFrame( i, iFrameIndex );
				const tKeyFrame* pNextKey = m_vecChannel[m_iNextChannel].pClip->GetKeyFrame( i, iNextFrameIndex );

				// 현재 프레임의 시간을 얻어온다.
				double dFrameTime = pCurKey->dTime;

				float fPercent = ( fAnimationTime - dFrameTime ) / m_fFrameTime;

				XMVECTOR vS = XMVectorLerp( XMLoadFloat3( &pCurKey->vScale ), XMLoadFloat3( &pNextKey->vScale ), fPercent );
				XMVECTOR vT = XMVectorLerp( XMLoadFloat3( &pCurKey->vPos ), XMLoadFloat3( &pNextKey->vPos ), fPercent );
				XMVECTOR vR = XMQuaternionSlerp( XMLoadFloat4( &pCurKey->vRot ), XMLoadFloat4( &pNextKey->vRot ), fPercent );

				XMVECTOR vZero = XMVectorSet( 0.f, 0.f, 0.f, 1.f );

				Matrix matBone = XMMatrixAffineTransformation( vS, vZero, vR, vT );

				*m_vecBones[i]->matBone = matBone;

				matBone = *m_vecBones[i]->matOffset * matBone;

				vecBones.push_back( matBone );
			}
		}
	}
}

void CAnimation::FinalUpdate()
{
}
