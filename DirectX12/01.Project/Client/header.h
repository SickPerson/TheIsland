#pragma once
#define _CRT_SECURE_NI_WARNINGS

#ifdef DEBUG
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#endif DEBUG

#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h>
#include <mutex>
#include <iostream>

#include <concurrent_unordered_set.h>
#include <concurrent_unordered_map.h>

#include "define.h"
#include "enum.h"

#include "../../../Server/Server/protocol.h"

using namespace std;
using namespace concurrency;