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

// Transform extern
extern tTransform	g_tTransform;

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

// Light
struct tLight2D
{
	Vec4	vLightPos;
	Vec4	vLightColor;
	Vec4	vLightDir;
	int		iLightType;
	float	fRange;
	float	fAngle;
	int		iPadding;
};

struct tLight2DInfo
{
	tLight2D	arrLight2D[100];
	UINT		iCount;
};

struct tLightColor
{
	Vec4 vDiff;
	Vec4 vSpec;
	Vec4 vAmb;
};

struct tLight3D
{
	tLightColor	tColor;
	Vec4		vLightPos;
	Vec4		vLightDir;
	int			iLightType;
	float		fRange;
	float		fAngle;
	int			iPadding;
};

struct tLight3DInfo
{
	tLight3D	arrLight3D[100];
	UINT		iCurCount;
	Vec3		vPadding;
};

struct tAnimation2D
{
	Vec2	vLT;				// Left Top UV
	Vec2	vLen;				// UV길이
	Vec2	vOffset;			// Offset Pos
	int		IsAnimation2D;		// Animation2D 컴포넌트 사용여부
	int		iPadding;
};