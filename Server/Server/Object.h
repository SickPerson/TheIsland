#pragma once
#include "stdafx.h"

enum OBJ_LOCK_TYPE {
	OBJ_LOCK_POS,
	OBJ_LOCK_ROT,
	OBJ_LOCK_STATE,
	OBJ_LOCK_END
};

class CObject
{
private:
	char m_cState;

	Vec3	m_vPos;
	Vec3	m_vRot;

	shared_mutex m_ObjMutex[OBJ_LOCK_END];

public:
	CObject();
	~CObject();

public:
	void SetPos( float& fPosX,  float& fPosY,  float& fPosZ);
	void SetPos( Vec3 vPos);
	void SetRot( float& fRotX,  float& fRotY,  float& fRotZ);
	void SetRot( Vec3 vRot);
	void SetState( char cState);

public:
	const Vec3	GetPos();
	const Vec3	GetRot();
	const  char GetState();
};