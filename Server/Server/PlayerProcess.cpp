#include "PlayerProcess.h"
#include "Network.h"
#include "PacketMgr.h"
#include "Player.h"

CPlayerProcess::CPlayerProcess()
{
	if (!CProcess::m_pPlayerPool)
		CProcess::m_pPlayerPool = new class CPlayerpool();
}


CPlayerProcess::~CPlayerProcess()
{
}

void CPlayerProcess::AcceptClient(const SOCKET & _sSocket, unsigned short _usID)
{
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetPlayerSocket(_sSocket);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetPlayerID(_usID);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetRecvState();
}

void CPlayerProcess::PacketProcess(unsigned short _usID, char * _packet)
{
	m_fppacketProcess[_packet[1]](_usID, _packet);
}

void CPlayerProcess::RecvPacket(unsigned short _usID, DWORD _dwSize, char * _Packet)
{
	char* packet = CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->RecvEvent(_dwSize, _Packet);

	if (packet != nullptr)
	{
		m_fppacketProcess[packet[1]](_usID, packet);
		CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetRecvState();
	}
	return;
}

void CPlayerProcess::PlayerLoginCheck(unsigned short _usID, char * _packet)
{
	cs_login_packet* login_packet = reinterpret_cast<cs_login_packet*>(_packet);

	// 로그인 시도시, IP 주소가 다를 경우
	if (0 != strcmp(login_packet->player_ip, CNetwork::GetInst()->GetIpAddr()))
	{
		CPacketMgr::Send_Login_Fail_Packet(_usID);
		return;
	}

	// 로그인 시도시, IP 주소가 같을 경우 -> 진행
	CPacketMgr::Send_Login_OK_Packet(_usID);
	// DataBase로 로그인
	// Not DataBase
	sc_accept_packet accept_packet;
	accept_packet.size = sizeof(sc_accept_packet);
	accept_packet.type = SC_LOGIN;
	accept_packet.CurHp = 100;
	accept_packet.Curhunger = 100;
	accept_packet.CurStamina = 100;
	accept_packet.Curhunger = 100;
	accept_packet.thirst = 100;

	accept_packet.fPosX = 0.f;
	accept_packet.fPosY = 25.f;
	accept_packet.fPosZ = 0.f;
	accept_packet.fDirX = 0.f;
	accept_packet.fDirY = 0.f;
	accept_packet.fDirZ = 0.f;

	concurrent_unordered_set<unsigned short>viewlist;

	InsertList(_usID);
}

void CPlayerProcess::PlayerLogin(DataBase_Event& _event)
{
	sc_login_state_packet packet;
	packet.size = sizeof(sc_login_state_packet);
	packet.type = SC_LOGIN_OK;
	CPacketMgr::Send_Packet(_event.client_num, &packet);

	concurrency::concurrent_unordered_set<unsigned short>viewlist;

	InsertList(_event.client_num);

	sc_accept_packet accept_packet;
	accept_packet.size = sizeof(sc_accept_packet);
	accept_packet.type = SC_LOGIN_OK;
	accept_packet.CurHp = _event.CurHp;
	accept_packet.Curhunger = _event.Curhunger;
	accept_packet.CurStamina = _event.CurStamina;
	accept_packet.thirst = _event.thirst;

	accept_packet.fPosX = _event.fPosX;
	accept_packet.fPosY = _event.fPosY;
	accept_packet.fPosZ = _event.fPosZ;
	accept_packet.fDirX = _event.fDirX;
	accept_packet.fDirY = _event.fDirY;
	accept_packet.fDirZ = _event.fDirZ;

	CPacketMgr::Send_Packet(_event.client_num, &accept_packet);

	// DB에서 받아온 정보를 플레이어에게 옮긴다.
	CProcess::m_pPlayerPool->m_cumPlayerPool[_event.client_num]->SetHP(_event.CurHp);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_event.client_num]->SetHunger(_event.Curhunger);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_event.client_num]->SetStamina(_event.CurStamina);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_event.client_num]->SetThirst(_event.thirst);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_event.client_num]->SetPos(_event.fPosX, _event.fPosY, _event.fPosZ);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_event.client_num]->SetDir(_event.fDirX, _event.fDirY, _event.fDirZ);

	CProcess::m_pPlayerPool->m_cumPlayerPool[_event.client_num]->SetPlayerConnect(true);

	CopyBefore(viewlist);
	CPacketMgr::Send_Accept_Packet(_event.client_num, viewlist);
}

void CPlayerProcess::PlayerLogout(unsigned short _usID)
{
}

void CPlayerProcess::PlayerDinconnect(unsigned short _usID)
{
	// 이미 Disconnect인 상태일 경우
	if (!CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->GetPlayerConnect()) return;

	concurrent_unordered_set<unsigned short>ViewList;
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetPlayerConnect(false);
}

void CPlayerProcess::PlayerPos(unsigned short _usID, char * _Packet)
{
}

void CPlayerProcess::PlayerLook(unsigned short _usID, char * _Packet)
{
}
