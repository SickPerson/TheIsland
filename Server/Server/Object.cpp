#include "Object.h"


CObject::CObject() :
	m_vLocalPos(Vec3(0.f, 0.f, 0.f)),
	m_vLocalRot(Vec3(0.f, 0.f, 0.f)),
	m_vLocalScale(Vec3(1.f, 1.f, 1.f))
{
}

CObject::~CObject()
{
}

void CObject::SetLocalPos(float& fPosX, float& fPosY, float& fPosZ)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[(UINT)OBJ_LOCK_TYPE::LOCAL_POS]);
	m_vLocalPos = Vec3(fPosX, fPosY, fPosZ);
}

void CObject::SetLocalPos(Vec3 vPos)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[(UINT)OBJ_LOCK_TYPE::LOCAL_POS]);
	m_vLocalPos = vPos;
}

void CObject::SetLocalRot(float& fRotX, float& fRotY, float& fRotZ)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[(UINT)OBJ_LOCK_TYPE::LOCAL_ROT]);
	m_vLocalRot = Vec3(fRotX, fRotY, fRotZ);
}

void CObject::SetLocalRot(Vec3 vRot)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[(UINT)OBJ_LOCK_TYPE::LOCAL_ROT]);
	m_vLocalRot = vRot;
}

void CObject::SetLocalScale(float & fScaleX, float & fScaleY, float & fScaleZ)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[(UINT)OBJ_LOCK_TYPE::LOCAL_SCALE]);
	m_vLocalScale = Vec3(fScaleX, fScaleY, fScaleZ);
}

void CObject::SetLocalScale(Vec3 vScale)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[(UINT)OBJ_LOCK_TYPE::LOCAL_SCALE]);
	m_vLocalScale = vScale;
}

void CObject::SetState(char cState)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[(UINT)OBJ_LOCK_TYPE::STATE]);
	m_cState = cState;
}

const Vec3 CObject::GetLocalPos()
{
	shared_lock<shared_mutex>lock(m_ObjMutex[(UINT)OBJ_LOCK_TYPE::LOCAL_POS]);
	return m_vLocalPos;
}

const Vec3 CObject::GetLocalRot()
{
	shared_lock<shared_mutex>lock(m_ObjMutex[(UINT)OBJ_LOCK_TYPE::LOCAL_ROT]);
	return m_vLocalRot;
}

const Vec3 CObject::GetLocalScale()
{
	shared_lock<shared_mutex> lock(m_ObjMutex[(UINT)OBJ_LOCK_TYPE::LOCAL_SCALE]);
	return m_vLocalScale;
}

const char CObject::GetState()
{
	shared_lock<shared_mutex>lock(m_ObjMutex[(UINT)OBJ_LOCK_TYPE::STATE]);
	return m_cState;
}