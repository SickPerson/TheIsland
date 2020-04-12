#pragma once
#include "stdafx.h"

class CObject
{
private:
	Object m_Obj;
	shared_mutex m_ObjMutex[OBJ_END];

public:
	CObject();
	~CObject();

protected:
	void Init();

public:
	void SetHP(int iHP);
	void SetStamina(int iStamina);
	void SetHunger(int iHunger);
	void SetThirst(int iThirst);

	void SetPos(float _px, float _py, float _pz);
	void SetPos(const DirectX::XMFLOAT3& _pos);
	void SetDir(float _dx, float _dy, float _dz);
	void SetDir(const DirectX::XMFLOAT3& _dir);
	void SetAniTime();

public:
	const int GetHP();
	const int GetStamina();
	const int GetHunger();
	const int GetThirst();
	const DirectX::XMFLOAT3&	GetPos();
	const DirectX::XMFLOAT3&	GetDir();

};

