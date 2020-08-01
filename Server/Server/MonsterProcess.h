#pragma once
#include "Process.h"

class CMonsterProcess : public CProcess
{
public:
	explicit CMonsterProcess();
	virtual ~CMonsterProcess();

private:
	function<void(unsigned int, unsigned int)> m_fpMonsterUpdate[OBJ_STATE_END];
	function<void(unsigned int)>	m_fpMonsterByPlayer;

public:
	void BindMonsterUpdate() {
		m_fpMonsterUpdate[OBJ_STATE_ATTACK] = [&](unsigned int uiMonster, unsigned int uiTarget) {this->AttackEvent(uiMonster, uiTarget); };
		m_fpMonsterUpdate[OBJ_STATE_FOLLOW] = [&](unsigned int uiMonster, unsigned int uiTarget) {this->FollowEvent(uiMonster, uiTarget); };
		m_fpMonsterUpdate[OBJ_STATE_EVASION] = [&](unsigned int uiMonster, unsigned int uiTarget) {EvastionEvent(uiMonster, uiTarget); };
		m_fpMonsterUpdate[OBJ_STATE_IDLE] = [&](unsigned int uiMonster, unsigned int uiTarget) {this->IdleEvent(uiTarget); };
		m_fpMonsterUpdate[OBJ_STATE_DIE] = [&](unsigned int uiMonster, unsigned int uiTarget) {this->DieEvent(uiMonster); };
		m_fpMonsterUpdate[OBJ_STATE_HEAL] = [&](unsigned int uiMonster, unsigned int uiTarget) {this->HealEvent(uiMonster); };
	}
	void BindMonsterDamaged() {

	}

public:
	void UpdateMonster(char _state, unsigned int uiID, unsigned int uiTarget) {
		m_fpMonsterUpdate[_state](uiID, uiTarget);
	}
	void UpdateMonsterByPlayer() {

	}
	void UpdatePlayerByMonster() {

	}
public:
	bool ObjectRangeCheck(Vec3& player, Vec3& other, float fDistance);

public:
	void AttackEvent(unsigned int uiMonster, unsigned int uiTarget);
	void FollowEvent(unsigned int uiMonster, unsigned int uiTarget);
	void EvastionEvent(unsigned int uiMonster, unsigned int uiTarget);
	void IdleEvent(unsigned int uiMonster);
	void DieEvent(unsigned int uiMonster);
	void RespawnEvent(unsigned int uiMonster);
	void HealEvent(unsigned int uiMonster);

public:
	void InRangePlayer(concurrent_unordered_set<unsigned int>& cusLogin, concurrent_unordered_set<unsigned int>& cusList, unsigned int uiMonster);
};

