#pragma once
#include "Component.h"

class CLandScape :
	public CComponent
{
public:
	CLandScape();
	virtual ~CLandScape();

private:
	UINT				m_iNumX;
	UINT				m_iNumZ;
	UINT				m_iDetailLevel;
	UINT				m_iSplatCount;
	vector<Vec3>		m_vecPos;
	vector<Vec3>		m_vecFaceNormal;

	tLandScape			m_tLandScape;

public:
	// CComponent을(를) 통해 상속됨
	virtual void SaveToScene( FILE * _pFile ) override;
	virtual void LoadFromScene( FILE * _pFile ) override;
	CLONE( CLandScape );

public:

public:
	void CreateLandScape( const wstring& strPath, UINT iNumX, UINT iNumZ );

private:
	void ComputeNormal( vector<VTX>& vecVtx, const vector<UINT>& vecIdx );
	void ComputeTangent( vector<VTX>& vecVtx, const vector<UINT>& vecIdx );
};

