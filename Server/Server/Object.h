#pragma once
#include "stdafx.h"

enum class OBJ_LOCK_TYPE {
	LOCAL_POS,
	LOCAL_SCALE,
	LOCAL_ROT,
	SOCAL_DIR,
	OFFSET_POS,
	OFFSET_SCALE,
	STATE,
	END
};

class CObject
{
private:
	char	m_cState;
	Vec3	m_vLocalPos;
	Vec3	m_vLocalScale;
	Vec3	m_vLocalRot;
	Vec3	m_vLocalDir[(UINT)DIR_TYPE::END];

	// Collider
	Vec3	m_vOffsetPos;
	Vec3	m_vOffsetScale;

	shared_mutex m_ObjMutex[(UINT)OBJ_LOCK_TYPE::END];

public:
	CObject();
	~CObject();

public:
	void SetLocalPos(float& fPosX, float& fPosY, float& fPosZ);
	void SetLocalPos(Vec3 vPos);
	void SetLocalRot(float& fRotX, float& fRotY, float& fRotZ);
	void SetLocalRot(Vec3 vRot);
	void SetLocalScale(float& fScaleX, float& fScaleY, float& fScaleZ);
	void SetLocalScale(Vec3 vScale);
	void SetState(char cState);
	
	void SetOffsetPos(Vec3 vPos);
	void SetOffsetScale(Vec3 vScale);

public:
	const Vec3	GetLocalPos();
	const Vec3	GetLocalScale();
	const Vec3	GetLocalRot();
	const  char GetState();

	const Vec3	GetOffsetPos();
	const Vec3	GetOffsetScale();
};