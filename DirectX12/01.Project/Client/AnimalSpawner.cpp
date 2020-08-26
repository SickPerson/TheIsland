#include "stdafx.h"
#include "AnimalSpawner.h"

#include "AnimalScript.h"

#include <Engine/NaviMgr.h>

#include <iostream>
#include "../../../Server/Server/protocol.h"

CAnimalSpawner::CAnimalSpawner(BEHAVIOR_TYPE eType) :
	CScript((UINT)SCRIPT_TYPE::WORLDSCRIPT),
	m_eSpawnType(eType),
	m_iCurrentCount(0),
	m_fCurrentTime(0.f)
{
	switch (eType)
	{
	case B_WARLIKE:
		m_fSpawnTime = WARLIKE_RESPAWN_TIME;
		m_iMaxSpawnCount = MAX_WARLIKE_COUNT;
		m_fRadius = WARLIKE_SPAWN_RADIUS;
		break;
	case B_PASSIVE:
		m_fSpawnTime = PASSIVE_RESPAWN_TIME;
		m_iMaxSpawnCount = MAX_PASSIVE_COUNT;
		m_fRadius = PASSIVE_SPAWN_RADIUS;
		break;
	case B_EVASION:
		m_fSpawnTime = EVASION_RESPAWN_TIME;
		m_iMaxSpawnCount = MAX_EVASION_COUNT;
		m_fRadius = EVASION_SPAWN_RADIUS;
		break;
	default:
		break;
	}
}

CAnimalSpawner::~CAnimalSpawner()
{
}

void CAnimalSpawner::Update()
{
	if (m_iCurrentCount >= m_iMaxSpawnCount)
	{
		return;
	}

	m_fCurrentTime += DT;
	if (m_fCurrentTime >= m_fSpawnTime)
	{
		Respawn();
	}
}

void CAnimalSpawner::SpawnStartAnimal()
{
	Respawn();
	Respawn();
	Respawn();
}

