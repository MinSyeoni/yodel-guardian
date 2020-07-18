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


	float4 Color2 = cColor;
	float A = 0.15f;
	float B = 0.5f;
	float C = 0.1f;
	float D = 0.2f;
	float E = 0.02f;
	float F = 0.3f;
	float3 linearWhite = float3(11.2f, 11.2f, 11.2f);

	float3 color = cColor.rgb * 2.f;

	color = ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - (E / F);
	linearWhite = ((linearWhite * (A * linearWhite + C * B) + D * E) / (linearWhite * (A * linearWhite + B) + D * F)) - (E / F);

	cColor = float4(color / linearWhite, Color2.a);
	cColor = pow(cColor, 1 / 2.2);


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


	float4 Color2 = cColor;
	float A = 0.15f;
	float B = 0.5f;
	float C = 0.1f;
	float D = 0.2f;
	float E = 0.02f;
	float F = 0.3f;
	float3 linearWhite = float3(11.2f, 11.2f, 11.2f);

	float3 color = cColor.rgb*2.f;

	color = ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - (E / F);
	linearWhite = ((linearWhite * (A * linearWhite + C * B) + D * E) / (linearWhite * (A * linearWhite + B) + D * F)) - (E / F);

	cColor = float4(color / linearWhite, Color2.a);
	cColor = pow(cColor, 1 / 2.2);


	return(cColor);
}