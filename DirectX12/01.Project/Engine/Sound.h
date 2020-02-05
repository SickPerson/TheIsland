#pragma once

#include <FMOD/fmod.h>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_codec.h>

#ifdef _DEBUG
#pragma comment(lib, "FMOD/x64/fmodL64_vc.lib")
#else
#pragma comment(lib, "FMOD/x64/fmod64_vc.lib")
#endif
	

#include "Resource.h"

class CSound :
	public CResource
{
public:
	CSound();
	virtual ~CSound();

public:
	static FMOD::System* g_pFMOD;

private:
	FMOD::Sound*				m_pSound;
	list<FMOD::Channel*>		m_listChannel;

public:
	void Play( int iLoopCount, bool bOverlap = false );
	void Stop();

	void RemoveChannel( FMOD::Channel* pTargetChannel );

public:
	virtual void Load( const wstring& strFilePath );
};

