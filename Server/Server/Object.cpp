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
	m_Obj.iHp = 100;
	m_Obj.iHunger = 100;
	m_Obj.iStamina = 100;
	m_Obj.iThirst = 100;

	m_Obj.fDirX = 0.f;
	m_Obj.fDirY = 0.f;
	m_Obj.fDirZ = 0.f;

	m_Obj.fPosX = 0.f;
	m_Obj.fPosY = 25.f;
	m_Obj.fPosZ = 0.f;

	m_Obj.m_tStartTime = chrono::high_resolution_clock::now();
}

void CObject::SetHP(int iHP)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_HP]);
	m_Obj.iHp = iHP;
}

void CObject::SetStamina(int iStamina)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_STAMINA]);
	m_Obj.iStamina = iStamina;
}

void CObject::SetHunger(int iHunger)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_HUNGER]);
	m_Obj.iHunger = iHunger;
}

void CObject::SetThirst(int iThirst)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_THIRST]);
	m_Obj.iThirst = iThirst;
}

void CObject::SetPos(float _px, float _py, float _pz)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_POS]);
	m_Obj.fPosX = _px;
	m_Obj.fPosY = _py;
	m_Obj.fPosZ = _pz;
}

void CObject::SetPos(const DirectX::XMFLOAT3 & _pos)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_POS]);
	m_Obj.fPosX = _pos.x;
	m_Obj.fPosY = _pos.y;
	m_Obj.fPosZ = _pos.z;
}

void CObject::SetDir(float _dx, float _dy, float _dz)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_DIR]);
	m_Obj.fDirX = _dx;
	m_Obj.fDirY = _dy;
	m_Obj.fDirZ = _dz;
}

void CObject::SetDir(const DirectX::XMFLOAT3 & _dir)
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_DIR]);
	m_Obj.fDirX = _dir.x;
	m_Obj.fDirY = _dir.y;
	m_Obj.fDirZ = _dir.z;
}

void CObject::SetAniTime()
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_ANITIME]);
	m_Obj.m_tStartTime = high_resolution_clock::now();
}

const int CObject::GetHP()
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_HP]);
	return m_Obj.iHp;
}

const int CObject::GetStamina()
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_STAMINA]);
	return m_Obj.iStamina;
}

const int CObject::GetHunger()
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_HUNGER]);
	return m_Obj.iHunger;
}

const int CObject::GetThirst()
{
	unique_lock<shared_mutex>lock(m_ObjMutex[OBJ_THIRST]);
	return m_Obj.iThirst;
}

const DirectX::XMFLOAT3 & CObject::GetPos()
{
	shared_lock<shared_mutex>lock(m_ObjMutex[OBJ_POS]);
	DirectX::XMFLOAT3 xmfPos = DirectX::XMFLOAT3(m_Obj.fPosX, m_Obj.fPosY, m_Obj.fPosZ);
	return xmfPos;
	// TODO: 여기에 반환 구문을 삽입합니다.
}

const DirectX::XMFLOAT3 & CObject::GetDir()
{
	shared_lock<shared_mutex>lock(m_ObjMutex[OBJ_DIR]);
	DirectX::XMFLOAT3 xmfDir = DirectX::XMFLOAT3(m_Obj.fDirX, m_Obj.fDirX, m_Obj.fDirZ);
	return xmfDir;
	// TODO: 여기에 반환 구문을 삽입합니다.
}
