#include "Shader_Base.hpp"

texture g_DepthTexture;

sampler  DepthSampler = sampler_state
{
texture = g_DepthTexture;

minfilter = linear;
magfilter = linear;
};

// ¾Æ¿À ½¬¹ß Á¿°°´Ù 


struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
	vector      vDepth : COLOR2;
};

struct PS_OUT
{
	vector vColor : COLOR0;

};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = vector(In.vDepth.rrr , 1.f);



	return Out;
}

technique Default_Device
{
	pass    // ±â´ÉÀÇ Ä¸½¶È­
{

	vertexshader =  NULL;
     pixelshader = compile ps_3_0 PS_MAIN();
}

};