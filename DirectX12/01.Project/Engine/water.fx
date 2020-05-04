#ifndef _WATER
#define _WATER

#include "value.fx"
#include "func.fx"

//========================
// Water Shader
// AlphaBlend = false
// PostEffect = true
// g_Tex_0 : Posteffect Texture
// g_Tex_1 : Water Texture
// g_Tex_2 : Position Texture
//========================
struct VS_WATER_INPUT
{
	float3 vPos : POSITION;
	float2 vUV : TEXCOORD;
};

struct VS_WATER_OUTPUT
{
	float4 vOutPos : SV_Position;
	float2 vOutUV : TEXCOORD;
};

VS_WATER_OUTPUT VS_Water(VS_WATER_INPUT _input)
{
	// input 으로 들어온 위치정보를 투영좌표계 까지 변환한다.
	VS_WATER_OUTPUT output = (VS_WATER_OUTPUT) 0.f;

	float height = cos(g_fAccTime * 0.6f + _input.vUV.x * 1.f) * 50.f;

	_input.vPos.z -= height;

	output.vOutPos = mul(float4(_input.vPos, 1.f), g_matWVP);

	output.vOutUV = _input.vUV;

	//output.vOutUV = _input.vUV + float2(g_fAccTime * 0.006f, 0);

	return output;
}

struct PS_STD3D_OUT
{
	float4 vDiffuse : SV_Target0;
	float4 vNormal : SV_Target1;
	float4 vPosition : SV_Target2;
};

float4 PS_Water(VS_WATER_OUTPUT _input) : SV_Target
{
	float2 vScreenUV = float2(_input.vOutPos.x / g_vResolution.x, _input.vOutPos.y / g_vResolution.y);
	//return g_tex_0.Sample(g_sam_0, vScreenUV);


	float2 vDir = normalize(float2(0.5f, 0.5f) - _input.vOutUV);

	float fDist = distance(float2(0.5f, 0.5f), _input.vOutUV);

	// 왜곡 강도에 영향을 주는 중심으로부터 떨어진 비율( 중심에 가까울 수록 0에 가깝다.)
	float fRatio = (fDist / 0.5f);
	fRatio = fRatio * (0.2f + (fRatio) * 0.4f);

	vScreenUV += vDir * sin(abs((fRatio - g_fAccTime * 0.06f) * 80.f)) * 0.03f;

	PS_STD3D_OUT output = (PS_STD3D_OUT) 0.f;
	output.vDiffuse = g_tex_0.Sample(g_sam_0, vScreenUV);

	float4 color = g_tex_0.Sample(g_sam_0, vScreenUV);

	//float4 water = g_tex_1.Sample(g_sam_0, _input.vOutUV);
	float4 water = g_tex_1.Sample(g_sam_0, vScreenUV);

	float4 water_detail = g_tex_3.Sample(g_sam_0, vScreenUV);

	float blend = 0.8f;

	float4 final_water = (water * blend) + ((1 - blend) * water_detail);

	float4 pos = g_tex_2.Sample(g_sam_0, vScreenUV);
	float dist = pos.z;

	if (dist == 0.f)
	{
		return color * final_water;
	}
	else
	{
		//float a = 20000.f;
		//return float4(dist / a, dist / a, dist / a, dist);
		dist = dist / 10000.f;
		if (dist > 0.8f)
		{
			return final_water;
		}
		water *= ((1 - dist) + 0.2f);
	}

	//color *= 0.5f;
	//color.b += 0.25f;

	return color * final_water;
}


// Tessellation Test

struct HS_TEST_INPUT
{
	float3 vPos : POSITION;
	float2 vUV : TEXCOORD;
};

struct CONSTANT_OUTPUT
{
	float edges[3] : SV_TessFactor;
	float inside : SV_InsideTessFactor;
};

struct HS_TEST_OUTPUT
{
	float3 vPos : POSITION;
	float2 vUV : TEXCOORD;
};

