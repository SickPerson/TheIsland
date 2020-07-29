#pragma once
#include <Engine/Script.h>

enum HOUSING_TYPE {

	HOUSING_FOUNDATION,
	HOUSING_WALL,
	HOUSING_DOOR,
	HOUSING_WINDOW,
	HOUSING_FLOOR,

	HOUSING_END,

	HOUSING_ETC
};

class CBuildScript :
	public CScript
{
	bool m_bBuild;
	bool m_bCollision;

	HOUSING_TYPE m_eType;
public:
	CBuildScript(HOUSING_TYPE eType);
	virtual ~CBuildScript();

public:
	virtual void Update();
	CLONE(CBuildScript);

	virtual void OnCollision(CCollider2D* _pOther) override;
	virtual void OnCollisionEnter(CCollider2D* _pOther) override;
	virtual void OnCollisionExit(CCollider2D* _pOther) override;

public:
	void Init();
	bool Build();

	HOUSING_TYPE GetHousingType();
	Vec3 GetOffsetScale();
};

