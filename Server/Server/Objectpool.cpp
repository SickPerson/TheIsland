#include "Objectpool.h"

CObjectpool::CObjectpool()
{
	Initalize();
}


CObjectpool::~CObjectpool()
{
	Release();
}

void CObjectpool::Initalize()
{
	Init_PlayerPool();
	Init_AnimalPool();
	Init_NaturalPool();
	Init_HousingPool();
}

void CObjectpool::Release()
{
	Release_PlayerPool();
	Release_AnimalPool();
	Release_NaturalPool();
	Release_HousingPool();
}

void CObjectpool::Init_PlayerPool()
{
	m_cumPlayerPool.clear();

	for (int i = 0; i < MAX_USER; ++i)
		m_cumPlayerPool.insert(make_pair(i, new CPlayer()));
}

void CObjectpool::Init_AnimalPool()
{
	// MonsterPool ÃÊ±âÈ­
	m_cumAnimalPool.clear();

	Vec3 Evastion_Pawner = Vec3(10000.f, 0.f, 10000.f);
	Vec3 Warlike_Pawner = Vec3(8100.f, 0.f, 16200.f);
	Vec3 Passive_Pawner = Vec3(6900.f, 0.f, 5285.f);

	float fRadius;

	CMonster*	Animal;

	for (int i = 0; i < MAX_ANIMAL; ++i)
	{
		Animal = new CMonster();
		Animal->SetWakeUp(false);
		Animal->SetState(OBJ_STATE_TYPE::OST_LIVE);
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
		Animal->SetLocalPos(Vec3(18000.f, 200.f, 2000.f));
		Animal->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		m_cumAnimalPool.insert(make_pair(i, Animal));
	}
}

