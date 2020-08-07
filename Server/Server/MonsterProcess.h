#pragma once
#include "Process.h"

class CMonsterProcess : public CProcess
{
public:
	explicit CMonsterProcess();
	virtual ~CMonsterProcess();

private:
	function<void(USHORT, USHORT)> m_fpMonsterUpdate[OBJ_STATE_END];
	function<void(USHORT)>	m_fpMonsterByPlayer;

public:
	void BindMonsterUpdate() {
		m_fpMonsterUpdate[OBJ_STATE_ATTACK] = [&](USHORT uiMonster, USHORT uiTarget) {this->AttackEvent(uiMonster, uiTarget); };
		m_fpMonsterUpdate[OBJ_STATE_FOLLOW] = [&](USHORT uiMonster, USHORT uiTarget) {this->FollowEvent(uiMonster, uiTarget); };
		m_fpMonsterUpdate[OBJ_STATE_EVASION] = [&](USHORT uiMonster, USHORT uiTarget) {EvastionEvent(uiMonster, uiTarget); };
		m_fpMonsterUpdate[OBJ_STATE_IDLE] = [&](USHORT uiMonster, USHORT uiTarget) {this->IdleEvent(uiTarget); };
		m_fpMonsterUpdate[OBJ_STATE_DIE] = [&](USHORT uiMonster, USHORT uiTarget) {this->DieEvent(uiMonster); };
		m_fpMonsterUpdate[OBJ_STATE_HEAL] = [&](USHORT uiMonster, USHORT uiTarget) {this->HealEvent(uiMonster); };
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

public:
	void InRangePlayer(concurrent_unordered_set<USHORT>& cusLogin, concurrent_unordered_set<USHORT>& cusList, USHORT uiMonster);
};

