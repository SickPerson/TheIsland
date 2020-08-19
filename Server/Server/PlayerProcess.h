#pragma once
#include "stdafx.h"
#include "Process.h"

enum class ATTACK_TYPE {
	ANIMAL,
	NATURAL,
	END
};
enum class COLLISION_TYPE {
	ANIMAL,
	NATURAL,
	HOUSE,
	END
};

class CPlayerProcess:
	public CProcess
{
private:
	recursive_mutex					m_rmPlayerProcessLock;
	function<void(USHORT, char*)> m_fpPacketProcess[CS_END];

public:
	explicit CPlayerProcess();
	virtual ~CPlayerProcess();

public:
	void Init_Player(USHORT playerId, wchar_t* wcId);
public:
	void BindPacketProcess()
	{
		m_fpPacketProcess[CS_LOGIN] = [&](USHORT playerId, char* packet){
			PlayerLogin(playerId, packet);
		};
		m_fpPacketProcess[CS_MOVE] = [&](USHORT playerId, char* packet) {
			PlayerMove(playerId, packet);
		};
		m_fpPacketProcess[CS_CHAT] = [&](USHORT playerId, char* packet){
			PlayerChat(playerId, packet);
		};
		m_fpPacketProcess[CS_LOGOUT] = [&](USHORT playerId, char* packet) {
			PlayerLogout(playerId);
		};
		m_fpPacketProcess[CS_ROT] = [&](USHORT playerId, char* packet) {
			PlayerRot(playerId, packet);
		};
		m_fpPacketProcess[CS_COLLISION] = [&](USHORT playerId, char* packet) {
			PlayerCollision(playerId, packet);
		};
		m_fpPacketProcess[CS_HOUSING_INSTALL] = [&](USHORT playerId, char* packet) {
			PlayerInstallHousing(playerId, packet);
		};
		m_fpPacketProcess[CS_HOUSING_REMOVE] = [&](USHORT playerId, char* packet) {
			PlayerRemoveHousing(playerId, packet);
		};
		m_fpPacketProcess[CS_ATTACK] = [&](USHORT playerId, char* packet) {
			PlayerAttack(playerId, packet);
		};
	}

	void AcceptClient(const SOCKET& sSocket, USHORT playerId);
	void RecvPacket(USHORT playerId, char* packet, DWORD bytesize);

	void PlayerLogin(USHORT playerId, char* packet); // No DB
	void PlayerMove(USHORT playerId, char* packet);
	void PlayerChat(USHORT playerId, char* _packet);
	void PlayerLogout(USHORT playerId);
	void PlayerRot(USHORT playerId, char* packet);
	void PlayerAttack(USHORT playerId, char* packet);

public: // Collision
	void PlayerCollision(USHORT playerId, char* packet);
	void PlayerCollisionAnimal(USHORT playerId, USHORT AnimalId, bool bRun);
	void PlayerCollisionNatural(USHORT playerId, USHORT NaturalId, bool bRun);
	void PlayerCollisionHouse(USHORT playerId, USHORT HouseId, float fHouseHeight);
	void PlayerInstallHousing(USHORT playerId, char* packet);
	void PlayerRemoveHousing(USHORT playerId, char* packet);
	void PlayerNaturalAttack(USHORT playerId, char* packet);

public:
	void InitViewList(USHORT playerId);
	void UpdateViewList(USHORT playerId);
};