Texture2D gTexture : register(t0);
SamplerState gsamLinearWrap       : register(s0);

cbuffer cbMatrixInfo	: register(b0)
{
	float4x4 matWorld : packoffset(c0);
	float4x4 matView : packoffset(c4);
	float4x4 matProj : packoffset(c8);
	float4x4 matWVP : packoffset(c12);
};
cbuffer cbEffectInfo : register(b1)
{

	float4 SpriteInfo : packoffset(c0);
	float4 AlphaTimeInfo :packoffset(c1);
	float4 ColorInfo : packoffset(c2);

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

VS_TEXTURED_OUTPUT VS_MAIN(VS_TEXTURED_INPUT Input)
{
	VS_TEXTURED_OUTPUT output;
	output.position = mul(float4(Input.position, 1.0f), matWVP);
	output.uv = Input.uv;
	return(output);
}

float4 PS_MAIN(VS_TEXTURED_OUTPUT input) : SV_TARGET
{
	float4 vColor;
	float2 uv = input.uv;
	uv.x = (uv.x / SpriteInfo.z) + SpriteInfo.x;
	uv.y = (uv.y / SpriteInfo.w) + SpriteInfo.y;

	vColor = gTexture.Sample(gsamLinearWrap, uv);


	vColor.a = vColor.a * AlphaTimeInfo.x;
	vColor = vColor * ColorInfo;


	return(vColor);
}

