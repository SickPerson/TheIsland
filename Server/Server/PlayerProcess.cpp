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

void CPlayerProcess::AcceptClient(const SOCKET& sSocket, unsigned int playerId)
{
	CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->SetSocket(sSocket);
	CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->SetNumID(playerId);
	CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->SetRecvState();
}

void CPlayerProcess::RecvPacket(unsigned int playerId, char * packet, DWORD bytesize)
{
	char* cPacket = CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->RecvEvent(bytesize, packet);

	if (cPacket != nullptr) {
		m_fpPacketProcess[packet[1]](playerId, packet);
		CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->SetRecvState();
	}
}

void CPlayerProcess::PlayerLogin(unsigned int playerId, char * packet)
{
	cs_login_packet*	login_packet = reinterpret_cast<cs_login_packet*>(packet);
	
	if (!ExistLoginList(playerId))
	{
		InsertLoginList(playerId);
		CPacketMgr::Send_Login_OK_Packet(playerId);
	}
	else {
		CPacketMgr::Send_Login_Fail_Packet(playerId);
		return;
	}

	m_pPlayerPool->m_cumPlayerPool[playerId]->SetWcID(login_packet->player_id);
	tPlayerStatus tStatus;
	tStatus.fHP = 100.f;
	tStatus.fHungry = 100.f;
	tStatus.fStamina = 100.f;
	tStatus.fThirst = 100.f;
	m_pPlayerPool->m_cumPlayerPool[playerId]->SetPlayerStatus(tStatus);
	m_pPlayerPool->m_cumPlayerPool[playerId]->SetPos(Vec3(0.f, rand() % 1000, 0.f));
	m_pPlayerPool->m_cumPlayerPool[playerId]->SetRot(Vec3(0.f, 180.f, 0.f));
	m_pPlayerPool->m_cumPlayerPool[playerId]->SetConnect(true);

	Vec3 player_pos = m_pPlayerPool->m_cumPlayerPool[playerId]->GetPos();

	concurrent_unordered_set<unsigned int> list;
	CopyBeforeLoginList(list);

	// [ Add Player List ]
	for (auto& au : list) {
		if (false == CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->GetConnect()) continue;
		Vec3 other_pos = CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->GetPos();

		if (true == ObjectRangeCheck(player_pos, other_pos, PLAYER_BETWEEN_RANGE)) {
			CPacketMgr::Send_Put_Player_Packet(au, playerId);
			if (au != playerId)
				CPacketMgr::Send_Put_Player_Packet(playerId, au);
		}
	}
	
	// [ Add Monster List ]
	for (auto& au : m_pMonsterPool->m_cumMonsterPool) {
		if (au.second->GetState() == OBJ_STATE_DIE) continue;
		Vec3 monster_pos = au.second->GetPos();

		if (true == ObjectRangeCheck(player_pos, monster_pos, PLAYER_BETWEEN_RANGE))
		{
			//if (false == au.second->GetWakeUp()) // 몬스터가 깨어나지 않았을때
			//{
			//	Update_Event ev;
			//	ev.m_Do_Object = au.first;
			//	ev.m_EventType = EVENT_TYPE::EV_MONSTER_UPDATE;
			//	ev.m_From_Object = playerId;
			//	ev.m_ObjState = OBJ_STATE_IDLE;
			//	ev.wakeup_time = high_resolution_clock::now();
			//	PushEventQueue(ev);
			//}
			//else // 몬스터가 깨어나 있는 상황일때
			//{
			//	
			//}
			CPacketMgr::Send_Put_Npc_Packet(playerId, au.first);
		}
	}
}

void CPlayerProcess::PlayerMove(unsigned int playerId, char * packet)
{
	cs_move_packet* move_packet = reinterpret_cast<cs_move_packet*>(packet);
	
	Vec3 dir = move_packet->vDir;
	Vec3 pos = CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->GetPos();
	Vec3 rot = move_packet->vRot;
	float speed = move_packet->fSpeed;

	//cout << "전)좌표: " << pos.x << ", " << pos.y << ", " << pos.z << endl;
	/*CTimerMgr::GetInst()->Tick();
	cout << CTimerMgr::GetInst()->GetDeltaTime() << endl;*/
	pos += dir * speed * 0.02f;//CTimerMgr::GetInst()->GetDeltaTime();
	pos.y = 0.f;
	//cout << "후)좌표: " << pos.x << ", " << pos.y << ", " << pos.z << endl;
	CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->SetPos(pos);
	CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->SetRot(rot);

	UpdateViewList(playerId);
}

void CPlayerProcess::PlayerDinconnect(unsigned int playerId)
{
	// 이미 Disconnect인 상태일 경우
	if (!CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->GetConnect()) return;
	// 아직 Diconnect인 상태가 아닐 경우
	CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->SetConnect(false);

	if (ExistLoginList(playerId))
		DeleteLoginList(playerId);

	concurrent_unordered_set<unsigned int>list;
	CopyBeforeLoginList(list);
	for(auto& au : list)
		CPacketMgr::Send_Remove_Player_Packet(au, playerId);
}

void CPlayerProcess::PlayerChat(unsigned int _usID, char * _packet)
{
	cs_chat_packet* chat_packet = reinterpret_cast<cs_chat_packet*>(_packet);

	concurrent_unordered_set<unsigned int> loginList;

	CopyBeforeLoginList(loginList);

	for (auto& player : loginList) {
		CPacketMgr::GetInst()->Send_Chat_Packet(_usID, player, chat_packet->meesage);
	}
}

