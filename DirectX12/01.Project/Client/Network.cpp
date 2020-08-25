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
#include <Engine/Animator3D.h>
#include "SunshineScript.h"

#include <Engine/NaviMgr.h>
#include <Engine/Layer.h>

unsigned int CNetwork::m_usID = 0;
concurrent_unordered_map<unsigned int, CGameObject*> CNetwork::m_cumPlayer;
concurrent_unordered_map<unsigned int, CGameObject*> CNetwork::m_cumAnimal;

CNetwork::CNetwork():
	m_bClientClose(true)
{
	m_SendWsaBuf.buf = m_cSendBuf;
	m_SendWsaBuf.len = BUF_SIZE;

	m_RecvWsaBuf.buf = m_cRecvbuf;
	m_RecvWsaBuf.len = BUF_SIZE;

	m_in_packet_size = 0;
	m_saved_packet_size = 0;

	BindfpPacket();
}

CNetwork::~CNetwork()
{
	//DisConnect();
}

void CNetwork::BindfpPacket()
{
	// - Login
	m_fpPacketProcess[SC_LOGIN_OK] = [&](char* packet) {Recv_Login_OK_Packet(packet); };
	m_fpPacketProcess[SC_LOGIN_FAIL] = [&](char* packet) {Recv_Login_Fail_Packet(packet); };
	m_fpPacketProcess[SC_DISCONNECT_SERVER] = [&](char* packet) {Recv_Disconnect_Server_Packet(packet); };
	// - Player
	m_fpPacketProcess[SC_STATUS_PLAYER] = [&](char* packet) {Recv_Status_Player_Packet(packet); };
	m_fpPacketProcess[SC_PUT_PLAYER] = [&](char* packet) {Recv_Put_Player_Packet(packet); };
	m_fpPacketProcess[SC_POS_PLAYER] = [&](char* packet) {Recv_Pos_Player_Packet(packet); };
	m_fpPacketProcess[SC_ROT_PLAYER] = [&](char* packet) {Recv_Put_Player_Packet(packet); };
	m_fpPacketProcess[SC_REMOVE_PLAYER] = [&](char* packet) {Recv_Remove_Player_Packet(packet); };
	m_fpPacketProcess[SC_CHAT] = [&](char* packet) {Recv_Chat_Packet(packet); };
	m_fpPacketProcess[SC_ANIMATION_PLAYER] = [&](char* packet) {};
	// - Animal
	m_fpPacketProcess[SC_PUT_ANIMAL] = [&](char* packet) {};
	m_fpPacketProcess[SC_POS_ANIMAL] = [&](char* packet) {};
	m_fpPacketProcess[SC_REMOVE_ANIMAL] = [&](char* packet) {};
	m_fpPacketProcess[SC_ANIMATION_ANIMAL] = [&](char* packet) {};
	// - Natural
	m_fpPacketProcess[SC_PUT_NATURAL] = [&](char* packet) {};
	m_fpPacketProcess[SC_DESTROY_NATURAL] = [&](char* packet) {};
	// - House
	m_fpPacketProcess[SC_INSTALL_HOUSE] = [&](char* packet) {};
	m_fpPacketProcess[SC_REMOVE_HOUSE] = [&](char* packet) {};
	// - Item
	m_fpPacketProcess[SC_ADD_ITEM] = [&](char* packet) {};
	m_fpPacketProcess[SC_REMOVE_ITEM] = [&](char* packet) {};
	// - Etc
	m_fpPacketProcess[SC_WEATHER] = [&](char* packet) {};
	m_fpPacketProcess[SC_TIME] = [&](char* packet) {};
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
	m_bClientClose = false;
	return true;
	//// IP 주소로 연결되었을 경우
	//if (CreateEventSelect()) {
	//	m_bClientClose = false;
	//	//RunRecvThread();
	//	return true;
	//}
	//else
	//	return false;
}

void CNetwork::RunRecvThread()
{
	// Network Run
	m_tNetworkThread = make_shared<thread>([]() {CNetwork::GetInst()->RecvPacket(); });
}

