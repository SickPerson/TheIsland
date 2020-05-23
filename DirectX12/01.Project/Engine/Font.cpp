#include "stdafx.h"
#include "Font.h"
#include "Transform.h"
#include "StructuredBuffer.h"
#include "Texture.h"

#include "ResMgr.h"
#include "FontMgr.h"

CFont::CFont()
	: CComponent(COMPONENT_TYPE::FONT)
	, m_vFontColor(Vec4(1.f, 1.f, 1.f, 1.f))
	, m_vBackColor(Vec4(0.f, 0.f, 0.f, 0.f))
{
	m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
	m_pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"StrMtrl");
	m_pStrInfo = NULL;

	m_strValue = "NULL";

	Ptr<CTexture> pTex = CFontMgr::GetInst()->GetFontTex();
	m_pMtrl->SetData(SHADER_PARAM::TEX_0, pTex.GetPointer());
}


CFont::~CFont()
{
	SAFE_DELETE(m_pStrInfo);
}

void CFont::SaveToScene(FILE * _pFile)
{
}

void CFont::LoadFromScene(FILE * _pFile)
{
}

void CFont::Render()
{
	if (IsActive() == false || nullptr == m_pMesh)
		return;

	if (NULL == m_pMtrl || NULL == m_pMtrl->GetShader())
		return;

	SAFE_DELETE(m_pStrInfo);
	m_pStrInfo = new CStructuredBuffer;
	vector<tFontInfo> vecFontInfo;
	vecFontInfo.resize(m_strValue.size());

	Transform()->UpdateData();

	for (int i = 0; i < m_strValue.size(); ++i)
	{
		CharInfo tInfo = CFontMgr::GetInst()->GetFontInfo().mCharInfo[m_strValue[i]];
		float sizeX = (float)CFontMgr::GetInst()->GetFontInfo().iScaleX;
		float sizeY = (float)CFontMgr::GetInst()->GetFontInfo().iScaleY;
		float startU = tInfo.ix / sizeX;
		float startV = tInfo.iy / sizeY;
		float widthU = tInfo.iWidth / sizeX;
		float heightV = tInfo.iHeight / sizeY;

		vecFontInfo[i].vStartUV = Vec2(startU, startV);
		vecFontInfo[i].vWidthUV = Vec2(widthU, heightV);

		//m_pMtrl->SetData(SHADER_PARAM::FLOAT_0, &startU);
		//m_pMtrl->SetData(SHADER_PARAM::FLOAT_1, &widthU);
		//m_pMtrl->SetData(SHADER_PARAM::FLOAT_2, &startV);
		//m_pMtrl->SetData(SHADER_PARAM::FLOAT_3, &heightV);
		m_pMtrl->SetData(SHADER_PARAM::VEC4_0, &m_vFontColor);
		m_pMtrl->SetData(SHADER_PARAM::VEC4_1, &m_vBackColor);
	}

	m_pStrInfo->Create(sizeof(tFontInfo), m_strValue.size(), vecFontInfo.data());

	m_pStrInfo->UpdateData(TEXTURE_REGISTER::t10);

	int a = m_strValue.size();;
	m_pMtrl->SetData(SHADER_PARAM::INT_0, &a);

	m_pMtrl->UpdateData();

	m_pMesh->Render();
}

void CFont::SetString(string strValue)
{
	m_strValue = strValue;
}

string CFont::GetString()
{
	return m_strValue;
}
