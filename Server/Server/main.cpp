#include "stdafx.h"
#include "Network.h"

int main() {
	CNetwork::GetInst()->Initialize();
	CNetwork::GetInst()->StartServer();
	CNetwork::GetInst()->CloseServer();
}