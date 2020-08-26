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
CGameObject*	CNetwork::m_pPlayer;
CGameObject*	CNetwork::m_pChat;
concurrent_unordered_map<unsigned int, CGameObject*> CNetwork::m_cumPlayer;
concurrent_unordered_map<unsigned int, CGameObject*> CNetwork::m_cumAnimal;

CNetwork::CNetwork()
{
	
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
	m_fpPacketProcess[SC_ANIMATION_PLAYER] = [&](char* packet) {Recv_Animation_Player_Packet(packet); };
	// - Animal
	m_fpPacketProcess[SC_PUT_ANIMAL] = [&](char* packet) {Recv_Put_Animal_Packet(packet); };
	m_fpPacketProcess[SC_POS_ANIMAL] = [&](char* packet) {Recv_Pos_Animal_Packet(packet); };
	m_fpPacketProcess[SC_REMOVE_ANIMAL] = [&](char* packet) {Recv_Remove_Animal_Packet(packet); };
	m_fpPacketProcess[SC_ANIMATION_ANIMAL] = [&](char* packet) {Recv_Animation_Animal_Packet(packet); };
	// - Natural
	m_fpPacketProcess[SC_PUT_NATURAL] = [&](char* packet) {Recv_Put_Natural_Packet(packet); };
	m_fpPacketProcess[SC_DESTROY_NATURAL] = [&](char* packet) {Recv_Destroy_Natural_Packet(packet); };
	// - House
	m_fpPacketProcess[SC_INSTALL_HOUSE] = [&](char* packet) {Recv_Install_Housing_Packet(packet); };
	m_fpPacketProcess[SC_REMOVE_HOUSE] = [&](char* packet) {Recv_Remove_Housing_Packet(packet); };
	// - Item
	m_fpPacketProcess[SC_ADD_ITEM] = [&](char* packet) {};
	m_fpPacketProcess[SC_REMOVE_ITEM] = [&](char* packet) {};
	// - Etc
	m_fpPacketProcess[SC_WEATHER] = [&](char* packet) {};
	m_fpPacketProcess[SC_TIME] = [&](char* packet) {};
}

void CNetwork::Init(HWND hWnd)
{
	m_bClientClose = true;
	m_RecvWsaBuf.buf = m_cRecvbuf;
	m_RecvWsaBuf.len = BUF_SIZE;
	m_in_packet_size = 0;
	m_saved_packet_size = 0;
	SetHwnd(hWnd);
	BindfpPacket();
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
	WSAAsyncSelect(m_sock, m_hWnd, WM_SOCKET, FD_CLOSE | FD_READ);
	m_bClientClose = false;
	return true;
}

void CNetwork::DisConnect()
{
	closesocket(m_sock);
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

	//dynamic_cast<CIngameScene*>(pScene->GetSceneScript())->PlayerStatusUpdate(fHealth, fHungry, fThrist);
}

void CNetwork::Recv_Put_Player_Packet(char * packet)
{
	sc_put_player_packet* put_player_packet = reinterpret_cast<sc_put_player_packet*>(packet);
	USHORT player_id = put_player_packet->id;

	Vec3 vPos = put_player_packet->vPos;
	Vec3 vRot = put_player_packet->vRot;

	dynamic_cast<CIngameScene*>(pScene->GetSceneScript())->PlayerUpdate(player_id, vPos, vRot);
}

void CNetwork::Recv_Remove_Player_Packet(char * packet)
{
	sc_remove_player_packet* remove_player_packet = reinterpret_cast<sc_remove_player_packet*>(packet);
	unsigned int player_id = remove_player_packet->id;

	dynamic_cast<CIngameScene*>(pScene->GetSceneScript())->PlayerDestroy(player_id);
}

void CNetwork::Recv_Pos_Player_Packet(char * packet)
{
	sc_pos_player_packet*	pos_packet = reinterpret_cast<sc_pos_player_packet*>(packet);
	unsigned int player_id = pos_packet->id;
	Vec3 vPos = pos_packet->vPos;
	Vec3 vRot = pos_packet->vRot;

	dynamic_cast<CIngameScene*>(pScene->GetSceneScript())->PlayerUpdate(player_id, vPos, vRot);
}

void CNetwork::Recv_Chat_Packet(char * packet)
{
	sc_chat_packet* chat_packet = reinterpret_cast<sc_chat_packet*>(packet);

	wstring wname = chat_packet->wcid;
	string name;
	name.assign(wname.begin(), wname.end());
	string Msg(chat_packet->meesage);
	m_pChat->GetScript<CChatScript>()->AddChat(name, Msg);
	m_pChat->GetScript<CInputScript>()->SetEnable(false);
	m_pChat->GetScript<CInputScript>()->Clear();
}

void CNetwork::Recv_Animation_Player_Packet(char * packet)
{
	sc_animation_player_packet* animation_player_packet = reinterpret_cast<sc_animation_player_packet*>(packet);
	USHORT player_id = animation_player_packet->id;
	UINT uiType = animation_player_packet->animation_uiType;

	dynamic_cast<CIngameScene*>(pScene->GetSceneScript())->PlayerAnimationUpdate(player_id, uiType);
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

	//cout << fTime << endl;
}
