#include "value.fx"
#include "func.fx"

#ifndef _FONT
#define _FONT

struct FONT_INPUT
{
	float3 vPos : POSITION;
	float2 vUV : TEXCOORD;
};

struct FONT_OUTPUT
{
	float4 vOutPos : SV_Position;
	float2 vUV : TEXCOORD;
};

struct tFontInfo
{
	float2 vStartUV;
	float2 vWidthUV;
};

StructuredBuffer<tFontInfo>   g_arrFontInfo : register(t10);

// =======================
// Font Shader
// =======================
FONT_OUTPUT VS_Font(FONT_INPUT _input)
{
	FONT_OUTPUT output = (FONT_OUTPUT)0;

	// 투영좌표계를 반환할 때에는 float4 4번째 w 요소에 1.f 을 넣어준다.
	float4 vWorldPos = mul(float4(_input.vPos, 1.f), g_matWorld);
	float4 vViewPos = mul(vWorldPos, g_matView);
	float4 vProjPos = mul(vViewPos, g_matProj);

	output.vOutPos = vProjPos;
	output.vUV = _input.vUV;

	return output;
}
// 0일떄는 start
// 1일때는 start + width
float4 PS_Font(FONT_OUTPUT _input) : SV_Target
{
	float4 vColor = (float4) 0.f;

	int nStrSize = g_int_0;
	float2 vUV = _input.vUV;

	// g_float_0 : Start U
	// g_float_1 : Width U
	// g_float_2 : Start V
	// g_float_3 : Height V
	
	// 0.333 = 0
	// 0.666 = 1

	// 0.666 = 0
	// 0.999 = 1

	for (int i = 0; i < nStrSize; ++i)
	{
		if (_input.vUV.x < (float(i + 1) / (float)nStrSize) && _input.vUV.x > (float(i) / (float)nStrSize))
		{
			_input.vUV.x = (_input.vUV.x * (float)nStrSize - i) * g_arrFontInfo[i].vWidthUV.x + g_arrFontInfo[i].vStartUV.x;
			_input.vUV.y = _input.vUV.y * g_arrFontInfo[i].vWidthUV.y + g_arrFontInfo[i].vStartUV.y;
			break;
		}
	}


	/*if (_input.vUV.x < 1.f / 3.f)
	{
		_input.vUV.x = (_input.vUV.x * 3.f) * g_arrFontInfo[0].vWidthUV.x + g_arrFontInfo[0].vStartUV.x;
		_input.vUV.y = _input.vUV.y * g_arrFontInfo[0].vWidthUV.y + g_arrFontInfo[0].vStartUV.y;
	}
	else if (_input.vUV.x < (1.f / 3.f) * 2.f)
	{
		_input.vUV.x = (_input.vUV.x * 3.f - 1.f) * g_arrFontInfo[1].vWidthUV.x + g_arrFontInfo[1].vStartUV.x;
		_input.vUV.y = _input.vUV.y * g_arrFontInfo[1].vWidthUV.y + g_arrFontInfo[1].vStartUV.y;
	}
	else
	{
		_input.vUV.x = (_input.vUV.x * 3.f - 2.f) * g_arrFontInfo[2].vWidthUV.x + g_arrFontInfo[2].vStartUV.x;
		_input.vUV.y = _input.vUV.y * g_arrFontInfo[2].vWidthUV.y + g_arrFontInfo[2].vStartUV.y;
	}*/

	vColor = g_tex_0.Sample(g_sam_1, _input.vUV);

	vColor = vColor * g_vec4_0;

	if (vColor.w < 0.5f)
		vColor = g_vec4_1;

	return vColor;
}

#endif