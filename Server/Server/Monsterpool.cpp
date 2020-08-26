#include "Monsterpool.h"
#include <random>
concurrent_unordered_map<USHORT, CMonster*> CMonsterpool::m_cumMonsterPool;

CMonsterpool::CMonsterpool()
{
	// MonsterPool √ ±‚»≠
	m_cumMonsterPool.clear();

	Vec3 Evastion_Pawner = Vec3(10000.f, 0.f, 10000.f);
	Vec3 Warlike_Pawner = Vec3(8100.f, 0.f, 16200.f);
	Vec3 Passive_Pawner = Vec3(6900.f, 0.f, 5285.f);

	float fRadius;

	CMonster*	Animal;


	for (int i = 0; i < MAX_ANIMAL; ++i)
	{
		Animal = new CMonster();
		Animal->SetWakeUp(false);
		if (i < ANIMAL_BEAR)
		{
			tAnimalStatus tStatus;
			tStatus.fHealth = 200.f;
			tStatus.fStamina = 100.f;
			tStatus.fDamage = 20.f;
			tStatus.fSpeed = 150.f;
			tStatus.eType = BEHAVIOR_TYPE::B_WARLIKE;
			tStatus.eKind = ANIMAL_TYPE::A_BEAR;

			Animal->SetAnimalStatus(tStatus);

			Vec3 vOffsetScale = Vec3(2.f, 2.f, 2.f);
			Animal->SetOffsetScale(vOffsetScale);

			fRadius = 2000.f;

			float fDistance = (float)(rand() % (int)fRadius + 1);
			float fDegree = (float)(rand() % 360);
			float fRadian = fDegree / 100.f * 3.141592654f;

			Vec3 vPos = Warlike_Pawner;
			vPos.x += cos(fRadian) * fDistance;
			vPos.z += sin(fRadian) * fDistance;

			Animal->SetLocalPos(vPos);
		}
		else if (i < ANIMAL_BEAR + ANIMAL_BOAR)
		{
			tAnimalStatus tStatus;
			tStatus.fHealth = 200.f;
			tStatus.fStamina = 100.f;
			tStatus.fDamage = 20.f;
			tStatus.fSpeed = 150.f;
			tStatus.eType = BEHAVIOR_TYPE::B_WARLIKE;
			tStatus.eKind = ANIMAL_TYPE::A_BOAR;

			Animal->SetAnimalStatus(tStatus);

			Vec3 vOffsetScale = Vec3(60.f, 60.f, 60.f);
			Animal->SetOffsetScale(vOffsetScale);

			fRadius = 2000.f;

			float fDistance = (float)(rand() % (int)fRadius + 1);
			float fDegree = (float)(rand() % 360);
			float fRadian = fDegree / 100.f * 3.141592654f;

			Vec3 vPos = Warlike_Pawner;
			vPos.x += cos(fRadian) * fDistance;
			vPos.z += sin(fRadian) * fDistance;

			Animal->SetLocalPos(vPos);
		}
		else if (i < ANIMAL_BEAR + ANIMAL_BOAR + ANIMAL_DEER)
		{
			tAnimalStatus tStatus;
			tStatus.fHealth = 100.f;
			tStatus.fStamina = 100.f;
			tStatus.fDamage = 0.f;
			tStatus.fSpeed = 250.f;
			tStatus.eType = BEHAVIOR_TYPE::B_EVASION;
			tStatus.eKind = ANIMAL_TYPE::A_DEER;

			Animal->SetAnimalStatus(tStatus);

			Vec3 vOffsetScale = Vec3(30.f, 30.f, 30.f);
			Animal->SetOffsetScale(vOffsetScale);

			fRadius = 8000.f;

			float fDistance = (float)(rand() % (int)fRadius + 1);
			float fDegree = (float)(rand() % 360);
			float fRadian = fDegree / 100.f * 3.141592654f;

			Vec3 vPos = Evastion_Pawner;
			vPos.x += cos(fRadian) * fDistance;
			vPos.z += sin(fRadian) * fDistance;

			Animal->SetLocalPos(vPos);
		}
		else if (i < ANIMAL_BEAR + ANIMAL_BOAR + ANIMAL_DEER + ANIMAL_WOLF)
		{
			tAnimalStatus tStatus;
			tStatus.fHealth = 200.f;
			tStatus.fStamina = 100.f;
			tStatus.fDamage = 20.f;
			tStatus.fSpeed = 200.f;
			tStatus.eType = BEHAVIOR_TYPE::B_PASSIVE;
			tStatus.eKind = ANIMAL_TYPE::A_WOLF;

			Animal->SetAnimalStatus(tStatus);

			Vec3 vOffsetScale = Vec3(2.f, 2.f, 2.f);
			Animal->SetOffsetScale(vOffsetScale);

			fRadius = 2000.f;

			float fDistance = (float)(rand() % (int)fRadius + 1);
			float fDegree = (float)(rand() % 360);
			float fRadian = fDegree / 100.f * 3.141592654f;

			Vec3 vPos = Passive_Pawner;
			vPos.x += cos(fRadian) * fDistance;
			vPos.z += sin(fRadian) * fDistance;

			Animal->SetLocalPos(vPos);
		}
		Animal->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		m_cumMonsterPool.insert(make_pair(i, Animal));
	}
}


CMonsterpool::~CMonsterpool()
{
	for (auto& au : m_cumMonsterPool)
	{
		if (au.second) {
			delete au.second;
			au.second = nullptr;
		}
	}
	m_cumMonsterPool.clear();
}
