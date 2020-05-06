#include "PlayerProcess.h"
#include "Network.h"
#include "PacketMgr.h"
#include "Player.h"
#include "DataBase.h"
//#include "directXmath.h"

CPlayerProcess::CPlayerProcess()
{
	if (!CProcess::m_pPlayerPool)
		CProcess::m_pPlayerPool = new class CPlayerpool();
	BindPacketProcess();
}


CPlayerProcess::~CPlayerProcess()
{
	if (m_pPlayerPool)
	{
		delete m_pPlayerPool;
		m_pPlayerPool = nullptr;
	}
}

void CPlayerProcess::AcceptClient(const SOCKET & _sSocket, unsigned int _usID)
{
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetPlayerSocket(_sSocket);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetPlayerID(_usID);
	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetRecvState();
}

void CPlayerProcess::ProcessPacket(unsigned int _usID, char * _packet)
{
	//char* packet = CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->RecvEvent()
	m_fpPacketProcess[_packet[1]](_usID, _packet);
}

void CPlayerProcess::RecvPacket(unsigned int _usID, char * _Packet, DWORD bytesize)
{
	char* packet = CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->RecvEvent(bytesize, _Packet);

	if (packet != nullptr) {
		m_fpPacketProcess[_Packet[1]](_usID, _Packet);
		CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->SetRecvState();
	}
}

void CPlayerProcess::PlayerLogin(unsigned int _usID, char * _packet)
{
	cout << "보내자" << endl;
	concurrent_unordered_set<unsigned int> viewlist;
	// DB USE

	// DB NOT USE
	// 임시적으로 무조건 로그인 시키기
	CPacketMgr::Send_Login_OK_Packet(_usID);
	
	// 처음 플레이어의 정보 보내기
	CPacketMgr::Send_First_Status_Packet(_usID);

	//// 게임에 들어와있는 리스트에 추가하기
	//InsertList(_usID);

	//// 현재 게임의 뷰리스트 추가하기
	//CopyBefore(viewlist);

	//// 들어온 유저의 정보를 접속자들에게 보내기
	//CPacketMgr::Send_Accept_Packet(_usID, viewlist);
}

void CPlayerProcess::PlayerLogin(DataBase_Event& _event)
{
	//sc_login_state_packet packet;
	//packet.size = sizeof(sc_login_state_packet);
	//packet.type = SC_LOGIN_OK;
	//CPacketMgr::Send_Packet(_event.client_num, &packet);

	//concurrency::concurrent_unordered_set<unsigned int>viewlist;

	//InsertList(_event.client_num);

	//sc_first_status_packet accept_packet;
	//accept_packet.size = sizeof(sc_first_status_packet);
	//accept_packet.type = SC_LOGIN_OK;
	//accept_packet.HP = _event.HP;
	//accept_packet.Hunger = _event.Hunger;
	//accept_packet.Stamina = _event.Stamina;
	//accept_packet.Thirst = _event.Thirst;

	//accept_packet.fPosX = _event.fPosX;
	//accept_packet.fPosY = _event.fPosY;
	//accept_packet.fPosZ = _event.fPosZ;
	//accept_packet.fDirX = _event.fDirX;
	//accept_packet.fDirY = _event.fDirY;
	//accept_packet.fDirZ = _event.fDirZ;

	//CPacketMgr::Send_Packet(_event.client_num, &accept_packet);

	//// DB에서 받아온 정보를 플레이어에게 옮긴다.
	//CProcess::m_pPlayerPool->m_cumPlayerPool[_event.client_num]->SetHP(_event.HP);
	//CProcess::m_pPlayerPool->m_cumPlayerPool[_event.client_num]->SetHunger(_event.Hunger);
	//CProcess::m_pPlayerPool->m_cumPlayerPool[_event.client_num]->SetStamina(_event.Stamina);
	//CProcess::m_pPlayerPool->m_cumPlayerPool[_event.client_num]->SetThirst(_event.Thirst);
	//CProcess::m_pPlayerPool->m_cumPlayerPool[_event.client_num]->SetPos(_event.fPosX, _event.fPosY, _event.fPosZ);
	//CProcess::m_pPlayerPool->m_cumPlayerPool[_event.client_num]->SetDir(_event.fDirX, _event.fDirY, _event.fDirZ);

	//CProcess::m_pPlayerPool->m_cumPlayerPool[_event.client_num]->SetPlayerConnect(true);

	//CopyBefore(viewlist);
	//CPacketMgr::Send_Accept_Packet(_event.client_num, viewlist);
}

