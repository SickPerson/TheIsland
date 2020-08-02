#include "stdafx.h"
#include "HousingMgr.h"

#include <Engine/MeshData.h>
#include <Engine/ResMgr.h>

CHousingMgr::CHousingMgr()
{

}


CHousingMgr::~CHousingMgr()
{
}

void CHousingMgr::Init()
{
	//Ptr<CMeshData> pTestMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\door_basic.fbx", 1);
	//pTestMeshData->Save(pTestMeshData->GetPath());

	for (int i = HOUSING_FOUNDATION; i < HOUSING_END; ++i)
	{
		switch ((HOUSING_TYPE)i)
		{
		case HOUSING_FOUNDATION:
		{
			m_pTex[i][0] = CResMgr::GetInst()->Load<CMeshData>(L"housing_foundation.mdat", L"MeshData\\housing_foundation.mdat");
			m_pTex[i][1] = CResMgr::GetInst()->Load<CMeshData>(L"housing_foundation.mdat", L"MeshData\\housing_foundation.mdat");
		}
			break;
		case HOUSING_WALL:
		{
			m_pTex[i][0] = CResMgr::GetInst()->Load<CMeshData>(L"housing_wall.mdat", L"MeshData\\housing_wall.mdat");
			m_pTex[i][1] = CResMgr::GetInst()->Load<CMeshData>(L"housing_wall_stone.mdat", L"MeshData\\housing_wall_stone.mdat");
		}
			break;
		case HOUSING_DOOR:
		{
			m_pTex[i][0] = CResMgr::GetInst()->Load<CMeshData>(L"housing_door.mdat", L"MeshData\\housing_door.mdat");
			m_pTex[i][1] = CResMgr::GetInst()->Load<CMeshData>(L"housing_door_stone.mdat", L"MeshData\\housing_door_stone.mdat");
		}
			break;
		case HOUSING_WINDOW:
		{
			m_pTex[i][0] = CResMgr::GetInst()->Load<CMeshData>(L"housing_window.mdat", L"MeshData\\housing_window.mdat");
			m_pTex[i][1] = CResMgr::GetInst()->Load<CMeshData>(L"housing_window_stone.mdat", L"MeshData\\housing_window_stone.mdat");
		}
			break;
		case HOUSING_FLOOR:
		{
			m_pTex[i][0] = CResMgr::GetInst()->Load<CMeshData>(L"housing_floor.mdat", L"MeshData\\housing_floor.mdat");
			m_pTex[i][1] = CResMgr::GetInst()->Load<CMeshData>(L"housing_floor_stone.mdat", L"MeshData\\housing_floor_stone.mdat");
		}
			break;
		default:
			break;
		}
	}
}

Ptr<CMeshData> CHousingMgr::GetHousingMeshData(HOUSING_TYPE eType, UINT iGrade)
{
	return m_pTex[eType][iGrade];
}