//bool CNetwork::CreateEventSelect()
//{
//	m_hEvent = WSACreateEvent();
//
//	int retval = WSAEventSelect(m_sock, m_hEvent, FD_READ || FD_CLOSE);
//	if (retval != SOCKET_ERROR) {
//		cout << "CreateSelectEvent Success" << endl;
//		return true;
//	}
//	else {
//		int err_no = WSAGetLastError();
//		Err_display("CreateSelectEvent err", err_no);
//		return false;
//	}
//}

void CNetwork::DisConnect()
{
	m_tNetworkThread->join();

	closesocket(m_sock);
	//WSACloseEvent(m_hEvent);
	WSACleanup();
	cout << "DisConnect" << endl;
}

void CNetwork::RecvPacket()
{
	if (!m_bClientClose) {
		DWORD iobyte, ioflag = 0;

		int ret = WSARecv(m_sock, &m_RecvWsaBuf, 1, &iobyte, &ioflag, NULL, NULL);
		if (ret) {
			int err_no = WSAGetLastError();
			Err_display("Recv Error", err_no);
		}

		BYTE *ptr = reinterpret_cast<BYTE*>(m_cRecvbuf);

		while (0 != iobyte) {
			if (0 == m_in_packet_size) m_in_packet_size = ptr[0];
			if (iobyte + m_saved_packet_size >= m_in_packet_size) {
				memcpy(m_cPacketBuf + m_saved_packet_size, ptr, m_in_packet_size - m_saved_packet_size);
				ProcessPacket(m_cPacketBuf);
				ptr += m_in_packet_size - m_saved_packet_size;
				iobyte -= m_in_packet_size - m_saved_packet_size;
				m_in_packet_size = 0;
				m_saved_packet_size = 0;
			}
			else {
				memcpy(m_cPacketBuf + m_saved_packet_size, ptr, iobyte);
				m_saved_packet_size += iobyte;
				iobyte = 0;
			}
		}
	}
	/*while (!m_bClientClose)
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
	}*/
	//cout << "Network Close" << endl;
}

