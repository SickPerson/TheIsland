#include "stdafx.h"
#include "ConstStringScript.h"

#include <Engine/FontMgr.h>
#include <Engine/SceneMgr.h>


CConstStringScript::CConstStringScript() :
	CScript((UINT)SCRIPT_TYPE::UISCRIPT),
	m_vFontColor(Vec4(1.f, 1.f, 1.f, 1.f)),
	m_vFontBackColor(Vec4(0.f, 0.f, 0.f, 0.f))
{
}


CConstStringScript::~CConstStringScript()
{
}

void CConstStringScript::Update()
{
}

void CConstStringScript::Init(string str, Vec4 vFontColor, Vec4 vFontBackColor)
{
	m_strValue = str;
	m_vFontColor = vFontColor;
	m_vFontBackColor = vFontBackColor;

	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vScale = Transform()->GetLocalScale();

	Ptr<CTexture> pFontTex = CFontMgr::GetInst()->GetFontTex();
	for (int i = 0; i < str.size(); ++i)
	{
		CGameObject* pFont = new CGameObject();

		pFont->AddComponent(new CTransform);
		pFont->AddComponent(new CMeshRender);
		pFont->Transform()->SetLocalPos(Vec3(vPos.x + (vScale.x * i), vPos.y, 1.f));
		pFont->Transform()->SetLocalScale(Vec3(vScale.x, vScale.y, 1.f));

		pFont->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pFont->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"FontMtrl"));

		pFont->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, pFontTex.GetPointer());

		CharInfo tInfo = CFontMgr::GetInst()->GetFontInfo().mCharInfo[str[i]];
		Ptr<CMaterial> pMtrl = pFont->MeshRender()->GetSharedMaterial();
		float sizeX = (float)CFontMgr::GetInst()->GetFontInfo().iScaleX;
		float sizeY = (float)CFontMgr::GetInst()->GetFontInfo().iScaleY;
		float startU = tInfo.ix / sizeX;
		float startV = tInfo.iy / sizeY;
		float widthU = tInfo.iWidth / sizeX;
		float heightV = tInfo.iHeight / sizeY;
		pMtrl->SetData(SHADER_PARAM::FLOAT_0, &startU);
		pMtrl->SetData(SHADER_PARAM::FLOAT_1, &widthU);
		pMtrl->SetData(SHADER_PARAM::FLOAT_2, &startV);
		pMtrl->SetData(SHADER_PARAM::FLOAT_3, &heightV);
		pMtrl->SetData(SHADER_PARAM::VEC4_0, &m_vFontColor);
		pMtrl->SetData(SHADER_PARAM::VEC4_1, &m_vFontBackColor);

		AddChild(pFont);
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(pFont);
	}
}

string CConstStringScript::GetString()
{
	return m_strValue;
}

void CConstStringScript::SetFontColor(Vec4 color)
{
	m_vFontColor = color;
	Ptr<CMaterial> pMtrl = MeshRender()->GetSharedMaterial();
	pMtrl->SetData(SHADER_PARAM::VEC4_0, &m_vFontColor);
}

void CConstStringScript::SetBackColor(Vec4 color)
{
	m_vFontBackColor = color;
	Ptr<CMaterial> pMtrl = MeshRender()->GetSharedMaterial();
	pMtrl->SetData(SHADER_PARAM::VEC4_1, &m_vFontBackColor);
}
