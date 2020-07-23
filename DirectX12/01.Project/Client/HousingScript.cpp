#include "stdafx.h"
#include "HousingScript.h"

#include <iostream>

CHousingScript::CHousingScript(ITEM_TYPE eType, int iCount)
	: CItemScript((UINT)eType)
{
	m_iMaxCount = HOUSING_MAX_COUNT;
	if (iCount > m_iMaxCount)
		iCount = HOUSING_MAX_COUNT;
	m_iCount = iCount;
}


CHousingScript::~CHousingScript()
{
}

void CHousingScript::Update()
{
	CItemScript::Update();
}

void CHousingScript::Use_Right(CGameObject* pHost, CGameObject* pObj, int num)
{
	std::cout << "Housing Item Right Use" << std::endl;
}

void CHousingScript::Use_Left(CGameObject* pHost, CGameObject* pObj, int num)
{
	std::cout << "Housing Item Left Use" << std::endl;
}