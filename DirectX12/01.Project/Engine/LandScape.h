#pragma once
#include "Component.h"
#include "StructuredBuffer.h"

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

	CStructuredBuffer*	m_pBuffer;

public:
	// CComponent을(를) 통해 상속됨
	virtual void SaveToScene( FILE * _pFile ) override;
	virtual void LoadFromScene( FILE * _pFile ) override;
	CLONE( CLandScape );

public:
	UINT GetNumX() const;
	UINT GetNumZ() const;
	vector<Vec3>* GetVecPos();

	void SetDetailLevel( int iDetailLevel );

public:
	void CreateLandScape( const wstring& strPath, UINT iNumX, UINT iNumZ );

	void SetDiffuseSplatting( const wstring& strKey, const vector<wstring>* pvecPath );
	void SetNormalSplatting( const wstring& strKey, const vector<wstring>* pvecPath );
	void SetAlphaSplatting( const wstring& strKey, const vector<wstring>* pvecPath );

private:
	void ComputeNormal( vector<VTX>& vecVtx, const vector<UINT>& vecIdx );
	void ComputeTangent( vector<VTX>& vecVtx, const vector<UINT>& vecIdx );

public:
	void Render();
};