void CNetwork::ProcessPacket(char* packet)
{
	m_fpPacketProcess[packet[1]](packet);
	//switch (packet[1])
	//{
	//case SC_LOGIN_OK:
	//{
	//	Recv_Login_OK_Packet(packet);
	//	break;
	//}
	//case SC_LOGIN_FAIL:
	//{
	//	break;
	//}
	//case SC_STATUS_PLAYER:
	//{
	//	Recv_Status_Player_Packet(packet);
	//	break;
	//}
	//case SC_PUT_PLAYER: {
	//	Recv_Put_Player_Packet(packet);
	//	break;
	//}
	//case SC_REMOVE_PLAYER: {
	//	Recv_Remove_Player_Packet(packet);
	//	break;
	//}
	//case SC_POS_PLAYER: {
	//	Recv_Pos_Player_Packet(packet);
	//	break;
	//}
	//case SC_ROT_PLAYER: {
	//	break;
	//}
	//case SC_CHAT: {
	//	Recv_Chat_Packet(packet);
	//	break;
	//}
	//// NPC, MONSTER 관련
	//case SC_PUT_ANIMAL: {
	//	Recv_Put_Animal_Packet(packet);
	//	break;
	//}
	//case SC_POS_ANIMAL:
	//{
	//	Recv_Pos_Animal_Packet(packet);
	//	break;
	//}
	//case SC_REMOVE_ANIMAL:
	//{
	//	Recv_Remove_Animal_Packet(packet);
	//	break;
	//}
	//// Natural
	//case SC_PUT_NATURAL:
	//{
	//	Recv_Put_Natural_Packet(packet);
	//	break;
	//}
	//case SC_DESTROY_NATURAL:
	//{
	//	Recv_Destroy_Natural_Packet(packet);
	//	break;
	//}
	//// Housing
	//case SC_INSTALL_HOUSE:
	//{
	//	Recv_Install_Housing_Packet(packet);
	//	break;
	//}
	//
	//// etc
	//case SC_WEATHER:
	//{
	//	Recv_Weather_Packet(packet);
	//	break;
	//}
	//case SC_TIME:
	//{
	//	Recv_Time_Packet(packet);
	//	break;
	//}
	//}
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
	//packet->vLocalRot = m_pPlayer->Transform()->GetLocalRot();
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

void CNetwork::Send_Collision_Player_Packet(UINT Collision_type, USHORT Collision_Id, bool bRun)
{
	DWORD	size, flag = 0;

	cs_collision_packet* collision_packet = reinterpret_cast<cs_collision_packet*>(m_cSendBuf);

	collision_packet->type = CS_COLLISION;
	collision_packet->collision_uitype = Collision_type;
	collision_packet->collision_id = Collision_Id;
	collision_packet->bRun = bRun;

	int retval = WSASend(m_sock, &m_SendWsaBuf, 1, &size, flag, NULL, NULL);

	if (retval != 0)
	{
		int err_no = WSAGetLastError();
		Err_display("Err while sending packet - ", err_no);
	}
}

void CNetwork::Send_Attack_Player_Packet(UINT attack_type, USHORT attack_Id)
{
	DWORD	size, flag = 0;

	cs_attack_packet* attack_packet = reinterpret_cast<cs_attack_packet*>(m_cSendBuf);

	attack_packet->type = CS_ATTACK;
	attack_packet->attack_uiType = attack_type;
	attack_packet->attack_id = attack_Id;

	int retval = WSASend(m_sock, &m_SendWsaBuf, 1, &size, flag, NULL, NULL);

	if (retval != 0)
	{
		int err_no = WSAGetLastError();
		Err_display("Err while sending packet - ", err_no);
	}
}

void CNetwork::Send_Animation_Player_Packet(UINT uiAnimationType)
{
	DWORD size, flag = 0;
	cs_animation_packet* animation_packet = reinterpret_cast<cs_animation_packet*>(m_cSendBuf);
	animation_packet->size = sizeof(cs_animation_packet);
	animation_packet->type = CS_ANIMATION;
	animation_packet->uiType = uiAnimationType;

	int retval = WSASend(m_sock, &m_SendWsaBuf, 1, &size, flag, NULL, NULL);

	if (retval != 0) {
		int err_no = WSAGetLastError();
		Err_display("Err while sending packet - ", err_no);
	}

}

void CNetwork::Send_Install_Housing_Packet(UINT uiType, Vec3 vLocalPos, Vec3 vLocalRot, Vec3 vLocalScale, Vec3 vOffsetPos, Vec3 vOffsetScale)
{
	DWORD size, flag = 0;

	cs_install_housing_packet* install_housing_packet = reinterpret_cast<cs_install_housing_packet*>(m_cSendBuf);
	install_housing_packet->type = CS_HOUSING_INSTALL;
	install_housing_packet->housing_type = uiType;
	install_housing_packet->vLocalPos = vLocalPos;
	install_housing_packet->vLocalRot = vLocalRot;
	install_housing_packet->vLocalScale = vLocalScale;
	install_housing_packet->vOffsetPos = vOffsetPos;
	install_housing_packet->vOffsetScale = vOffsetScale;

	int retval = WSASend(m_sock, &m_SendWsaBuf, 1, &size, flag, NULL, NULL);

	if (retval != 0)
	{
		int err_no = WSAGetLastError();
		Err_display("Err while sending packet - ", err_no);
	}
}

void CNetwork::Send_Remove_Housing_Packet(USHORT house_id)
{
	DWORD size, flag = 0;

	cs_remove_housing_packet* remove_housing_packet = reinterpret_cast<cs_remove_housing_packet*>(m_cSendBuf);
	remove_housing_packet->type = CS_HOUSING_REMOVE;
	remove_housing_packet->house_id = house_id;

	int retval = WSASend(m_sock, &m_SendWsaBuf, 1, &size, flag, NULL, NULL);

	if (retval != 0)
	{
		int err_no = WSAGetLastError();
		Err_display("Err while sending packet - ", err_no);
	}
}

void CNetwork::Send_Get_Item_Packet(UINT uiItemType, UINT uiInvenNum)
{
	DWORD size, flag = 0;

	cs_item_get_packet* item_get_packet = reinterpret_cast<cs_item_get_packet*>(m_cSendBuf);
	item_get_packet->type = CS_GET_ITEM;
	item_get_packet->uiType = uiItemType;
	item_get_packet->uiInvenNum = uiInvenNum;

	int retval = WSASend(m_sock, &m_SendWsaBuf, 1, &size, flag, NULL, NULL);

	if (retval != 0)
	{
		int err_no = WSAGetLastError();
		Err_display("Err while sending packet - ", err_no);
	}
}

void CNetwork::Send_Remove_Item_Packet(UINT uiItemType, UINT uiInvenNum)
{
	DWORD size, flag = 0;

	cs_item_remove_packet* item_remove_packet = reinterpret_cast<cs_item_remove_packet*>(m_cSendBuf);
	item_remove_packet->type = CS_GET_ITEM;
	item_remove_packet->uiType = uiItemType;
	item_remove_packet->uiInvenNum = uiInvenNum;

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
	dynamic_cast<CLoginScene*>(pScene->GetSceneScript())->NextScene();
	cout << "Login Success" << endl;
}

void CNetwork::Recv_Login_Fail_Packet(char * packet)
{
	sc_login_fail_packet* login_packet = reinterpret_cast<sc_login_fail_packet*>(packet);
	cout << "Login Failed" << endl;
}

void CNetwork::Recv_Disconnect_Server_Packet(char * packet)
{
	sc_disconnect_server_packet* disconnect_server_packet = reinterpret_cast<sc_disconnect_server_packet*>(packet);
	cout << "Server Exit" << endl;
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CSceneMgr::GetInst()->GetCurScene()->GetLayer(i)->RemoveAll();
	}
	PostQuitMessage(0);
	return;
}

