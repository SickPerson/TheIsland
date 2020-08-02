#include "stdafx.h"
#include "NaviMgr.h"

#include "LandScape.h"
#include "Transform.h"


CNaviMgr::CNaviMgr()
{
}


CNaviMgr::~CNaviMgr()
{
}

void CNaviMgr::SetLandScape( CLandScape * pLandScape )
{
	m_tLandScapeInfo.iNumX = pLandScape->GetNumX();
	m_tLandScapeInfo.iNumZ = pLandScape->GetNumZ();
	m_tLandScapeInfo.pvecPos = pLandScape->GetVecPos();
	m_tLandScapeInfo.pTransform = pLandScape->Transform();

	vector<Vec3>::iterator iter = m_tLandScapeInfo.pvecPos->begin();
	vector<Vec3>::iterator iterEnd = m_tLandScapeInfo.pvecPos->end();

	m_tLandScapeInfo.vMin = *iter;
	m_tLandScapeInfo.vMax = *iter;

	for ( iter = m_tLandScapeInfo.pvecPos->begin() + 1; iter != iterEnd; ++iter )
	{
		if ( m_tLandScapeInfo.vMin.x > ( *iter ).x )
			m_tLandScapeInfo.vMin.x = ( *iter ).x;

		if ( m_tLandScapeInfo.vMin.y > ( *iter ).y )
			m_tLandScapeInfo.vMin.y = ( *iter ).y;

		if ( m_tLandScapeInfo.vMin.z > ( *iter ).z )
			m_tLandScapeInfo.vMin.z = ( *iter ).z;

		if ( m_tLandScapeInfo.vMax.x < ( *iter ).x )
			m_tLandScapeInfo.vMax.x = ( *iter ).x;

		if ( m_tLandScapeInfo.vMax.y < ( *iter ).y )
			m_tLandScapeInfo.vMax.y = ( *iter ).y;

		if ( m_tLandScapeInfo.vMax.z < ( *iter ).z )
			m_tLandScapeInfo.vMax.z = ( *iter ).z;
	}
}

void CNaviMgr::SetLandScapeInfo( const tLandScapeInfo & tLandScapeInfo )
{
	m_tLandScapeInfo = tLandScapeInfo;
}

float CNaviMgr::GetY( const Vec3 & vPos )
{
	Vec3 vScale = m_tLandScapeInfo.pTransform->GetWorldScale();

	float fWidth = ( *m_tLandScapeInfo.pvecPos )[1].x - ( *m_tLandScapeInfo.pvecPos )[0].x;
	float fHeight = ( *m_tLandScapeInfo.pvecPos )[0].z - ( *m_tLandScapeInfo.pvecPos )[m_tLandScapeInfo.iNumZ].z;

	fWidth *= vScale.x;
	fHeight *= vScale.z;


	Vec3 vLandScapePos = m_tLandScapeInfo.pTransform->GetWorldPos();

	Vec3 vOriginPos = vPos;

	vOriginPos -= vLandScapePos;

	vOriginPos.x /= fWidth;
	vOriginPos.z /= fHeight;

	if ( vOriginPos.x <= 0 || vOriginPos.x >= m_tLandScapeInfo.iNumX ||
		vOriginPos.z <= 0 || vOriginPos.z >= m_tLandScapeInfo.iNumZ )
		return 0.f;

	int xIdx = ( int )vOriginPos.x;
	int zIdx = m_tLandScapeInfo.iNumZ - 1 - ( int )vOriginPos.z - 1;

	int iIdx = zIdx * m_tLandScapeInfo.iNumX + xIdx;

	if ( iIdx < 0 )
		return vPos.y;

	float fLandScapeY[4] = {};
	fLandScapeY[0] = ( *m_tLandScapeInfo.pvecPos )[iIdx].y;
	fLandScapeY[1] = ( *m_tLandScapeInfo.pvecPos )[iIdx + 1].y;
	fLandScapeY[2] = ( *m_tLandScapeInfo.pvecPos )[iIdx + m_tLandScapeInfo.iNumX].y;
	fLandScapeY[3] = ( *m_tLandScapeInfo.pvecPos )[iIdx + m_tLandScapeInfo.iNumX + 1].y;

	float fX, fY;
	fX = ( vPos.x - ( ( *m_tLandScapeInfo.pvecPos )[iIdx].x ) * vScale.x ) / fWidth;
	fY = ( ( ( *m_tLandScapeInfo.pvecPos )[iIdx].z * vScale.z ) - vPos.z ) / fHeight;

	if ( fX >= fY )
		return ( fLandScapeY[0] + ( fLandScapeY[1] - fLandScapeY[0] ) * fX + ( fLandScapeY[3] - fLandScapeY[1] ) * fY ) * vScale.y;

	return ( fLandScapeY[0] + ( fLandScapeY[3] - fLandScapeY[2] ) * fX + ( fLandScapeY[2] - fLandScapeY[0] ) * fY ) * vScale.y;
}
