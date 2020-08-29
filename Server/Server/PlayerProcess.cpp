#include "stdafx.h"
#include "PlayerProcess.h"
#include "Network.h"
#include "PacketMgr.h"
#include "Player.h"
#include "DataBase.h"
#include "TimerMgr.h"

CPlayerProcess::CPlayerProcess()
{
	BindPacketProcess();
}


CPlayerProcess::~CPlayerProcess()
{
}

void CPlayerProcess::Init_Player(USHORT playerId, char* wcId)
{
	//m_pObjectPool->m_cumPlayerPool[playerId]->SetState();
	{
		tPlayerStatus tStatus;
		tStatus.fHealth = 100.f;
		tStatus.fHungry = 100.f;
		tStatus.fThirst = 100.f;
		tStatus.fSpeed = 200.f;
		tStatus.fDamage = 20.f;

		m_pObjectPool->m_cumPlayerPool[playerId]->SetPlayerStatus(tStatus);
	}
	int DB_Num = 9999;
	m_pObjectPool->m_cumPlayerPool[playerId]->SetDbNum(DB_Num);
	m_pObjectPool->m_cumPlayerPool[playerId]->SetNumID(playerId);
	m_pObjectPool->m_cumPlayerPool[playerId]->SetWcID(wcId);
	m_pObjectPool->m_cumPlayerPool[playerId]->SetLocalPos(Vec3(18000.f, 200.f, 2000.f));
	m_pObjectPool->m_cumPlayerPool[playerId]->SetLocalScale(Vec3(1.5f, 1.5f, 1.5f));
	//m_pObjectPool->m_cumPlayerPool[playerId]->SetLocalRot(Vec3(0.f, 0.f, 0.f));
}

void CPlayerProcess::AcceptClient(SOCKET& sSocket, USHORT playerId)
{
	CProcess::m_pObjectPool->m_cumPlayerPool[playerId]->SetSocket(sSocket);
	CProcess::m_pObjectPool->m_cumPlayerPool[playerId]->SetNumID(playerId);
	CProcess::m_pObjectPool->m_cumPlayerPool[playerId]->SetRecvState();
}

void CPlayerProcess::RecvPacket(USHORT playerId, char * packet, DWORD bytesize)
{
	char* cPacket = CProcess::m_pObjectPool->m_cumPlayerPool[playerId]->RecvEvent(bytesize, packet);

	if (cPacket != nullptr) {
		m_fpPacketProcess[packet[1]](playerId, packet);
		CProcess::m_pObjectPool->m_cumPlayerPool[playerId]->SetRecvState();
	}
}

void CPlayerProcess::PlayerLogin(USHORT playerId, char * packet)
{
	cs_login_packet*	login_packet = reinterpret_cast<cs_login_packet*>(packet);

	if (!ExistLoginList(playerId))
		InsertLoginList(playerId);
#ifdef DB_ON
	// USE DB
	wstring wName = login_packet->player_id;
	if (CDataBase::GetInst()->IsIDExist(wName)) {
		DB_Event UserInfo = CDataBase::GetInst()->GetUserInfo(wName);
		Vec3 vPos = Vec3(UserInfo.x, UserInfo.y, UserInfo.z);
		{

			tPlayerStatus tStatus;
			tStatus.fHealth = UserInfo.fHealth;
			tStatus.fHungry = UserInfo.fHungry;
			tStatus.fThirst = UserInfo.fThirst;
			tStatus.fSpeed = 200.f;
			DatabtStatus.fDamage = 20.f;
			m_pObjectPool->m_cumPlayerPool[playerId]->SetPlayerStatus(tStatus);
		}
		m_pObjectPool->m_cumPlayerPool[playerId]->SetDbNum(UserInfo.inum);
		m_pObjectPool->m_cumPlayerPool[playerId]->SetNumID(playerId);
		m_pObjectPool->m_cumPlayerPool[playerId]->SetWcID(login_packet->player_id);
		m_pObjectPool->m_cumPlayerPool[playerId]->SetLocalPos(vPos);
		m_pObjectPool->m_cumPlayerPool[playerId]->SetLocalScale(Vec3(1.5f, 1.5f, 1.5f));
		m_pObjectPool->m_cumPlayerPool[playerId]->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		m_pObjectPool->m_cumPlayerPool[playerId]->SetConnect(true);

		CPacketMgr::Send_Login_OK_Packet(playerId);
	}
#else
	// NO DB
	 //Player Init
	CPacketMgr::Send_Login_OK_Packet(playerId);
	cout << login_packet->player_id << ": 접속 하였습니다. " << endl;
	Init_Player(playerId, login_packet->player_id);
#endif // DB_ON

	// Server -> Client에 초기 플레이어 값 패킷 전송
	//CPacketMgr::Send_Status_Player_Packet(playerId, playerId);
	//CPacketMgr::Send_Pos_Player_Packet(playerId, playerId);
	InitViewList(playerId);
}

