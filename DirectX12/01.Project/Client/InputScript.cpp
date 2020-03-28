#include "stdafx.h"
#include "InputScript.h"

#include <Engine/KeyMgr.h>
#include <Engine/FontMgr.h>
#include <Engine/Material.h>

#include <iostream>

CInputScript::CInputScript() :
	CScript((UINT)SCRIPT_TYPE::UISCRIPT),
	m_bEnable(true),
	m_iCount(0)
{
	m_strInput.reserve(10);
	m_vFont.reserve(10);
}


CInputScript::~CInputScript()
{
}

void CInputScript::Init()
{
}

void CInputScript::Update()
{
	if (m_bEnable)
	{
		if (KEY_TAB(KEY_TYPE::KEY_1))
		{
			m_strInput.push_back('1');
			CharInfo tInfo = CFontMgr::GetInst()->GetFontInfo().mCharInfo['1'];
			Ptr<CMaterial> pMtrl = m_vFont[m_iCount++]->MeshRender()->GetSharedMaterial();
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
		}
		else if (KEY_TAB(KEY_TYPE::KEY_2))
		{
			m_strInput.push_back('2');
			CharInfo tInfo = CFontMgr::GetInst()->GetFontInfo().mCharInfo['2'];
			Ptr<CMaterial> pMtrl = m_vFont[m_iCount++]->MeshRender()->GetSharedMaterial();
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
		}
		else if (KEY_TAB(KEY_TYPE::KEY_3))
		{
			m_strInput.push_back('3');
			CharInfo tInfo = CFontMgr::GetInst()->GetFontInfo().mCharInfo['3'];
			Ptr<CMaterial> pMtrl = m_vFont[m_iCount++]->MeshRender()->GetSharedMaterial();
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
		}
		else if (KEY_TAB(KEY_TYPE::KEY_4))
		{
			m_strInput.push_back('4');
			CharInfo tInfo = CFontMgr::GetInst()->GetFontInfo().mCharInfo['4'];
			Ptr<CMaterial> pMtrl = m_vFont[m_iCount++]->MeshRender()->GetSharedMaterial();
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
		}
		else if (KEY_TAB(KEY_TYPE::KEY_1))
		{
			m_strInput.push_back('5');
			CharInfo tInfo = CFontMgr::GetInst()->GetFontInfo().mCharInfo['5'];
			Ptr<CMaterial> pMtrl = m_vFont[m_iCount++]->MeshRender()->GetSharedMaterial();
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
		}
		else if (KEY_TAB(KEY_TYPE::KEY_6))
		{
			m_strInput.push_back('6');
			CharInfo tInfo = CFontMgr::GetInst()->GetFontInfo().mCharInfo['6'];
			Ptr<CMaterial> pMtrl = m_vFont[m_iCount++]->MeshRender()->GetSharedMaterial();
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
		}
		else if (KEY_TAB(KEY_TYPE::KEY_7))
		{
			m_strInput.push_back('7');
			CharInfo tInfo = CFontMgr::GetInst()->GetFontInfo().mCharInfo['7'];
			Ptr<CMaterial> pMtrl = m_vFont[m_iCount++]->MeshRender()->GetSharedMaterial();
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
		}
		else if (KEY_TAB(KEY_TYPE::KEY_8))
		{
			m_strInput.push_back('8');
			CharInfo tInfo = CFontMgr::GetInst()->GetFontInfo().mCharInfo['8'];
			Ptr<CMaterial> pMtrl = m_vFont[m_iCount++]->MeshRender()->GetSharedMaterial();
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
		}
		else if (KEY_TAB(KEY_TYPE::KEY_9))
		{
			m_strInput.push_back('9');
			CharInfo tInfo = CFontMgr::GetInst()->GetFontInfo().mCharInfo['9'];
			Ptr<CMaterial> pMtrl = m_vFont[m_iCount++]->MeshRender()->GetSharedMaterial();
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
		}
		else if (KEY_TAB(KEY_TYPE::KEY_0))
		{
			m_strInput.push_back('0');
			CharInfo tInfo = CFontMgr::GetInst()->GetFontInfo().mCharInfo['0'];
			Ptr<CMaterial> pMtrl = m_vFont[m_iCount++]->MeshRender()->GetSharedMaterial();
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
		}
		else if (KEY_TAB(KEY_TYPE::KEY_DOT))
		{
			m_strInput.push_back('.');
			CharInfo tInfo = CFontMgr::GetInst()->GetFontInfo().mCharInfo['.'];
			Ptr<CMaterial> pMtrl = m_vFont[m_iCount++]->MeshRender()->GetSharedMaterial();
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
		}
		else if (KEY_TAB(KEY_TYPE::KEY_BACKSPACE))
		{
			m_strInput.pop_back();
			Ptr<CMaterial> pMtrl = m_vFont[--m_iCount]->MeshRender()->GetSharedMaterial();
			float startU = 0.f;
			float startV = 0.f;
			float widthU = 0.f;
			float heightV = 0.f;
			pMtrl->SetData(SHADER_PARAM::FLOAT_0, &startU);
			pMtrl->SetData(SHADER_PARAM::FLOAT_1, &widthU);
			pMtrl->SetData(SHADER_PARAM::FLOAT_2, &startV);
			pMtrl->SetData(SHADER_PARAM::FLOAT_3, &heightV);
		}
	}
	else
	{
		/*POINT vPos = CKeyMgr::GetInst()->GetMousePos();
		std::cout << vPos.x << std::endl;
		std::cout << vPos.y << std::endl;
		if (vPos.x >= m_rcBox.x && vPos.x <= m_rcBox.x + m_rcBox.w)
		{
			if (vPos.y >= m_rcBox.y && vPos.x <= m_rcBox.y + m_rcBox.h)
			{
				m_bEnable = true;
			}
		}*/
	}
}

void CInputScript::AddInputObject(CGameObject * pObject)
{
	m_vFont.emplace_back(pObject);
}

void CInputScript::SetRect(Rect rc)
{
	m_rcBox = rc;
}