void CAnimalSpawner::Respawn()
{
	if (m_iCurrentCount < m_iMaxSpawnCount)
	{
		CGameObject* pObject = NULL;
		switch (m_eSpawnType)
		{
		case B_WARLIKE:
		{
			// °õ
			Ptr<CMeshData> pBearTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\bear.mdat", L"MeshData\\bear.mdat");
			pObject = pBearTex->Instantiate();
#ifdef CHECK_COLLISTION
			pObject->AddComponent( new CCollider2D );

			pObject->Collider2D()->SetCollider2DType( COLLIDER2D_TYPE::SPHERE );
			pObject->Collider2D()->SetOffsetPos( Vec3( 0.f, 50.f, 0.f ) );
			pObject->Collider2D()->SetOffsetScale( Vec3( 30.f, 30.f, 30.f ) );
#endif
			pObject->AddComponent(new CAnimalScript);

			{
				tAnimalStatus tStatus;
				tStatus.fHp = 200.f;
				tStatus.fStamina = 100.f;
				tStatus.fDamage = 20.f;
				tStatus.fSpeed = 150.f;
				tStatus.fBehaviorTime = 4.f;
				tStatus.eType = BEHAVIOR_TYPE::B_WARLIKE;
				tStatus.eKind = ANIMAL_TYPE::A_BEAR;

				Vec3 vOffsetScale = Vec3(2.f, 2.f, 2.f);

				pObject->GetScript<CAnimalScript>()->SetAnimalStatus(tStatus);
				pObject->GetScript<CAnimalScript>()->SetOffsetScale(vOffsetScale);
			}

			pObject->SetName(L"Bear");

			//pObject->Transform()->SetLocalPos(Vec3(1500.f, 20.f, 2000.f));
			pObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
			pObject->Transform()->SetLocalScale(Vec3(20.f, 20.f, 20.f));
		}
			break;
		case B_PASSIVE:
		{
			bool iRandom = rand() % 2;
			if (iRandom)
			{
				Ptr<CMeshData> pWolfTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\wolf.mdat", L"MeshData\\wolf.mdat");
				pObject = pWolfTex->Instantiate();
#ifdef CHECK_COLLISTION
				pObject->AddComponent( new CCollider2D );

				pObject->Collider2D()->SetCollider2DType( COLLIDER2D_TYPE::SPHERE );
				pObject->Collider2D()->SetOffsetPos( Vec3( 0.f, 50.f, 0.f ) );
				pObject->Collider2D()->SetOffsetScale( Vec3( 30.f, 30.f, 30.f ) );
#endif
				pObject->AddComponent(new CAnimalScript);

				{
					tAnimalStatus tStatus;
					tStatus.fHp = 200.f;
					tStatus.fStamina = 100.f;
					tStatus.fDamage = 20.f;
					tStatus.fSpeed = 200.f;
					tStatus.fBehaviorTime = 4.f;
					tStatus.eType = BEHAVIOR_TYPE::B_PASSIVE;
					tStatus.eKind = ANIMAL_TYPE::A_WOLF;

					Vec3 vOffsetScale = Vec3(2.f, 2.f, 2.f);

					pObject->GetScript<CAnimalScript>()->SetAnimalStatus(tStatus);
					pObject->GetScript<CAnimalScript>()->SetOffsetScale(vOffsetScale);
				}

				pObject->SetName(L"Wolf");

				//pObject->Transform()->SetLocalPos(Vec3(-0.f, 20.f, 4000.f));
				pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
				pObject->Transform()->SetLocalScale(Vec3(20.f, 20.f, 20.f));
			}
			else
			{
				Ptr<CMeshData> pBoarTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\boar.mdat", L"MeshData\\boar.mdat");
				pObject = pBoarTex->Instantiate();
#ifdef CHECK_COLLISTION
				pObject->AddComponent( new CCollider2D );

				pObject->Collider2D()->SetCollider2DType( COLLIDER2D_TYPE::SPHERE );
				pObject->Collider2D()->SetOffsetPos( Vec3( 0.f, 50.f, 0.f ) );
				pObject->Collider2D()->SetOffsetScale( Vec3( 600.f, 600.f, 600.f ) );
#endif
				pObject->AddComponent(new CAnimalScript);

				{
					tAnimalStatus tStatus;
					tStatus.fHp = 200.f;
					tStatus.fStamina = 100.f;
					tStatus.fDamage = 20.f;
					tStatus.fSpeed = 150.f;
					tStatus.fBehaviorTime = 4.f;
					tStatus.eType = BEHAVIOR_TYPE::B_PASSIVE;
					tStatus.eKind = ANIMAL_TYPE::A_BOAR;

					Vec3 vOffsetScale = Vec3(60.f, 60.f, 60.f);

					pObject->GetScript<CAnimalScript>()->SetAnimalStatus(tStatus);
					pObject->GetScript<CAnimalScript>()->SetOffsetScale(vOffsetScale);
				}

				pObject->SetName(L"Boar");

				//pObject->Transform()->SetLocalPos(Vec3(-1500.f, 20.f, 3000.f));
				pObject->Transform()->SetLocalRot(Vec3(-XM_PI / 2.f, 0.f, 0.f));
				pObject->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
			}
		}
			break;
		case B_EVASION:
		{
			Ptr<CMeshData> pDeerTex = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\deer.mdat", L"MeshData\\deer.mdat");
			pObject = pDeerTex->Instantiate();
#ifdef CHECK_COLLISTION
			pObject->AddComponent( new CCollider2D );

			pObject->Collider2D()->SetCollider2DType( COLLIDER2D_TYPE::SPHERE );
			pObject->Collider2D()->SetOffsetPos( Vec3( 0.f, 50.f, 0.f ) );
			pObject->Collider2D()->SetOffsetScale( Vec3( 300.f, 300.f, 300.f ) );
#endif
			pObject->AddComponent(new CAnimalScript);

			{
				tAnimalStatus tStatus;
				tStatus.fHp = 100.f;
				tStatus.fStamina = 100.f;
				tStatus.fDamage = 0.f;
				tStatus.fSpeed = 250.f;
				tStatus.fBehaviorTime = 4.f;
				tStatus.eType = BEHAVIOR_TYPE::B_EVASION;
				tStatus.eKind = ANIMAL_TYPE::A_DEER;

				Vec3 vOffsetScale = Vec3(30.f, 30.f, 30.f);

				pObject->GetScript<CAnimalScript>()->SetAnimalStatus(tStatus);
				pObject->GetScript<CAnimalScript>()->SetOffsetScale(vOffsetScale);
			}

			pObject->SetName(L"Deer");
			pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
			pObject->Transform()->SetLocalScale(Vec3(2.f, 2.f, 2.f));

		}
			break;
		default:
			break;
		}

		CAnimalScript* pAnimalScript = pObject->GetScript<CAnimalScript>();
		pAnimalScript->SetAnimation( pObject->Animator3D() );

		CSceneMgr::GetInst()->GetCurScene()->FindLayer( L"Animal" )->AddGameObject( pObject );

		Vec3 vPos = Transform()->GetLocalPos();
		while (1)
		{
			vPos = Transform()->GetLocalPos();
			float fDistance = (float)(rand() % (int)m_fRadius + 1);
			float fDegree = (float)(rand() % 360);
			float fRadian = fDegree / 180.f * XM_PI;

			vPos.x += cos(fRadian) * fDistance;
			vPos.z += sin(fRadian) * fDistance;

			vPos.y = CNaviMgr::GetInst()->GetY(vPos);

			if (vPos.y > 80.f)
				break;
		}

		pObject->MeshRender()->SetDynamicShadow(true);
		pObject->Transform()->SetLocalPos(vPos);

		pObject->GetScript<CAnimalScript>()->SetAnimalSpawner(this);
		m_iCurrentCount++;
		m_fCurrentTime = 0.f;
	}
}

void CAnimalSpawner::DeadAnimal(int iCount)
{
	m_iCurrentCount += iCount;

	if (m_iCurrentCount > m_iMaxSpawnCount)
		m_iCurrentCount = m_iMaxSpawnCount;
	if (m_iCurrentCount < 0)
		m_iCurrentCount = 0;
}
