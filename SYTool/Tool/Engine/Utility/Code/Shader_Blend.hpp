texture		g_AlbedoTexture;

sampler AlbedoSampler = sampler_state
{
	texture = g_AlbedoTexture;

minfilter = linear;
magfilter = linear;
};

texture		g_ShaderTexture;

sampler ShadeSampler = sampler_state
{
	texture = g_ShaderTexture;

minfilter = linear;
magfilter = linear;
};

texture		g_FlashTexture;

sampler FlashSampler = sampler_state
{
	texture = g_FlashTexture;

minfilter = linear;
magfilter = linear;
};

texture		g_DepthTexture;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;

minfilter = linear;
magfilter = linear;
};
texture		g_LimTexture;

sampler LimSampler = sampler_state
{
	texture = g_LimTexture;

minfilter = linear;
magfilter = linear;
};




struct PS_IN
{
	float2		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector		vColor : COLOR0;
};

PS_OUT	PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vAlbedo = tex2D(AlbedoSampler, In.vTexUV);
	vector		vDepth = tex2D(DepthSampler, In.vTexUV);
	vector		vShade = tex2D(ShadeSampler, In.vTexUV);
	vector      vFlash = tex2D(FlashSampler, In.vTexUV);
	vector      vLim = tex2D(LimSampler, In.vTexUV);
	vector vLight = vShade+vLim;
	//if(vDepth.r>0.99998) vLight = vShade + ((1-vDepth.a)*3.f*vFlash)/2.f+vLim;
	saturate(vLight);


	Out.vColor = vAlbedo*vLight;
	return Out;
}

technique Default_Device
{
	pass Blend
{
	zwriteEnable = false;
vertexshader = NULL;
pixelshader = compile ps_3_0 PS_MAIN();
}

};