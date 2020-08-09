#include "stdafx.h"
#include "Network.h"
#include "LoginScene.h"
#include "IngameScene.h"

#include <Engine/Device.h>

#include <Engine/Ptr.h>

#include <Engine/Texture.h>

#include <Engine/ResMgr.h>
#include <Engine/CollisionMgr.h>
#include <Engine/FontMgr.h>

#include <Engine/GameObject.h>
#include <Engine/MeshRender.h>
#include <Engine/Transform.h>
#include <Engine/Camera.h>
#include <Engine/Light3D.h>
#include <Engine/LandScape.h>
#include <Engine/GridScript.h>

#include <Engine/PlayerScript.h>
#include <Engine/FPSCamScript.h>
#include <Engine/ToolCamScript.h>
#include <Engine/MonsterScript.h>
#include <Engine/StatusScript.h>
#include <Engine/QuickSlotScript.h>

#include "PlayerCamScript.h"
#include "InventoryScript.h"
#include "ChatScript.h"
#include <Engine/TestScript.h>
#include "InputScript.h"
#include "AnimalScript.h"

#include <Engine/NaviMgr.h>

unsigned int CNetwork::m_usID = 0;
concurrent_unordered_map<unsigned int, CGameObject*> CNetwork::m_cumPlayer;
concurrent_unordered_map<unsigned int, CGameObject*> CNetwork::m_cumAnimal;

CNetwork::CNetwork()
{
	Init();
}

CNetwork::~CNetwork()
{
	DisConnect();
}

void CNetwork::Err_quit(const char * msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

void CNetwork::Err_display(const char * msg, int err_no)
{
	WCHAR *lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err_no,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	cout << "[ " << msg << " ]";
	wcout << L"에러 " << lpMsgBuf << endl;
	while (true);
	LocalFree(lpMsgBuf);
}

void CNetwork::Init()
{
	for (auto i = 0; i < LT_END; ++i)
		m_bLoginState[i] = false;
	m_bPushKey = false;
	m_bClientClose = false;
	m_bCollision = false;

	m_SendWsaBuf.buf = m_cSendBuf;
	m_SendWsaBuf.len = BUF_SIZE;

	m_RecvWsaBuf.buf = m_cRecvbuf;
	m_RecvWsaBuf.len = BUF_SIZE;

	m_in_packet_size = 0;
	m_saved_packet_size = 0;
}

bool CNetwork::ConnectServer(string ipAddr)
{
	char IP[15];
	strcpy_s(IP, ipAddr.c_str());

	WSADATA	wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	m_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, NULL, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(SOCKADDR_IN));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVER_PORT);
	serveraddr.sin_addr.s_addr = inet_addr(IP);
	int retval = WSAConnect(m_sock, reinterpret_cast<sockaddr*>(&serveraddr), sizeof(serveraddr), NULL, NULL, NULL, NULL);
	if (retval == SOCKET_ERROR) {
		int err_no = WSAGetLastError();
		//Err_display("socket err", err_no);
		return false;
	}

	// IP 주소로 연결되었을 경우
	if (CreateEventSelect()) {
		RunRecvThread();
		return true;
	}
	else
		return false;
}

void CNetwork::RunRecvThread()
{
	// Network Run
	m_tNetworkThread = make_shared<thread>([]() {CNetwork::GetInst()->RecvPacket(); });

}

bool CNetwork::CreateEventSelect()
{
	m_hEvent = WSACreateEvent();

	int retval = WSAEventSelect(m_sock, m_hEvent, FD_READ || FD_CLOSE);
	if (retval != SOCKET_ERROR) {
		cout << "CreateSelectEvent Success" << endl;
		return true;
	}
	else {
		int err_no = WSAGetLastError();
		Err_display("CreateSelectEvent err", err_no);
		return false;
	}
}

void CNetwork::DisConnect()
{
	m_tNetworkThread->join();

	closesocket(m_sock);
	WSACloseEvent(m_hEvent);
	WSACleanup();
	cout << "DisConnect" << endl;
}

