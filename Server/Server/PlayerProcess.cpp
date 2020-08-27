#include "stdafx.h"
#include "PlayerProcess.h"
#include "Network.h"
#include "PacketMgr.h"
#include "Player.h"
#include "DataBase.h"
#include "TimerMgr.h"

CPlayerProcess::CPlayerProcess()
{
	if (!CProcess::m_pPlayerPool)
		CProcess::m_pPlayerPool = new class CPlayerpool();
	BindPacketProcess();
}


CPlayerProcess::~CPlayerProcess()
{
	if (m_pPlayerPool)
	{
		delete m_pPlayerPool;
		m_pPlayerPool = nullptr;
	}
}

void CPlayerProcess::Init_Player(USHORT playerId, wchar_t* wcId)
{
	//m_pPlayerPool->m_cumPlayerPool[playerId]->SetState();
	{
		tPlayerStatus tStatus;
		tStatus.fHealth = 100.f;
		tStatus.fHungry = 100.f;
		tStatus.fThirst = 100.f;
		tStatus.fSpeed = 200.f;
		tStatus.fDamage = 20.f;

		m_pPlayerPool->m_cumPlayerPool[playerId]->SetPlayerStatus(tStatus);
	}
	m_pPlayerPool->m_cumPlayerPool[playerId]->SetNumID(playerId);
	m_pPlayerPool->m_cumPlayerPool[playerId]->SetWcID(wcId);
	m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalPos(Vec3(18000.f, 200.f, 2000.f));
	m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalScale(Vec3(1.5f, 1.5f, 1.5f));
	//m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	m_pPlayerPool->m_cumPlayerPool[playerId]->SetConnect(true);
}

void CPlayerProcess::AcceptClient(const SOCKET& sSocket, USHORT playerId)
{
	CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->SetSocket(sSocket);
	CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->SetNumID(playerId);
	CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->SetRecvState();
}

void CPlayerProcess::RecvPacket(USHORT playerId, char * packet, DWORD bytesize)
{
	char* cPacket = CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->RecvEvent(bytesize, packet);

	if (cPacket != nullptr) {
		m_fpPacketProcess[packet[1]](playerId, packet);
		CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->SetRecvState();
	}
}

void CPlayerProcess::PlayerLogin(USHORT playerId, char * packet)
{
	cs_login_packet*	login_packet = reinterpret_cast<cs_login_packet*>(packet);
	
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
			m_pPlayerPool->m_cumPlayerPool[playerId]->SetPlayerStatus(tStatus);
		}
		m_pPlayerPool->m_cumPlayerPool[playerId]->SetDbID(UserInfo.inum);
		m_pPlayerPool->m_cumPlayerPool[playerId]->SetNumID(playerId);
		m_pPlayerPool->m_cumPlayerPool[playerId]->SetWcID(login_packet->player_id);
		m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalPos(vPos);
		m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalScale(Vec3(1.5f, 1.5f, 1.5f));
		m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		m_pPlayerPool->m_cumPlayerPool[playerId]->SetConnect(true);

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
	if(!ExistLoginList(playerId))
		InsertLoginList(playerId);
	InitViewList(playerId);
}

void CPlayerProcess::PlayerMove(USHORT playerId, char * packet)
{
	cout << "Worker " << CTimerMgr::GetInst()->GetDeltaTime() << endl;
	cs_move_packet* move_packet = reinterpret_cast<cs_move_packet*>(packet);

	Vec3 vLocalPos = move_packet->vLocalPos;

	bool bRun = move_packet->bRun;
	Vec3 vWorldDir = move_packet->vWorldDir;

	Vec3 vPos = CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalPos();
	Vec3 vOriginPos = CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalPos();
	float fSpeed = CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->GetSpeed();
	float fHeight = move_packet->fHeight;
	// Walk or Run
	if (bRun)
	{
		fSpeed *= 5.f;
	}
	vPos += vWorldDir * fSpeed * 0.05f;

	//CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalPos(vLocalPos);
	CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalPos(Vec3(vPos.x, fHeight, vPos.z));

	UpdateViewList(playerId);
}

void CPlayerProcess::PlayerLogout(USHORT playerId)
{
	// 이미 Disconnect인 상태일 경우
	if (!m_pPlayerPool->m_cumPlayerPool[playerId]->GetConnect()) 
		return;
	// 아직 Diconnect인 상태가 아닐 경우
	m_pPlayerPool->m_cumPlayerPool[playerId]->SetConnect(false);

	if (ExistLoginList(playerId))
		DeleteLoginList(playerId);

	concurrent_unordered_set<USHORT>list;
	CopyBeforeLoginList(list);
	for(auto& au : list)
		CPacketMgr::Send_Remove_Player_Packet(au, playerId);
}

