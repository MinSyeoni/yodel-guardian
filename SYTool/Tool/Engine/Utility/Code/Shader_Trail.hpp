#include "Shader_Base.hpp"

float		g_fDetail = 0.f;

struct VS_IN
{
	vector position : POSITION;
	float2 texture_uv : TEXCOORD0;
};

struct VS_OUT
{
	vector position : POSITION;
	float2 texture_uv : TEXCOORD0;
};

struct VS_IN_COLOR
{
	vector position : POSITION;
	float4 color : COLOR;
};

struct VS_OUT_COLOR
{
	vector position : POSITION;
	float4 color : COLOR;
};


VS_OUT_COLOR VS_COLOR(VS_IN_COLOR In)
{
	VS_OUT_COLOR Out = (VS_OUT_COLOR)0;

	matrix mat_transform;

	mat_transform = mul(/*mul(g_matWorld, */g_matView/*)*/, g_matProj);

	Out.position = mul(vector(In.position.xyz, 1.f), mat_transform);
	Out.color = In.color;

	return Out;
}

struct PS_IN
{
	vector color : COLOR0;
};

struct PS_OUT
{
	vector color : COLOR0;
	vector          vNormal : COLOR1;
	vector          vDepth : COLOR2;
	vector         vLIm : COLOR3;
};


PS_OUT PS_COLOR(PS_IN In)
{
	PS_OUT Out = (PS_OUT)0;

	Out.color = In.color;
	
//	Out.color.a = 0.4f;
	Out.vLIm = Out.color;






	return Out;
}



technique Default_Device
{
	pass 
	{
	ALPHABLENDENABLE = TRUE;
	SRCBLEND = SRCALPHA;
	DESTBLEND = INVSRCALPHA;
	CULLMODE = NONE;

	VertexShader = compile vs_3_0 VS_COLOR();
	PixelShader = compile ps_3_0 PS_COLOR();
	}
};