void CNetwork::RecvPacket()
{
	cout << "Network Run" << endl;
	while (!m_bClientClose)
	{
		m_iIndex = WSAWaitForMultipleEvents(1, &m_hEvent, FALSE, WSA_INFINITE, FALSE);

		if ((m_iIndex == WSA_WAIT_FAILED) || (m_iIndex == WSA_WAIT_TIMEOUT)) {
			int err_no = WSAGetLastError();
			Err_display("WSAWaitForMultipleEvents Err", err_no);
			break;
		}
		int retval = WSAEnumNetworkEvents(m_sock, m_hEvent, &m_weEvent);
		if (retval == SOCKET_ERROR) {
			int err_no = WSAGetLastError();
			Err_display("WSAEnumNetworkEvents Err", err_no);
			break;
		}

		if (m_weEvent.lNetworkEvents & FD_READ) {
			if (m_weEvent.iErrorCode[FD_READ_BIT] != 0) {
				int err_no = GetLastError();
				Err_display("m_weEvent.iErrorCode Err", err_no);
				break;
			}
			DWORD	size, flag = 0;
			int ret = WSARecv(m_sock, &m_RecvWsaBuf, 1, &size, &flag, NULL, NULL);
			if (ret == SOCKET_ERROR) {
				int err_no = GetLastError();
				Err_display("WSARecv SOCKET_ERROR", err_no);
				break;
			}

			BYTE* packet = reinterpret_cast<BYTE*>(m_cRecvbuf);

			while (size != 0) {
				if (0 == m_in_packet_size) m_in_packet_size = packet[0];
				if (size + m_saved_packet_size >= m_in_packet_size) {
					memcpy(m_cPacketBuf + m_saved_packet_size, packet, m_in_packet_size - m_saved_packet_size);
					ProcessPacket(m_cPacketBuf);
					packet += m_in_packet_size - m_saved_packet_size;
					size -= m_in_packet_size - m_saved_packet_size;
					m_in_packet_size = 0;
					m_saved_packet_size = 0;
				}
				else {
					memcpy(m_cPacketBuf + m_saved_packet_size, packet, size);
					m_saved_packet_size += size;
					size = 0;
				}
			}
		}
		if (m_weEvent.lNetworkEvents & FD_CLOSE)
		{
			cout << "FD_CLOSE" << endl;
			m_bClientClose = true;
			break;
		}
	}
	//cout << "Network Close" << endl;
}

void CNetwork::ProcessPacket(char* packet)
{
	switch (packet[1])
	{
	case SC_LOGIN_OK:
	{
		SetLogin(true);
		Recv_Login_OK_Packet(packet);
		break;
	}
	case SC_LOGIN_FAIL:
	{

	}
	case SC_STATUS_PLAYER:
	{
		Recv_Status_Player_Packet(packet);
		break;
	}
	case SC_PUT_PLAYER: {
		SetLoopStart(true);
		Recv_Put_Player_Packet(packet);
		break;
	}
	case SC_REMOVE_PLAYER: {
		Recv_Remove_Player_Packet(packet);
		break;
	}
	case SC_POS: {
		Recv_Pos_Player_Packet(packet);
		break;
	}
	case SC_ROT: {
		break;
	}
	case SC_CHAT: {
		Recv_Chat_Packet(packet);
		break;
	}
	// NPC, MONSTER 관련
	case SC_PUT_NPC: {
		Recv_Put_Npc_Packet(packet);
		break;
	}
	case SC_POS_NPC:
	{
		Recv_Pos_Npc_Packet(packet);
		break;
	}
	case SC_REMOVE_NPC:
	{
		Recv_Remove_Npc_Packet(packet);
		break;
	}
	case SC_INSTALL_HOUSING:
	{
		Recv_Install_Housing_Packet(packet);
		break;
	}
	case SC_WEATHER:
	{
		Recv_Weather_Packet(packet);
		break;
	}
	case SC_TIME:
	{
		Recv_Time_Packet(packet);
		break;
	}
	}
}

void CNetwork::Send_Login_Packet(wstring playerID)
{
	DWORD size{ 0 }, flag{ 0 };

	cs_login_packet*	login_packet = reinterpret_cast<cs_login_packet*>(m_cSendBuf);

	login_packet->size = sizeof(cs_login_packet);
	login_packet->type = CS_LOGIN;
	wcscpy_s(login_packet->player_id, playerID.c_str());

	m_SendWsaBuf.len = sizeof(cs_login_packet);

	int ret = WSASend(m_sock, &m_SendWsaBuf, 1, &size, flag, NULL, NULL);

	if (ret != 0) {
		int err_no = WSAGetLastError();
		Err_display("Err while sending packet - ", err_no);
	}
}

