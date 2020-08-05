#pragma once
#include "Process.h"

class CMonsterProcess : public CProcess
{
public:
	explicit CMonsterProcess();
	virtual ~CMonsterProcess();

private:
	function<void(unsigned short, unsigned short)> m_fpMonsterUpdate[OBJ_STATE_END];
	function<void(unsigned short)>	m_fpMonsterByPlayer;

public:
	void BindMonsterUpdate() {
		m_fpMonsterUpdate[OBJ_STATE_ATTACK] = [&](unsigned short uiMonster, unsigned short uiTarget) {this->AttackEvent(uiMonster, uiTarget); };
		m_fpMonsterUpdate[OBJ_STATE_FOLLOW] = [&](unsigned short uiMonster, unsigned short uiTarget) {this->FollowEvent(uiMonster, uiTarget); };
		m_fpMonsterUpdate[OBJ_STATE_EVASION] = [&](unsigned short uiMonster, unsigned short uiTarget) {EvastionEvent(uiMonster, uiTarget); };
		m_fpMonsterUpdate[OBJ_STATE_IDLE] = [&](unsigned short uiMonster, unsigned short uiTarget) {this->IdleEvent(uiTarget); };
		m_fpMonsterUpdate[OBJ_STATE_DIE] = [&](unsigned short uiMonster, unsigned short uiTarget) {this->DieEvent(uiMonster); };
		m_fpMonsterUpdate[OBJ_STATE_HEAL] = [&](unsigned short uiMonster, unsigned short uiTarget) {this->HealEvent(uiMonster); };
	}
	void BindMonsterDamaged() {

	}

public:
	void UpdateMonster(char _state, unsigned short uiID, unsigned short uiTarget) {
		m_fpMonsterUpdate[_state](uiID, uiTarget);
	}
	void UpdateMonsterByPlayer() {

	}
	void UpdatePlayerByMonster() {

	}

public:
	void AttackEvent(unsigned short uiMonster, unsigned short uiTarget);
	void FollowEvent(unsigned short uiMonster, unsigned short uiTarget);
	void EvastionEvent(unsigned short uiMonster, unsigned short uiTarget);
	void IdleEvent(unsigned short uiMonster);
	void DieEvent(unsigned short uiMonster);
	void RespawnEvent(unsigned short uiMonster);
	void HealEvent(unsigned short uiMonster);

public:

public:
	void InRangePlayer(concurrent_unordered_set<unsigned short>& cusLogin, concurrent_unordered_set<unsigned short>& cusList, unsigned short uiMonster);
};

