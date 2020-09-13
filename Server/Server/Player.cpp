#include "Player.h"
#include "Network.h"

CPlayer::CPlayer() :
	m_sSocket(INVALID_SOCKET),
	m_bConnect(false),
	m_iCursize(0),
	m_iPrevsize(0),
	m_fArmor(0)
{
	ZeroMemory(&m_over, sizeof(OVER_EX));

	m_over.m_Event = EV_RECV;
	m_over.m_DataBuffer.buf = m_over.m_MessageBuffer;
	m_over.m_DataBuffer.len = MAX_BUF;

	for (int i = 0; i < CS_END; ++i) {
		m_Count[i] = 0;
	}
}

CPlayer::~CPlayer()
{
	m_cusViewList.clear();
}

void CPlayer::SetRecvState()
{
	ZeroMemory(&m_over.m_Overlapped, sizeof(WSAOVERLAPPED));

	DWORD size{ 0 }, flag{ 0 };

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
	unique_lock<shared_mutex> lock(m_smPlayerStatusMutex);
	m_tPlayerStatus.fHealth = status.fHealth;
	m_tPlayerStatus.fHungry = status.fHungry;
	m_tPlayerStatus.fThirst = status.fThirst;
	m_tPlayerStatus.fSpeed = status.fSpeed;
	m_tPlayerStatus.fDamage = status.fDamage;
}

void CPlayer::SetHealth(float & fHealth)
{
	unique_lock<shared_mutex> lock(m_smPlayerStatusMutex);
	m_tPlayerStatus.fHealth = fHealth;
}

void CPlayer::SetHungry(float & fHungry)
{
	unique_lock<shared_mutex> lock(m_smPlayerStatusMutex);
	m_tPlayerStatus.fHungry = fHungry;
}

void CPlayer::SetThirst(float & fThirst)
{
	unique_lock<shared_mutex> lock(m_smPlayerStatusMutex);
	m_tPlayerStatus.fThirst = fThirst;
}

void CPlayer::SetSpeed(float & fSpeed)
{
	unique_lock<shared_mutex> lock(m_smPlayerStatusMutex);
	m_tPlayerStatus.fSpeed = fSpeed;
}

void CPlayer::SetArmor(float & fArmor)
{
	unique_lock<shared_mutex>	lock(m_smPlayerStatusMutex);
	m_fArmor = fArmor;
}

void CPlayer::SetIncreaseHealth(float & fAmount)
{
	unique_lock<shared_mutex>	lock(m_smPlayerStatusMutex);
	m_tPlayerStatus.fHealth += fAmount;
	if (m_tPlayerStatus.fHealth > 100.f) {
		m_tPlayerStatus.fHealth = 100.f;
	}
}

void CPlayer::SetIncreaseHungry(float & fAmount)
{
	unique_lock<shared_mutex>	lock(m_smPlayerStatusMutex);
	m_tPlayerStatus.fHungry += fAmount;
	if (m_tPlayerStatus.fHungry > 100.f) {
		m_tPlayerStatus.fHungry = 100.f;
	}
}

void CPlayer::SetIncreaseThirst(float & fAmount)
{
	unique_lock<shared_mutex>	lock(m_smPlayerStatusMutex);
	m_tPlayerStatus.fThirst += fAmount;
	if (m_tPlayerStatus.fThirst > 100.f) {
		m_tPlayerStatus.fThirst = 100.f;
	}
}

void CPlayer::SetNumID(USHORT & numID)
{
	unique_lock<shared_mutex> lock(m_smPlayerStatusMutex);
	m_usID = numID;
}

void CPlayer::SetWcID(char * wcID)
{
	unique_lock<shared_mutex> lock(m_smPlayerStatusMutex);
	strcpy_s(m_ID, wcID);
}

void CPlayer::SetDbNum(int & dbID)
{
	unique_lock<shared_mutex> lock(m_smPlayerStatusMutex);
	m_dbNum = dbID;
}

void CPlayer::SetConnect(bool bConnect)
{
	unique_lock<shared_mutex> lock(m_smPlayerStatusMutex);
	m_bConnect = bConnect;
}

void CPlayer::SetSocket(SOCKET & socket)
{
	unique_lock<shared_mutex>	lock(m_smPlayerStatusMutex);
	m_sSocket = socket;
}

tPlayerStatus & CPlayer::GetPlayerStatus()
{
	shared_lock<shared_mutex>lock(m_smPlayerStatusMutex);
	return m_tPlayerStatus;
}

float & CPlayer::GetHealth()
{
	shared_lock<shared_mutex> lock(m_smPlayerStatusMutex);
	return m_tPlayerStatus.fHealth;
}

float & CPlayer::GetHungry()
{
	shared_lock<shared_mutex> lock(m_smPlayerStatusMutex);
	return m_tPlayerStatus.fHungry;
}

float & CPlayer::GetThirst()
{
	shared_lock<shared_mutex> lock(m_smPlayerStatusMutex);
	return m_tPlayerStatus.fThirst;
}

float & CPlayer::GetSpeed()
{
	shared_lock<shared_mutex> lock(m_smPlayerStatusMutex);
	return m_tPlayerStatus.fSpeed;
}

float & CPlayer::GetArmor()
{
	shared_lock<shared_mutex>	lock(m_smPlayerStatusMutex);
	return m_fArmor;
}

const USHORT & CPlayer::GetNumID()
{
	shared_lock<shared_mutex>lock(m_smPlayerStatusMutex);
	return m_usID;
}

const char * CPlayer::GetWcID()
{
	shared_lock<shared_mutex>lock(m_smPlayerStatusMutex);
	return m_ID;
}

const int& CPlayer::GetDbNum()
{
	shared_lock<shared_mutex>lock(m_smPlayerStatusMutex);
	return m_dbNum;
}

const bool CPlayer::GetConnect()
{
	shared_lock<shared_mutex>lock(m_smPlayerStatusMutex);
	return m_bConnect;
}

const SOCKET & CPlayer::GetSocket()
{
	shared_lock<shared_mutex>lock(m_smPlayerStatusMutex);
	return m_sSocket;
}
