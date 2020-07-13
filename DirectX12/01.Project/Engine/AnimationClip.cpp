#include "stdafx.h"
#include "AnimationClip.h"


CAnimationClip::CAnimationClip() :
	m_iAnimationLimitFrame(24)
{
}


CAnimationClip::~CAnimationClip()
{
	for ( size_t i = 0; i < m_vecKeyFrame.size(); ++i )
	{
		Safe_Delete_Vector( m_vecKeyFrame[i]->vecKeyFrame );
	}
}

tAnimation3DClip CAnimationClip::GetClipInfo() const
{
	return m_tInfo;
}

tKeyFrame * CAnimationClip::GetKeyFrame( int iBone, int iFrame ) const
{
	assert( iBone >= 0 && iBone < m_vecKeyFrame.size() );
	
	return m_vecKeyFrame[iBone]->vecKeyFrame[iFrame];
}

bool CAnimationClip::IsEmptyFrame( int iBone ) const
{
	return m_vecKeyFrame[iBone]->vecKeyFrame.empty();
}

void CAnimationClip::SetClipInfo( const wstring & strName, ANIMATION_OPTION eOption, int iFrameMode, int iStartFrame, int iEndFrame, double dStartTime, double dEndTime )
{
	m_tInfo.eOption = eOption;
	m_tInfo.strName = strName;
	m_tInfo.iStartFrame = iStartFrame;
	m_tInfo.iEndFrame = iEndFrame;
	m_tInfo.iFrameLength = iEndFrame - iStartFrame;
	m_tInfo.dStartTime = dStartTime;
	m_tInfo.dEndTime = dEndTime;
	m_tInfo.dTimeLength = dEndTime - dStartTime;
	m_iAnimationLimitFrame = iFrameMode;
}

void CAnimationClip::SetClipInfo( ANIMATION_OPTION eOption, tFbxAnimClip * pClip )
{
	// 인자로 들어온 애니메이션 옵션정보를 설정한다.
	m_tInfo.eOption = eOption;
	m_tInfo.strName = pClip->strName;

	// FBX Animation Clip에있는 StartTime과 EndTime을 이용하여 KeyFrame을 얻어온다.
	m_tInfo.iStartFrame = pClip->tStartTime.GetFrameCount( pClip->eMode );
	m_tInfo.iEndFrame = pClip->tEndTime.GetFrameCount( pClip->eMode );
	m_tInfo.iFrameLength = m_tInfo.iEndFrame - m_tInfo.iStartFrame;

	// 시간 정보를 저장해준다.
	m_tInfo.dStartTime = pClip->tStartTime.GetSecondDouble();
	m_tInfo.dEndTime = pClip->tEndTime.GetSecondDouble();
	m_tInfo.dTimeLength = m_tInfo.dEndTime - m_tInfo.dStartTime;

	// 키 프레임 수만큼 반복하며 각각의 프레임을 보간할 행렬 정보를 위치, 크기, 회전정보로 뽑아온다.
	for ( size_t i = 0; i < pClip->vecBoneFrame.size(); ++i )
	{
		tBoneKeyFrame* pBoneKeyFrame = new tBoneKeyFrame;
		pBoneKeyFrame->iBoneIndex = pClip->vecBoneFrame[i].iBoneIdx;
		m_vecKeyFrame.push_back( pBoneKeyFrame );

		// 아래부터 키 프레임 정보를 저장한다.
		pBoneKeyFrame->vecKeyFrame.reserve( pClip->vecBoneFrame[i].vecKeyFrame.size() );

		for ( size_t j = 0; j < pClip->vecBoneFrame[i].vecKeyFrame.size(); ++j )
		{
			tKeyFrame* pKeyFrame = new tKeyFrame;

			pKeyFrame->dTime = pClip->vecBoneFrame[i].vecKeyFrame[j].dTime;

			// 현재 본의 키 프레임에 해당하는 행렬 정보를 얻어온다.
			FbxAMatrix mat = pClip->vecBoneFrame[i].vecKeyFrame[j].matTransform;

			FbxVector4 vPos, vScale;
			FbxQuaternion qRot;

			// 행렬로부터 위치, 크기, 회전 정보를 얻어온다.
			vPos = mat.GetT();
			vScale = mat.GetS();
			qRot = mat.GetQ();

			pKeyFrame->vScale = Vec3( ( float )vScale.mData[0], ( float )vScale.mData[1], ( float )vScale.mData[2] );
			pKeyFrame->vPos = Vec3( ( float )vPos.mData[0], ( float )vPos.mData[1], ( float )vPos.mData[2] );
			pKeyFrame->vRot = Vec4( ( float )qRot.mData[0], ( float )qRot.mData[1], ( float )qRot.mData[2], (float)qRot.mData[3] );

			pBoneKeyFrame->vecKeyFrame.push_back( pKeyFrame );
		}
	}
}

void CAnimationClip::AddCallback( int iFrame, void( *pFunc )( float ) )
{
	// 애니메이션 중간에 호출할 콜백함수를 추가
	tAnimationCallback* pCallback = new tAnimationCallback;

	// 지정 프레임 설정
	pCallback->iAnimationProgress = iFrame;
	// 지정 프레임에서 시작 프레임을 빼주고 길이로 나누어서 현재 진행률을 구한다.
	pCallback->fAnimationProgress = ( iFrame - m_tInfo.iStartFrame ) / ( float )( m_tInfo.iFrameLength );
	pCallback->func = bind( pFunc, std::placeholders::_1 );
	pCallback->bCall = false;

	m_tInfo.vecCallback.push_back( pCallback );
}

void CAnimationClip::AddCallback( float fProgress, void( *pFunc )( float ) )
{
	tAnimationCallback* pCallback = new tAnimationCallback;

	// 진행율이 들어오면 역으로 프레임을 구해준다.
	// 진행율 * 시간길이 + 시작시간 = 현재시간
	// 현재시간에 1초당 프레임 값을 곱해주어 프레임을 구한다.
	pCallback->iAnimationProgress = ( fProgress * m_tInfo.dTimeLength + m_tInfo.dStartTime ) * m_iAnimationLimitFrame;
	pCallback->fAnimationProgress = fProgress;
	pCallback->func = bind( pFunc, std::placeholders::_1 );
	pCallback->bCall = false;
	m_tInfo.vecCallback.push_back( pCallback );
}

void CAnimationClip::SetAnimationLimitFrame( int iAnimationLimitFrame )
{
	m_iAnimationLimitFrame = iAnimationLimitFrame;
}
