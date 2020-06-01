Texture2D gTexture : register(t0);
Texture2D gNormalTexture : register(t1);
Texture2D gShadowTexture : register(t2);

SamplerState gsamLinearWrap : register(s0);

cbuffer cbMatrixInfo : register(b0)
{
    float4x4 matWorld : packoffset(c0);
    float4x4 matView : packoffset(c4);
    float4x4 matProj : packoffset(c8);
    float4x4 matWVP : packoffset(c12);
};
cbuffer cbLightInfo : register(b1)
{
    float4x4 matLightView : packoffset(c0);
    float4x4 matLightProj : packoffset(c4);
    
}

struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 vLightPos : TEXCOORD1;
    float3 vT : TEXCOORD2;
    float3 vB : TEXCOORD3;
    float3 vN : TEXCOORD4;
    float4 vProjPos : TEXCOORD5;
    
};


VS_OUTPUT VS_MAIN(VS_INPUT Input)
{
    VS_OUTPUT output;
    
    float4x4 matLightWV, matLightWVP;
    
    matLightWV = mul(matWorld, matLightView);
    matLightWVP = mul(matLightWV, matLightProj);
    output.vLightPos = mul(float4(Input.position, 1.0f), matLightWVP);
    output.position = mul(float4(Input.position, 1.0f), matWVP);



    output.uv = Input.uv;
    
 
    float3 worldNormal = mul(Input.normal, (float3x3) matWorld);
    output.vN = normalize(worldNormal);
    
    float3 vTangent = cross((float3) Input.normal, float3(0.f, 0.f, 1.f));
    float3 worldTangent = mul(vTangent, (float3x3) matWorld);
    output.vT = normalize(worldTangent);
    
    float3 vBinormal = cross((float3) Input.normal, vTangent);
    float3 worldBinormal = mul(vBinormal, (float3x3) matWorld);
    output.vB = normalize(worldBinormal);
    
    output.vProjPos = output.position;
    return (output);
}

struct ps_output
{
    float4 albedo : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 depth : SV_TARGET2;
};



ps_output PS_MAIN(VS_OUTPUT input) : SV_TARGET
{
    
    ps_output output;
    
    output.albedo = gTexture.Sample(gsamLinearWrap, input.uv*8.f);
    
    float2 uv = input.vLightPos.xy / input.vLightPos.w;
    uv.y = -uv.y;
    uv = uv * 0.5 + 0.5;
    
  
    float4 vShadow = gShadowTexture.Sample(gsamLinearWrap, uv);
    if (vShadow.r + 0.000125f < input.vLightPos.z / input.vLightPos.w)
        output.albedo.rgb *= 0.5f;

    float3 tangentNormal = gNormalTexture.Sample(gsamLinearWrap, input.uv*8.f).xyz;
    tangentNormal = normalize(tangentNormal * 2.f - 1.f);
    float3x3 TBN = float3x3(normalize(input.vT), normalize(input.vB), normalize(input.vN));
    TBN = transpose(TBN);
    
    float3 WorldNormal = mul(TBN, tangentNormal);
    output.normal = float4(WorldNormal * 0.5 + 0.5, 1.f);
    

    output.depth = float4(input.vProjPos.z / input.vProjPos.w, input.vProjPos.w / 500.f, 0.f, 0.f);
    return (output);
}



