#include "stdafx.h"
#include "PacketMgr.h"


void CPacketMgr::SendLoginPacket(wchar_t * _wcPlayerID, wchar_t * _wcPlayerPW)
{
	DWORD io_byte, flag = 0;

	cs_packet_login* login_packet = reinterpret_cast<cs_packet_login*>(m_cSendBuf);

	login_packet->type = CS_LOGIN;
	login_packet->size = sizeof(cs_packet_login);

	wcsncpy_s(login_packet->player_id, MAX_STR_LEN, _wcPlayerID, wcslen(_wcPlayerID));
	wcsncpy_s(login_packet->player_pw, MAX_STR_LEN, _wcPlayerPW, wcslen(_wcPlayerPW));

	m_wbSendWsaBuf.len = sizeof(cs_packet_login);

	int retval = WSASend(CNetwork::GetInst()->GetSocket(), &m_wbSendWsaBuf, 1, &io_byte, flag, NULL, NULL);

	if (retval != 0) {

	}

}

CPacketMgr::CPacketMgr()
{
}


CPacketMgr::~CPacketMgr()
{
}