void CNetwork::Recv_Status_Player_Packet(char * packet)
{
	sc_status_player_packet* status_player_packet = reinterpret_cast<sc_status_player_packet*>(packet);
	USHORT player_id = status_player_packet->id;
	float fHealth = status_player_packet->fHealth;
	float fHungry = status_player_packet->fHungry;
	float fThrist = status_player_packet->fThrist;

	// 추가
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
	USHORT player_id = animation_player_packet->id;
	UINT uiType = animation_player_packet->animation_uiType;

	if (m_usID == player_id)
	{
		if (uiType == (UINT)PLAYER_ANIMATION_TYPE::WALK) {
			m_pPlayer->Animator3D()->ChangeAnimation(L"Walk");
		}
		else if (uiType == (UINT)PLAYER_ANIMATION_TYPE::RUN) {
			m_pPlayer->Animator3D()->ChangeAnimation(L"Run");
		}
		else if (uiType == (UINT)PLAYER_ANIMATION_TYPE::IDLE1) {
			m_pPlayer->Animator3D()->ChangeAnimation(L"Idle1");
		}
		else if (uiType == (UINT)PLAYER_ANIMATION_TYPE::IDLE2) {
			m_pPlayer->Animator3D()->ChangeAnimation(L"Idle2");
		}
		else if (uiType == (UINT)PLAYER_ANIMATION_TYPE::DIE) {
			m_pPlayer->Animator3D()->ChangeAnimation(L"Die");
		}
		else if (uiType == (UINT)PLAYER_ANIMATION_TYPE::TAKE_WEAPON) {
			m_pPlayer->Animator3D()->ChangeAnimation(L"TakeWeapon");
		}
		else if (uiType == (UINT)PLAYER_ANIMATION_TYPE::ATTACK1) {
			m_pPlayer->Animator3D()->ChangeAnimation(L"Attack1");
		}
		else if (uiType == (UINT)PLAYER_ANIMATION_TYPE::ATTACK2) {
			m_pPlayer->Animator3D()->ChangeAnimation(L"Attack2");
		}
		else if (uiType == (UINT)PLAYER_ANIMATION_TYPE::ATTACK3) {
			m_pPlayer->Animator3D()->ChangeAnimation(L"Attack3");
		}
		else if (uiType == (UINT)PLAYER_ANIMATION_TYPE::HIT1) {
			m_pPlayer->Animator3D()->ChangeAnimation(L"Hit1");
		}
		else if (uiType == (UINT)PLAYER_ANIMATION_TYPE::HIT2) {
			m_pPlayer->Animator3D()->ChangeAnimation(L"Hit2");
		}
		else if (uiType == (UINT)PLAYER_ANIMATION_TYPE::JUMP) {
			m_pPlayer->Animator3D()->ChangeAnimation(L"Jump");
		}
		else
			return;
	}
	else
	{
		if (uiType == (UINT)PLAYER_ANIMATION_TYPE::WALK) {
			m_cumPlayer[player_id]->Animator3D()->ChangeAnimation(L"Walk");
		}
		else if (uiType == (UINT)PLAYER_ANIMATION_TYPE::RUN) {
			m_cumPlayer[player_id]->Animator3D()->ChangeAnimation(L"Run");
		}
		else if (uiType == (UINT)PLAYER_ANIMATION_TYPE::IDLE1) {
			m_cumPlayer[player_id]->Animator3D()->ChangeAnimation(L"Idle1");
		}
		else if (uiType == (UINT)PLAYER_ANIMATION_TYPE::IDLE2) {
			m_cumPlayer[player_id]->Animator3D()->ChangeAnimation(L"Idle2");
		}
		else if (uiType == (UINT)PLAYER_ANIMATION_TYPE::DIE) {
			m_cumPlayer[player_id]->Animator3D()->ChangeAnimation(L"Die");
		}
		else if (uiType == (UINT)PLAYER_ANIMATION_TYPE::TAKE_WEAPON) {
			m_cumPlayer[player_id]->Animator3D()->ChangeAnimation(L"TakeWeapon");
		}
		else if (uiType == (UINT)PLAYER_ANIMATION_TYPE::ATTACK1) {
			m_cumPlayer[player_id]->Animator3D()->ChangeAnimation(L"Attack1");
		}
		else if (uiType == (UINT)PLAYER_ANIMATION_TYPE::ATTACK2) {
			m_cumPlayer[player_id]->Animator3D()->ChangeAnimation(L"Attack2");
		}
		else if (uiType == (UINT)PLAYER_ANIMATION_TYPE::ATTACK3) {
			m_cumPlayer[player_id]->Animator3D()->ChangeAnimation(L"Attack3");
		}
		else if (uiType == (UINT)PLAYER_ANIMATION_TYPE::HIT1) {
			m_cumPlayer[player_id]->Animator3D()->ChangeAnimation(L"Hit1");
		}
		else if (uiType == (UINT)PLAYER_ANIMATION_TYPE::HIT2) {
			m_cumPlayer[player_id]->Animator3D()->ChangeAnimation(L"Hit2");
		}
		else if (uiType == (UINT)PLAYER_ANIMATION_TYPE::JUMP) {
			m_cumPlayer[player_id]->Animator3D()->ChangeAnimation(L"Jump");
		}
		else
			return;
	}
}