void CPlayerProcess::PlayerMove(USHORT playerId, char * packet)
{
	cout << "Worker " << CTimerMgr::GetInst()->GetDeltaTime() << endl;
	cs_move_packet* move_packet = reinterpret_cast<cs_move_packet*>(packet);

	Vec3 vLocalPos = move_packet->vLocalPos;

	bool bRun = move_packet->bRun;
	Vec3 vWorldDir = move_packet->vWorldDir;

	Vec3 vPos = CProcess::m_pObjectPool->m_cumPlayerPool[playerId]->GetLocalPos();
	Vec3 vOriginPos = CProcess::m_pObjectPool->m_cumPlayerPool[playerId]->GetLocalPos();
	float fSpeed = CProcess::m_pObjectPool->m_cumPlayerPool[playerId]->GetSpeed();
	float fHeight = move_packet->fHeight;
	// Walk or Run
	if (bRun)
	{
		fSpeed *= 5.f;
	}
	vPos += vWorldDir * fSpeed * 0.05f;

	//CProcess::m_pObjectPool->m_cumPlayerPool[playerId]->SetLocalPos(vLocalPos);
	CProcess::m_pObjectPool->m_cumPlayerPool[playerId]->SetLocalPos(Vec3(vPos.x, fHeight, vPos.z));

	UpdateViewList(playerId);
}

void CPlayerProcess::PlayerLogout(USHORT playerId)
{
	// 이미 Disconnect인 상태일 경우
	if (!m_pObjectPool->m_cumPlayerPool[playerId]->GetConnect())
		return;
	// 아직 Diconnect인 상태가 아닐 경우
	m_pObjectPool->m_cumPlayerPool[playerId]->SetConnect(false);
	string name = m_pObjectPool->m_cumPlayerPool[playerId]->GetWcID();
	cout << name << " 님이 로그아웃 하셨습니다. " << endl;
	if (ExistLoginList(playerId))
		DeleteLoginList(playerId);

	concurrent_unordered_set<USHORT>list;
	CopyBeforeLoginList(list);
	for (auto& au : list)
		CPacketMgr::Send_Remove_Packet(au, playerId);
}

void CPlayerProcess::PlayerPos(USHORT playerId, char * packet)
{
	cs_pos_packet* pos_packet = reinterpret_cast<cs_pos_packet*>(packet);

	//Vec3 vPrePos = m_pObjectPool->m_cumPlayerPool[playerId]->GetLocalPos();
	Vec3 vCurrPos = pos_packet->vLocalPos;

	m_pObjectPool->m_cumPlayerPool[playerId]->SetLocalPos(vCurrPos);
	UpdateViewList(playerId);
	/*if (ObjectRangeCheck(vPrePos, vCurrPos, 10.f)) {
		m_pObjectPool->m_cumPlayerPool[playerId]->SetLocalPos(vCurrPos);
		UpdateViewList(playerId);
	}
	else
		return;*/
}

