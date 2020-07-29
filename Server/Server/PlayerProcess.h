#pragma once
#include "stdafx.h"
#include "Process.h"

class CPlayerProcess:
	public CProcess
{
private:
<<<<<<< HEAD
	recursive_mutex						m_rmPlayerProcessLock;
=======
	recursive_mutex	m_rmPlayerProcessLock;
	function<void(unsigned int)>	m_fpUpdateViewList;
>>>>>>> master
	function<void(unsigned int, char*)> m_fpPacketProcess[CS_END];
public:
	explicit CPlayerProcess();
	virtual ~CPlayerProcess();

public:
	void BindPacketProcess()
	{
<<<<<<< HEAD
		m_fpPacketProcess[CS_LOGIN] = [&](unsigned int playerId, char* packet){
			PlayerLogin(playerId, packet);
		};
		m_fpPacketProcess[CS_MOVE] = [&](unsigned int playerId, char* packet) {
			PlayerMove(playerId, packet);
		};
		m_fpPacketProcess[CS_CHAT] = [&](unsigned int playerId, char* packet){
			PlayerChat(playerId, packet);
		};
	}

	void AcceptClient(const SOCKET& sSocket, unsigned int playerId);
	void RecvPacket(unsigned int playerId, char* packet, DWORD bytesize);

	void PlayerLogin(unsigned int playerId, char* packet); // No DB
	void PlayerMove(unsigned int playerId, char* packet);
	void PlayerChat(unsigned int playerId, char* _packet);
	void PlayerDinconnect(unsigned int playerId);
=======
		m_fpPacketProcess[CS_LOGIN] = [&](unsigned int _usID, char* _packet)
		{
			PlayerLogin(_usID, _packet);
		};
		m_fpPacketProcess[CS_LOOK] = [&](unsigned int _usID, char* _packet)
		{
			PlayerLook(_usID, _packet);
		};
		m_fpPacketProcess[CS_POS] = [&](unsigned int _usID, char* _packet)
		{
			PlayerPos(_usID, _packet);
		};
	}
	void BindUpdateViewListFunction()
	{
		m_fpUpdateViewList = [&](unsigned int usID) {UpdateViewList(usID); };
	}
	void AcceptClient(const SOCKET& _sSocket, unsigned int _usID);
	void ProcessPacket(unsigned int _usID, char* _packet);
	void RecvPacket(unsigned int _usID, char* _Packet, DWORD bytesize);

	void PlayerLogin(unsigned int _usID, char* _packet);
	void PlayerLogin(DataBase_Event& _event);
	void PlayerPos(unsigned int _usID, char* _Packet);
	void PlayerLook(unsigned int _usID, char* _Packet);
	void PlayerDinconnect(unsigned int _usID);

public:
	void UpdateViewList(unsigned int _usID);
};
>>>>>>> master

public:
	void UpdateViewList(unsigned int playerId);
	bool ObjectRangeCheck(Vec3& player, Vec3& other, float fDistance);
};