#pragma once
#include "Process.h"

//enum OBJ_STATE_TYPE {
//	(UINT)ANIMAL_UPDATE_TYPE::ATTACK,
//	(UINT)ANIMAL_UPDATE_TYPE::FOLLOW,
//	(UINT)ANIMAL_UPDATE_TYPE::EVASION,
//	(UINT)ANIMAL_UPDATE_TYPE::IDLE,
//	(UINT)ANIMAL_UPDATE_TYPE::DIE,
//	(UINT)ANIMAL_UPDATE_TYPE::RESPAWN,
//	(UINT)ANIMAL_UPDATE_TYPE::HEAL,
//	OBJ_STATE_END
//};

class CMonsterProcess : public CProcess
{
public:
	explicit CMonsterProcess();
	virtual ~CMonsterProcess();

private:
	function<void(USHORT, USHORT)> m_fpMonsterUpdate[(UINT)ANIMAL_UPDATE_TYPE::END];
	function<void(USHORT)>	m_fpMonsterByPlayer;

public:
	void BindMonsterUpdate() {
		m_fpMonsterUpdate[(UINT)ANIMAL_UPDATE_TYPE::ATTACK] = [&](USHORT monster_id, USHORT target_id) {this->AttackEvent(monster_id, target_id); };
		m_fpMonsterUpdate[(UINT)ANIMAL_UPDATE_TYPE::FOLLOW] = [&](USHORT monster_id, USHORT target_id) {this->FollowEvent(monster_id, target_id); };
		m_fpMonsterUpdate[(UINT)ANIMAL_UPDATE_TYPE::EVASION] = [&](USHORT monster_id, USHORT target_id) {EvastionEvent(monster_id, target_id); };
		m_fpMonsterUpdate[(UINT)ANIMAL_UPDATE_TYPE::IDLE] = [&](USHORT monster_id, USHORT target_id) {this->IdleEvent(target_id); };
		m_fpMonsterUpdate[(UINT)ANIMAL_UPDATE_TYPE::DIE] = [&](USHORT monster_id, USHORT target_id) {this->DieEvent(monster_id); };
		m_fpMonsterUpdate[(UINT)ANIMAL_UPDATE_TYPE::HEAL] = [&](USHORT monster_id, USHORT target_id) {this->HealEvent(monster_id); };
	}
	void BindMonsterDamaged() {

	}

public:
	void UpdateMonster(UINT _state, USHORT uiID, USHORT target_id) {
		m_fpMonsterUpdate[_state](uiID, target_id);
	}
	void UpdateMonsterByPlayer() {

	}
	void UpdatePlayerByMonster() {

	}

public:
	void AttackEvent(USHORT monster_id, USHORT target_id);
	void FollowEvent(USHORT monster_id, USHORT target_id);
	void EvastionEvent(USHORT monster_id, USHORT target_id);
	void IdleEvent(USHORT monster_id);
	void DieEvent(USHORT monster_id);
	void RespawnEvent(USHORT monster_id);
	void HealEvent(USHORT monster_id);

public:
	void PushEvent_Attack(USHORT monster_id, USHORT target_id);
	void PushEvent_Follow(USHORT monster_id, USHORT target_id);
	void PushEvent_Evastion(USHORT monster_id, USHORT target_id);
	void PushEvent_Idle(USHORT monster_id, USHORT target_id);
	void PushEvent_Die(USHORT monster_id, USHORT target_id);
	void PushEvent_Respawn(USHORT monster_id, USHORT target_id);
	void PushEvent_Heal(USHORT monster_id, USHORT target_id);
public:
	void InRangePlayer(concurrent_unordered_set<USHORT>& cusLogin, concurrent_unordered_set<USHORT>& cusList, USHORT monster_id);
};

