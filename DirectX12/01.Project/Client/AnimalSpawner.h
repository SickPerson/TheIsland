#pragma once
#include <Engine/Script.h>
#include "Animal.h"

class CAnimalSpawner :
	public CScript
{
	BEHAVIOR_TYPE	m_eSpawnType;

	float			m_fSpawnTime;
	float			m_fCurrentTime;

	UINT			m_iMaxSpawnCount;
	UINT			m_iCurrentCount;

	float			m_fRadius;

public:
	CAnimalSpawner(BEHAVIOR_TYPE eType);
	virtual ~CAnimalSpawner();

	CLONE(CAnimalSpawner);

	virtual void Update() override;

public:
	void SpawnStartAnimal();

	void Respawn();
	void DeadAnimal(int iCount = 1);
};

