#pragma once
#include "stdafx.h"
#include "Process.h"

class CPlayerProcess:
	public CProcess
{
private:
	recursive_mutex						m_rmPlayerProcessLock;
	function<void(unsigned short, char*)> m_fpPacketProcess[CS_END];
public:
	explicit CPlayerProcess();
	virtual ~CPlayerProcess();

public:
	void BindPacketProcess()
	{
		m_fpPacketProcess[CS_LOGIN] = [&](unsigned short playerId, char* packet){
			PlayerLogin(playerId, packet);
		};
		m_fpPacketProcess[CS_MOVE] = [&](unsigned short playerId, char* packet) {
			PlayerMove(playerId, packet);
		};
		m_fpPacketProcess[CS_CHAT] = [&](unsigned short playerId, char* packet){
			PlayerChat(playerId, packet);
		};
		m_fpPacketProcess[CS_LOGOUT] = [&](unsigned short playerId, char* packet) {
			PlayerLogout(playerId);
		};
		m_fpPacketProcess[CS_ROT] = [&](unsigned short playerId, char* packet) {
			PlayerRot(playerId, packet);
		};
		m_fpPacketProcess[CS_ANIMAL_COLLISION] = [&](unsigned short playerId, char* packet) {
			PlayerCollisionAnimal(playerId, packet);
		};
	}

	void AcceptClient(const SOCKET& sSocket, unsigned short playerId);
	void RecvPacket(unsigned short playerId, char* packet, DWORD bytesize);

	void PlayerLogin(unsigned short playerId, char* packet); // No DB
	void PlayerMove(unsigned short playerId, char* packet);
	void PlayerChat(unsigned short playerId, char* _packet);
	void PlayerLogout(unsigned short playerId);
	void PlayerRot(unsigned short playerId, char* packet);
	void PlayerCollisionAnimal(unsigned short playerId, char* packet);
	void PlayerCollisionNatural(unsigned short playerId, char* packet);

public:
	void UpdateViewList(unsigned short playerId);
};