void CPlayerProcess::PlayerRot(USHORT playerId, char * packet)
{
	cs_rot_packet* rot_packet = reinterpret_cast<cs_rot_packet*>(packet);
	Vec3 vPreRot = m_pObjectPool->m_cumPlayerPool[playerId]->GetLocalRot();
	Vec3 vCurrRot = rot_packet->vRot;

	if (vPreRot != vCurrRot) {
		m_pObjectPool->m_cumPlayerPool[playerId]->SetLocalRot(vCurrRot);
	}
}

void CPlayerProcess::PlayerAttack(USHORT playerId, char * packet)
{
	cs_attack_packet* attack_packet = reinterpret_cast<cs_attack_packet*>(packet);
	UINT	uiType = attack_packet->attack_uiType;
	USHORT	attack_id = attack_packet->attack_id;

	if ((UINT)ATTACK_TYPE::ANIMAL == uiType)
	{
		PushEvnet_Animal_Damage(attack_id, playerId);
	}
	else if ((UINT)ATTACK_TYPE::NATURAL == uiType)
	{
		Update_Event ev;
		ev.m_Do_Object = attack_id;
		ev.m_EventType = EV_NATURAL_UPDATE;
		ev.m_From_Object = playerId;
		ev.m_eObjUpdate = NUT_DAMAGE;
		ev.wakeup_time = high_resolution_clock::now();
		PushEventQueue(ev);
	}
}

void CPlayerProcess::PlayerAnimation(USHORT playerId, char * packet)
{
	USHORT User = playerId;

	cs_animation_packet* animation_packet = reinterpret_cast<cs_animation_packet*>(packet);

	UINT uiType = animation_packet->uiType;

	concurrent_unordered_set<USHORT>	UserViewList;
	m_pObjectPool->m_cumPlayerPool[User]->CopyUserViewList(UserViewList);

	for (auto& other : UserViewList) {
		bool bConnect = m_pObjectPool->m_cumPlayerPool[other]->GetConnect();
		if (!bConnect) continue;
		CPacketMgr::Send_Animation_Packet(other, User, uiType);
	}
}

void CPlayerProcess::PlayerUseItem(USHORT playerId, char * packet)
{
	cs_use_item_packet* use_item_packet = reinterpret_cast<cs_use_item_packet*>(packet);
	char eType = use_item_packet->eType;

	float fValue{};
	switch (eType) {
	case ITEM_MEAT:
		fValue = 15.f;
		break;
	case ITEM_COOKMEAT:
		fValue = 20.f;
		break;
	case ITEM_CHOCOLATEBAR:
		fValue = 50.f;
		break;
	case ITEM_CAN:
		fValue = 25.f;
		break;
	case ITEM_APPLE:
		fValue = 15.f;
		break;
	case ITEM_EMPTY_BOTTLE:
		fValue = 0.f;
		break;
	case ITEM_WATER_BOTTLE:
		fValue = 25.f;
		break;
	case ITEM_MEDICKIT:
		fValue = 100.f;
		break;
	case ITEM_BANDAGE:
		fValue = 10.f;
		break;
	default:
		fValue = 0;
		break;
	}
}

bool CPlayerProcess::CollisionSphere(USHORT playerId, USHORT otherId, UINT uiColType, float fOffset)
{
	return false;
}

void CPlayerProcess::PlayerCollision(USHORT playerId, char * packet)
{
	cs_collision_packet* collision_packet = reinterpret_cast<cs_collision_packet*>(packet);
	UINT	uiType = collision_packet->collision_uitype;
	USHORT	collision_id = collision_packet->collision_id;
	bool	bRun = collision_packet->bRun;
	if ((UINT)COLLISION_TYPE::ANIMAL == uiType)
	{
		PlayerCollisionAnimal(playerId, collision_id, bRun);
		//PlayerCollisionAnimal(playerId, packet);
	}
	else if ((UINT)COLLISION_TYPE::NATURAL == uiType)
	{
		PlayerCollisionNatural(playerId, collision_id, bRun);
		//PlayerCollisionNatural(playerId, packet);
	}
	else if ((UINT)COLLISION_TYPE::NATURAL == uiType)
	{
		//PlayerCollisionHouse(playerId, collision_id);
		//PlayerCollisionHouse(playerId, packet);
	}
	else
		return;
}

