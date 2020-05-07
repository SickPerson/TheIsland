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

#include <Engine/Layer.h>
#include <Engine/Scene.h>

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
#include <Engine/TestScript.h>

unsigned short CNetwork::m_usID = 0;

concurrent_unordered_map<unsigned int, CGameObject*> CNetwork::m_cumPlayer;
concurrent_unordered_set<unsigned int> CNetwork::m_cusViewList;

CNetwork::CNetwork()
{
	Init();
	//ConnectServer();
}

CNetwork::~CNetwork()
{
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


	Ptr<CMeshData> pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\barghest.mdat", L"MeshData\\barghest.mdat");

	for (int i = 0; i < MAX_USER; ++i) {
		m_cumPlayer.insert(make_pair(i, pMeshData->Instantiate()));
	}

	m_cusViewList.clear();
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
		// IP가 틀렸을 경우
		Err_display("socket err", err_no);
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
	if (retval != SOCKET_ERROR)
	{
		cout << "CreateSelectEvent Success" << endl;
		return true;
	}
	else
	{
		int err_no = WSAGetLastError();
		Err_display("CreateSelectEvent err", err_no);
		return false;
	}
}

void CNetwork::DisConnect()
{
	closesocket(m_sock);
	WSACloseEvent(m_hEvent);
	WSACleanup();
	cout << "DisConnect" << endl;
}