void CPlayerProcess::UpdateViewList(unsigned int playerId)
{
	concurrent_unordered_set<unsigned int> loginList; // 현재 로그인 리스트
	concurrent_unordered_set<unsigned int> beforeList; // 수정 전 리스트
	concurrent_unordered_set<unsigned int> afterList; // 수정 후 리스트

	// 동접자 리스트를 받아옵니다.
	CopyBeforeLoginList(loginList);
	// 현재 플레이어의 시야처리 내의 리스트를 받아옵니다.
	CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->CopyPlayerList(beforeList);
	// 현재 플레이어 위치를 받아옵니다.
	Vec3 pos = CProcess::m_pPlayerPool->m_cumPlayerPool[playerId]->GetPos();

	// After List에 시야처리 리스트 추가 작업 [ Player ] 
	for (auto& au : loginList) 
	{
		if (au == playerId) continue;
		if (false == CProcess::m_pPlayerPool->m_cumPlayerPool[au]->GetConnect()) continue;
		Vec3 other_pos = CProcess::m_pPlayerPool->m_cumPlayerPool[au]->GetPos();
		if (ObjectRangeCheck(pos, other_pos, PLAYER_BETWEEN_RANGE)) {
			afterList.insert(au);
		}
	}

	// After List에 시야처리 리스트 추가 작업 [ Monster ]
	for (auto& au : m_pMonsterPool->m_cumMonsterPool) {
		// 만약 몬스터가 죽어있으면 continue;
		if (au.second->GetState() == OBJ_STATE_DIE) continue;
		
		Vec3 monster_pos = au.second->GetPos();
		// 만약 몬스터가 플레이어의 범위 내에 들어와 있다면 시야처리에 넣어준다.
		if (ObjectRangeCheck(pos, monster_pos, PLAYER_BETWEEN_RANGE)) {
			afterList.insert(au.first + MAX_USER);
			continue;
		}
	}

	CPacketMgr::Send_Pos_Player_Packet(playerId, playerId);

	for (auto& au : beforeList) {
		if (0 != afterList.count(au)) // ViewList 업데이트 후에도 존재할 경우,
		{
			if(au < MAX_USER)
				CPacketMgr::Send_Pos_Player_Packet(au, playerId);
			else
			{
			}
		}
		else // ViewList 업데이트 후에 존재하지 않을 경우
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

	for (auto& au : afterList) {
		if (0 == beforeList.count(au)) // ViewList 업데이트 전에 존재하지 않았던 경우
		{
			if (au > MAX_USER)
			{
				m_pPlayerPool->m_cumPlayerPool[playerId]->InsertList(au);
				unsigned int monster_id = au - MAX_USER;
				if (!m_pMonsterPool->m_cumMonsterPool[monster_id]->GetWakeUp())
				{
					m_pMonsterPool->m_cumMonsterPool[monster_id]->SetWakeUp(true);

					char type = m_pMonsterPool->m_cumMonsterPool[monster_id]->GetAnimalStatus().eType;
					if (type == B_EVASION) // 회픠형 동물
					{
						Update_Event ev;
						ev.m_Do_Object = monster_id;
						ev.m_From_Object = NO_TARGET;
						ev.m_EventType = EV_MONSTER_UPDATE;
						ev.m_ObjState = OBJ_STATE_IDLE;

						CProcess::PushEventQueue(ev);
					}
					else if (type == B_PASSIVE) // 비선공
					{

					}
					else if (type == B_WARLIKE) // 선공몹
					{

					}
				}
			}
			else
			{
				if (au > MAX_USER)
				{

				}
				CPacketMgr::Send_Put_Player_Packet(playerId, au);
				CPacketMgr::Send_Put_Player_Packet(au, playerId);
			}
		}
	}
}

bool CPlayerProcess::ObjectRangeCheck(Vec3& player, Vec3& other, float fDistance)
{
	if (fDistance < abs(player.x - other.x)) return false;
	if (fDistance < abs(player.z - other.z)) return false;
	return true;
}

//void CPlayerProcess::PlayerPos(unsigned int _usID, char * _Packet)
//{
//	cs_pos_packet* pos_packet = reinterpret_cast<cs_pos_packet*>(_Packet);
//	
//	Vec3 pos = Vec3(pos_packet->vDir);
//	
//	//cout << packet->fPosX << ", " << packet->fPosY << ", " << packet->fPosZ << endl;
//	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetPos(pos);
//
//	UpdateViewList(_usID);
//}
//
//void CPlayerProcess::PlayerRot(unsigned int _usID, char * packet)
//{
//	cs_rot_packet* rot_packet = reinterpret_cast<cs_rot_packet*>(packet);
//
//	Vec2 vDrag = rot_packet->vDrag;
//	Vec3 vRot = rot_packet->vRot;
//
//	CTimerMgr::GetInst()->Tick();
//	vRot.y += vDrag.x * CTimerMgr::GetInst()->GetDeltaTime() * 1.5f;
//
//	if (vRot.y > 360.f)
//		vRot.y -= 360.f;
//
//	CProcess::m_pPlayerPool->m_cumPlayerPool[rot_packet->id]->SetRot(Vec3(0.f, vRot.y, 0.f));
//
//	//cout << rot_packet->id  << ": " << rot_packet->fRotX << ", " << rot_packet->fRotY << ", " << rot_packet->fRotZ << endl;
//
//	UpdateViewList(_usID);
//	//concurrent_unordered_set<unsigned int> loginList;
//
//	/*CopyBefore(loginList);
//
//	for (auto& player : loginList) {
//		if (_usID == player) continue;
//		else
//			CPacketMgr::GetInst()->Send_Rot_Packet(_usID, player);
//	}*/
//}