void CPlayerProcess::PlayerCollisionAnimal(USHORT playerId, USHORT AnimalId, bool bRun)
{
	if (PlayerAndAnimal_CollisionSphere(playerId, AnimalId, 0.2f)) {
		Vec3 vPlayerPos = m_pObjectPool->m_cumPlayerPool[playerId]->GetLocalPos();
		Vec3 vAnimalPos = m_pObjectPool->m_cumAnimalPool[AnimalId]->GetLocalPos();
		float fPlayerSpeed = m_pObjectPool->m_cumPlayerPool[playerId]->GetSpeed();
		Vec3 vDir = XMVector3Normalize(vPlayerPos - vAnimalPos);
		vDir.y = 0.f;

		if (bRun)
		{
			vPlayerPos += vDir * fPlayerSpeed * 0.02f * 5.f;
		}
		else
		{
			vPlayerPos += vDir * fPlayerSpeed * 0.02f;
		}

		m_pObjectPool->m_cumPlayerPool[playerId]->SetLocalPos(vPlayerPos);

		UpdateViewList(playerId);
	}
	else
		return;
}

void CPlayerProcess::PlayerCollisionNatural(USHORT playerId, USHORT NaturalId, bool bRun)
{
	if (PlayerAndAnimal_CollisionSphere(playerId, NaturalId, 0.2f))
	{
		Vec3 vAnimalPos = m_pObjectPool->m_cumAnimalPool[NaturalId]->GetLocalPos();
		Vec3 vPlayerPos = m_pObjectPool->m_cumPlayerPool[playerId]->GetLocalPos();
		float fPlayerSpeed = m_pObjectPool->m_cumPlayerPool[playerId]->GetSpeed();
		Vec3 vDir = XMVector3Normalize(vPlayerPos - vAnimalPos);
		vDir.y = 0.f;

		if (bRun)
		{
			vPlayerPos += vDir * fPlayerSpeed * 0.02f * 5.f;
		}
		else
		{
			vPlayerPos += vDir * fPlayerSpeed * 0.02f;
		}

		m_pObjectPool->m_cumPlayerPool[playerId]->SetLocalPos(vPlayerPos);

		UpdateViewList(playerId);
	}
	else
		return;
}

void CPlayerProcess::PlayerCollisionHouse(USHORT playerId, USHORT HouseId, float fHouseHeight)
{
	HOUSING_TYPE eType = m_pObjectPool->m_cumHousingPool[HouseId]->GetType();
	if (eType >= HOUSING_TYPE::HOUSING_FOUNDATION && eType < HOUSING_TYPE::HOUSING_END)
	{
		bool bCollision = PlayerAndHouse_Collision(playerId, HouseId, eType);
		if (bCollision)
		{
			Vec3 vPlayerPos = m_pObjectPool->m_cumPlayerPool[playerId]->GetLocalPos();
			Vec3 vBuildPos = m_pObjectPool->m_cumHousingPool[playerId]->GetLocalPos();

			float fDiff = vBuildPos.y - vPlayerPos.y;
			if (fDiff < 30.f && (eType == HOUSING_FOUNDATION || eType == HOUSING_FLOOR))
			{
				if (fHouseHeight < vBuildPos.y)
				{

				}
			}
		}
	}
	else
		return;
}

//void CPlayerProcess::PlayerCollisionAnimal(USHORT playerId, char * packet)
//{
//	cs_collision_packet* collision_packet = reinterpret_cast<cs_collision_packet*>(packet);
//
//	USHORT Animal_Id = collision_packet->collision_id;
//	bool bRun = collision_packet->bRun;
//
//	if (PlayerAndAnimal_CollisionSphere(playerId, Animal_Id, 0.2f))
//	{
//		Vec3 vAnimalPos = m_pObjectPool->m_cumAnimalPool[Animal_Id]->GetLocalPos();
//		Vec3 vPlayerPos = m_pObjectPool->m_cumPlayerPool[playerId]->GetLocalPos();
//		float fPlayerSpeed = m_pObjectPool->m_cumPlayerPool[playerId]->GetSpeed();
//		Vec3 vDir = XMVector3Normalize(vPlayerPos - vAnimalPos);
//		vDir.y = 0.f;
//
//		if (bRun)
//		{
//			vPlayerPos += vDir * fPlayerSpeed * 0.02f * 5.f;
//		}
//		else
//		{
//			vPlayerPos += vDir * fPlayerSpeed * 0.02f;
//		}
//
//		m_pObjectPool->m_cumPlayerPool[playerId]->SetLocalPos(vPlayerPos);
//
//		UpdateViewList(playerId);
//	}
//	else
//		return;
//}

