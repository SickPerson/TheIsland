#pragma once
#include "header.h"

class CGameObject;

class CPacketMgr
{
	SINGLE(CPacketMgr)

private:
	static WSABUF	m_SendWsaBuf;
	static char		m_cSendBuf[BUF_SIZE];

public:
	static CGameObject*	m_pPlayer;

public:
	void SetPlayerObj(CGameObject* pObj) { m_pPlayer = pObj; }

public:
	static void Send_Login_Packet(wstring strID);
	static void Send_Move_Packet(Vec3 vWorldDir, bool bRun);
	static void Send_Chat_Packet(string message);
	static void Send_Logout_Packet();
	static void Send_Pos_Player_Packet();
	static void Send_Rot_player_Packet();
	static void Send_Collision_Player_Packet(UINT ColuiType, USHORT ColId, bool bRun);
	static void Send_Install_Housing_Packet(UINT uiType, Vec3 vLocalPos, Vec3 vLocalRot, Vec3 vLocalScale, Vec3 vOffsetPos, Vec3 vOffsetScale);
	static void Send_Remove_Housing_Packet(USHORT houseId);
	static void Send_Attack_Player_Packet(UINT attackType, USHORT attackId);
	static void Send_Animation_Player_Packet(UINT uiAnimationType);
};

