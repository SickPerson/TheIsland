#pragma once
#include "Component.h"

class CTransform :
	public CComponent
{
private:
	CTransform();
	virtual ~CTransform();

	CLONE( CTransform );

private:
	Vec3		m_vLocalPos;				// GameObject 좌표
	Vec3		m_vLocalScale;				// GameObject 크기
	Vec3		m_vLocalRot;				// GameObject 회전량

	Vec3		m_vLocalDir[( UINT )DIR_TYPE::END];		// GameObject의 Local Front, Up, Right 방향
	Vec3		m_vWorldDir[( UINT )DIR_TYPE::END];		// GameObject의 World Front, Up, Right 방향

	Matrix		m_matWorld;

public:
	const Vec3& GetLocalPos();
	Vec3 GetWorldPos();

	
	const Vec3& GetLocalScale();
	Vec3 GetWorldScale();

	const Vec3& GetLocalRot();

	const Matrix& GetWorldMat();

	const Vec3& GetLocalDir( DIR_TYPE eType );
	const Vec3& GetWorldDir( DIR_TYPE eType );

	void SetLocalPos( const Vec3& vPos );
	void SetLocalScale( const Vec3& vScale );
	void SetLocalRot( const Vec3& vRot );

	bool IsCasting( const Vec3& vPos );
	float GetMatScale();

	// Transform 정보를 상수데이터 및 레지스터로 전달
	void UpdateDate();

public:
	virtual void FinalUpdate();

	// CComponent을(를) 통해 상속됨
	virtual void SaveToScene( FILE * pFile ) override;
	virtual void LoadFromScene( FILE * pFile ) override;
};

