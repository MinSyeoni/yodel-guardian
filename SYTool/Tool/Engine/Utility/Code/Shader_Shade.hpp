matrix		g_matProjInv;
matrix		g_matViewInv;
vector		g_vLightPos;
float		g_fRange = 0.f;
vector		g_vLightDir;
float      g_vCosPhi;



texture				g_NormalTexture;
sampler NormalSampler = sampler_state
{
	texture = g_NormalTexture;
};

texture		g_DepthTexture;

sampler DepthSampler = sampler_state
{
	texture = g_DepthTexture;
};


vector		g_vLightDiffuse;
vector		g_vLightAmbient;

vector		g_vMtrlDiffuse = (vector)1.f;
vector		g_vMtrlAmbient = (vector)1.f;

struct PS_IN
{
	vector		vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector	vShade : COLOR0;
};

PS_OUT			PS_MAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vNormal = tex2D(NormalSampler, In.vTexUV);


	vNormal = vector(vNormal.xyz * 2.f - 1.f, 0.f);

	Out.vShade = max(dot(normalize(g_vLightDir) * -1.f, vNormal), 0.f) * (g_vLightDiffuse * g_vMtrlDiffuse)
		+ (g_vLightAmbient * g_vMtrlAmbient);
	Out.vShade.a = 1.f;

	return Out;
}

PS_OUT			PS_SPOTMAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vNormal = tex2D(NormalSampler, In.vTexUV);
	vector		vDepthInfo = tex2D(DepthSampler, In.vTexUV);

	float		fViewZ = vDepthInfo.y * 300.f;

	vector		vWorldNormal = vector(vNormal.xyz * 2.f - 1.f, 0.f);

	vector		vPosition;

	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepthInfo.x * fViewZ;
	vPosition.w = 1.0f * fViewZ;

	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);

	vector		vLightDir = vPosition - g_vLightPos;

	float		fDistance = length(vLightDir);

	vector		vShade = saturate(dot(normalize(vLightDir) * -1.f, vWorldNormal));


	float		fAtt = saturate((g_fRange - fDistance) / g_fRange);


	Out.vShade = (max(dot(normalize(g_vLightDir) * -1.f, vNormal), 0.f) * (g_vLightDiffuse * g_vMtrlDiffuse)
		+(g_vLightAmbient * g_vMtrlAmbient))*fAtt;
	Out.vShade.a = 1.f;

	return Out;
}


PS_OUT			PS_POINTMAIN(PS_IN In)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vNormal = tex2D(NormalSampler, In.vTexUV);
	vector		vDepthInfo = tex2D(DepthSampler, In.vTexUV);

	float		fViewZ = vDepthInfo.y * 300.f;

	vector		vWorldNormal = vector(vNormal.xyz * 2.f - 1.f, 0.f);

	vector		vPosition;

	vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
	vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
	vPosition.z = vDepthInfo.x * fViewZ;
	vPosition.w = 1.0f * fViewZ;

	vPosition = mul(vPosition, g_matProjInv);
	vPosition = mul(vPosition, g_matViewInv);

	vector		vLightDir = vPosition - g_vLightPos;

	float		fDistance = length(vLightDir);

	//vector		vShade = saturate(dot(normalize(vLightDir) * -1.f, vWorldNormal));
	float fAngle = dot(normalize(vLightDir), g_vLightDir);


	if (fAngle > g_vCosPhi)
	{
		Out.vShade = (max(dot(normalize(g_vLightDir) * -1.f, vNormal), 0.f) * (g_vLightDiffuse * g_vMtrlDiffuse)
			* (g_vLightAmbient * g_vMtrlAmbient));
	}

	Out.vShade.a = 1.f;
	return Out;
}


technique Default_Device
{
	pass    Directional
{
	zwriteenable = false;
AlphaBlendEnable = true;
SrcBlend = one;
DestBlend = one;
vertexshader = NULL;
pixelshader = compile ps_3_0 PS_MAIN();
}

pass   Point
{
	zwriteenable = false;
AlphaBlendEnable = true;
SrcBlend = one;
DestBlend = one;
	vertexshader = NULL;
pixelshader = compile ps_3_0 PS_SPOTMAIN();
}
pass SPOT
{
	zwriteenable = false;
AlphaBlendEnable = true;
SrcBlend = one;
DestBlend = one;
vertexshader = NULL;
pixelshader = compile ps_3_0 PS_POINTMAIN();


}




};