#include "stdafx.h"
#include "GameOverScript.h"

#include <Engine/RenderMgr.h>
#include <Engine/Font.h>
#include "LoginScene.h"

CGameOverScript::CGameOverScript() :
	CScript((UINT)SCRIPT_TYPE::UISCRIPT),
	m_fTime(GAMEOVER_INTERPOLATION_TIME),
	m_pBackground(NULL),
	m_pMessage(NULL),
	m_pButton(NULL),
	m_pTitle(NULL),
	m_pTitleImage(NULL)
{
}

CGameOverScript::~CGameOverScript()
{
}

void CGameOverScript::Update()
{
	if (m_fTime >= 0.f)
	{
		m_fTime -= DT;
		Vec4 vColor = Vec4(0.0f, 0.0f, 0.0f, 1.f - (m_fTime / GAMEOVER_INTERPOLATION_TIME));
		m_pBackground->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::VEC4_0, &vColor);

		Vec4 vFontColor = Vec4(1.f, 1.f, 1.f, 1.f - (m_fTime / GAMEOVER_INTERPOLATION_TIME));
		m_pTitle->Font()->SetFontColor(vFontColor);
		m_pMessage->Font()->SetFontColor(vFontColor);

		Vec4 vFontColor2 = Vec4(1.f, 0.5f, 0.f, 1.f - (m_fTime / GAMEOVER_INTERPOLATION_TIME));
		m_pButton->Font()->SetFontColor(vFontColor2);
	}

	if (KEY_TAB(KEY_TYPE::KEY_LBTN))
	{
		POINT vPoint = CKeyMgr::GetInst()->GetMousePos();
		tResolution vResolution = CRenderMgr::GetInst()->GetResolution();

		Vec2 vMousePos = Vec2((float)vPoint.x - vResolution.fWidth / 2.f, vResolution.fHeight / 2.f - (float)vPoint.y);
		Vec3 vPos = m_pButton->Transform()->GetLocalPos();
		Vec3 vScale = m_pButton->Transform()->GetLocalScale();

		if (vMousePos.x > vPos.x - vScale.x / 2.f && vMousePos.x < vPos.x + vScale.x / 2.f)
		{
			if (vMousePos.y > vPos.y - vScale.y / 2.f && vMousePos.y < vPos.y + vScale.y / 2.f)
			{
				//
				// 씬 넘어가기전 오브젝트들 정리
				for (UINT i = 0; i < MAX_LAYER; ++i)
				{
					CSceneMgr::GetInst()->GetCurScene()->GetLayer(i)->RemoveAll();
				}

				//DestroyWindow(hWnd);
				PostQuitMessage(0);
				//CScene* pScene = CSceneMgr::GetInst()->GetCurScene();
				//CLoginScene* pLoginScene = pScene->CreateSceneScript<CLoginScene>(L"LoginScene");
			}
		}
	}
}

void CGameOverScript::Init()
{
	tResolution vResolution = CRenderMgr::GetInst()->GetResolution();

	m_pBackground = new CGameObject;
	m_pBackground->AddComponent(new CTransform);
	m_pBackground->AddComponent(new CMeshRender);
	m_pBackground->SetName(L"GameOver Background");

	m_pBackground->Transform()->SetLocalPos(Vec3(0.f, 0.f, 2.f));
	m_pBackground->Transform()->SetLocalScale(Vec3(vResolution.fWidth, vResolution.fHeight, 1.f));
	m_pBackground->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_pBackground->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"HighUIMtrl"));

	Vec4 vColor = Vec4(0.0f, 0.0f, 0.0f, 1.f);
	m_pBackground->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::VEC4_0, &vColor);

	GetObj()->AddChild(m_pBackground);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(m_pBackground);


	m_pTitle = new CGameObject;
	m_pTitle->AddComponent(new CTransform);
	m_pTitle->AddComponent(new CFont);
	m_pTitle->SetName(L"GameOver Title");
	
	m_pTitle->Transform()->SetLocalPos(Vec3(0.f, 120.f, 1.f));
	m_pTitle->Transform()->SetLocalScale(Vec3(400.f, 120.f, 1.f));

	m_pTitle->Font()->SetString("Game Over");

	GetObj()->AddChild(m_pTitle);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(m_pTitle);


	m_pMessage = new CGameObject;
	m_pMessage->AddComponent(new CTransform);
	m_pMessage->AddComponent(new CFont);
	m_pMessage->SetName(L"Message Info");

	m_pMessage->Transform()->SetLocalPos(Vec3(0.f, 0.f, 1.f));
	m_pMessage->Transform()->SetLocalScale(Vec3(850.f, 80.f, 1.f));

	m_pMessage->Font()->SetString("You are dead. Please click the text below to Exit.");

	GetObj()->AddChild(m_pMessage);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(m_pMessage);



	m_pButton = new CGameObject;
	m_pButton->AddComponent(new CTransform);
	m_pButton->AddComponent(new CFont);
	m_pButton->SetName(L"Restart Text");

	m_pButton->Transform()->SetLocalPos(Vec3(0.f, -120.f, 1.f));
	m_pButton->Transform()->SetLocalScale(Vec3(200.f, 80.f, 1.f));

	m_pButton->Font()->SetString("Exit");

	GetObj()->AddChild(m_pButton);
	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(m_pButton);



	Ptr<CTexture> pTitle = CResMgr::GetInst()->Load<CTexture>(L"Title", L"Texture\\Title1.png");
	m_pTitleImage = new CGameObject;
	m_pTitleImage->SetName(L"Title Image");
	m_pTitleImage->AddComponent(new CTransform);
	m_pTitleImage->AddComponent(new CMeshRender);

	m_pTitleImage->Transform()->SetLocalPos(Vec3(0.f, 270.f, 1.f));
	m_pTitleImage->Transform()->SetLocalScale(Vec3(800.f, 260.f, 1.f));

	m_pTitleImage->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_pTitleImage->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"IconMtrl"));
	m_pTitleImage->MeshRender()->GetCloneMaterial()->SetData(SHADER_PARAM::TEX_0, pTitle.GetPointer());
	float fa = 1.f;
	m_pTitleImage->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::FLOAT_0, &fa);

	CSceneMgr::GetInst()->GetCurScene()->FindLayer(L"UI")->AddGameObject(m_pTitleImage);

}
