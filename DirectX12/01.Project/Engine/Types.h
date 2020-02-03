#pragma once

typedef struct tResolution
{
	float fWidth;
	float fHeight;
}RESOLUTION;

// 정점 구조체
struct VTX
{
	Vec3 vPos;
	Vec4 vColor;
	Vec2 vUV;

	Vec3 vTangent;
	Vec3 vNormal;
	Vec3 vBinormal;
};

// 상수버퍼 메모리
struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWV;
	Matrix matWVP;
};

// Material
struct tMtrlParam
{
	Vec4			m_vDiff;	// 색상 계수
	Vec4			m_vSpec;	// 반사광 계수
	Vec4			m_vEmv;		// 발광 계수

	int				m_arrInt[( UINT )SHADER_PARAM::INT_END - ( UINT )SHADER_PARAM::INT_0];
	float			m_arrFloat[( UINT )SHADER_PARAM::FLOAT_END - ( UINT )SHADER_PARAM::FLOAT_0];
	Vec2			m_arrVec2[( UINT )SHADER_PARAM::VEC2_END - ( UINT )SHADER_PARAM::VEC2_0];
	Vec4			m_arrVec4[( UINT )SHADER_PARAM::VEC4_END - ( UINT )SHADER_PARAM::VEC4_0];
	Matrix			m_arrMat[( UINT )SHADER_PARAM::MATRIX_END - ( UINT )SHADER_PARAM::MATRIX_0];

	int				m_iArrTex[( UINT )SHADER_PARAM::TEX_END - ( UINT )SHADER_PARAM::TEX_0];
};
