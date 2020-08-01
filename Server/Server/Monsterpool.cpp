#include "Monsterpool.h"
#include <random>
concurrent_unordered_map<unsigned int, CMonster*> CMonsterpool::m_cumMonsterPool;

CMonsterpool::CMonsterpool()
{
	CMonster*	Animal;

	// MonsterPool √ ±‚»≠
	m_cumMonsterPool.clear();

	for (int i = 0; i < MAX_ANIMAL; ++i)
	{
		Animal = new CMonster();
		Animal->SetWakeUp(false);

		if (i < ANIMAL_BEAR)
		{
			tAnimalStatus tStatus;
			tStatus.fHP = 200.f;
			tStatus.fStamina = 100.f;
			tStatus.fDamage = 20.f;
			tStatus.fSpeed = 150.f;
			tStatus.fBehaviorTime = 4.f;
			tStatus.eType = BEHAVIOR_TYPE::B_WARLIKE;
			tStatus.eKind = ANIMAL_TYPE::A_BEAR;

			Animal->SetAnimalStatus(tStatus);
		}
		else if (i < ANIMAL_BOAR)
		{
			tAnimalStatus tStatus;
			tStatus.fHP = 200.f;
			tStatus.fStamina = 100.f;
			tStatus.fDamage = 20.f;
			tStatus.fSpeed = 150.f;
			tStatus.fBehaviorTime = 4.f;
			tStatus.eType = BEHAVIOR_TYPE::B_WARLIKE;
			tStatus.eKind = ANIMAL_TYPE::A_BOAR;

			Animal->SetAnimalStatus(tStatus);
		}
		else if (i < ANIMAL_DEER)
		{
			tAnimalStatus tStatus;
			tStatus.fHP = 100.f;
			tStatus.fStamina = 100.f;
			tStatus.fDamage = 0.f;
			tStatus.fSpeed = 250.f;
			tStatus.fBehaviorTime = 4.f;
			tStatus.eType = BEHAVIOR_TYPE::B_EVASION;
			tStatus.eKind = ANIMAL_TYPE::A_DEER;

			Animal->SetAnimalStatus(tStatus);
		}
		else if (i < ANIMAL_WOLF)
		{
			tAnimalStatus tStatus;
			tStatus.fHP = 200.f;
			tStatus.fStamina = 100.f;
			tStatus.fDamage = 20.f;
			tStatus.fSpeed = 200.f;
			tStatus.fBehaviorTime = 4.f;
			tStatus.eType = BEHAVIOR_TYPE::B_PASSIVE;
			tStatus.eKind = ANIMAL_TYPE::A_WOLF;

			Animal->SetAnimalStatus(tStatus);
		}
		random_device rd;
		default_random_engine dre(rd());
		uniform_real_distribution<float> urd{ -2000.f, 2000.f };
		
		Animal->SetPos(Vec3(urd(dre), 20.f, urd(dre)));
		Animal->SetRot(Vec3(0.f, 0.f, 0.f));
		m_cumMonsterPool.insert(make_pair(i, Animal));
	}
}


CMonsterpool::~CMonsterpool()
{
	m_cumMonsterPool.clear();
}
