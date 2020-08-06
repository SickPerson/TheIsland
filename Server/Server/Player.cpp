#include "Player.h"
#include "Network.h"

CPlayer::CPlayer()
{
	SetSocket(INVALID_SOCKET);
	SetConnect(false);
	m_iCursize = 0;
	m_iPrevsize = 0;

	ZeroMemory(&m_over, sizeof(OVER_EX));

	m_over.m_Event = EV_RECV;
	m_over.m_DataBuffer.buf = m_over.m_MessageBuffer;
	m_over.m_DataBuffer.len = MAX_BUF;
}

CPlayer::~CPlayer()
{
	m_cusViewList.clear();
}

void CPlayer::SetRecvState()
{
	DWORD flag{};
	DWORD size{};

	ZeroMemory(&m_over.m_Overlapped, sizeof(WSAOVERLAPPED));

	int retval = WSARecv(m_sSocket, &m_over.m_DataBuffer, 1, &size, &flag, &m_over.m_Overlapped, NULL);

	if (0 != retval)
	{
		int err_no = WSAGetLastError();
		if (WSA_IO_PENDING != err_no)
			CNetwork::Err_display("SetRecvState - ", err_no);
	}
}

char* CPlayer::RecvEvent(DWORD dataSize, char * packet)
{
	int rest_size = dataSize;
	char* cPacket = packet;
	int curr = m_iCursize;
	int prev = m_iPrevsize;
	char* packetBuf = m_over.m_MessageBuffer;

	while (0 < rest_size)
	{
		int packet_size;
		if (0 != curr)
			packet_size = curr;
		else
		{
			packet_size = cPacket[0];
			curr = packet_size;
		}
		int need_size = packet_size - prev;
		if (need_size <= rest_size)
		{
			memcpy(packetBuf + prev, cPacket, need_size);
			prev = 0;
			curr = 0;
			rest_size -= need_size;
			cPacket += need_size;
			return cPacket;
		}
		else
		{
			memcpy(packetBuf + prev, cPacket, rest_size);
			prev += rest_size;
			rest_size = -rest_size;
			cPacket += rest_size;
		}
	}
	return nullptr;
}

void CPlayer::SetPlayerStatus(tPlayerStatus & status)
{
	unique_lock<shared_mutex> lock(m_smPlayerStatusMutex[(UINT)PLAYER_LOCK_TYPE::STATUS]);
	m_tPlayerStatus.fHealth = status.fHealth;
	m_tPlayerStatus.fHungry = status.fHungry;
	m_tPlayerStatus.fThirst = status.fThirst;
	m_tPlayerStatus.fSpeed = status.fSpeed;
	m_tPlayerStatus.fDamage = status.fDamage;
}

void CPlayer::SetHealth(float & fHealth)
{
	unique_lock<shared_mutex> lock(m_smPlayerStatusMutex[(UINT)PLAYER_LOCK_TYPE::HEALTH]);
	m_tPlayerStatus.fHealth = fHealth;
}

void CPlayer::SetHungry(float & fHungry)
{
	unique_lock<shared_mutex> lock(m_smPlayerStatusMutex[(UINT)PLAYER_LOCK_TYPE::HUNGRY]);
	m_tPlayerStatus.fHungry = fHungry;
}

void CPlayer::SetThirst(float & fThirst)
{
	unique_lock<shared_mutex> lock(m_smPlayerStatusMutex[(UINT)PLAYER_LOCK_TYPE::THIRST]);
	m_tPlayerStatus.fThirst = fThirst;
}

void CPlayer::SetSpeed(float & fSpeed)
{
	unique_lock<shared_mutex> lock(m_smPlayerStatusMutex[(UINT)PLAYER_LOCK_TYPE::SPEED]);
	m_tPlayerStatus.fSpeed = fSpeed;
}

void CPlayer::SetDamage(float & fDamage)
{
	unique_lock<shared_mutex> lock(m_smPlayerStatusMutex[(UINT)PLAYER_LOCK_TYPE::DAMAGE]);
	m_tPlayerStatus.fSpeed = fDamage;
}

void CPlayer::SetNumID(unsigned short & numID)
{
	unique_lock<shared_mutex> lock(m_smPlayerStatusMutex[(UINT)PLAYER_LOCK_TYPE::NUMID]);
	m_uiID = numID;
}

void CPlayer::SetWcID(wchar_t * wcID)
{
	unique_lock<shared_mutex> lock(m_smPlayerStatusMutex[(UINT)PLAYER_LOCK_TYPE::WCID]);
	wcscpy_s(m_wcID, wcID);
}

void CPlayer::SetConnect(bool bConnect)
{
	unique_lock<shared_mutex> lock(m_smPlayerStatusMutex[(UINT)PLAYER_LOCK_TYPE::CONNECT]);
	m_bConnect = bConnect;
}

void CPlayer::SetSocket(const SOCKET & socket)
{
	unique_lock<shared_mutex>	lock(m_smPlayerStatusMutex[(UINT)PLAYER_LOCK_TYPE::SOCKET]);
	m_sSocket = socket;
}

tPlayerStatus & CPlayer::GetPlayerStatus()
{
	shared_lock<shared_mutex>lock(m_smPlayerStatusMutex[(UINT)PLAYER_LOCK_TYPE::STATUS]);
	return m_tPlayerStatus;
}

float & CPlayer::GetHealth()
{
	shared_lock<shared_mutex> lock(m_smPlayerStatusMutex[(UINT)PLAYER_LOCK_TYPE::HEALTH]);
	return m_tPlayerStatus.fHealth;
}

float & CPlayer::GetHungry()
{
	shared_lock<shared_mutex> lock(m_smPlayerStatusMutex[(UINT)PLAYER_LOCK_TYPE::HUNGRY]);
	return m_tPlayerStatus.fHungry;
}

float & CPlayer::GetThirst()
{
	shared_lock<shared_mutex> lock(m_smPlayerStatusMutex[(UINT)PLAYER_LOCK_TYPE::THIRST]);
	return m_tPlayerStatus.fThirst;
}

float & CPlayer::GetSpeed()
{
	shared_lock<shared_mutex> lock(m_smPlayerStatusMutex[(UINT)PLAYER_LOCK_TYPE::SPEED]);
	return m_tPlayerStatus.fSpeed;
}

float & CPlayer::GetDamage()
{
	shared_lock<shared_mutex> lock(m_smPlayerStatusMutex[(UINT)PLAYER_LOCK_TYPE::DAMAGE]);
	return m_tPlayerStatus.fDamage;
}

unsigned short & CPlayer::GetNumID()
{
	shared_lock<shared_mutex>lock(m_smPlayerStatusMutex[(UINT)PLAYER_LOCK_TYPE::NUMID]);
	return m_uiID;
}

wchar_t * CPlayer::GetWcID()
{
	shared_lock<shared_mutex>lock(m_smPlayerStatusMutex[(UINT)PLAYER_LOCK_TYPE::WCID]);
	return m_wcID;
}

bool CPlayer::GetConnect()
{
	shared_lock<shared_mutex>lock(m_smPlayerStatusMutex[(UINT)PLAYER_LOCK_TYPE::CONNECT]);
	return m_bConnect;
}

SOCKET & CPlayer::GetSocket()
{
	shared_lock<shared_mutex>lock(m_smPlayerStatusMutex[(UINT)PLAYER_LOCK_TYPE::SOCKET]);
	return m_sSocket;
}