void CNetwork::Send_Move_Packet()
{
	DWORD size, flag = 0;

	cs_move_packet* packet = reinterpret_cast<cs_move_packet*>(m_cSendBuf);

	//Vec3 vRot = m_pPlayer->Transform()->GetLocalRot();
	packet->size = sizeof(cs_move_packet);
	packet->type = CS_MOVE;
	packet->id = m_usID;
	packet->vLocalPos = m_pPlayer->Transform()->GetLocalPos();
	packet->vLocalRot = m_pPlayer->Transform()->GetLocalRot();
	//packet->bRun = bRun;
	//packet->vWorldDir = vWorldDir;
	//packet->fHeight = CNaviMgr::GetInst()->GetY(m_pPlayer->Transform()->GetLocalPos());

	m_SendWsaBuf.len = sizeof(cs_move_packet);

	int retval = WSASend(m_sock, &m_SendWsaBuf, 1, &size, flag, NULL, NULL);

	if (retval != 0)
	{
		int err_no = WSAGetLastError();
		Err_display("Err while sending packet - ", err_no);
	}
}

//void CNetwork::Send_Move_Packet(Vec3 vWorldDir, bool bRun)
//{
//	DWORD size, flag = 0;
//
//	cs_move_packet* packet = reinterpret_cast<cs_move_packet*>(m_cSendBuf);
//
//	//Vec3 vRot = m_pPlayer->Transform()->GetLocalRot();
//	packet->size = sizeof(cs_move_packet);
//	packet->type = CS_MOVE;
//	packet->id = m_usID;
//	packet->bRun = bRun;
//	packet->vWorldDir = vWorldDir;
//	packet->fHeight = CNaviMgr::GetInst()->GetY(m_pPlayer->Transform()->GetLocalPos());
//
//	m_SendWsaBuf.len = sizeof(cs_move_packet);
//
//	int retval = WSASend(m_sock, &m_SendWsaBuf, 1, &size, flag, NULL, NULL);
//
//	if (retval != 0)
//	{
//		int err_no = WSAGetLastError();
//		Err_display("Err while sending packet - ", err_no);
//	}
//}

void CNetwork::Send_Chat_Packet(string message)
{
	DWORD size, flag = 0;

	cs_chat_packet* chat_packet = reinterpret_cast<cs_chat_packet*>(m_cSendBuf);

	chat_packet->size = sizeof(cs_chat_packet);
	chat_packet->type = CS_CHAT;
	chat_packet->id = m_usID;
	strcpy_s(chat_packet->meesage, message.c_str());

	m_SendWsaBuf.len = sizeof(cs_chat_packet);

	int retval = WSASend(m_sock, &m_SendWsaBuf, 1, &size, flag, NULL, NULL);

	if (retval != 0)
	{
		int err_no = WSAGetLastError();
		Err_display("Err while sending packet - ", err_no);
	}
}

void CNetwork::Send_Collision_House_Packet(USHORT houseId, bool bRun)
{
	DWORD size, flag = 0;

	cs_collision_packet* collision_packet = reinterpret_cast<cs_collision_packet*>(m_cSendBuf);

	collision_packet->type = CS_HOUSE_COLLISION;
	collision_packet->id = houseId;
	collision_packet->bRun = bRun;

	int retval = WSASend(m_sock, &m_SendWsaBuf, 1, &size, flag, NULL, NULL);

	if (retval != 0)
	{
		int err_no = WSAGetLastError();
		Err_display("Err while sending packet - ", err_no);
	}
}

void CNetwork::Send_Collision_Animal_Packet(USHORT animalId, bool bRun)
{
	DWORD size, flag = 0;

	cs_collision_packet* collision_packet = reinterpret_cast<cs_collision_packet*>(m_cSendBuf);

	collision_packet->type = CS_ANIMAL_COLLISION;
	collision_packet->id = animalId;
	collision_packet->bRun = bRun;

	int retval = WSASend(m_sock, &m_SendWsaBuf, 1, &size, flag, NULL, NULL);

	if (retval != 0)
	{
		int err_no = WSAGetLastError();
		Err_display("Err while sending packet - ", err_no);
	}
}

