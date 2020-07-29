#pragma once
#include "stdafx.h"

enum OBJ_LOCK_TYPE {
	OBJ_LOCK_HP,
	OBJ_LOCK_STAMINA,
	OBJ_LOCK_HUNGRY,
	OBJ_LOCK_THIRST,
	OBJ_LOCK_ANITIME,
	OBJ_LOCK_POS,
	OBJ_LOCK_ROT,
	OBJ_LOCK_STATE,
	OBJ_LOCK_DAMAGE,
	OBJ_LOCK_CONNECT,
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
	void SetPos(const float& fPosX, const float& fPosY, const float& fPosZ);
	void SetPos(const Vec3& vPos);
	void SetRot(const float& fRotX, const float& fRotY, const float& fRotZ);
	void SetRot(const Vec3& vRot);
	void SetState(const char& cState);

public:
	const Vec3&	GetPos();
	const Vec3&	GetRot();
	const  char& GetState();
};