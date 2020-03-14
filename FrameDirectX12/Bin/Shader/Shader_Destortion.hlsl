
Texture2D gNormalTexture : register(t0);
Texture2D gDepthTexture : register(t1);

SamplerState gsamLinearWrap : register(s0);

cbuffer cbMatrixInfo : register(b0)
{
    float4x4 matWorld : packoffset(c0);
    float4x4 matView : packoffset(c4);
    float4x4 matProj : packoffset(c8);
    float4x4 matWVP : packoffset(c12);
};

cbuffer cbDistortScale : register(b1)
{
    float4 fDistortionScale;//x´Â Å©±â

}


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
    float2 uv : TEXCOORD0;
    float3 vT : TEXCOORD1;
    float3 vB : TEXCOORD2;
    float3 vN : TEXCOORD3;
    float4 vProj : TEXCOORD4;
};
VS_OUTPUT VS_MAIN(VS_INPUT Input)
{
    float4x4 matBone = { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f };
  
    VS_OUTPUT output;
    float4 modelpos = mul(float4(Input.position, 1.0f), matBone);
    output.position = mul(modelpos, matWVP);
    output.uv = Input.uv;
     
    float3 worldNormal = mul(Input.normal, (float3x3) matWorld);
    output.vN = normalize(worldNormal);
    
    float3 vTangent = cross((float3) Input.normal, float3(0.f, 1.f, 0.f));
    float3 worldTangent = mul(vTangent, (float3x3) matWorld);
    output.vT = normalize(worldTangent);
    
    float3 vBinormal = cross((float3) Input.normal, vTangent);
    float3 worldBinormal = mul(vBinormal, (float3x3) matWorld);
    output.vB = normalize(worldBinormal);
    
    output.vProj = output.position;
    
    return (output);
}

struct ps_output
{
    float4 Distortion : SV_TARGET0;
};


ps_output PS_MAIN(VS_OUTPUT input) : SV_TARGET
{

    ps_output output;
     
    float2 uv = input.uv;
    uv.x += fDistortionScale.x;
    uv.y += fDistortionScale.y;
    float3 tangentNormal = gNormalTexture.Sample(gsamLinearWrap, uv).xyz;
    tangentNormal = normalize(tangentNormal * 2.f - 1.f);
    float3x3 TBN = float3x3(normalize(input.vT), normalize(input.vB), normalize(input.vN));
    TBN = transpose(TBN);
    
    float3 WorldNormal = mul(TBN, tangentNormal);
 
    float2 Projuv = input.vProj.xy/input.vProj.w;
    
    float4 vDepth = gDepthTexture.Sample(gsamLinearWrap, Projuv);
      
    output.Distortion = float4(WorldNormal * 0.5 + 0.5, 1.f);
    output.Distortion.r = 0.f;
    
      


    return (output);
};