void CPlayerProcess::PlayerPos(USHORT playerId, char * packet)
{
	cs_pos_packet* pos_packet = reinterpret_cast<cs_pos_packet*>(packet);

	//Vec3 vPrePos = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalPos();
	Vec3 vCurrPos = pos_packet->vLocalPos;

	m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalPos(vCurrPos);
	UpdateViewList(playerId);
	/*if (ObjectRangeCheck(vPrePos, vCurrPos, 10.f)) {
		m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalPos(vCurrPos);
		UpdateViewList(playerId);
	}
	else
		return;*/
}

void CPlayerProcess::PlayerRot(USHORT playerId, char * packet)
{
	cs_rot_packet* rot_packet = reinterpret_cast<cs_rot_packet*>(packet);
	Vec3 vPreRot = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalRot();
	Vec3 vCurrRot = rot_packet->vRot;

	if (vPreRot != vCurrRot) {
		m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalRot(vCurrRot);

		concurrent_unordered_set<USHORT> viewList;

		m_pPlayerPool->m_cumPlayerPool[playerId]->CopyBefore(viewList);

		for (auto& player : viewList)
		{
			CPacketMgr::Send_Rot_Player_Packet(player, playerId);
		}
	}
}

void CPlayerProcess::PlayerAttack(USHORT playerId, char * packet)
{
	cs_attack_packet* attack_packet = reinterpret_cast<cs_attack_packet*>(packet);
	UINT	uiType = attack_packet->attack_uiType;
	USHORT	attack_id = attack_packet->attack_id;

	if ((UINT)ATTACK_TYPE::ANIMAL == uiType)
	{
		m_pMonsterPool->m_cumMonsterPool[attack_id]->SetState(AUT_DAMAGE);
		m_pMonsterPool->m_cumMonsterPool[attack_id]->SetTarget(playerId);
		Update_Event ev;
		ev.m_Do_Object = attack_id;
		ev.m_EventType = EV_MONSTER_UPDATE;
		ev.m_From_Object = playerId;
		ev.m_eObjUpdate = AUT_DAMAGE;
		ev.wakeup_time = high_resolution_clock::now();
		PushEventQueue(ev);
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
	cout << "Animation Packet Recv" << endl;
	cs_animation_packet* animation_packet = reinterpret_cast<cs_animation_packet*>(packet);

	UINT uiType = animation_packet->uiType;

	concurrent_unordered_set<USHORT>	viewList;
	m_pPlayerPool->m_cumPlayerPool[playerId]->CopyPlayerList(viewList);
	for (auto& other : viewList) {
		bool bConnect = m_pPlayerPool->m_cumPlayerPool[other]->GetConnect();
		if (!bConnect) continue;
		CPacketMgr::Send_Animation_Player_Packet(other, uiType);
	}
}

void CPlayerProcess::PlayerUseItem(USHORT playerId, char * packet)
{
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
		Vec3 vPlayerPos = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalPos();
		Vec3 vAnimalPos = m_pMonsterPool->m_cumMonsterPool[AnimalId]->GetLocalPos();
		float fPlayerSpeed = m_pPlayerPool->m_cumPlayerPool[playerId]->GetSpeed();
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

		m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalPos(vPlayerPos);

		UpdateViewList(playerId);
	}
	else
		return;
}

void CPlayerProcess::PlayerCollisionNatural(USHORT playerId, USHORT NaturalId, bool bRun)
{
	if (PlayerAndAnimal_CollisionSphere(playerId, NaturalId, 0.2f))
	{
		Vec3 vAnimalPos = m_pMonsterPool->m_cumMonsterPool[NaturalId]->GetLocalPos();
		Vec3 vPlayerPos = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalPos();
		float fPlayerSpeed = m_pPlayerPool->m_cumPlayerPool[playerId]->GetSpeed();
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

		m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalPos(vPlayerPos);

		UpdateViewList(playerId);
	}
	else
		return;
}

