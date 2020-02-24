#include "Shader_Base.hpp"

texture g_AlphaTexture1;
texture g_AlphaTexture2;
texture g_AlphaTexture3;

texture g_BrushTexture;



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

sampler  BrushSampler = sampler_state
{
	texture = g_BrushTexture;

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
float  g_fRange;
bool  g_DrawBrush;


vector g_vBrushPos;
vector	g_vLightDir = (vector)0.f;
vector  g_vCamPos;

vector  g_vLightDiffuse;
vector  g_vLightAmbient;

vector	g_vLightSpecular;

vector  g_vMtrlDiffuse;
vector  g_vMtrlAmbient;

vector  g_vMtrlSpecular;

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
	vector	vWorldPos : TEXCOORD1;
};

VS_OUT		VS_MAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);

	Out.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));

	vector		vWorldPos = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);

	Out.vTexUV = In.vTexUV;

	Out.vWorldPos = vWorldPos;

	return Out;
}

struct PS_IN
{
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	vector		vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
	vector vColor : COLOR0;
	vector vNormal : COLOR1;

};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	vector vColor[4];

	vColor[0] = tex2D(BaseSampler, In.vTexUV * g_fDetail);
	vColor[1] = tex2D(BaseSampler1, In.vTexUV * g_fDetail);
	vColor[2] = tex2D(BaseSampler2, In.vTexUV * g_fDetail);
	vColor[3] = tex2D(BaseSampler3, In.vTexUV * g_fDetail);


	vector vFilter = tex2D(FilterSampler, In.vTexUV);



	Out.vColor = (vColor[0] * (1.f-vFilter.a) + vColor[1] * (1.f - vFilter.r)+ vColor[2] * (1.f - vFilter.g)+vColor[3]*(1.f-vFilter.b));

	if (g_DrawBrush == true)
	{
		vector		vBrushColor = (vector)0.f;

		if (g_vBrushPos.x - g_fRange < In.vWorldPos.x &&
			g_vBrushPos.x + g_fRange > In.vWorldPos.x &&
			g_vBrushPos.z - g_fRange < In.vWorldPos.z &&
			g_vBrushPos.z + g_fRange > In.vWorldPos.z)
		{
			float2			vTexUV;

			vTexUV.x = (In.vWorldPos.x - (g_vBrushPos.x - g_fRange)) / (g_fRange * 2.f);
			vTexUV.y = ((g_vBrushPos.z + g_fRange) - In.vWorldPos.z) / (g_fRange * 2.f);

			vBrushColor = tex2D(BrushSampler, vTexUV);
		}

		Out.vColor += vBrushColor;

	}
	
	Out.vNormal = vector(In.vNormal.xyz * 0.5f + 0.5f, 0.f);
	return Out;
}

technique Default_Deviceasadwa
{
	pass    // ±â´ÉÀÇ Ä¸½¶È­
{

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
pixelshader = compile ps_3_0 PS_MAIN();
}

};