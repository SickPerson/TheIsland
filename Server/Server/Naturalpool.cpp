#include "Naturalpool.h"

concurrent_unordered_map<USHORT, CNatural*> CNaturalpool::m_cumNaturalPool;

CNaturalpool::CNaturalpool()
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


CNaturalpool::~CNaturalpool()
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
