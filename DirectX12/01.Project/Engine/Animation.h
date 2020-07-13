#pragma once
#include "Component.h"

struct tBone
{
	wstring strName;
	int	iDepth;
	int	iParentIndex;
	Matrix* matOffset;
	Matrix* matBone;
};

struct tAnimationChannel
{
	class CAnimationClip*	 pClip;
	float									fWeight;
	int										iChangeFrame;

	tAnimationChannel() :
		pClip( NULL ),
		fWeight( 1.f ),
		iChangeFrame( 0 )
	{
	}
};

class CAnimation :
	public CComponent
{
public:
	CAnimation();
	virtual ~CAnimation();
	   
	// CComponent을(를) 통해 상속됨
	virtual void SaveToScene( FILE * _pFile ) override;
	virtual void LoadFromScene( FILE * _pFile ) override;
	CLONE( CAnimation );

private:
	vector<tBone*>			m_vecBones;
	ID3D12Resource*			m_pBoneTex;
	// ShaderResourceView

	int									m_iFrameMode;
	unordered_map<wstring, class CAnimationClip*> m_mapClip;
	vector<tAnimationChannel>	m_vecChannel;
	int			m_iCurChannel;
	int			m_iNextChannel;
	float		m_fFrameTime;
	bool		m_bEnd;
	float		m_fAnimationGlobalTime;
	float		m_fClipProgress;
	float		m_fChangeTime;
	float		m_fChangeLimitTime;

private:
	wstring		m_strDefualtClip;
	wstring		m_strCurrentClip;
	class CAnimationClip*	m_pLastAddClip;

public:
	bool IsAnimationEnd() const;
	class CAnimationClip* FindClip( const wstring& strKey );
	class CAnimationClip* GetCurrentClip() const;
	class CAnimationClip* GetLastAddClip() const;
	const unordered_map<wstring, class CAnimationClip*>* GetAllClip() const;

public:
	void AddClipCallback( const wstring& strName, int iFrame, void( *pFunc )( float ) );
	void AddClipCallback( const wstring& strName, float fProgress, void( *pFunc )( float ) );

	void AddClip( ANIMATION_OPTION eOption, struct tFbxAnimClip* pClip );
	void AddClip( const wstring& strName, ANIMATION_OPTION eOption, int iAnimationLimitFrame, int iStartFrame, int iEndFrame, double dStartTime, double dEndTime );
	void AddClip( const wstring& strName, ANIMATION_OPTION eOption, int iStartFrame, int iEndFrame, double dStartTime, double dEndTime );
	void AddClip( const wchar_t* pFullPath );
	void AddClip( const char* pFullPath );

	void SetDefaultClip( const wstring& strClip );
	void SetCurrentClip( const wstring& strClip );
	bool ChangeClip( const wstring& strClip );
	bool ModifyClip( const wstring& strKey, const wstring& strChangeKey, ANIMATION_OPTION eOption, int iStartFrame, int iEndFrame );
	bool DeleteClip( const wstring& strKey );
	bool ReturnDefaultClip();

public:
	virtual void LateUpdate();
	virtual void FinalUpdate();
};