void CNetwork::RecvPacket()
{
	while (!m_bClientClose)
	{
		cout << "Network Run" << endl;
		m_iIndex = WSAWaitForMultipleEvents(1, &m_hEvent, FALSE, WSA_INFINITE, FALSE);

		if ((m_iIndex == WSA_WAIT_FAILED) || (m_iIndex == WSA_WAIT_TIMEOUT))
		{
			int err_no = WSAGetLastError();
			Err_display("WSAWaitForMultipleEvents Err", err_no);
			break;
		}
		int retval = WSAEnumNetworkEvents(m_sock, m_hEvent, &m_weEvent);
		if (retval == SOCKET_ERROR)
		{
			int err_no = WSAGetLastError();
			Err_display("WSAEnumNetworkEvents Err", err_no);
			break;
		}

		if (m_weEvent.lNetworkEvents & FD_READ)
		{
			if (m_weEvent.iErrorCode[FD_READ_BIT] != 0)
			{
				int err_no = GetLastError();
				Err_display("m_weEvent.iErrorCode Err", err_no);
				break;
			}
			DWORD	size, flag = 0;
			int ret = WSARecv(m_sock, &m_RecvWsaBuf, 1, &size, &flag, NULL, NULL);
			if (ret == SOCKET_ERROR)
			{
				int err_no = GetLastError();
				Err_display("WSARecv SOCKET_ERROR", err_no);
				break;
			}

			BYTE* packet = reinterpret_cast<BYTE*>(m_cRecvbuf);

			cout << "SIZE:" << size << endl;
			cout << "packet size: " << (int)packet[0] << endl;
			while (size != 0)
			{
				if (0 == m_in_packet_size) m_in_packet_size = packet[0];
				if (size + m_saved_packet_size >= m_in_packet_size)
				{
					memcpy(m_cPacketBuf + m_saved_packet_size, packet, m_in_packet_size - m_saved_packet_size);
					ProcessPacket(m_cPacketBuf);
					packet += m_in_packet_size - m_saved_packet_size;
					size -= m_in_packet_size - m_saved_packet_size;
					m_in_packet_size = 0;
					m_saved_packet_size = 0;
				}
				else
				{
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

void CNetwork::ProcessPacket(char* _packet)
{
	switch (_packet[1])
	{
	case SC_LOGIN_OK: // 로그인이 성공한다면 서버에 연결을 시도합니다.
	{
		//cout << "ID 일치" << endl;
		SetLogin(true);
		break;
	}
	case SC_LOGIN_FAIL: // 로그인이 실패한다면 서버에 연결이 되지 않습니다.
		//cout << "ID 불일치" << endl;
		SetLogin(false);
		break;
	case SC_LOGIN:
	{
		pScene = CSceneMgr::GetInst()->GetCurScene();
		RecvLoginPacket(_packet);
		SetLoopStart(true);
		break;
	}
	case SC_CONNECT:
	{
		RecvConnectPacket(_packet);
		break;
	}
	case SC_POS:
	{
		RecvPosPacket(_packet);
		break;
	}
	case SC_DISCONNECT:
	{
		sc_disconnect_packet* packet = reinterpret_cast<sc_disconnect_packet*>(_packet);
		DeleteList(packet->id);
		pScene->FindLayer(L"Other")->RemoveParentObj(m_cumPlayer[packet->id]);
		break;
	}
	}
}

void CNetwork::SetID(unsigned short _id)
{
	m_usID = _id;
}

unsigned short CNetwork::GetID()
{
	return m_usID;
}

// 로그인 패킷을 서버로 보냅니다. string에서 char*으로 바꿀예정입니다.
void CNetwork::SendLoginPacket(string _sPlayerID)
{
	DWORD size, flag = 0;

	cs_login_packet* packet = reinterpret_cast<cs_login_packet*>(m_cSendBuf);

	packet->size = sizeof(cs_login_packet);
	packet->type = CS_LOGIN;
	packet->id = m_usID;
	//strcpy(packet->player_id, _sPlayerID.c_str());

	m_SendWsaBuf.len = sizeof(cs_login_packet);

	int retval = WSASend(m_sock, &m_SendWsaBuf, 1, &size, flag, NULL, NULL);

	if (retval != 0)
	{
		int err_no = WSAGetLastError();
		Err_display("Err while sending packet - ", err_no);
	}
}

void CNetwork::SendPosPacket()
{
	DWORD size, flag = 0;

	Vec3 pos = m_cumPlayer[m_usID]->Transform()->GetLocalPos();
	Vec3 dir = m_cumPlayer[m_usID]->Transform()->GetLocalRot();

	cs_pos_packet* packet = reinterpret_cast<cs_pos_packet*>(m_cSendBuf);

	packet->size = sizeof(cs_pos_packet);
	packet->type = CS_POS;
	packet->fposX = pos.x;
	packet->fposY = pos.y;
	packet->fposZ = pos.z;
	packet->fDirX = dir.x;
	packet->fDirY = dir.y;
	packet->fDirZ = dir.z;

	m_SendWsaBuf.len = sizeof(cs_pos_packet);

	int retval = WSASend(m_sock, &m_SendWsaBuf, 1, &size, flag, NULL, NULL);

	if (retval != 0)
	{
		int err_no = WSAGetLastError();
		Err_display("Err while sending packet - ", err_no);
	}
}

void CNetwork::SendDirPacket()
{
}

void CNetwork::SendChatPacket(string _message)
{
	DWORD size, flag = 0;

	cs_chat_packet packet;
	packet.size = sizeof(packet);
	packet.type = CS_CHAT;
	//strcpy(packet.meesage, _message.c_str());

	m_SendWsaBuf.len = sizeof(cs_pos_packet);

	int retval = WSASend(m_sock, &m_SendWsaBuf, 1, &size, flag, NULL, NULL);

	if (retval != 0)
	{
		int err_no = WSAGetLastError();
		Err_display("Err while sending packet - ", err_no);
	}
}

void CNetwork::RecvLoginPacket(char * _packet)
{
	sc_first_status_packet* packet = reinterpret_cast<sc_first_status_packet*>(_packet);
	m_usID = packet->id;
	InsertList(m_usID);

	m_cumPlayer[m_usID]->SetName(L"Player Object");
	m_cumPlayer[m_usID]->AddComponent(new CPlayerScript);
	m_cumPlayer[m_usID]->AddComponent(new CCollider2D);
	m_cumPlayer[m_usID]->Collider2D()->SetOffsetScale(Vec3(20.f, 40.f, 20.f));

	m_cumPlayer[m_usID]->FrustumCheck(false);
	m_cumPlayer[m_usID]->Transform()->SetLocalPos(Vec3(packet->fPosX, packet->fPosY, packet->fPosZ));
	m_cumPlayer[m_usID]->Transform()->SetLocalScale(Vec3(2.f, 2.f, 1.f));
	m_cumPlayer[m_usID]->Transform()->SetLocalRot(Vec3(packet->fDirX, packet->fDirY, packet->fDirZ));

	pScene->FindLayer(L"Player")->AddGameObject(m_cumPlayer[m_usID]);
}

void CNetwork::RecvConnectPacket(char * _packet)
{
	sc_accept_packet* packet = reinterpret_cast<sc_accept_packet*>(_packet);

	if (!CheckList(packet->id)) {
		InsertList(packet->id);
	}

	m_cumPlayer[packet->id]->SetName(L"Player Object");
	m_cumPlayer[packet->id]->AddComponent(new CPlayerScript);
	m_cumPlayer[packet->id]->AddComponent(new CCollider2D);
	m_cumPlayer[packet->id]->Collider2D()->SetOffsetScale(Vec3(20.f, 40.f, 20.f));

	m_cumPlayer[packet->id]->FrustumCheck(false);
	m_cumPlayer[packet->id]->Transform()->SetLocalPos(Vec3(packet->fPosX, packet->fPosY, packet->fPosZ));
	m_cumPlayer[packet->id]->Transform()->SetLocalScale(Vec3(2.f, 2.f, 1.f));
	m_cumPlayer[packet->id]->Transform()->SetLocalRot(Vec3(packet->fDirX, packet->fDirY, packet->fDirZ));

	pScene->FindLayer(L"Other")->AddGameObject(m_cumPlayer[packet->id]);
}

void CNetwork::RecvPosPacket(char * _packet)
{
	sc_pos_packet* packet = reinterpret_cast<sc_pos_packet*>(_packet);

	m_cumPlayer[packet->id]->Transform()->SetLocalPos(Vec3(packet->fPosX, packet->fPosY, packet->fPosZ));
	m_cumPlayer[packet->id]->Transform()->SetLocalRot(Vec3(packet->fDirX, packet->fDirY, packet->fDirZ));
}

void CNetwork::RecvChatPacket(char* _packet)
{
	sc_chat_packet* packet = reinterpret_cast<sc_chat_packet*>(_packet);

}
