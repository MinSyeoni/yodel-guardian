#include "Shader_Base.hpp"

texture		g_NoiseTexture;


sampler  NoiseSampler = sampler_state
{
	texture = g_NoiseTexture;

minfilter = linear;
magfilter = linear;
};


float  g_fPower = 1.f;
float  g_fTime;
float  g_fSpeed = 5.f;

vector	g_vLightDir = (vector)0.f;
vector  g_vCamPos;

vector  g_vLightDiffuse;
vector  g_vLightAmbient;
vector	g_vLightSpecular;

vector  g_vMtrlDiffuse;
vector  g_vMtrlAmbient;
vector  g_vMtrlSpecular;
vector  g_vPos;

struct VS_IN
{
	vector	 vPosition : POSITION;
	vector   vNormal : NORMAL;
	vector   vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	vector	vPosition : POSITION;
	vector  vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	vector  vDepth : TEXCOORD1;
};

VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);


	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));
	Out.vDepth = Out.vPosition;
	Out.vTexUV = In.vTexUV;

	return Out;
}

struct PS_IN
{
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	vector  vDepth : TEXCOORD1;
};

struct PS_OUT
{
	vector vColor : COLOR0;
	vector vNormal : COLOR1;
	vector vDepth : COLOR2;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;


	Out.vColor = tex2D(BaseSampler, In.vTexUV);

	Out.vNormal = vector(In.vNormal.xyz * 0.5 + 0.5f, 0.f);
	Out.vDepth.r = In.vDepth.z;

	return Out;
}

PS_OUT			PS_MAIN_ALPHA(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	float vNoise = tex2D(NoiseSampler, In.vTexUV);

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	vNoise = vNoise*((g_fSpeed - g_fTime));

	Out.vColor *= vNoise;


	Out.vNormal = vector(In.vNormal.xyz * 0.5 + 0.5f, 0.f);
	Out.vDepth.r = In.vDepth.z;



	return Out;
}


technique Default_Device
{
	pass    // ±â´ÉÀÇ Ä¸½¶È­
{
	alphatestenable = true;
alphafunc = Greater;
alpharef = 0xc0;




	vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_MAIN();
}

pass alphatest
{
	alphatestenable = true;
alphafunc = Greater;
alpharef = 0xc0;
cullmode = none;

vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_MAIN_ALPHA();
}


};