void CPlayerProcess::PlayerCollisionHouse(USHORT playerId, USHORT HouseId, float fHouseHeight)
{
	HOUSING_TYPE eType = m_pHousingPool->m_cumHousingPool[HouseId]->GetType();
	if (eType >= HOUSING_TYPE::HOUSING_FOUNDATION && eType < HOUSING_TYPE::HOUSING_END)
	{
		bool bCollision = PlayerAndHouse_Collision(playerId, HouseId, eType);
		if (bCollision)
		{
			Vec3 vPlayerPos = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalPos();
			Vec3 vBuildPos = m_pHousingPool->m_cumHousingPool[playerId]->GetLocalPos();

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
//		Vec3 vAnimalPos = m_pMonsterPool->m_cumMonsterPool[Animal_Id]->GetLocalPos();
//		Vec3 vPlayerPos = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalPos();
//		float fPlayerSpeed = m_pPlayerPool->m_cumPlayerPool[playerId]->GetSpeed();
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
//		m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalPos(vPlayerPos);
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
//		Vec3 vAnimalPos = m_pMonsterPool->m_cumMonsterPool[naturalId]->GetLocalPos();
//		Vec3 vPlayerPos = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalPos();
//		float fPlayerSpeed = m_pPlayerPool->m_cumPlayerPool[playerId]->GetSpeed();
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
//		m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalPos(vPlayerPos);
//
//		UpdateViewList(playerId);
//	}
//}

//void CPlayerProcess::PlayerCollisionHouse(USHORT playerId, char * packet)
//{
//	cs_collision_packet* collision_packet = reinterpret_cast<cs_collision_packet*>(packet);
//
//	USHORT houseId = collision_packet->id;
//	UINT uiType = m_pHousingPool->m_cumHousingPool[houseId]->GetType();
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

	if(uiType >= HOUSING_WALL && uiType < HOUSING_FLOOR)
		House->SetOffsetPos(Vec3(0.f, 0.f, 120.f));
	House->SetOffsetScale(Vec3(195.f, 195.f, 195.f));
	m_pHousingPool->Install_House(House, house_id);
	
	concurrent_unordered_set<USHORT> loginList;

	CopyBeforeLoginList(loginList);

	for (auto& au : loginList)
	{
		bool bConnect = m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
		if (!bConnect)	continue;
		if (au == playerId) continue;
		CPacketMgr::Send_Install_Housing_Packet(au, house_id);
	}
}

void CPlayerProcess::PlayerRemoveHousing(USHORT playerId, char * packet)
{
	cs_remove_housing_packet* remove_housing_packet = reinterpret_cast<cs_remove_housing_packet*>(packet);

	USHORT house_id = remove_housing_packet->house_id;

	m_pHousingPool->Remove_House(house_id);

	concurrent_unordered_set<USHORT> loginList;

	CopyBeforeLoginList(loginList);

	for (auto& au : loginList)
	{
		bool bConnect = m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
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
	// Player ViewList Update
	Vec3 player_pos = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalPos();

	cout << player_pos.x << " | " << player_pos.y << " | " << player_pos.z << endl;
	concurrent_unordered_set<USHORT> list;
	CopyBeforeLoginList(list);

	// [ Add Player List ]
	for (auto& au : list) {
		if (playerId == au) continue;
		bool bConnect = m_pPlayerPool->m_cumPlayerPool[au]->GetConnect();
		if (false == bConnect) continue;
		else
		{
			Vec3 other_pos = m_pPlayerPool->m_cumPlayerPool[au]->GetLocalPos();

			if (ObjectRangeCheck(player_pos, other_pos, PLAYER_VIEW_RANGE))
			{
				CPacketMgr::Send_Put_Player_Packet(au, playerId);
				CPacketMgr::Send_Put_Player_Packet(playerId, au);
			}
		}
	}

	// [ Add Animal List ]
	for (auto& au : m_pMonsterPool->m_cumMonsterPool) {
		char eType = au.second->GetState();
		if (eType == OBJ_STATE_TYPE::OST_DIE) continue;

		Vec3 vAnimal_Pos = au.second->GetLocalPos();

		if (!au.second->GetWakeUp())
		{
			au.second->SetWakeUp(true);
			if (ObjectRangeCheck(player_pos, vAnimal_Pos, PLAYER_VIEW_RANGE))
			{
				if (ObjectRangeCheck(player_pos, vAnimal_Pos, ANIMAL_VIEW_RANGE))
				{
					PushEvent_Animal_Behavior(au.first, playerId);
				}
				else
				{
					PushEvent_Animal_Idle(au.first, playerId);
				}
				CPacketMgr::Send_Put_Npc_Packet(playerId, au.first);
			}
		}
		else {
			CPacketMgr::Send_Put_Npc_Packet(playerId, au.first);
		}
	}

	// [ Add Housing ]
	for (auto& au : m_pHousingPool->m_cumHousingPool)
	{
		bool bInstall = m_pHousingPool->m_cumHousingPool[au.first]->GetInstall();
		if (!bInstall) continue;
		CPacketMgr::Send_Install_Housing_Packet(playerId, au.first);
	}

	// [ Add Natural ]
	for (auto& au : m_pNaturalPool->m_cumNaturalPool)
	{
		bool bDestroy = m_pNaturalPool->m_cumNaturalPool[au.first]->GetDestroy();
		if (bDestroy) continue;
		CPacketMgr::Send_Put_Natural_Packet(playerId, au.first);
	}
}

void CPlayerProcess::UpdateViewList(USHORT playerId)
{
	// [ Initalize ]
	concurrent_unordered_set<USHORT>	loginList;
	concurrent_unordered_set<USHORT>	beforeViewList;
	concurrent_unordered_set<USHORT>	afterViewList;

	CopyBeforeLoginList(loginList);
	m_pPlayerPool->m_cumPlayerPool[playerId]->CopyBefore(beforeViewList);

	Vec3 vPlayer_Pos = m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalPos();

	// [ Update Player List ]

	// - Player ViewList Update
	for (auto& player : loginList)
	{
		if (player == playerId) continue;
		bool bConnect = m_pPlayerPool->m_cumPlayerPool[player]->GetConnect();
		if (!bConnect) continue;
		Vec3 vOther_Pos = m_pPlayerPool->m_cumPlayerPool[player]->GetLocalPos();
		if (ObjectRangeCheck(vPlayer_Pos, vOther_Pos, PLAYER_VIEW_RANGE)) {
			afterViewList.insert(player);
		}

	}
	// [ Update Animal List ]

	// - Animal ViewList Update
	for (auto& animal : m_pMonsterPool->m_cumMonsterPool)
	{
		char eType = animal.second->GetState();
		if (eType == OBJ_STATE_TYPE::OST_DIE) continue;
		Vec3 vOther_Pos = animal.second->GetLocalPos();
		if (ObjectRangeCheck(vPlayer_Pos, vOther_Pos, PLAYER_VIEW_RANGE))
		{
			afterViewList.insert(animal.first + BEGIN_ANIMAL);
		}
	}

	// - Player Pos Send
	CPacketMgr::Send_Pos_Player_Packet(playerId, playerId);

	// [ Before ViewList Obejct Remove ]
	for (auto& before : beforeViewList)
	{
		if (0 == afterViewList.count(before)) // 업데이트된 ViewList에 존재하지 않을 경우
		{
			if (before < MAX_USER) // Object가 플레이어일 경우
			{
				CPacketMgr::Send_Remove_Player_Packet(playerId, before);
				CPacketMgr::Send_Remove_Player_Packet(before, playerId);
			}
			else if (before < END_ANIMAL) // Object가 동물일 경우
			{
				USHORT usAnimal = before - MAX_USER;
				CPacketMgr::Send_Remove_Npc_Packet(playerId, before);
			}
			else
			{
				cout << before << ": 잘못된 Index 입니다." << endl;
				continue;
			}
		}
	}

	// [ After ViewList Object Update ]
	for (auto& after : afterViewList)
	{
		if (0 == beforeViewList.count(after)) // New ViewList 인 경우
		{
			if (after < MAX_USER)
			{
				CPacketMgr::Send_Put_Player_Packet(playerId, after);
				CPacketMgr::Send_Put_Player_Packet(after, playerId);
			}
			else if (after < END_ANIMAL)
			{
				USHORT usAnimal = after - BEGIN_ANIMAL;
				Vec3 vAnimal_Pos = m_pMonsterPool->m_cumMonsterPool[usAnimal]->GetLocalPos();

				if (ObjectRangeCheck(vPlayer_Pos, vAnimal_Pos, ANIMAL_VIEW_RANGE))
				{
					PushEvent_Animal_Behavior(usAnimal, playerId);
				}
				else
				{
					PushEvent_Animal_Idle(usAnimal, playerId);
				}
				CPacketMgr::Send_Put_Npc_Packet(playerId, usAnimal);
			}
		}
		else // 기존의 ViewList 인 경우
		{
			if (after < MAX_USER)
			{
				CPacketMgr::Send_Pos_Player_Packet(after, playerId);
			}
			else if (after < END_ANIMAL)
			{
				USHORT usAnimal = after - BEGIN_ANIMAL;
				Vec3 vAnimal_Pos = m_pMonsterPool->m_cumMonsterPool[usAnimal]->GetLocalPos();

				if (ObjectRangeCheck(vPlayer_Pos, vAnimal_Pos, ANIMAL_VIEW_RANGE))
				{
					PushEvent_Animal_Behavior(usAnimal, playerId);
				}
				CPacketMgr::Send_Pos_Npc_Packet(playerId, usAnimal);
			}
			else
			{
				cout << after << ": 잘못된 Index 입니다." << endl;
				continue;
			}
		}
	}
}