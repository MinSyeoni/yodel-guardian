Texture2D gTexture : register(t0);
SamplerState gsamLinearWrap       : register(s0);

cbuffer cbMatrixInfo	: register(b0)
{
	float4x4 matWorld : packoffset(c0);
	float4x4 matView : packoffset(c4);
	float4x4 matProj : packoffset(c8);
	float4x4 matWVP : packoffset(c12);
};

struct VS_TEXTURED_INPUT
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_TEXTURED_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv       : TEXCOORD;
};

/*노말없는거*/
struct VS_NTEXTURED_INPUT//노말없는거
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
};

struct VS_NTEXTURED_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv       : TEXCOORD;
};

VS_TEXTURED_OUTPUT VS_MAIN(VS_TEXTURED_INPUT Input)
{
	VS_TEXTURED_OUTPUT output;
	output.position = mul(float4(Input.position, 1.0f), matWVP);
	output.uv = Input.uv;
	return(output);
}

float4 PS_MAIN(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float4 cColor = gTexture.Sample(gsamLinearWrap, input.uv);
	return(cColor);
}

float4 PS_HPBAR(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float fAlpha = 1.f;
	float2 fTemp = input.uv - float2(0.08f, 0.83f);
	float fAngle = 0.f;

	float fHp = matWorld._11;

	fTemp = normalize(fTemp);
	float fRad = acos(dot(fTemp, float2(0.f, 1.f)));

	if (input.uv.x <= 0.08f)
		fRad *= -1.f;

	if (fRad >= fHp)
		fAlpha = 0.f;

	float4 cColor = gTexture.Sample(gsamLinearWrap, input.uv);
	cColor.a *= fAlpha;

	return(cColor);
}

float4 PS_HPBAR1(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float fAlpha = 1.f;
	float2 fTemp = input.uv - float2(0.18f, 0.86f);
	float fAngle = 0.f;

	float fHp = matWorld._11;

	fTemp = normalize(fTemp);
	float fRad = acos(dot(fTemp, float2(0.f, 1.f)));

	if (input.uv.x <= 0.18f)
		fRad *= -1.f;

	if (fRad >= fHp)
		fAlpha = 0.f;

	float4 cColor = gTexture.Sample(gsamLinearWrap, input.uv);
	cColor.a *= fAlpha;

	return(cColor);
}

float4 PS_HPBAR2(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float fAlpha = 1.f;
	float2 fTemp = input.uv - float2(0.265f, 0.86f);
	float fAngle = 0.f;

	float fHp = matWorld._11;

	fTemp = normalize(fTemp);
	float fRad = acos(dot(fTemp, float2(0.f, 1.f)));

	if (input.uv.x <= 0.265f)
		fRad *= -1.f;

	if (fRad >= fHp)
		fAlpha = 0.f;

	float4 cColor = gTexture.Sample(gsamLinearWrap, input.uv);
	cColor.a *= fAlpha;

	return(cColor);
}

VS_TEXTURED_OUTPUT VS_NNOMAL(VS_NTEXTURED_INPUT Input)
{
	VS_NTEXTURED_OUTPUT output;
	output.position = mul(float4(Input.position, 1.0f), matWVP);
	output.uv = Input.uv;
	return(output);
}

float4 PS_NNOMAL(VS_NTEXTURED_OUTPUT input) : SV_TARGET
{
	float4 cColor = gTexture.Sample(gsamLinearWrap, input.uv);

	return(cColor);
}