void CNetwork::Recv_WakeUp_Npc_Packet(char * packet)
{
	sc_wake_up_packet* wakeup_npc_packet = reinterpret_cast<sc_wake_up_packet*>(packet);

	unsigned int monster_id = wakeup_npc_packet->id;

	m_cumAnimal[monster_id]->GetScript<CAnimalScript>()->SetWakeUp(true);
}

void CNetwork::Recv_Put_Animal_Packet(char * packet)
{
	sc_put_npc_packet* put_npc_packet = reinterpret_cast<sc_put_npc_packet*>(packet);
	USHORT monster_id = put_npc_packet->id;

	UINT eType = put_npc_packet->eType;
	Vec3 vPos = put_npc_packet->vPos;
	Vec3 vRot = put_npc_packet->vRot;

	dynamic_cast<CIngameScene*>(pScene->GetSceneScript())->AnimalUpdate(monster_id, vPos, vRot, eType);
}

void CNetwork::Recv_Remove_Animal_Packet(char * packet)
{
	sc_remove_npc_packet* remove_npc_packet = reinterpret_cast<sc_remove_npc_packet*>(packet);
	USHORT monster_id = remove_npc_packet->id;

	dynamic_cast<CIngameScene*>(pScene->GetSceneScript())->AnimalDestory(monster_id);
}

void CNetwork::Recv_Pos_Animal_Packet(char * packet)
{
	sc_pos_npc_packet* pos_npc_packet = reinterpret_cast<sc_pos_npc_packet*>(packet);
	unsigned int monster_id = pos_npc_packet->id;
	UINT eType = pos_npc_packet->eType;
	Vec3 vPos = pos_npc_packet->vPos;
	Vec3 vRot = pos_npc_packet->vRot;

	dynamic_cast<CIngameScene*>(pScene->GetSceneScript())->AnimalUpdate(monster_id, vPos, vRot, eType);
}

