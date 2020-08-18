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
	m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalPos(Vec3(10000.f, 200.f, 10000.f));
	m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalScale(Vec3(1.5f, 1.5f, 1.5f));
	m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalRot(Vec3(0.f, 0.f, 0.f));
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
	
	// Database USE
	//wstring wname = login_packet->player_id;
	//if (CDataBase::GetInst()->IsIDExist(wname))
	//{
	//	DB_Event UserInfo = CDataBase::GetInst()->GetUserInfo(wname);
	//	Vec3 vPos = Vec3(UserInfo.fX, UserInfo.fY, UserInfo.fZ);
	//	{
	//		tPlayerStatus tStatus;
	//		tStatus.fHealth = UserInfo.fHealth;
	//		tStatus.fHungry = UserInfo.fHungry;
	//		tStatus.fThirst = UserInfo.fThirst;
	//		tStatus.fSpeed = 200.f;
	//		tStatus.fDamage = 20.f;

	//		m_pPlayerPool->m_cumPlayerPool[playerId]->SetPlayerStatus(tStatus);
	//	}
	//	m_pPlayerPool->m_cumPlayerPool[playerId]->SetDbID(UserInfo.inum);
	//	m_pPlayerPool->m_cumPlayerPool[playerId]->SetNumID(playerId);
	//	m_pPlayerPool->m_cumPlayerPool[playerId]->SetWcID(login_packet->player_id);
	//	m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalPos(vPos);
	//	m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalScale(Vec3(1.5f, 1.5f, 1.5f));
	//	m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalRot(Vec3(0.f, 0.f, 0.f));
	//	m_pPlayerPool->m_cumPlayerPool[playerId]->SetConnect(true);

	//	CPacketMgr::Send_Login_OK_Packet(playerId);
	//}
	//else
	//{
	//	// Player Init
	//	Init_Player(playerId, login_packet->player_id);
	//}

	// NO DB
	 //Player Init
	CPacketMgr::Send_Login_OK_Packet(playerId);
	Init_Player(playerId, login_packet->player_id);
	/*if (!ExistLoginList(playerId))
	{
		InsertLoginList(playerId);
		CPacketMgr::Send_Login_OK_Packet(playerId);
	}
	else {
		CPacketMgr::Send_Login_Fail_Packet(playerId);
		return;
	}*/
	
	// Server -> Client에 초기 플레이어 값 패킷 전송
	CPacketMgr::Send_Status_Player_Packet(playerId, playerId);

	InitViewList(playerId);
}