//void CPlayerProcess::PlayerCollisionNatural(USHORT playerId, char * packet)
//{
//	cs_collision_packet* collision_packet = reinterpret_cast<cs_collision_packet*>(packet);
//
//	USHORT naturalId = collision_packet->id;
//	bool bRun = collision_packet->bRun;
//
//	if (PlayerAndAnimal_CollisionSphere(playerId, naturalId, 0.2f))
//	{
//		Vec3 vAnimalPos = m_pObjectPool->m_cumAnimalPool[naturalId]->GetLocalPos();
//		Vec3 vPlayerPos = m_pObjectPool->m_cumPlayerPool[playerId]->GetLocalPos();
//		float fPlayerSpeed = m_pObjectPool->m_cumPlayerPool[playerId]->GetSpeed();
//		Vec3 vDir = XMVector3Normalize(vPlayerPos - vAnimalPos);
//		vDir.y = 0.f;
//
//		if (bRun)
//		{
//			vPlayerPos += vDir * fPlayerSpeed * 0.02f * 5.f;
//		}
//		else
//		{
//			vPlayerPos += vDir * fPlayerSpeed * 0.02f;
//		}
//
//		m_pObjectPool->m_cumPlayerPool[playerId]->SetLocalPos(vPlayerPos);
//
//		UpdateViewList(playerId);
//	}
//}

//void CPlayerProcess::PlayerCollisionHouse(USHORT playerId, char * packet)
//{
//	cs_collision_packet* collision_packet = reinterpret_cast<cs_collision_packet*>(packet);
//
//	USHORT houseId = collision_packet->id;
//	UINT uiType = m_pObjectPool->m_cumHousingPool[houseId]->GetType();
//
//	if (uiType >= HOUSING_TYPE::HOUSING_FOUNDATION && uiType < HOUSING_TYPE::HOUSING_END)
//	{
//		bool bCollision = PlayerAndHouse_Collision(playerId, houseId, uiType);
//		if (bCollision)
//		{
//
//		}
//	}
//}

void CPlayerProcess::PlayerInstallHousing(USHORT playerId, char * packet)
{
	cs_install_housing_packet* install_housing_packet = reinterpret_cast<cs_install_housing_packet*>(packet);

	USHORT house_id = m_houseNum++;
	UINT uiType = install_housing_packet->housing_type;
	Vec3 vLocalPos = install_housing_packet->vLocalPos;
	Vec3 vLocalRot = install_housing_packet->vLocalRot;
	Vec3 vLocalScale = install_housing_packet->vLocalScale;
	Vec3 vOffsetPos = install_housing_packet->vOffsetPos;
	Vec3 vOffsetScale = install_housing_packet->vOffsetScale;

	CHousing* House = new CHousing();
	House->SetInstall(true);
	House->SetType((HOUSING_TYPE)uiType);
	House->SetLocalPos(vLocalPos);
	House->SetLocalRot(vLocalRot);
	House->SetLocalScale(vLocalScale);

	if (uiType >= HOUSING_WALL && uiType < HOUSING_FLOOR)
		House->SetOffsetPos(Vec3(0.f, 0.f, 120.f));
	House->SetOffsetScale(Vec3(195.f, 195.f, 195.f));
	m_pObjectPool->Install_House(House, house_id);

	concurrent_unordered_set<USHORT> loginList;

	CopyBeforeLoginList(loginList);

	for (auto& au : loginList)
	{
		bool bConnect = m_pObjectPool->m_cumPlayerPool[au]->GetConnect();
		if (!bConnect)	continue;
		if (au == playerId) continue;
		CPacketMgr::Send_Install_Housing_Packet(au, house_id);
	}
}

