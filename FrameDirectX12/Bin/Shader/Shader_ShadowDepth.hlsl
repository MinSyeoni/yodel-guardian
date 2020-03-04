
cbuffer cbShadowInfo : register(b0)
{
    float4x4 matWorld : packoffset(c0);
    float4x4 matView : packoffset(c4);
    float4x4 matProj : packoffset(c8);
    bool1    bIsMesh : packoffset(c12);
};
cbuffer cbBoneInfo : register(b1)
{
    float4x4 gmatBones[64] : packoffset(c0);
};

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4 BoneId[2] : TEXCOORD1;
    float4 BoneWeights[2] : TEXCOORD3;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 proj : TEXCOORD;
};


VS_OUTPUT VS_MAIN(VS_INPUT Input)
{
    float4x4 matBone = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
    
    float4x4 matWV, matWVP;
    if (bIsMesh && Input.BoneWeights[0].x != 0.0f)
    {
        matBone = gmatBones[Input.BoneId[0].x] * Input.BoneWeights[0].x;
        matBone += gmatBones[Input.BoneId[0].y] * Input.BoneWeights[0].y;
        matBone += gmatBones[Input.BoneId[0].z] * Input.BoneWeights[0].z;
        matBone += gmatBones[Input.BoneId[1].x] * Input.BoneWeights[1].x;
    }
    
    VS_OUTPUT output;
    float4 modelpos = mul(float4(Input.position, 1.0f), matBone);
    matWV = mul(matWorld, matView);
    matWVP = mul(matWV, matProj);
    output.position = mul(modelpos, matWVP);
    output.proj = output.position;
     
    return (output);
}

float4 PS_MAIN(VS_OUTPUT input) : SV_TARGET
{
    return float4(input.proj.z / input.proj.w, input.proj.w / 500.f, 0.f, 1.f);
}
