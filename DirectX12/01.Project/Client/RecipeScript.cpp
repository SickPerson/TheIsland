#include "stdafx.h"
#include "RecipeScript.h"
#include <Engine/Font.h>

CRecipeScript::CRecipeScript(ITEM_TYPE eType) :
	CScript((UINT)SCRIPT_TYPE::ITEMSCRIPT),
	m_eType(eType),
	m_iNum(-1),
	m_bActive(false)
{
	m_vecRecipe = GetItemRecipe(eType);
}

CRecipeScript::~CRecipeScript()
{
	m_vecRecipe.clear();
}

void CRecipeScript::Init()
{
	CGameObject* pObject = new CGameObject;
	pObject->SetName(L"BackGround");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	m_vClickPos = Vec3(470.f, 270.f, 90.f);
	m_vClickScale = Vec3(270.f, 75.f, 1.f);
	pObject->Transform()->SetLocalPos(m_vClickPos);
	pObject->Transform()->SetLocalScale(m_vClickScale);

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"HighUIMtrl")->Clone());

	Vec4 vColor = Vec4(0.3f, 0.3f, 0.5f, 1.f);
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC4_0, &vColor);

	GetObj()->AddChild(pObject);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(pObject); 
	
	pObject = new CGameObject;
	pObject->SetName(L"Icon");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->Transform()->SetLocalPos(Vec3(375.f, 270.f, 50.f));
	pObject->Transform()->SetLocalScale(Vec3(50.f, 50.f, 1.f));

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ItemMtrl")->Clone());

	Ptr<CTexture> pTex = GetItemIconByType(m_eType);
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTex.GetPointer());

	GetObj()->AddChild(pObject);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(pObject);

	pObject = new CGameObject;
	pObject->SetName(L"ItemRecipeName");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CFont);
	pObject->Transform()->SetLocalPos(Vec3(460.f, 290.f, 50.f));
	pObject->Transform()->SetLocalScale(Vec3(90.f, 25.f, 1.f));

	pObject->Font()->SetString(GetItemNameByType(m_eType));
	//pObject->Font()->SetFontColor(Vec4(0.f, 0.f, 0.f, 1.f));

	GetObj()->AddChild(pObject);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(pObject);

	for (int i = 0; i < m_vecRecipe.size(); ++i)
	{
		pObject = new CGameObject;
		pObject->SetName(L"Icon");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CMeshRender);
		pObject->Transform()->SetLocalPos(Vec3(430.f + (65.f * i), 260.f, 50.f));
		pObject->Transform()->SetLocalScale(Vec3(30.f, 30.f, 1.f));

		pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ItemMtrl")->Clone());

		Ptr<CTexture> pTex = GetItemIconByType(m_vecRecipe[i].eItem);
		pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, pTex.GetPointer());

		GetObj()->AddChild(pObject);
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(pObject);

		pObject = new CGameObject;
		pObject->SetName(L"ItemCount");
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CFont);
		pObject->Transform()->SetLocalPos(Vec3(455.f + (65.f * i), 260.f, 50.f));
		pObject->Transform()->SetLocalScale(Vec3(25.f, 25.f, 1.f));

		string strFont = std::to_string(m_vecRecipe[i].iCount);
		pObject->Font()->SetString(strFont);
		//pObject->Font()->SetFontColor(Vec4(0.f, 0.f, 0.f, 1.f));

		GetObj()->AddChild(pObject);
		CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"Invisible")->AddGameObject(pObject);
	}
}

void CRecipeScript::SetNum(UINT iNum)
{
	m_iNum = iNum;
	Vec4 vPos = Vec3(0.f, 0.f, 0.f);
	vPos.y -= 85.f * iNum;
	Transform()->SetLocalPos(vPos);
}

void CRecipeScript::Show(bool bActive)
{
	m_bActive = bActive;

	if (!m_bActive)
	{
		TransferLayer(29, true);
	}
	else
	{
		TransferLayer(30, true);
	}
}

vector<tItemRecipe> CRecipeScript::GetRecipe()
{
	return m_vecRecipe;
}

Vec3 CRecipeScript::GetClickPosition()
{
	Vec3 vPos = Transform()->GetLocalPos() + m_vClickPos;
	return vPos;
}

Vec3 CRecipeScript::GetClickScale()
{
	Vec3 vScale = Transform()->GetLocalScale() * m_vClickScale;
	return vScale;
}

string CRecipeScript::GetRecipeName()
{
	return GetItemNameByType(m_eType);
}

ITEM_TYPE CRecipeScript::GetItemType()
{
	return m_eType;
}

void CRecipeScript::Update()
{
	if (!m_bActive)
	{
		return;
	}
}
