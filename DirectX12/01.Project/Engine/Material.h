#pragma once
#include "Resource.h"

class CShader;
class CTexture;

class CMaterial :
	public CResource
{
public:
	CMaterial();
	virtual ~CMaterial();

private:
	Ptr<CShader>		m_pShader;
	tMtrlParam			m_tParam;
	Ptr<CTexture>		m_arrTex[( UINT )SHADER_PARAM::TEX_END - ( UINT )SHADER_PARAM::TEX_0];

	bool				m_bFileSave;

public:
	void SetShader( Ptr<CShader> pShader );
	void SetData( SHADER_PARAM eParam, void* pData );
	void DisableFileSave();
	void UpdateData();

	virtual void Load( const wstring& strFullPath );
	virtual void Save( const wstring& strFullPath );

	Ptr<CShader> GetShader();

	virtual CMaterial* Clone();
};

