#pragma once
#include "stdafx.h"
#include "Process.h"

class CPlayerProcess:
	public CProcess
{
private:
	recursive_mutex						m_rmPlayerProcessLock;
	function<void(USHORT, char*)> m_fpPacketProcess[CS_END];

public:
	explicit CPlayerProcess();
	virtual ~CPlayerProcess();

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
		m_fpPacketProcess[CS_ANIMAL_COLLISION] = [&](USHORT playerId, char* packet) {
			PlayerCollisionAnimal(playerId, packet);
		};
		m_fpPacketProcess[CS_NATURAL_COLLISION] = [&](USHORT playerId, char* packet) {
			PlayerCollisionNatural(playerId, packet);
		};
		m_fpPacketProcess[CS_HOUSING_COLLISION] = [&](USHORT playerId, char* packet) {
			PlayerCollisionHousing(playerId, packet);
		};
		m_fpPacketProcess[CS_HOUSING_INSTALL] = [&](USHORT playerId, char* packet) {
			PlayerInstallHousing(playerId, packet);
		};
	}

	void AcceptClient(const SOCKET& sSocket, USHORT playerId);
	void RecvPacket(USHORT playerId, char* packet, DWORD bytesize);

	void PlayerLogin(USHORT playerId, char* packet); // No DB
	void PlayerMove(USHORT playerId, char* packet);
	void PlayerChat(USHORT playerId, char* _packet);
	void PlayerLogout(USHORT playerId);
	void PlayerRot(USHORT playerId, char* packet);
	void PlayerCollisionAnimal(USHORT playerId, char* packet);
	void PlayerCollisionNatural(USHORT playerId, char* packet);
	void PlayerCollisionHousing(USHORT playerId, char* packet);
	void PlayerInstallHousing(USHORT playerId, char* packet);

public:
	void UpdateViewList(USHORT playerId);
};