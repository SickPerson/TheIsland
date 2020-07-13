#pragma once

#include "Entity.h"
#include "FBXLoader.h"

struct tKeyFrame
{
	double dTime;
	Vec3 vPos;
	Vec3 vScale;
	Vec4 vRot;
};

struct tBoneKeyFrame
{
	int iBoneIndex;
	vector<tKeyFrame*> vecKeyFrame;
};

struct tAnimationCallback
{
	int			iAnimationProgress;
	float		fAnimationProgress;
	std::function<void( float )>	func;
	bool		bCall;
};

struct tAnimation3DClip
{
	ANIMATION_OPTION		eOption;
	wstring								strName;
	int										iStartFrame;
	int										iEndFrame;
	int										iFrameLength;
	double								dStartTime;
	double								dEndTime;
	double								dTimeLength;

	vector<tAnimationCallback*>	vecCallback;

	tAnimation3DClip() :
		eOption( ANIMATION_OPTION::LOOP ),
		strName( L"" ),
		iStartFrame( 0 ),
		iEndFrame( 0 ),
		iFrameLength( 0 ),
		dStartTime( 0 ),
		dEndTime( 0 ),
		dTimeLength( 0 )
	{
	}
};

class CAnimationClip :
	public CEntity
{
public:
	CAnimationClip();
	virtual ~CAnimationClip();

private:
	tAnimation3DClip						m_tInfo;
	int												m_iFrameMode;
	vector<tBoneKeyFrame* >	m_vecKeyFrame;
	int												m_iAnimationLimitFrame;

public:
	tAnimation3DClip GetClipInfo() const;
	tKeyFrame* GetKeyFrame( int iBone, int iFrame ) const;
	bool IsEmptyFrame( int iBone ) const;

public:
	void SetClipInfo( const wstring& strName, ANIMATION_OPTION eOption, int iFrameMode, int iStartFrame,
		int iEndFrame, double dStartTime, double dEndTime );
	void SetClipInfo( ANIMATION_OPTION eOption, tFbxAnimClip* pClip );
	void AddCallback( int iFrame, void( *pFunc )( float ) );
	void AddCallback( float fProgress, void( *pFunc )( float ) );

	void SetAnimationLimitFrame( int iAnimationLimitFrame );
};
