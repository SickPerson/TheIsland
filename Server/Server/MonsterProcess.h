#pragma once
#include "Process.h"

class CMonsterProcess : public CProcess
{
public:
	explicit CMonsterProcess();
	virtual ~CMonsterProcess();

private:
	function<void(USHORT, USHORT)> m_fpMonsterUpdate[AUT_END];
	function<void(USHORT)>	m_fpMonsterByPlayerUpdate;

public:
	void BindMonsterUpdate() {
		m_fpMonsterUpdate[AUT_ATTACK] = [&](USHORT uiMonster, USHORT uiTarget) {this->AttackEvent(uiMonster, uiTarget); };
		m_fpMonsterUpdate[AUT_FOLLOW] = [&](USHORT uiMonster, USHORT uiTarget) {this->FollowEvent(uiMonster, uiTarget); };
		m_fpMonsterUpdate[AUT_EVASION] = [&](USHORT uiMonster, USHORT uiTarget) {EvastionEvent(uiMonster, uiTarget); };
		m_fpMonsterUpdate[AUT_IDLE] = [&](USHORT uiMonster, USHORT uiTarget) {this->IdleEvent(uiTarget); };
		m_fpMonsterUpdate[AUT_DIE] = [&](USHORT uiMonster, USHORT uiTarget) {this->DieEvent(uiMonster); };
		m_fpMonsterUpdate[AUT_DAMAGE] = [&](USHORT AnimalId, USHORT playerId) {DamageEvent(AnimalId, playerId); };
	}
	void BindMonsterDamaged() {

	}

public:
	void UpdateMonster(char udpateType, USHORT AnimalId, USHORT TargetId) {
		m_fpMonsterUpdate[udpateType](AnimalId, TargetId);
	}

public:
	void AttackEvent(USHORT AnimalId, USHORT uiTarget);
	void FollowEvent(USHORT AnimalId, USHORT uiTarget);
	void EvastionEvent(USHORT AnimalId, USHORT uiTarget);
	void IdleEvent(USHORT AnimalId);
	void DieEvent(USHORT AnimalId);
	void RespawnEvent(USHORT AnimalId);
	void DamageEvent(USHORT AnimalId, USHORT playerId);


public:
	void InRangePlayer(concurrent_unordered_set<USHORT>& cusLogin, concurrent_unordered_set<USHORT>& cusList, USHORT uiMonster);
};