HS_TEST_INPUT VS_TestWater(VS_WATER_INPUT _input)
{
	// input 으로 들어온 위치정보를 투영좌표계 까지 변환한다.
	HS_TEST_INPUT output = (HS_TEST_INPUT) 0.f;

	output.vPos = _input.vPos;
	output.vUV = _input.vUV;

	return output;
}

CONSTANT_OUTPUT Patch_Constant_Func(InputPatch<HS_TEST_INPUT, 3> input, uint patchID : SV_PrimitiveID)
{
	CONSTANT_OUTPUT output;

	// g_float_0 = tessllationAmout
	output.edges[0] = g_float_0;
	output.edges[1] = g_float_0;
	output.edges[2] = g_float_0;

	output.inside = g_float_0;

	return output;
}

// Hull Shader
[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("Patch_Constant_Func")]
HS_TEST_OUTPUT HS_TestWater(InputPatch<HS_TEST_INPUT, 3> patch, uint pointID : SV_OutputControlPointID, uint patchID : SV_PrimitiveID)
{
	HS_TEST_OUTPUT output;

	output.vPos = patch[pointID].vPos;

	output.vUV = patch[pointID].vUV;

	return output;
}

struct PS_TEST_INPUT
{
	float4 vOutPos : SV_Position;
	float2 vOutUV : TEXCOORD;
};

// Domain Shader
[domain("tri")]
PS_TEST_INPUT DS_TestWater(CONSTANT_OUTPUT input, float3 uvwCoord : SV_DomainLocation, const OutputPatch<HS_TEST_OUTPUT, 3> patch)
{
	float3 vertexPos;
	PS_TEST_INPUT output;

	vertexPos = uvwCoord.x * patch[0].vPos + uvwCoord.y * patch[1].vPos + uvwCoord.z * patch[2].vPos;
	output.vOutUV = patch[0].vUV * uvwCoord.x + patch[1].vUV * uvwCoord.y + patch[2].vUV * uvwCoord.z;

	float height = cos(g_fAccTime * 1.f + output.vOutUV.x * 10.f) * 50.f;

	vertexPos.z += height;

	output.vOutPos = mul(float4(vertexPos, 1.f), g_matWVP);

	return output;

	////output.vOutUV = _input.vUV + float2(g_fAccTime * 0.006f, 0);

	//return output;
}

float4 PS_TestWater(PS_TEST_INPUT _input) : SV_Target
{
	float2 vScreenUV = float2(_input.vOutPos.x / g_vResolution.x, _input.vOutPos.y / g_vResolution.y);
	//return g_tex_0.Sample(g_sam_0, vScreenUV);


	float2 vDir = normalize(float2(0.5f, 0.5f) - _input.vOutUV);

	float fDist = distance(float2(0.5f, 0.5f), _input.vOutUV);

	// 왜곡 강도에 영향을 주는 중심으로부터 떨어진 비율( 중심에 가까울 수록 0에 가깝다.)
	float fRatio = (fDist / 0.5f);
	fRatio = fRatio * (0.2f + (fRatio) * 0.4f);

	vScreenUV += vDir * sin(abs((fRatio - g_fAccTime * 0.06f) * 80.f)) * 0.03f;

	PS_STD3D_OUT output = (PS_STD3D_OUT) 0.f;
	output.vDiffuse = g_tex_0.Sample(g_sam_0, vScreenUV);

	float4 color = g_tex_0.Sample(g_sam_0, vScreenUV);

	//float4 water = g_tex_1.Sample(g_sam_0, _input.vOutUV);
	float4 water = g_tex_1.Sample(g_sam_0, vScreenUV);

	float4 water_detail = g_tex_3.Sample(g_sam_0, vScreenUV);

	float4 pos = g_tex_2.Sample(g_sam_0, vScreenUV);
	float dist = pos.z;

	if (dist == 0.f)
	{
		return color * water;
	}
	else
	{
		//float a = 20000.f;
		//return float4(dist / a, dist / a, dist / a, dist);
		dist = dist / 15000.f;
		if (dist > 0.8f)
		{
			return water;
		}
		water *= ((1 - dist) + 0.2f);
	}

	//color *= 0.5f;
	//color.b += 0.25f;

	return color * water;
}

#endif