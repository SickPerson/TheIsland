#pragma once
#include "Component.h"

class CTransform :
	public CComponent
{
public:
	CTransform();
	virtual ~CTransform();

	CLONE( CTransform );

private:
	Vec3		m_vLocalPos;				// GameObject ��ǥ
	Vec3		m_vLocalScale;				// GameObject ũ��
	Vec3		m_vLocalRot;				// GameObject ȸ����

	Vec3		m_vLocalDir[( UINT )DIR_TYPE::END];		// GameObject�� Local Front, Up, Right ����
	Vec3		m_vWorldDir[( UINT )DIR_TYPE::END];		// GameObject�� World Front, Up, Right ����

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
	float GetMaxScale();

	// Transform ������ ��������� �� �������ͷ� ����
	void UpdateDate();

public:
	virtual void FinalUpdate();

	// CComponent��(��) ���� ��ӵ�
	virtual void SaveToScene( FILE * pFile ) override;
	virtual void LoadFromScene( FILE * pFile ) override;
};