void CNetwork::Send_Install_Housing_Packet(UINT uiType, Vec3 vLocalPos, Vec3 vLocalRot, Vec3 vLocalScale)
{
	DWORD size, flag = 0;

	cs_install_housing_packet* install_housing_packet = reinterpret_cast<cs_install_housing_packet*>(m_cSendBuf);
	install_housing_packet->type = CS_HOUSING_INSTALL;
	install_housing_packet->housing_type = uiType;
	install_housing_packet->vLocalPos = vLocalPos;
	install_housing_packet->vLocalRot = vLocalRot;
	install_housing_packet->vLocalScale = vLocalScale;

	int retval = WSASend(m_sock, &m_SendWsaBuf, 1, &size, flag, NULL, NULL);

	if (retval != 0)
	{
		int err_no = WSAGetLastError();
		Err_display("Err while sending packet - ", err_no);
	}
}

// ============================== RECV ============================
void CNetwork::Recv_Login_OK_Packet(char * packet)
{
	pScene = CSceneMgr::GetInst()->GetCurScene();
	sc_login_ok_packet* login_packet = reinterpret_cast<sc_login_ok_packet*>(packet);
	m_usID = login_packet->id;
	//pScene->FindLayer(L"Player")->AddGameObject(m_pPlayer);
}

void CNetwork::Recv_Login_Fail_Packet(char * packet)
{
	SetLogin(false);
}

void CNetwork::Recv_Status_Player_Packet(char * packet)
{
	sc_status_player_packet* status_player_packet = reinterpret_cast<sc_status_player_packet*>(packet);
	unsigned short player_id = status_player_packet->id;

	if (m_usID == player_id)
	{
		//m_pPlayer->GetScript<CStatusScript>()->Set
		//m_pPlayer->GetScript<CStatusScript>()->
		//m_pPlayer->Transform()->SetLocalPos(status_player_packet->vLocalPos);
		//m_pPlayer->Transform()->SetLocalRot(status_player_packet->vLocalRot);
	}
	else
	{

	}
}

void CNetwork::Recv_Put_Player_Packet(char * packet)
{
	sc_put_player_packet* put_player_packet = reinterpret_cast<sc_put_player_packet*>(packet);
	unsigned int player_id = put_player_packet->id;

	Vec3 vPos = put_player_packet->vPos;
	Vec3 vRot = put_player_packet->vRot;

	if (m_usID == player_id) {
		m_pPlayer->Transform()->SetLocalPos(vPos);
	}
	else {
		m_cumPlayer[player_id]->Transform()->SetLocalPos(vPos);
		m_cumPlayer[player_id]->Transform()->SetLocalRot(vRot);
		pScene->FindLayer(L"Human")->AddGameObject(CNetwork::m_cumPlayer[player_id]);
	}
}

void CNetwork::Recv_Remove_Player_Packet(char * packet)
{
	sc_remove_player_packet* remove_player_packet = reinterpret_cast<sc_remove_player_packet*>(packet);
	unsigned int player_id = remove_player_packet->id;

	tEvent tEv;
	tEv.eType = EVENT_TYPE::DELETE_OBJECT;

	if (m_usID == player_id) {
		tEv.wParam = (DWORD_PTR)m_pPlayer;
		CEventMgr::GetInst()->AddEvent(tEv);
	}
	else {
		tEv.wParam = (DWORD_PTR)m_cumPlayer[player_id];
		CEventMgr::GetInst()->AddEvent(tEv);
	}
}

void CNetwork::Recv_Pos_Player_Packet(char * packet)
{
	sc_pos_player_packet*	pos_packet = reinterpret_cast<sc_pos_player_packet*>(packet);
	unsigned int player_id = pos_packet->id;
	Vec3 vPos = pos_packet->vPos;
	Vec3 vRot = pos_packet->vRot;
	if (m_usID == player_id) {
		m_pPlayer->Transform()->SetLocalPos(vPos);
		//m_pPlayer->Transform()->SetLocalRot(vRot);
	}
	else {
		m_cumPlayer[player_id]->Transform()->SetLocalPos(vPos);
		m_cumPlayer[player_id]->Transform()->SetLocalRot(vRot);
	}
}