void CPlayerProcess::PlayerRemoveHousing(USHORT playerId, char * packet)
{
	cs_remove_housing_packet* remove_housing_packet = reinterpret_cast<cs_remove_housing_packet*>(packet);

	USHORT house_id = remove_housing_packet->house_id;

	m_pObjectPool->Remove_House(house_id);

	concurrent_unordered_set<USHORT> loginList;

	CopyBeforeLoginList(loginList);

	for (auto& au : loginList)
	{
		bool bConnect = m_pObjectPool->m_cumPlayerPool[au]->GetConnect();
		if (!bConnect)	continue;
		if (au == playerId) continue;
		CPacketMgr::Send_Remove_Housing_Packet(au, house_id);
	}
}

void CPlayerProcess::PlayerNaturalAttack(USHORT playerId, char * packet)
{
	cs_natural_attack_packet* natural_attack_packet = reinterpret_cast<cs_natural_attack_packet*>(packet);
}

void CPlayerProcess::PlayerChat(USHORT _usID, char * _packet)
{
	cs_chat_packet* chat_packet = reinterpret_cast<cs_chat_packet*>(_packet);

	concurrent_unordered_set<USHORT> loginList;

	CopyBeforeLoginList(loginList);

	for (auto& player : loginList) {
		CPacketMgr::GetInst()->Send_Chat_Packet(_usID, player, chat_packet->meesage);
	}
}

void CPlayerProcess::InitViewList(USHORT playerId)
{
	USHORT user = playerId;
	// Player ViewList Update

	concurrent_unordered_set<USHORT> loginList;
	CopyBeforeLoginList(loginList);

	// [ Add Player List ]
	Vec3 player_pos = m_pObjectPool->m_cumPlayerPool[user]->GetLocalPos();
	// ------------------- CHECK -------------------- &?????
	for (auto au : loginList) {
		bool bConnect = m_pObjectPool->m_cumPlayerPool[au]->GetConnect();
		if (!bConnect) continue;
		Vec3 other_pos = m_pObjectPool->m_cumPlayerPool[au]->GetLocalPos();
		if (ObjectRangeCheck(player_pos, other_pos, PLAYER_VIEW_RANGE))
		{
			CPacketMgr::Send_Pos_Packet(au, user);
			CPacketMgr::Send_Pos_Packet(user, au);
		}
	}

	// [ Add Animal List ]
	for (auto& au : m_pObjectPool->m_cumAnimalPool) {
		char eType = au.second->GetState();
		if (eType == OBJ_STATE_TYPE::OST_DIE) continue;

		Vec3 Animal_Pos = au.second->GetLocalPos();

		if (ObjectRangeCheck(player_pos, Animal_Pos, PLAYER_VIEW_RANGE))
		{
			if (!au.second->GetWakeUp())
			{
				au.second->SetWakeUp(true);

				// Collision 재확인
				if (PlayerAndAnimal_CollisionSphere(user, au.first))
				{
					PushEvent_Animal_Behavior(au.first, playerId);
				}
				else
				{
					PushEvent_Animal_Idle(au.first, NO_TARGET);
				}
			}
			CPacketMgr::Send_Pos_Packet(playerId, au.first);
		}
	}

	// [ Add Housing ]
	for (auto& au : m_pObjectPool->m_cumHousingPool)
	{
		bool bInstall = m_pObjectPool->m_cumHousingPool[au.first]->GetInstall();
		if (!bInstall) continue;
		CPacketMgr::Send_Install_Housing_Packet(playerId, au.first);
	}

	// [ Add Natural ]
	for (auto& au : m_pObjectPool->m_cumNaturalPool)
	{
		bool bDestroy = m_pObjectPool->m_cumNaturalPool[au.first]->GetDestroy();
		if (bDestroy) continue;
		CPacketMgr::Send_Put_Natural_Packet(playerId, au.first);
	}
}

