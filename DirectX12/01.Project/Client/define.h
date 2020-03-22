#pragma once
#define SINGLE(type) private: type(); ~type();\
public:\
static type* GetInst()\
{\
	static type mgr;\
	return &mgr;\
}

#define BUF_SIZE 1024
#define MAX_PACKET_SIZE 255