void CNetwork::Recv_Chat_Packet(char * packet)
{
	sc_chat_packet* chat_packet = reinterpret_cast<sc_chat_packet*>(packet);

	wstring wname = chat_packet->wcid;
	string name;
	name.assign(wname.begin(), wname.end());
	string Msg(chat_packet->meesage);
	cout << "[" << name << "]" << Msg << endl;
	m_pChat->GetScript<CChatScript>()->AddChat(name, Msg);
	m_pChat->GetScript<CInputScript>()->SetEnable(false);
	m_pChat->GetScript<CInputScript>()->Clear();
}

void CNetwork::Recv_Animation_Player_Packet(char * packet)
{
	sc_animation_player_packet* animation_player_packet = reinterpret_cast<sc_animation_player_packet*>(packet);
	unsigned int player_id = animation_player_packet->id;
	char player_animation = animation_player_packet->animation;

	if (m_usID == player_id)
	{
		// 자기 자신 애니메이션 키값 변환
	}
	else
	{
		// 타 플레이어 애니메이션 키값 변환
	}
}

void CNetwork::Recv_WakeUp_Npc_Packet(char * packet)
{
	sc_wake_up_packet* wakeup_npc_packet = reinterpret_cast<sc_wake_up_packet*>(packet);

	unsigned int monster_id = wakeup_npc_packet->id;

	m_cumAnimal[monster_id]->GetScript<CAnimalScript>()->SetWakeUp(true);
}

void CNetwork::Recv_Put_Npc_Packet(char * packet)
{
	sc_put_npc_packet* put_npc_packet = reinterpret_cast<sc_put_npc_packet*>(packet);
	USHORT monster_id = put_npc_packet->id;

	UINT eType = put_npc_packet->eType;
	Vec3 vPos = put_npc_packet->vPos;
	Vec3 vRot = put_npc_packet->vRot;

	dynamic_cast<CIngameScene*>(pScene->GetSceneScript())->AnimalUpdate(monster_id, vPos, vRot, eType);
}

void CNetwork::Recv_Remove_Npc_Packet(char * packet)
{
	sc_remove_npc_packet* remove_npc_packet = reinterpret_cast<sc_remove_npc_packet*>(packet);
	USHORT monster_id = remove_npc_packet->id;

	dynamic_cast<CIngameScene*>(pScene->GetSceneScript())->AnimalDestory(monster_id);
}

void CNetwork::Recv_Pos_Npc_Packet(char * packet)
{
	sc_pos_npc_packet* pos_npc_packet = reinterpret_cast<sc_pos_npc_packet*>(packet);
	unsigned int monster_id = pos_npc_packet->id;
	UINT eType = pos_npc_packet->eType;
	Vec3 vPos = pos_npc_packet->vPos;
	Vec3 vRot = pos_npc_packet->vRot;

	dynamic_cast<CIngameScene*>(pScene->GetSceneScript())->AnimalUpdate(monster_id, vPos, vRot, eType);
}

void CNetwork::Recv_Animation_Npc_Packet(char * packet)
{
	sc_animation_npc_packet* animation_npc_packet = reinterpret_cast<sc_animation_npc_packet*>(packet);
	unsigned int monster_id = animation_npc_packet->id;
	char		monster_animation = animation_npc_packet->animation;

	// 몬스터 애니메이션 키값 바꾸기
}

void CNetwork::Recv_Install_Housing_Packet(char * packet)
{
	sc_install_housing_packet* install_housing_packet = reinterpret_cast<sc_install_housing_packet*>(packet);
	UINT type = install_housing_packet->housing_type;
	Vec3 vPos = install_housing_packet->vLocalPos;
	Vec3 vRot = install_housing_packet->vLocalRot;
	Vec3 vScale = install_housing_packet->vLocalScale;

	// INSTALL 전송
	dynamic_cast<CIngameScene*>(pScene->GetSceneScript())->InstallHousing(type, vPos, vRot, vScale);
}

void CNetwork::Recv_Weather_Packet(char * packet)
{
	sc_weather_packet* weather_packet = reinterpret_cast<sc_weather_packet*>(packet);
	bool bRain = weather_packet->bRain;

	// 받기
	//cout << bRain << endl;
}

void CNetwork::Recv_Time_Packet(char * packet)
{
	sc_time_packet* time_packet = reinterpret_cast<sc_time_packet*>(packet);
	float fTime = time_packet->fTime;

	cout << fTime << endl;
}
