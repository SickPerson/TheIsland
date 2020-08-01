#pragma once
#include "Housing.h"
#include <Engine/Ptr.h>

#define MAX_GRADE 3

class CMeshData;

class CHousingMgr
{
	SINGLE(CHousingMgr);
private:
	Ptr<CMeshData>	m_pTex[HOUSING_END][MAX_GRADE];
public:
	void Init();
	Ptr<CMeshData> GetHousingMeshData(HOUSING_TYPE eType, UINT iGrade = 0);
};

