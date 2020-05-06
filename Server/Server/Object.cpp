#include "Object.h"


CObject::CObject()
{
	Init();
}

CObject::~CObject()
{
}

void CObject::Init()
{
	m_iHp = 100;
	m_iHunger = 100;
	m_iStamina = 100;
	m_iThirst = 100;

	m_fDirX = 0.f;
	m_fDirY = 0.f;
	m_fDirZ = 0.f;

	m_fPosX = 0.f;
	m_fPosY = 25.f;
	m_fPosZ = 0.f;
}

void CObject::SetHP(int iHP)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_HP]);
	m_iHp = iHP;
}

void CObject::SetStamina(int iStamina)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_STAMINA]);
	m_iStamina = iStamina;
}

void CObject::SetHunger(int iHunger)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_HUNGER]);
	m_iHunger = iHunger;
}

void CObject::SetThirst(int iThirst)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_THIRST]);
	m_iThirst = iThirst;
}

void CObject::SetPos(float _px, float _py, float _pz)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_POS]);
	m_fPosX = _px;
	m_fPosY = _py;
	m_fPosZ = _pz;
}

void CObject::SetPos(const DirectX::XMFLOAT3 & _pos)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_POS]);
	m_fPosX = _pos.x;
	m_fPosY = _pos.y;
	m_fPosZ = _pos.z;
}

void CObject::SetDir(float _dx, float _dy, float _dz)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_DIR]);
	m_fDirX = _dx;
	m_fDirY = _dy;
	m_fDirZ = _dz;
}

void CObject::SetDir(const DirectX::XMFLOAT3 & _dir)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_DIR]);
	m_fDirX = _dir.x;
	m_fDirY = _dir.y;
	m_fDirZ = _dir.z;
}

void CObject::SetAniTime()
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_ANITIME]);
	//m_Obj.m_tStartTime = high_resolution_clock::now();
}

const int CObject::GetHP()
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_HP]);
	return m_iHp;
}

const int CObject::GetStamina()
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_STAMINA]);
	return m_iStamina;
}

const int CObject::GetHunger()
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_HUNGER]);
	return m_iHunger;
}

const int CObject::GetThirst()
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_THIRST]);
	return m_iThirst;
}

const DirectX::XMFLOAT3 & CObject::GetPos()
{
	shared_lock<shared_mutex>lock(m_ObjMutex[OBJ_POS]);
	DirectX::XMFLOAT3 xmfPos = DirectX::XMFLOAT3(m_fPosX, m_fPosY, m_fPosZ);
	return xmfPos;
}

const DirectX::XMFLOAT3 & CObject::GetDir()
{
	shared_lock<shared_mutex>lock(m_ObjMutex[OBJ_DIR]);
	DirectX::XMFLOAT3 xmfDir = DirectX::XMFLOAT3(m_fDirX, m_fDirX, m_fDirZ);
	return xmfDir;
}
