#include "Object.h"


CObject::CObject() :
	m_vPos(Vec3(0.f, 25.f, 0.f)),
	m_vRot(Vec3(0.f, 180.f, 0.f))
{
}

CObject::~CObject()
{
}

void CObject::SetPos(const float& fPosX, const float& fPosY, const float& fPosZ)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_LOCK_POS]);
	m_vPos = Vec3(fPosX, fPosY, fPosZ);
}

void CObject::SetPos(const Vec3& vPos)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_LOCK_POS]);
	m_vPos = vPos;
}

void CObject::SetRot(const float& fRotX, const float& fRotY, const float& fRotZ)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_LOCK_ROT]);
	m_vRot = Vec3(fRotX, fRotY, fRotZ);
}

void CObject::SetRot(const Vec3& vRot)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_LOCK_ROT]);
	m_vRot = vRot;
}

void CObject::SetState(const char& cState)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_LOCK_STATE]);
	m_cState = cState;
}

const Vec3& CObject::GetPos()
{
	shared_lock<shared_mutex>lock(m_ObjMutex[OBJ_LOCK_POS]);
	return m_vPos;
}

const Vec3& CObject::GetRot() 
{
	shared_lock<shared_mutex>lock(m_ObjMutex[OBJ_LOCK_ROT]);
	return m_vRot;
}

const char & CObject::GetState()
{
	shared_lock<shared_mutex>lock(m_ObjMutex[OBJ_LOCK_STATE]);
	return m_cState;
}