Texture2D gTexture : register(t0);

SamplerState gsamPointWrap        : register(s0);
SamplerState gsamPointClamp       : register(s1);
SamplerState gsamLinearWrap       : register(s2);
SamplerState gsamLinearClamp      : register(s3);
SamplerState gsamAnisotropicWrap  : register(s4);
SamplerState gsamAnisotropicClamp : register(s5);


cbuffer cbMatrixInfo	: register(b0)
{
	float4x4 matWVP		: packoffset(c0);

};

struct VS_TEXTURED_INPUT
{
	float3 position : POSITION;
	float3 Normal : NORMAL;
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