void CNetwork::Recv_Animation_Animal_Packet(char * packet)
{
	sc_animation_npc_packet* animation_npc_packet = reinterpret_cast<sc_animation_npc_packet*>(packet);
	unsigned int monster_id = animation_npc_packet->id;
	UINT		uiType = animation_npc_packet->animation;

	// 몬스터 애니메이션 키값 바꾸기
	dynamic_cast<CIngameScene*>(pScene->GetSceneScript())->AnimalAnimationUpdate(monster_id, uiType);
}

void CNetwork::Recv_Put_Natural_Packet(char * packet)
{
	sc_put_natural_packet* put_natural_packet = reinterpret_cast<sc_put_natural_packet*>(packet);
	float fHealth = put_natural_packet->fHealth;
	bool bDestroy = put_natural_packet->bDestroy;
	USHORT natural_id = put_natural_packet->natural_id;
	Vec3 vLocalPos = put_natural_packet->vLocalPos;
	Vec3 vLocalScale = put_natural_packet->vLocalScale;
	Vec3 vOffsetPos = put_natural_packet->vOffsetPos;
	Vec3 vOffsetScale = put_natural_packet->vOffsetScale;

	UINT uiType = 0;
	Vec3 vLocalRot = Vec3(0.f, 0.f, 0.f);

	// 추가
	dynamic_cast<CIngameScene*>(pScene->GetSceneScript())->InstallNatural(uiType, natural_id, vLocalPos, vLocalRot, vLocalScale, vOffsetPos, vOffsetScale, fHealth, bDestroy);
}

void CNetwork::Recv_Destroy_Natural_Packet(char * packet)
{
	sc_destroy_natural_packet* destroy_natural_packet = reinterpret_cast<sc_destroy_natural_packet*>(packet);
	USHORT natural_id = destroy_natural_packet->natural_id;

	// 제거
	dynamic_cast<CIngameScene*>(pScene->GetSceneScript())->DestroyNatural(natural_id);
}

void CNetwork::Recv_Install_Housing_Packet(char * packet)
{
	sc_install_housing_packet* install_housing_packet = reinterpret_cast<sc_install_housing_packet*>(packet);
	USHORT house_id = install_housing_packet->house_id;
	UINT type = install_housing_packet->housing_type;
	Vec3 vPos = install_housing_packet->vLocalPos;
	Vec3 vRot = install_housing_packet->vLocalRot;
	Vec3 vScale = install_housing_packet->vLocalScale;

	// INSTALL 전송
	dynamic_cast<CIngameScene*>(pScene->GetSceneScript())->InstallHousing(type, house_id, vPos, vRot, vScale);
}

void CNetwork::Recv_Remove_Housing_Packet(char * packet)
{
	sc_remove_housing_packet* remove_housing_packet = reinterpret_cast<sc_remove_housing_packet*>(packet);
	USHORT house_id = remove_housing_packet->house_id;

	// REMOVE 
	dynamic_cast<CIngameScene*>(pScene->GetSceneScript())->DestroyHousing(house_id);
}

void CNetwork::Recv_Add_Item_Packet(char * packet)
{
	sc_add_item_packet* add_item_packet = reinterpret_cast<sc_add_item_packet*>(packet);
	UINT uiType = add_item_packet->uiItemType;
	UINT uiInvenNum = add_item_packet->uiInven_num;
}

void CNetwork::Recv_Weather_Packet(char * packet)
{
	sc_weather_packet* weather_packet = reinterpret_cast<sc_weather_packet*>(packet);
	bool bRain = weather_packet->bRain;

	// 받기
	//cout << bRain << endl;

	CGameObject* pSunshineObject = pScene->GetLayer( 0 )->FindObject( L"Sunshine" );

	if ( pSunshineObject )
	{
		CSunshineScript* pSunshineScript = pSunshineObject->GetScript<CSunshineScript>();
		pSunshineScript->SetRainDrop( bRain );
	}

}

void CNetwork::Recv_Time_Packet(char * packet)
{
	sc_time_packet* time_packet = reinterpret_cast<sc_time_packet*>(packet);
	float fTime = time_packet->fTime;

	cout << fTime << endl;
}
