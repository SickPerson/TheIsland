#include "stdafx.h"
#include "Animation2D.h"
#include "TimeManager.h"
#include "ConstantBuffer.h"
#include "Device.h"
#include "Texture.h"

CAnimation2D::CAnimation2D() : 
	m_pAnimator(NULL),
	m_iCurFrame(0),
	m_fAccTime(0.f),
	m_bFinish(false)
{
}


CAnimation2D::~CAnimation2D()
{
}

void CAnimation2D::FinalUpdate()
{
	if ( m_bFinish )
		return;

	m_fAccTime += DT;

	if ( m_vecFrame[m_iCurFrame].fDuration < m_fAccTime )
	{
		m_iCurFrame += 1;
		m_fAccTime = 0.f;

		if ( m_vecFrame.size() <= m_iCurFrame )
		{
			m_iCurFrame = ( int )m_vecFrame.size() - 1;
			m_bFinish = true;
		}
	}
}

void CAnimation2D::UpdateData()
{
	static CConstantBuffer* pAniBuffer = GET_SINGLE( CDevice )->GetConstBuffer( CONST_REGISTER::b2 );
	static tAnimation2D data{};

	data.IsAnimation2D = true;
	data.vLen = m_vecFrame[m_iCurFrame].vLen;
	data.vLT = m_vecFrame[m_iCurFrame].vLT;
	data.vOffset = m_vecFrame[m_iCurFrame].vOffsetPos;

	// 현재 프레임 정보를 상수버퍼에 저장 후 b2 레지스터로 업데이트
	GET_SINGLE( CDevice )->SetConstBufferToRegister( pAniBuffer, pAniBuffer->AddData( &data ) );

	// 현재 프레임 텍스쳐 업데이트(t12 레지스터로)
	GET_SINGLE( CDevice )->SetTextrueToRegister( m_vecFrame[m_iCurFrame].pTex.GetPointer(), TEXTURE_REGISTER::t12 );
}

void CAnimation2D::Play()
{
	m_iCurFrame = 0;
	m_bFinish = false;
	m_fAccTime = 0.f;
}

bool CAnimation2D::IsFinish()
{
	return m_bFinish;
}

void CAnimation2D::Create( Ptr<CTexture> pTex, Vec2 & vLT, Vec2 & vLen, int iFrameCount, float fDuration )
{
	for ( int i = 0; i < iFrameCount; ++i )
	{
		tAnimation2DFrame frame{};

		Vec2 LT = vLT;
		LT.x += vLen.x * i;
	}
}

void CAnimation2D::SaveToScene( FILE * pFile )
{
}

void CAnimation2D::LoadFromScene( FILE * pFile )
{
}

void CAnimation2D::SetAnimatior2D( CAnimator2D * pAnimator2D )
{
}
