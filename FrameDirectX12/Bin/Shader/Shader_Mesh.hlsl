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
cbuffer cbBoneInfo	: register(b1)
{
	float4x4 gmatBones[36]		: packoffset(c0);

};
struct VS_INPUT
{
	float3 position : POSITION;
	float3 Normal : NORMAL;
	float2 uv : TEXCOORD;
	float4 BoneId[2] : TEXCOORD1;
	float4 BoneWeights[2] : TEXCOORD3;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv       : TEXCOORD;
};


VS_OUTPUT VS_MAIN(VS_INPUT Input)
{
	float4x4 matBone = { 1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f };

	if (Input.BoneWeights[0].x != 0.0f)
	{
		matBone = gmatBones[Input.BoneId[0].x] * Input.BoneWeights[0].x;
		matBone += gmatBones[Input.BoneId[0].y] * Input.BoneWeights[0].y;
		matBone += gmatBones[Input.BoneId[0].z] * Input.BoneWeights[0].z;
		matBone += gmatBones[Input.BoneId[1].x] * Input.BoneWeights[1].x;
	}
	VS_OUTPUT output;
	float4 modelpos = mul(float4(Input.position, 1.0f), matBone);
	output.position = mul(modelpos, matWVP);
	output.uv = Input.uv;
	return(output);
}

float4 PS_MAIN(VS_OUTPUT input) : SV_TARGET
{
	float4 cColor = gTexture.Sample(gsamLinearWrap, input.uv);
	return(cColor);
}

