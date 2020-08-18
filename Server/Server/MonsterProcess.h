#pragma once
#include "Process.h"

class CMonsterProcess : public CProcess
{
public:
	explicit CMonsterProcess();
	virtual ~CMonsterProcess();

private:
	function<void(USHORT, USHORT)> m_fpMonsterUpdate[(UINT)ANIMAL_UPDATE_TYPE::END];
	function<void(USHORT)>	m_fpMonsterByPlayerUpdate;

public:
	void BindMonsterUpdate() {
		m_fpMonsterUpdate[(UINT)ANIMAL_UPDATE_TYPE::ATTACK] = [&](USHORT uiMonster, USHORT uiTarget) {this->AttackEvent(uiMonster, uiTarget); };
		m_fpMonsterUpdate[(UINT)ANIMAL_UPDATE_TYPE::FOLLOW] = [&](USHORT uiMonster, USHORT uiTarget) {this->FollowEvent(uiMonster, uiTarget); };
		m_fpMonsterUpdate[(UINT)ANIMAL_UPDATE_TYPE::EVASION] = [&](USHORT uiMonster, USHORT uiTarget) {EvastionEvent(uiMonster, uiTarget); };
		m_fpMonsterUpdate[(UINT)ANIMAL_UPDATE_TYPE::IDLE] = [&](USHORT uiMonster, USHORT uiTarget) {this->IdleEvent(uiTarget); };
		m_fpMonsterUpdate[(UINT)ANIMAL_UPDATE_TYPE::DIE] = [&](USHORT uiMonster, USHORT uiTarget) {this->DieEvent(uiMonster); };
		m_fpMonsterUpdate[(UINT)ANIMAL_UPDATE_TYPE::HEAL] = [&](USHORT uiMonster, USHORT uiTarget) {this->HealEvent(uiMonster); };
		m_fpMonsterUpdate[(UINT)ANIMAL_UPDATE_TYPE::DAMAGE] = [&](USHORT AnimalId, USHORT playerId) {DamageEvent(AnimalId, playerId); };
	}
	void BindMonsterDamaged() {

	}

public:
	void UpdateMonster(char _state, USHORT uiID, USHORT uiTarget) {
		m_fpMonsterUpdate[_state](uiID, uiTarget);
	}

public:
	void AttackEvent(USHORT uiMonster, USHORT uiTarget);
	void FollowEvent(USHORT uiMonster, USHORT uiTarget);
	void EvastionEvent(USHORT uiMonster, USHORT uiTarget);
	void IdleEvent(USHORT uiMonster);
	void DieEvent(USHORT uiMonster);
	void RespawnEvent(USHORT uiMonster);
	void HealEvent(USHORT uiMonster);
	void DamageEvent(USHORT AnimalId, USHORT playerId);

public:
	void PushEvent_Attack(USHORT AnmimalId, USHORT PlayerId);
	void PushEvent_Follow(USHORT AnimalId, USHORT PlayerId);
	void PushEvent_Evastion(USHORT AnimalId, USHORT PlayerId);
	void PushEvent_Idle(USHORT AnimalId);
	void PushEvent_Die(USHORT AnimalId);
	void PushEvent_Respawn(USHORT AnimalId);
	void PushEvent_Heal(USHORT AnimalId);
	void PushEvent_Damage(USHORT AnimalId, USHORT playerId);

public:
	void InRangePlayer(concurrent_unordered_set<USHORT>& cusLogin, concurrent_unordered_set<USHORT>& cusList, USHORT uiMonster);
};

