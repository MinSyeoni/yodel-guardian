
#include "Shader_Base.hpp"



texture g_AlphaTexture1;
texture g_AlphaTexture2;
texture g_AlphaTexture3;

texture g_NormalTexture1;
texture g_NormalTexture2;
texture g_NormalTexture3;
texture g_NormalTexture4;

sampler  NomalSampler1 = sampler_state
{
	texture = g_NormalTexture1;

minfilter = linear;
magfilter = linear;
};
sampler  NomalSampler2 = sampler_state
{
	texture = g_NormalTexture2;

minfilter = linear;
magfilter = linear;
};
sampler  NomalSampler3 = sampler_state
{
	texture = g_NormalTexture3;

minfilter = linear;
magfilter = linear;
};
sampler  NomalSampler4 = sampler_state
{
	texture = g_NormalTexture4;

minfilter = linear;
magfilter = linear;
};


sampler  BaseSampler1 = sampler_state
{
	texture = g_AlphaTexture1;

minfilter = linear;
magfilter = linear;
};

sampler  BaseSampler2 = sampler_state
{
	texture = g_AlphaTexture2;

minfilter = linear;
magfilter = linear;
};
sampler  BaseSampler3 = sampler_state
{
	texture = g_AlphaTexture3;

minfilter = linear;
magfilter = linear;
};

texture g_FilterTexture;

sampler  FilterSampler = sampler_state
{
	texture = g_FilterTexture;

minfilter = linear;
magfilter = linear;
};


float  g_fDetail = 0.f;
float  g_fPower = 1.f;









struct VS_IN
{
	vector	 vPosition : POSITION;
	vector   vNormal : NORMAL;
	vector   vTexUV : TEXCOORD0;



};

struct VS_OUT
{
	vector	vPosition : POSITION;
	vector	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	vector  vDepth : TEXCOORD1;
	float3   vT : TEXCOORD2;
	float3   vB : TEXCOORD3;
	float3   vN : TEXCOORD4;

};

VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));


	Out.vTexUV = In.vTexUV;

	Out.vDepth = Out.vPosition;


	float3 worldNormal = mul(In.vNormal, (float3x3)g_matWorld);
	Out.vN = normalize(worldNormal);

	float3 vTangent = cross((float3)In.vNormal, float3(0.f, 1.f, 0.f));

	float3 worldTangent = mul(vTangent, (float3x3)g_matWorld);
	Out.vT = normalize(worldTangent);

	float3 vBinormal = cross((float3)In.vNormal, vTangent);

	float3 worldBinormal = mul(vBinormal, (float3x3)g_matWorld);
	Out.vB = normalize(worldBinormal);

	return Out;
}

struct PS_IN
{
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	vector  vDepth : TEXCOORD1;
	float3   vT : TEXCOORD2;
	float3   vB : TEXCOORD3;
	float3   vN : TEXCOORD4;


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


	vector vColor[4];
	vector vColorN[4];

	vColor[0] = tex2D(BaseSampler, In.vTexUV * g_fDetail);
	vColor[1] = tex2D(BaseSampler1, In.vTexUV * g_fDetail);
	vColor[2] = tex2D(BaseSampler2, In.vTexUV * g_fDetail);
	vColor[3] = tex2D(BaseSampler3, In.vTexUV * g_fDetail);


	vColorN[0] = tex2D(NomalSampler1, In.vTexUV * g_fDetail);
	vColorN[1] = tex2D(NomalSampler2, In.vTexUV * g_fDetail);
	vColorN[2] = tex2D(NomalSampler3, In.vTexUV * g_fDetail);
	vColorN[3] = tex2D(NomalSampler4, In.vTexUV * g_fDetail);

	



	Out.vDepth = vector(In.vDepth.z / In.vDepth.w, In.vDepth.w / 300.f, 0.f, 0.f);


	vector vFilter = tex2D(FilterSampler, In.vTexUV);
	Out.vColor = (vColor[0] * (1.f - vFilter.a) + vColor[1] * (1.f - vFilter.r) + vColor[2] * (1.f - vFilter.g) + vColor[3] * (1.f - vFilter.b));

vector Normal = (vColorN[0] * (1.f - vFilter.a) + vColorN[1] * (1.f - vFilter.r) + vColorN[2] * (1.f - vFilter.g) + vColorN[3] * (1.f - vFilter.b));
Out.vNormal = Normal;

	return Out;
}

technique Default_Device
{
	pass    // ±â´ÉÀÇ Ä¸½¶È­
{

	vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_MAIN();
}

};



