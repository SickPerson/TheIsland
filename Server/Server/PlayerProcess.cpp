#include "PlayerProcess.h"
#include "Network.h"
#include "PacketMgr.h"
#include "Player.h"
#include "DataBase.h"

CPlayerProcess::CPlayerProcess()
{
	if (!CProcess::m_pPlayerPool)
		CProcess::m_pPlayerPool = new class CPlayerpool();
	BindPacketProcess();
	BindUpdateViewListFunction();
}


CPlayerProcess::~CPlayerProcess()
{
	if (m_pPlayerPool)
	{
		delete m_pPlayerPool;
		m_pPlayerPool = nullptr;
	}
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

void CPlayerProcess::PlayerLogin(unsigned short _usID, char * _packet)
{
	cs_login_packet* login_packet = reinterpret_cast<cs_login_packet*>(_packet);

	// �α��� �õ���, IP �ּҰ� �ٸ� ���
	if (0 != strcmp(login_packet->player_ip, CNetwork::GetInst()->GetIP()))
	{
		cout << "���� �����Ϸ��� �÷��̾� IP�ּҰ� �ٸ��ϴ�." << endl;
		CPacketMgr::Send_Login_Fail_Packet(_usID);
		return;
	}
	// �α��� �õ���, IP �ּҰ� ���� ��� -> ����
	cout << "���� �����Ϸ��� �÷��̾� IP �ּ� ��ġ ���� ����" << endl;
	CPacketMgr::Send_Login_OK_Packet(_usID);
	CPacketMgr::Send_First_Status_Packet(_usID);

	InsertList(_usID);
}

void CPlayerProcess::PlayerLoginCheck(unsigned short _usID, char * _packet)
{
	cs_login_packet* login_packet = reinterpret_cast<cs_login_packet*>(_packet);

	// �α��� �õ���, IP �ּҰ� �ٸ� ���
	if (0 != strcmp(login_packet->player_ip, CNetwork::GetInst()->GetIP()))
	{
		cout << "���� �����Ϸ��� �÷��̾� IP�ּҰ� �ٸ��ϴ�." << endl;
		CPacketMgr::Send_Login_Fail_Packet(_usID);
		return;
	}

	// �α��� �õ���, IP �ּҰ� ���� ��� -> ����
	cout << "���� �����Ϸ��� �÷��̾� IP �ּ� ��ġ ���� ����" << endl;
	CPacketMgr::Send_Login_OK_Packet(_usID);
	// DataBase�� �α���
	// Not DataBase
	/*sc_first_status_packet accept_packet;
	accept_packet.size = sizeof(sc_first_status_packet);
	accept_packet.type = SC_LOGIN;
	accept_packet.HP = 100;
	accept_packet.Hunger = 100;
	accept_packet.Stamina = 100;
	accept_packet.Hunger = 100;
	accept_packet.Thirst = 100;

	accept_packet.fPosX = 0.f;
	accept_packet.fPosY = 25.f;
	accept_packet.fPosZ = 0.f;
	accept_packet.fDirX = 0.f;
	accept_packet.fDirY = 0.f;
	accept_packet.fDirZ = 0.f;

	concurrent_unordered_set<unsigned short>viewlist;

	InsertList(_usID);*/
}

void CPlayerProcess::PlayerLogin(DataBase_Event& _event)
{
	sc_login_state_packet packet;
	packet.size = sizeof(sc_login_state_packet);
	packet.type = SC_LOGIN_OK;
	CPacketMgr::Send_Packet(_event.client_num, &packet);

	concurrency::concurrent_unordered_set<unsigned short>viewlist;

	InsertList(_event.client_num);

	sc_first_status_packet accept_packet;
	accept_packet.size = sizeof(sc_first_status_packet);
	accept_packet.type = SC_LOGIN_OK;
	accept_packet.HP = _event.HP;
	accept_packet.Hunger = _event.Hunger;
	accept_packet.Stamina = _event.Stamina;
	accept_packet.Thirst = _event.Thirst;

	accept_packet.fPosX = _event.fPosX;
	accept_packet.fPosY = _event.fPosY;
	accept_packet.fPosZ = _event.fPosZ;
	accept_packet.fDirX = _event.fDirX;
	accept_packet.fDirY = _event.fDirY;
	accept_packet.fDirZ = _event.fDirZ;

	CPacketMgr::Send_Packet(_event.client_num, &accept_packet);

	// DB���� �޾ƿ� ������ �÷��̾�� �ű��.
	CProcess::m_pPlayerPool->m_cumPlayerPool[_event.client_num]->SetHP(_event.HP);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_event.client_num]->SetHunger(_event.Hunger);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_event.client_num]->SetStamina(_event.Stamina);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_event.client_num]->SetThirst(_event.Thirst);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_event.client_num]->SetPos(_event.fPosX, _event.fPosY, _event.fPosZ);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_event.client_num]->SetDir(_event.fDirX, _event.fDirY, _event.fDirZ);

	CProcess::m_pPlayerPool->m_cumPlayerPool[_event.client_num]->SetPlayerConnect(true);

	CopyBefore(viewlist);
	CPacketMgr::Send_Accept_Packet(_event.client_num, viewlist);
}

void CPlayerProcess::PlayerDinconnect(unsigned short _usID)
{
	// �̹� Disconnect�� ������ ���
	if (!CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->GetPlayerConnect()) return;
	// ���� Diconnect�� ���°� �ƴ� ���
	concurrent_unordered_set<unsigned short>ViewList;
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetPlayerConnect(false);

	// Database
	/*if (CDataBase::GetInst()->GetIsLogin(_usID))
		return;*/

	DirectX::XMFLOAT3 pos = CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->GetPos();

	// Database

	if (CheckList(_usID))
		DeleteList(_usID);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->CopyPlayerList(ViewList);
	CPacketMgr::Send_Disconnect_Packet(_usID);
}

void CPlayerProcess::PlayerPos(unsigned short _usID, char * _Packet)
{
}

void CPlayerProcess::PlayerLook(unsigned short _usID, char * _Packet)
{
}
