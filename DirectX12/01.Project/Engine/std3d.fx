#include "value.fx"
#include "func.fx"

#ifndef _STD3D
#define _STD3D

// ==========================
// Std3D Shader
// Deferred = true
//
// g_tex_0 : Diffuse Texture
// g_tex_1 : Normalmap Texture
// BlendState : false
// ==========================
struct VS_STD3D_INPUT
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
    
    float3 vTangent : TANGENT;
    float3 vNormal : NORMAL;
    float3 vBinormal : BINORMAL;
    
    float4 vWeight : BLENDWEIGHT;
    float4 vIndices : BLENDINDICES;
};

struct VS_STD3D_OUTPUT
{
    float4 vPosition : SV_Position;
    
    float3 vViewPos : POSITION;
    
    float3 vViewTangent : TANGENT;
    float3 vViewNormal : NORMAL;
    float3 vViewBinormal : BINORMAL;
    
    float2 vUV : TEXCOORD;
};

VS_STD3D_OUTPUT VS_Std3D(VS_STD3D_INPUT _in)
{
    VS_STD3D_OUTPUT output = (VS_STD3D_OUTPUT) 0.f;
        
    if (g_int_0)
    {
        Skinning(_in.vPos, _in.vTangent
                , _in.vBinormal, _in.vNormal
                , _in.vWeight, _in.vIndices, 0);        
    }
    
    output.vPosition = mul(float4(_in.vPos, 1.f), g_matWVP);
    
    output.vViewPos = mul(float4(_in.vPos, 1.f), g_matWV).xyz;
    output.vViewTangent = normalize(mul(float4(_in.vTangent, 0.f), g_matWV).xyz);
    output.vViewNormal = normalize(mul(float4(_in.vNormal, 0.f), g_matWV).xyz);
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), g_matWV).xyz);
    output.vUV = _in.vUV;
    
    return output;
}

struct PS_STD3D_OUTPUT
{
    float4 vTarget0 : SV_Target0; // Diffuse
    float4 vTarget1 : SV_Target1; // Normal
    float4 vTarget2 : SV_Target2; // Position
};

PS_STD3D_OUTPUT PS_Std3D(VS_STD3D_OUTPUT _in)
{
    PS_STD3D_OUTPUT output = (PS_STD3D_OUTPUT) 0.f;
        
    if (tex_0)
        output.vTarget0 = g_tex_0.Sample(g_sam_0, _in.vUV);
    else
        output.vTarget0 = float4(1.f, 0.f, 1.f, 1.f);
        
    float3 vViewNormal = _in.vViewNormal;
    // 노말맵이 있는경우
    if (tex_1)
    {
        float3 vTSNormal = g_tex_1.Sample(g_sam_0, _in.vUV).xyz;
        vTSNormal.xyz = (vTSNormal.xyz - 0.5f) * 2.f;
        float3x3 matTBN = { _in.vViewTangent, _in.vViewBinormal, _in.vViewNormal };
        vViewNormal = normalize(mul(vTSNormal, matTBN));
    }
    
    output.vTarget1.xyz = vViewNormal;
    output.vTarget2.xyz = _in.vViewPos;
    
    return output;
}


// =============
// Skybox Shader
// mesh         : sphere
// rasterizer   : CULL_FRONT
// DepthStencilState : Less_Equal
// g_tex_0 : Output Texture
// =============
struct VS_SKY_IN
{
    float3 vPos : POSITION;
    float2 vUV : TEXCOORD;
};

struct VS_SKY_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vUV : TEXCOORD;
};

VS_SKY_OUT VS_Skybox(VS_SKY_IN _in)
{
    VS_SKY_OUT output = (VS_SKY_OUT) 0.f; 
    
    float4 vViewPos = mul(float4(_in.vPos, 0.f), g_matView);
    float4 vProjPos = mul(vViewPos, g_matProj);
    
    // w 값으로 z 값을 나눌것이기 때문에 미리 w 값을 셋팅해두면
    // 어떤 상황에서도 깊이값이 1.f 로 판정된다.
    vProjPos.z = vProjPos.w;
    
    output.vPosition = vProjPos;
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Skybox(VS_SKY_OUT _in) : SV_Target
{
    float4 vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);  

    return vOutColor * g_float_0;
}

// =============
// Terrain Shader
// =============

struct VS_LANDSCAPE_IN
{
	float3 vPos			: POSTION;
	float3 vNormal		: NORMAL;
	float2 vUV			: TEXCOORD;
	float3 vTangent		: TANGENT;
	float3 vBinormal	: BINORMAL;
};

struct VS_LANDSCAPE_OUT
{
	float4	vPos		: SV_POSITION;
	float3	vNormal		: NORMAL;
	float2	vUV			: TEXCOORD;
	float3	vTangent	: TANGENT;
	float3	vBinormal	: BINORMAL;
	float3	vViewPos	: POSITION;
	float4	vProjPos	: POSITION1;
};

struct PS_LANDSCAPE_OUT
{
	float4	vColor	: SV_Target;
	float4	vColor1	: SV_Target1;
	float4	vColor2	: SV_Target2;
	float4	vColor3	: SV_Target3;
	float4	vColor4	: SV_Target4;
	float4	vColor5	: SV_Target5;
};