void CObjectpool::Init_NaturalPool()
{
	FILE* pFile = NULL;

	wstring ResPath = L"..\\Data\\Map.dat";
	string FullPath{ ResPath.begin(), ResPath.end() };

	fopen_s(&pFile, FullPath.c_str(), "r");

	int iSize = 0;
	fread(&iSize, sizeof(int), 1, pFile);

	cout << "Natural Number : " << iSize << endl;
	for (int i = 0; i < iSize; ++i)
	{
		CNatural* pNatural = new CNatural();

		wchar_t strName[MAX_PATH]{};
		size_t iLength = 0;
		fread(&iLength, sizeof(size_t), 1, pFile);
		fread(strName, sizeof(wchar_t), iLength, pFile);

		bool bNaturalScript;
		fread(&bNaturalScript, sizeof(bool), 1, pFile);
		if (bNaturalScript)
		{
			wchar_t strPath[MAX_PATH]{};
			fread(&iLength, sizeof(size_t), 1, pFile);
			fread(strPath, sizeof(wchar_t), iLength, pFile);

			if (iLength == 0)
				continue;

			NATURAL_TYPE eType;
			fread(&eType, sizeof(NATURAL_TYPE), 1, pFile);
			pNatural->SetType(eType);
			pNatural->SetOffsetPos(Vec3(0.f, 0.f, 60.f));
			pNatural->SetOffsetScale(Vec3(1.7f, 1.7f, 1.7f));
		}

		Vec3 vLocalPos;
		Vec3 vLocalScale;
		Vec3 vLocalRot;
		fread(&vLocalPos, sizeof(Vec3), 1, pFile);
		pNatural->SetLocalPos(vLocalPos);
		fread(&vLocalScale, sizeof(Vec3), 1, pFile);
		pNatural->SetLocalScale(vLocalScale);
		fread(&vLocalRot, sizeof(Vec3), 1, pFile);
		pNatural->SetLocalRot(vLocalRot);

		string str1;

		if (str1 == "sprucea" || str1 == "sprucec")
		{
		}
		else if (str1 == "spruceb")
		{
		}
		else if (str1 == "plainsgrass")
		{
			pNatural->SetOffsetPos(Vec3(0.f, 0.f, 20.f));
			pNatural->SetOffsetScale(Vec3(100.f, 100.f, 100.f));
		}
		else if (str1 == "mountainsrocks01")
		{
			//pObject->Collider2D()->SetOffsetPos(Vec3(20.f, 20.f, -40.f));
			//pObject->Collider2D()->SetOffsetScale(Vec3(350.f, 350.f, 350.f));
			pNatural->SetOffsetPos(Vec3(20.f, 20.f, -40.f));
			pNatural->SetOffsetScale(Vec3(350.f, 350.f, 350.f));
		}
		else if (str1 == "mountainsrocks02")
		{
			//pObject->Collider2D()->SetOffsetPos(Vec3(20.f, 0.f, 0.f));
			//pObject->Collider2D()->SetOffsetScale(Vec3(220.f, 220.f, 220.f));
			pNatural->SetOffsetPos(Vec3(20.f, 0.f, 0.f));
			pNatural->SetOffsetScale(Vec3(220.f, 220.f, 220.f));
		}
		else if (str1 == "genericcliffa")
		{
			//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 20.f));
			//pObject->Collider2D()->SetOffsetScale(Vec3(10.f, 10.f, 10.f));
			pNatural->SetOffsetPos(Vec3(0.f, 0.f, 20.f));
			pNatural->SetOffsetScale(Vec3(10.f, 10.f, 10.f));
		}
		else if (str1 == "mountainsrocks01_a")
		{
			//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
			//pObject->Collider2D()->SetOffsetScale(Vec3(200.f, 200.f, 200.f));
			pNatural->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
			pNatural->SetOffsetScale(Vec3(200.f, 200.f, 200.f));
		}
		else if (str1 == "mountainsrocks01_b")
		{
			//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, -60.f));
			//pObject->Collider2D()->SetOffsetScale(Vec3(220.f, 220.f, 220.f));
			pNatural->SetOffsetPos(Vec3(0.f, 0.f, -60.f));
			pNatural->SetOffsetScale(Vec3(220.f, 220.f, 220.f));
		}
		else if (str1 == "mountainsrocks01_c")
		{
			//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
			//pObject->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));
			pNatural->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
			pNatural->SetOffsetScale(Vec3(100.f, 100.f, 100.f));
		}
		else if (str1 == "mountainsrocks01_d")
		{
			//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
			//pObject->Collider2D()->SetOffsetScale(Vec3(100.f, 100.f, 100.f));
			pNatural->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
			pNatural->SetOffsetScale(Vec3(100.f, 100.f, 100.f));
		}
		else
		{
			//pObject->Collider2D()->SetOffsetPos(Vec3(0.f, 0.f, 20.f));
			//pObject->Collider2D()->SetOffsetScale(Vec3(4.f, 4.f, 4.f));
			pNatural->SetOffsetPos(Vec3(0.f, 0.f, 20.f));
			pNatural->SetOffsetScale(Vec3(4.f, 4.f, 4.f));
		}
	}
	fclose(pFile);
}

void CObjectpool::Init_HousingPool()
{
}

void CObjectpool::Release_PlayerPool()
{
	for (auto& au : m_cumPlayerPool)
	{
		if (au.second) {
			delete au.second;
			au.second = nullptr;
		}
	}
	m_cumPlayerPool.clear();
}

void CObjectpool::Release_AnimalPool()
{
	for (auto& au : m_cumAnimalPool)
	{
		if (au.second) {
			delete au.second;
			au.second = nullptr;
		}
	}
	m_cumAnimalPool.clear();
}

void CObjectpool::Release_NaturalPool()
{
	for (auto& au : m_cumNaturalPool)
	{
		if (au.second)
		{
			delete au.second;
			au.second = nullptr;
		}
	}
	m_cumNaturalPool.clear();
}

void CObjectpool::Release_HousingPool()
{
	for (auto& au : m_cumHousingPool)
	{
		if (au.second)
		{
			delete au.second;
			au.second = nullptr;
		}
	}
	m_cumHousingPool.clear();
}

void CObjectpool::Install_House(CHousing * pHouse, USHORT usIndex)
{
	m_cumHousingPool.insert(make_pair(usIndex, pHouse));
}

void CObjectpool::Remove_House(USHORT usIndex)
{
}
