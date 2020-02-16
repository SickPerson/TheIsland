#pragma once
#pragma comment(lib, "ws2_32")

#include <WinSock2.h>
#include <Windows.h>

#include <sqlext.h>  

#include <iostream>
#include <thread>

#include <cstdlib>
#include <mutex>

#include <vector>
#include <set>
#include <map>

using namespace std;
using namespace chrono;

// SingleTon
#include "SingleTon.h"

// enum 문
#include "types.h"

// 클라이언트와 서버가 통신하기 위한 기본적인 통신 규칙(서버 및 클라이언트 수정시, 클라이언트와 동기화에 영향을 미친다.)
#include "protocol.h"

// struct 구조체 문
#include "struct.h"

// boost asio 서버관련 라이브러리 (iocp와 boost 중 하나만 쓰면 된다.)
// #include "boost_asio.h"