//VS_LANDSCAPE_OUT LandScapeVS( VS_LANDSCAPE_IN input )
//{
//	VS_LANDSCAPE_OUT	output = ( VS_LANDSCAPE_OUT )0;
//
//	float3	vPos = input.vPos;
//
//	output.vProjPos = mul( float4( vPos, 1.f ), g_matWVP );
//	output.vPos = output.vProjPos;
//	// Normal을 뷰공간으로 만들어준다.
//	output.vNormal = normalize( mul( float4( input.vNormal, 0.f ), g_matWV ).xyz );
//	output.vViewPos = mul( float4( vPos, 1.f ), g_matWV ).xyz;
//	output.vTangent = normalize( mul( float4( input.vTangent, 0.f ), g_matWV ).xyz );
//	output.vBinormal = normalize( mul( float4( input.vBinormal, 0.f ), g_matWV ).xyz );
//	output.vUV = input.vUV;
//
//	return output;
//}
//
//PS_LANDSCAPE_OUT LandScapePS( VS_LANDSCAPE_OUT input )
//{
//	PS_LANDSCAPE_OUT	output = ( PS_LANDSCAPE_OUT )0;
//
//	float2	vUV = input.vUV * g_iDetailLevel;
//
//	float4	vColor = g_tex_0.Sample( g_sam_0, vUV );
//
//	if ( vColor.a == 0.f )
//		clip( -1 );
//
//	// Normal을 구해준다.
//	float3x3	mat =
//	{
//		input.vTangent,
//		input.vBinormal,
//		input.vNormal
//	};
//
//	output.vColor4.xyz = input.vTangent * 0.5f + 0.5f;
//	output.vColor4.w = 1.f;
//
//	output.vColor5.xyz = input.vBinormal * 0.5f + 0.5f;
//	output.vColor5.w = 1.f;
//
//	// Normal 텍스쳐에서 픽셀을 얻어와서 법선벡터로 변환한다.
//	// 색상은 0 ~ 1 사이의 값이므로 이 값을 단위벡터의 값인 -1 ~ 1 사이로
//	// 변환해주려면 * 2 - 1 을 해주면 된다.
//	float3	vBumpNormal = g_tex_1.Sample( g_sam_0, vUV ).xyz;
//	vBumpNormal = vBumpNormal * 2.f - 1.f;
//
//	for ( int i = 0; i < g_iSplatCount; i++ )
//	{
//		float3	vSplatUV;
//		vSplatUV.xy = vUV;
//		vSplatUV.z = i;
//		float4	vSplatColor = g_arrtex_0.Sample( g_sam_1, vSplatUV );
//		float3	vSplatNormal = g_arrtex_1.Sample( g_sam_1, vSplatUV ).xyz;
//		vSplatNormal = vSplatNormal * 2.f - 1.f;
//		vBumpNormal += vSplatNormal;
//
//		vSplatUV.xy = input.vUV;
//		float4	vSplatAlpha = g_arrtex_3.Sample( g_sam_1, vSplatUV );
//
//		vColor = ( vColor * ( float4( 1.f, 1.f, 1.f, 1.f ) - vSplatAlpha ) +
//			vSplatColor * vSplatAlpha );
//	}
//
//	vBumpNormal = normalize( vBumpNormal );
//
//	float3	vViewNormal = normalize( mul( vBumpNormal, mat ) );
//
//	output.vColor = vColor;
//	output.vColor1.xyz = vViewNormal * 0.5f + 0.5f;
//	output.vColor1.w = 1.f;
//	output.vColor2.x = input.vProjPos.z / input.vProjPos.w;
//	output.vColor2.w = input.vProjPos.w;
//
//	// 재질 Diffuse, Ambient 값을 저장한다.
//	output.vColor2.y = g_vDiff.x;
//	output.vColor2.z = g_vEmv.x;
//
//	float4	vMtrlSpc;
//	if ( g_vEmv.w == 1 )
//	{
//		vMtrlSpc = g_tex_2.Sample( g_sam_0, vUV );
//		vMtrlSpc.w = g_vSpec.w;
//		output.vColor4.w = vMtrlSpc.w;
//	}
//
//	else
//	{
//		vMtrlSpc = g_vSpec;
//		output.vColor4.w = g_vSpec.w;
//	}
//	output.vColor3 = vMtrlSpc;
//
//	return output;
//}

VS_STD3D_OUTPUT LandScapeVS( VS_STD3D_INPUT input )
{
	VS_STD3D_OUTPUT	output = ( VS_STD3D_OUTPUT )0;

	float3	vPos = input.vPos;

	output.vPosition = mul( float4( vPos, 1.f ), g_matWVP );
	output.vViewPos = mul( float4( vPos, 1.f ), g_matWV ).xyz;
	output.vViewNormal = normalize( mul( float4( input.vNormal, 0.f ), g_matWV ).xyz );
	output.vViewTangent = normalize( mul( float4( input.vTangent, 0.f ), g_matWV ).xyz );
	output.vViewBinormal = normalize( mul( float4( input.vBinormal, 0.f ), g_matWV ).xyz );
	output.vUV = input.vUV;

	return output;
}

#endif