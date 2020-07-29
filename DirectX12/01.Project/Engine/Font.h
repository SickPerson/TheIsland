#pragma once
#include "Component.h"

#include "Ptr.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"

class CStructuredBuffer;

struct tFontInfo
{
	Vec2 vStartUV;
	Vec2 vWidthUV;
};

class CFont :
	public CComponent
{
	Ptr<CMesh>				m_pMesh;
	Ptr<CMaterial>			m_pMtrl;

	CStructuredBuffer*		m_pStrInfo;
	string					m_strValue;

	int						m_iChangeColorIndex;
	Vec4					m_vFontColor[2];
	Vec4					m_vBackColor;

public:
	CFont();
	virtual ~CFont();
	CLONE(CFont);

	// CComponent을(를) 통해 상속됨
	virtual void SaveToScene(FILE * _pFile) override;
	virtual void LoadFromScene(FILE * _pFile) override;

public:
	void Render();

	void SetString(string strValue);
	string GetString();

	void SetFontColor(Vec4 vColor, int index = 0);
	void SetBackColor(Vec4 vColor);

	void SetChangeColorIndex(int index);
};