void CPlayerProcess::PlayerDinconnect(unsigned int _usID)
{
	// 이미 Disconnect인 상태일 경우
	if (!CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->GetPlayerConnect()) return;
	// 아직 Diconnect인 상태가 아닐 경우
	concurrent_unordered_set<unsigned int>ViewList;
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

void CPlayerProcess::UpdateViewList(unsigned int _usID)
{
	DirectX::XMFLOAT3 pos = CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->GetPos();

	concurrent_unordered_set<unsigned int> loginList;
	concurrent_unordered_set<unsigned int> beforeList;
	concurrent_unordered_set<unsigned int> afterList;

	CopyBefore(loginList);

	CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->CopyPlayerList(beforeList);

	for (auto& au : loginList) {
		if (au == _usID) continue;
		afterList.insert(au);
		DirectX::XMFLOAT3 other_pos = CProcess::m_pPlayerPool->m_cumPlayerPool[au]->GetPos();
		/*if (Vector3::ObjectRangeCheck(pos, other_pos, PLAYER_BETWEEN_RANGE)) {
			afterList.insert(au);
			continue;
		}*/
	}

	for (auto au : afterList) {
		if (au > MAX_USER) continue;
		if (beforeList.count(au) == 0) {
			CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->InsertList(au);
			CPacketMgr::Send_Status_Packet(au, _usID);

			if (CProcess::m_pPlayerPool->m_cumPlayerPool[au]->CheckList(_usID)) {
				CPacketMgr::Send_Pos_Packet(_usID, au);
				continue;
			}
			else {
				CPacketMgr::Send_Status_Packet(_usID, au);
				continue;
			}
			continue;
		}
		else {
			if (!CProcess::m_pPlayerPool->m_cumPlayerPool[au]->CheckList(_usID)) {
				CProcess::m_pPlayerPool->m_cumPlayerPool[au]->InsertList(_usID);
				CPacketMgr::Send_Status_Packet(_usID, au);
				continue;
			}
			else {
				CPacketMgr::Send_Pos_Packet(_usID, au);
				continue;
			}
			continue;
		}
		continue;
	}

	for (auto au : beforeList) {
		if (au >= MAX_USER) continue;
		if (afterList.count(au) == 0) {
			if (CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->CheckList(au)) {
				CProcess::m_pPlayerPool->m_cumPlayerPool[_usID]->DeleteList(au);
				CPacketMgr::Send_Remove_Packet(au, _usID);
				if (CProcess::m_pPlayerPool->m_cumPlayerPool[au]->CheckList(_usID)) {
					CProcess::m_pPlayerPool->m_cumPlayerPool[au]->DeleteList(_usID);
					CPacketMgr::Send_Remove_Packet(_usID, au);
					continue;
				}
				continue;
			}
		}
	}
}

void CPlayerProcess::PlayerPos(unsigned int _usID, char * _Packet)
{
	sc_pos_packet*	 packet = reinterpret_cast<sc_pos_packet*>(_Packet);
	DirectX::XMFLOAT3 pos = DirectX::XMFLOAT3(packet->fPosX, packet->fPosY, packet->fPosZ);
}

void CPlayerProcess::PlayerLook(unsigned int _usID, char * _Packet)
{
	
}
