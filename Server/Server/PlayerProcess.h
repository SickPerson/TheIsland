#pragma once
#include "stdafx.h"
#include "Process.h"

class CPlayerProcess:
	public CProcess
{
private:
	recursive_mutex						m_rmPlayerProcessLock;
	function<void(unsigned int, char*)> m_fpPacketProcess[CS_END];
public:
	explicit CPlayerProcess();
	virtual ~CPlayerProcess();

public:
	void BindPacketProcess()
	{
		m_fpPacketProcess[CS_LOGIN] = [&](unsigned int playerId, char* packet){
			PlayerLogin(playerId, packet);
		};
		m_fpPacketProcess[CS_MOVE] = [&](unsigned int playerId, char* packet) {
			PlayerMove(playerId, packet);
		};
		m_fpPacketProcess[CS_CHAT] = [&](unsigned int playerId, char* packet){
			PlayerChat(playerId, packet);
		};
		m_fpPacketProcess[CS_LOGOUT] = [&](unsigned int playerId, char* packet) {
			PlayerLogout(playerId);
		};
	}

	void AcceptClient(const SOCKET& sSocket, unsigned int playerId);
	void RecvPacket(unsigned int playerId, char* packet, DWORD bytesize);

	void PlayerLogin(unsigned int playerId, char* packet); // No DB
	void PlayerMove(unsigned int playerId, char* packet);
	void PlayerChat(unsigned int playerId, char* _packet);
	void PlayerLogout(unsigned int playerId);

public:
	void UpdateViewList(unsigned int playerId);
	bool ObjectRangeCheck(Vec3& player, Vec3& other, float fDistance);
};