void CPlayerProcess::UpdateViewList(USHORT playerId)
{
	// [ Initalize ]
	USHORT user = playerId;

	concurrent_unordered_set<USHORT>	loginList;
	concurrent_unordered_set<USHORT>	beforeViewList; // 이동하기 전의 ViewList
	concurrent_unordered_set<USHORT>	afterViewList; // 이동 후의 ViewList

	CopyBeforeLoginList(loginList);
	m_pObjectPool->m_cumPlayerPool[user]->CopyViewList(beforeViewList);

	Vec3 vPlayer_Pos = m_pObjectPool->m_cumPlayerPool[playerId]->GetLocalPos();

	// [ Update Player List ]
	// Player ViewList Update
	for (auto other : loginList)
	{
		if (other == playerId) continue;
		bool bConnect = m_pObjectPool->m_cumPlayerPool[other]->GetConnect();
		if (!bConnect) continue;
		Vec3 vOther_Pos = m_pObjectPool->m_cumPlayerPool[other]->GetLocalPos();
		if (ObjectRangeCheck(vPlayer_Pos, vOther_Pos, PLAYER_VIEW_RANGE)) {
			afterViewList.insert(other);
		}

	}
	// [ Update Animal List ]
	// - Animal ViewList Update
	for (auto& animal : m_pObjectPool->m_cumAnimalPool)
	{
		char eType = animal.second->GetState();
		if (eType == OBJ_STATE_TYPE::OST_DIE) continue;
		Vec3 Other_Pos = animal.second->GetLocalPos();
		if (ObjectRangeCheck(vPlayer_Pos, Other_Pos, PLAYER_VIEW_RANGE))
		{
			afterViewList.insert(animal.first);
		}
	}

	// - Player Pos Send
	CPacketMgr::Send_Pos_Packet(user, user);

	// [ Before ViewList Obejct Remove ]
	for (auto before : beforeViewList)
	{
		if (afterViewList.end() == afterViewList.find(before)) // 업데이트된 ViewList에 존재하지 않을 경우
		{
			if (before < MAX_USER) // Object가 플레이어일 경우
			{
				CPacketMgr::Send_Remove_Packet(user, before);
				CPacketMgr::Send_Remove_Packet(before, user);
			}
			else if (before < END_ANIMAL) // Object가 동물일 경우
			{
				CPacketMgr::Send_Remove_Packet(user, before);
			}
			else
			{
				cout << before << ": 잘못된 Index 입니다." << endl;
				continue;
			}
		}
	}

	// [ After ViewList Object Update ]
	for (auto after : afterViewList)
	{
		if (beforeViewList.end() == beforeViewList.find(after)) // New ViewList 인 경우
		{
			if (after < MAX_USER)
			{
				CPacketMgr::Send_Pos_Packet(user, after);
				CPacketMgr::Send_Pos_Packet(after, user);
			}
			else
			{
				bool bWakeUp = m_pObjectPool->m_cumAnimalPool[after]->GetWakeUp();
				if (!bWakeUp)
				{
					m_pObjectPool->m_cumAnimalPool[after]->SetWakeUp(true);
					
					if (PlayerAndAnimal_CollisionSphere(user, after))
					{
						PushEvent_Animal_Behavior(after, user);
					}
					else
					{
						PushEvent_Animal_Idle(after, NO_TARGET);
					}
				}
				CPacketMgr::Send_Pos_Packet(user, after);
			}
		}
		else // 기존의 ViewList 인 경우
		{
			if (after < MAX_USER)
			{
				CPacketMgr::Send_Pos_Packet(user, after);
				CPacketMgr::Send_Pos_Packet(after, user);
			}
			else
			{
				if (PlayerAndAnimal_CollisionSphere(user, after))
				{
					PushEvent_Animal_Behavior(after, user);
				}
				else
				{
					PushEvent_Animal_Idle(after, NO_TARGET);
				}
				CPacketMgr::Send_Pos_Packet(user, after);
			}
		}
	}
}