void CPlayerProcess::PlayerMove(USHORT playerId, char * packet)
{
	cout << "Worker " << CTimerMgr::GetInst()->GetDeltaTime() << endl;
	cs_move_packet* move_packet = reinterpret_cast<cs_move_packet*>(packet);

	Vec3 vLocalPos = move_packet->vLocalPos;

	//Vec3 vLocalRot = move_packet->vLocalRot;
	//bool bRun = move_packet->bRun;
	//Vec3 vWorldDir = move_packet->vWorldDir;

	//Vec3 vPos = CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalPos();
	//Vec3 vOriginPos = CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalPos();
	//float fSpeed = CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->GetSpeed();
	//float fHeight = move_packet->fHeight;
	//// Walk or Run
	//if (bRun)
	//{
	//	fSpeed *= 5.f;
	//}
	//vPos += vWorldDir * fSpeed * CTimerMgr::GetInst()->GetDeltaTime();

	CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalPos(vLocalPos);
	//CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalRot(vLocalRot);
	//CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalPos(Vec3(vPos.x, fHeight, vPos.z));

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

void CPlayerProcess::PlayerRot(USHORT playerId, char * packet)
{
	cs_rot_packet* rot_packet = reinterpret_cast<cs_rot_packet*>(packet);
	USHORT id = rot_packet->id;
	Vec3 vRot = rot_packet->vRot;

	m_pPlayerPool->m_cumPlayerPool[playerId]->SetLocalRot(vRot);

	concurrent_unordered_set<USHORT> viewList;

	m_pPlayerPool->m_cumPlayerPool[playerId]->CopyPlayerList(viewList);

	for (auto& au : viewList)
	{
		CPacketMgr::Send_Rot_Player_Packet(au, playerId);
	}
}

void CPlayerProcess::PlayerAttack(USHORT playerId, char * packet)
{
	cs_attack_packet* attack_packet = reinterpret_cast<cs_attack_packet*>(packet);
	UINT	uiType = attack_packet->attack_uiType;
	USHORT	attack_id = attack_packet->attack_id;

	if ((UINT)ATTACK_TYPE::ANIMAL == uiType)
	{
		m_pMonsterPool->m_cumMonsterPool[attack_id]->SetState((UINT)ANIMAL_UPDATE_TYPE::DAMAGE);
		m_pMonsterPool->m_cumMonsterPool[attack_id]->SetTarget(playerId);
		Update_Event ev;
		ev.m_Do_Object = attack_id;
		ev.m_EventType = EV_MONSTER_UPDATE;
		ev.m_From_Object = playerId;
		ev.m_ObjState = (UINT)ANIMAL_UPDATE_TYPE::DAMAGE;
		ev.wakeup_time = high_resolution_clock::now();
		PushEventQueue(ev);
	}
	else if ((UINT)ATTACK_TYPE::NATURAL == uiType)
	{
		Update_Event ev;
		ev.m_Do_Object = attack_id;
		ev.m_EventType = EV_NATURAL_UPDATE;
		ev.m_From_Object = playerId;
		ev.m_ObjState = (UINT)NATURAL_UPDATE_TYPE::DAMAGE;
		ev.wakeup_time = high_resolution_clock::now();
		PushEventQueue(ev);
	}
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
	House->SetOffsetPos(vOffsetPos);
	House->SetOffsetScale(vOffsetScale);
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

			if (ObjectRangeCheck(player_pos, other_pos, PLAYER_BETWEEN_RANGE))
			{
				CPacketMgr::Send_Put_Player_Packet(au, playerId);
				CPacketMgr::Send_Put_Player_Packet(playerId, au);
			}
		}
	}

	// [ Add Monster List ]
	for (auto& au : m_pMonsterPool->m_cumMonsterPool) {
		//if (au.second->GetState() == OBJ_STATE_DIE) continue;
		Vec3 monster_pos = au.second->GetLocalPos();

		if (true == ObjectRangeCheck(player_pos, monster_pos, PLAYER_BETWEEN_RANGE))
		{
			if (false == au.second->GetWakeUp())
			{
				CPacketMgr::Send_Wakeup_Npc_Packet(playerId, au.first);

				Update_Event ev;
				ev.m_Do_Object = au.first;
				ev.m_EventType = EVENT_TYPE::EV_MONSTER_UPDATE;
				ev.m_From_Object = NO_TARGET;
				//ev.m_ObjState = OBJ_STATE_IDLE;
				ev.wakeup_time = high_resolution_clock::now() + 1s;
				PushEventQueue(ev);
			}
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
	concurrent_unordered_set<USHORT> loginList; // 현재 로그인 리스트
	concurrent_unordered_set<USHORT> beforeList; // 수정 전 리스트
	concurrent_unordered_set<USHORT> afterList; // 수정 후 리스트

	// 동접자 리스트를 받아옵니다.
	CopyBeforeLoginList(loginList);
	// 현재 플레이어의 시야처리 내의 리스트를 받아옵니다.
	CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->CopyPlayerList(beforeList);
	// 현재 플레이어 위치를 받아옵니다.
	Vec3 player_pos = CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->GetLocalPos();

	// After List에 시야처리 리스트 추가 작업 [ Player ] 
	for (auto& au : loginList) 
	{
		if (au == playerId) continue;
		if (false == CProcess::m_pPlayerPool->m_cumPlayerPool[au]->GetConnect()) continue;
		Vec3 other_pos = CProcess::m_pPlayerPool->m_cumPlayerPool[au]->GetLocalPos();
		if (ObjectRangeCheck(player_pos, other_pos, PLAYER_BETWEEN_RANGE)) {
			afterList.insert(au);
			continue;
		}
	}

	//After List에 시야처리 리스트 추가 작업 [ Monster ]
	for (auto& au : m_pMonsterPool->m_cumMonsterPool) {
		// 만약 몬스터가 죽어있으면 continue;
		//if (au.second->GetState() == OBJ_STATE_DIE) continue;
		
		Vec3 monster_pos = au.second->GetLocalPos();
		// 만약 몬스터가 플레이어의 범위 내에 들어와 있다면 시야처리에 넣어준다.
		if (ObjectRangeCheck(player_pos, monster_pos, PLAYER_BETWEEN_RANGE)) {
			afterList.insert(au.first + MAX_USER);
			continue;
		}
	}

	CPacketMgr::Send_Pos_Player_Packet(playerId, playerId);

	for (auto& au : afterList) {
		if (0 == beforeList.count(au)) // ViewList 업데이트 전에 존재하지 않았던 경우
		{
			if (au < MAX_USER) // Player
			{
				CPacketMgr::GetInst()->Send_Put_Player_Packet(playerId, au);
				CPacketMgr::GetInst()->Send_Put_Player_Packet(au, playerId);
			}
			else // Animal
			{
				USHORT monster_id = au - MAX_USER;
				if (false == m_pMonsterPool->m_cumMonsterPool[monster_id]->GetWakeUp())
				{
					Update_Event ev;
					ev.m_Do_Object = monster_id;
					ev.m_EventType = EVENT_TYPE::EV_MONSTER_UPDATE;
					ev.m_From_Object = NO_TARGET;
					//ev.m_ObjState = OBJ_STATE_IDLE;
					ev.wakeup_time = high_resolution_clock::now() + 1s;
					PushEventQueue(ev);
				}
				else {
					char monster_type = m_pMonsterPool->m_cumMonsterPool[monster_id]->GetType();
					Update_Event ev;
					ev.m_Do_Object = monster_id;
					ev.m_EventType = EV_MONSTER_UPDATE;
					ev.m_From_Object = playerId;
					//if (B_WARLIKE == monster_type)
					//	//ev.m_ObjState = OBJ_STATE_FOLLOW;
					//else if (B_PASSIVE == monster_type)
					//	//ev.m_ObjState = OBJ_STATE_IDLE;
					//else if (B_EVASION == monster_type)
					//	//ev.m_ObjState = OBJ_STATE_EVASION;
					ev.wakeup_time = high_resolution_clock::now() + 1s;
					PushEventQueue(ev);
				}
				CPacketMgr::GetInst()->Send_Put_Npc_Packet(playerId, au - MAX_USER);
			}
		}
		else // VIewList 업데이트 전에 존재했을 경우
		{
			CPacketMgr::GetInst()->Send_Pos_Player_Packet(playerId, au);
			CPacketMgr::GetInst()->Send_Pos_Player_Packet(au, playerId);
		}
	}

	for (auto& au : beforeList) {
		if (0 == afterList.count(au)) // ViewList 업데이트 후에도 존재할 경우,
		{
			if (au < MAX_USER)
			{
				CPacketMgr::Send_Remove_Player_Packet(playerId, au);
				CPacketMgr::Send_Remove_Player_Packet(au, playerId);
			}
			else
			{
				CPacketMgr::Send_Remove_Npc_Packet(playerId, au - MAX_USER);
			}
		}
	}
}