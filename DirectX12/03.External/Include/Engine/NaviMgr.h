#pragma once

class CLandScape;

class CNaviMgr
{
	SINGLE( CNaviMgr );

private:
	tLandScapeInfo		m_tLandScapeInfo;

public:
	void SetLandScape( CLandScape* pLandScape );
	void SetLandScapeInfo( const tLandScapeInfo& tLandScapeInfo );
	float GetY( const Vec3& vPos );	
};

