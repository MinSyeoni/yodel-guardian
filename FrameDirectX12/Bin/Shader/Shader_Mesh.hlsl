Texture2D gTexture : register(t0);
Texture2D gNormalTexture : register(t1);
Texture2D gSpecTexture : register(t2);
Texture2D gEmisTexture : register(t3);

SamplerState gsamLinearWrap       : register(s0);

cbuffer cbMatrixInfo   : register(b0)
{
    float4x4 matWorld   : packoffset(c0);
    float4x4 matView : packoffset(c4);
    float4x4 matProj : packoffset(c8);
    float4x4 matWVP : packoffset(c12);
};
cbuffer cbBoneInfo   : register(b1)
{
    float4x4 gmatBones[80]      : packoffset(c0);
};

cbuffer cbTexutreInfo : register(b2)
{
    float4 gTexInfo : packoffset(c0);
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
    float2 uv       : TEXCOORD0;
    float3 vT       : TEXCOORD1;
    float3 vB       : TEXCOORD2;
    float3 vN       : TEXCOORD3;
    float4 vProjPos : TEXCOORD4;
};
struct VS_LIMOUTPUT
{
    float4 position : SV_POSITION;

};
VS_OUTPUT VS_MAIN(VS_INPUT Input)
{
    float4x4 matBone = { 1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f };

    if (Input.BoneWeights[0].x != 0.0f) { matBone = gmatBones[Input.BoneId[0].x] * Input.BoneWeights[0].x;      matBone += gmatBones[Input.BoneId[0].y] * Input.BoneWeights[0].y;      matBone += gmatBones[Input.BoneId[0].z] * Input.BoneWeights[0].z;      matBone += gmatBones[Input.BoneId[1].x] * Input.BoneWeights[1].x; }
    VS_OUTPUT output;
    float4 modelpos = mul(float4(Input.position, 1.0f), matBone);
    output.position = mul(modelpos, matWVP);
    output.uv = Input.uv;

    float3 worldNormal = mul(Input.normal, (float3x3)matWorld);   output.vN = normalize(worldNormal);

    float3 vTangent = cross((float3) Input.normal, float3(0.f, 1.f, 0.f));  float3 worldTangent = mul(vTangent, (float3x3) matWorld);   output.vT = normalize(worldTangent);

    float3 vBinormal = cross((float3) Input.normal, vTangent);  float3 worldBinormal = mul(vBinormal, (float3x3) matWorld);    output.vB = normalize(worldBinormal);

    output.vProjPos = output.position;
    return(output);
}



struct ps_output
{
    float4 albedo : SV_TARGET0;
    float4 normal : SV_TARGET1;
    float4 Depth  : SV_TARGET2;
    float4 Specular : SV_TARGET3;
    float4 Emissive : SV_Target4;
};


ps_output PS_MAIN(VS_OUTPUT input) : SV_TARGET
{

   ps_output output;

    float4 NomalMap = gNormalTexture.Sample(gsamLinearWrap, input.uv);
    float4 SpecMap = gSpecTexture.Sample(gsamLinearWrap, input.uv);
    NomalMap = (NomalMap * 2.f) - 1.f;

    output.albedo = gTexture.Sample(gsamLinearWrap, input.uv);
    output.albedo.a = 1.f;
    float3 tangentNormal = gNormalTexture.Sample(gsamLinearWrap, input.uv).xyz;
    tangentNormal = normalize(tangentNormal * 2.f - 1.f);
    float3x3 TBN = float3x3(normalize(input.vT), normalize(input.vB), normalize(input.vN));
    TBN = transpose(TBN);

    float3 WorldNormal = mul(TBN, tangentNormal);
    output.normal = float4(WorldNormal * 0.5 + 0.5, 1.f);

    float4 fSpec = float4((SpecMap.r + SpecMap.g) / 2, step(SpecMap.r + 0.05, SpecMap.g - SpecMap.b), 0.f, 0.f);
    output.Specular = fSpec * gTexInfo.r;


    output.Depth = float4(input.vProjPos.z / input.vProjPos.w, input.vProjPos.w / 500.f, 0.f, 0.f);

    output.Emissive = float4(0.f, 0.f, 0.f, 0.f);
    if (gTexInfo.g > 0)    output.Emissive = gEmisTexture.Sample(gsamLinearWrap, input.uv);


    return(output);
};


float4 PS_SKYDOME(VS_OUTPUT input) :SV_TARGET
{
    float4 vColor;
    vColor = gTexture.Sample(gsamLinearWrap, input.uv * 2.5f);

    return vColor;

}

//LIM LIGHT

VS_LIMOUTPUT VS_LIMMAIN(VS_INPUT Input)
{

    float4x4 matBone = { 1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f };

    if (Input.BoneWeights[0].x != 0.0f) { matBone = gmatBones[Input.BoneId[0].x] * Input.BoneWeights[0].x;      matBone += gmatBones[Input.BoneId[0].y] * Input.BoneWeights[0].y;      matBone += gmatBones[Input.BoneId[0].z] * Input.BoneWeights[0].z;      matBone += gmatBones[Input.BoneId[1].x] * Input.BoneWeights[1].x; }
    VS_LIMOUTPUT output;
    float4 modelpos = mul(float4(Input.position, 1.0f), matBone);
    output.position = mul(modelpos, matWVP);

    return output;
}


ps_output PS_LIMMAIN(VS_LIMOUTPUT input) :SV_TARGET
{

     ps_output output;

     output.albedo = float4(1.f, 1.f, 1.f, 1.f);
     output.Emissive = float4(1.f, 1.f, 1.f, 1.f);

     return output;
}