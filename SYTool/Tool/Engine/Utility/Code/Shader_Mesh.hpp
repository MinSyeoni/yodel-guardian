#include "Shader_Base.hpp"







vector  g_vPos;
vector g_vCamDir;

float  g_fTime;
float  g_fSpeed = 5.f;
texture				g_NormalTexture;

sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;
};


texture		g_NoiseTexture;
sampler  NoiseSampler = sampler_state
{
	texture = g_NoiseTexture;

minfilter = linear;
magfilter = linear;
};


struct VS_IN
{
	vector	 vPosition : POSITION;
	vector   vNormal : NORMAL;
	float2   vTexUV : TEXCOORD0;
};




struct VS_OUT
{
	vector	vPosition : POSITION;
	vector  vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	vector  vDepth : TEXCOORD1;
	vector vLim : TEXCOORD2;

	float3   vT : TEXCOORD3;
	float3   vB : TEXCOORD4;
	float3   vN : TEXCOORD5;
	
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




	float3 worldNormal = mul(In.vNormal, (float3x3)g_matWorld);
	Out.vN = normalize(worldNormal);

	float3 vTangent = cross((float3)In.vNormal, float3(0.f, 1.f, 0.f));

	float3 worldTangent = mul(vTangent, (float3x3)g_matWorld);
	Out.vT = normalize(worldTangent);

	float3 vBinormal = cross((float3)In.vNormal, vTangent);

	float3 worldBinormal = mul(vBinormal, (float3x3)g_matWorld);
	Out.vB = normalize(worldBinormal);

	Out.vDepth = Out.vPosition;

	return Out;
}

VS_OUT		VS_LIMMAIN(VS_IN In)
{
	VS_OUT Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWVP = mul(g_matView, g_matProj);
	In.vPosition = mul(vector(In.vPosition.xyz, 1.f), g_matWorld);
	In.vNormal = normalize(mul(vector(In.vNormal.xyz, 0.f), g_matWorld));

	In.vNormal *= 0.02f * 0.3f;
	In.vPosition += vector(In.vNormal.xyz, 0);


	Out.vPosition = mul(vector(In.vPosition.xyz, 1.f), matWVP);
	Out.vNormal = In.vNormal;



	return Out;
}



struct PS_IN
{
	vector		vNormal : NORMAL;
	float2		vTexUV : TEXCOORD0;
	vector      vDepth : TEXCOORD1;
	vector      vLim : COLOR0;


	float3   vT : TEXCOORD3;
	float3   vB : TEXCOORD4;
	float3   vN : TEXCOORD5;


};

struct PS_OUT
{
	vector vColor :  COLOR0;
	vector vNormal : COLOR1;
	vector vDepth : COLOR2;
	vector vLim :   COLOR3;


};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vDepth = vector( 0.f,0.f,0.f,0.f );

	Out.vLim = vector(0.f,0.f,0.f,0.f);

	float4 NomalMap = tex2D(NormalSampler, In.vTexUV);

	NomalMap = (NomalMap * 2.f) - 1.f;

	float3 Normal = (NomalMap.x * In.vT) + (NomalMap.y * In.vB) + (NomalMap.z * In.vN);
	Out.vNormal = vector(Out.vNormal.xyz * 2.f - 1.f, 0.f);
	Out.vNormal.xyz = normalize(Normal);


	Out.vDepth = vector(In.vDepth.z / In.vDepth.w, In.vDepth.w / 300.f, 0.f, 0.f);

	return Out;
}

PS_OUT			PS_MAIN_ALPHA(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vDepth = vector(0.f, 0.f, 0.f, 0.f);
	Out.vDepth = vector(In.vDepth.z / In.vDepth.w, In.vDepth.w / 300.f, 0.f, 0.f);

	float4 NomalMap = tex2D(NormalSampler, In.vTexUV);

	NomalMap = (NomalMap * 2.f) - 1.f;

	float3 Normal = (NomalMap.x * In.vT) + (NomalMap.y * In.vB) + (NomalMap.z * In.vN);

	Out.vNormal.xyz = normalize(Normal);



	return Out;
}






PS_OUT			PS_MAIN_SKY(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5 + 0.5f, 0.f);

	return Out;
}



PS_OUT			PS_MAIN_Lim(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vLim = In.vLim;
	Out.vColor = In.vLim;
	return Out;
}

PS_OUT			PS_MAIN_LIGHT(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vNormal = vector(In.vNormal.xyz * 0.5 + 0.5f, 0.f);
	Out.vDepth = vector(0.f, 0.f, 0.f, 0.f);
	Out.vDepth = vector(In.vDepth.z / In.vDepth.w, In.vDepth.w / 300.f, 0.f, 0.f);
	Out.vLim = Out.vColor;
	return Out;
}

PS_OUT          PS_MAIN_NONOMAL(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	Out.vDepth = vector(0.f, 0.f, 0.f, 0.f);
	Out.vDepth = vector(In.vDepth.z / In.vDepth.w, In.vDepth.w / 300.f, 0.f, 0.f);


	Out.vNormal = In.vNormal;


	return Out;
}

PS_OUT          PS_MAIN_NOISE(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	float vNoise = tex2D(NoiseSampler, In.vTexUV);

	Out.vColor = tex2D(BaseSampler, In.vTexUV);
	vNoise = vNoise*((g_fSpeed - g_fTime));

	Out.vColor *= vNoise;


	Out.vNormal = vector(In.vNormal.xyz * 0.5 + 0.5f, 0.f);
	Out.vDepth = vector(In.vDepth.z / In.vDepth.w, In.vDepth.w / 300.f, 0.f, 0.f);


	return Out;

}

PS_OUT         PS_MAIN_COLOR(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;
	Out.vColor = vector(0.2f,0.03f,0.03f,1.0f);
	Out.vDepth = vector(0.f, 0.f, 0.f, 0.f);
	Out.vDepth = vector(In.vDepth.z / In.vDepth.w, In.vDepth.w / 300.f, 0.f, 0.f);
	Out.vNormal = In.vNormal;




	return Out;
}
 
technique Default_Device
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
pixelshader = compile ps_3_0 PS_MAIN_ALPHA();
}
pass Sky
{

cullmode = none;
zwriteEnable = false;
vertexshader = compile vs_3_0 VS_MAIN();
pixelshader =  compile ps_3_0 PS_MAIN_SKY();


}
pass Lim
{
	cullmode = cw;
	vertexshader = compile vs_3_0 VS_LIMMAIN();
    pixelshader = compile ps_3_0 PS_MAIN_Lim();

}
pass Light//4
{


vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_MAIN_LIGHT();

}
pass NoNomal//5
{
	alphatestenable = true;
   alphafunc = Greater;
   alpharef = 0xc0;
	vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_MAIN_NONOMAL();

}
pass Noise//6
{
	alphatestenable = true;
  alphafunc = Greater;
  alpharef = 0xc0;
	vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_MAIN_NOISE();

}
pass ZWrite//7
{

zwriteEnable = false;
vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_MAIN();



}
pass Color//8
{
	cullmode = none;
	vertexshader = compile vs_3_0 VS_MAIN();
pixelshader = compile ps_3_0 PS_MAIN_COLOR();


}


};