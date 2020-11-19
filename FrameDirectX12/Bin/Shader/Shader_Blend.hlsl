Texture2D gAlbedoTexture : register(t0);
Texture2D gShadeTexture : register(t1);
Texture2D gSpecTexture : register(t2);
Texture2D gBlurTexture : register(t3);
Texture2D gEmissiveTexture : register(t4);
Texture2D gDestortionTexture : register(t5);
Texture2D gSSAOTexture : register(t6);
SamplerState gsamLinearWrap : register(s0);

cbuffer cbOptionInfo : register(b0)
{
    float4 OptionList : packoffset(c0); //01.SSAO,02.Spec,03.EMISSIVE,04.Distortion
};


struct VS_INPUT
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VS_OUTPUT VS_MAIN(VS_INPUT Input)
{

    VS_OUTPUT output;

    output.position = float4(Input.position, 1.f);
    output.uv = Input.uv;

    return output;
}


float4 PS_MAIN(VS_OUTPUT input) : SV_TARGET
{
    float4 OutColor;

    float4 vDistortion = gDestortionTexture.Sample(gsamLinearWrap, input.uv);

    float2 DistUV = vDistortion.xy * 0.02f;
    DistUV = (DistUV * 2.f - 1.f);

    DistUV = DistUV + input.uv;
    DistUV += vDistortion.z * 0.02;
    float4 vAlbedoInfo = gAlbedoTexture.Sample(gsamLinearWrap, DistUV);
    float4 vSpecInfo = gSpecTexture.Sample(gsamLinearWrap, DistUV);
    float4 vShadeInfo = gShadeTexture.Sample(gsamLinearWrap, DistUV);
    float4 vBlurInfo = gBlurTexture.Sample(gsamLinearWrap, DistUV) * 1.5f;
    float4 vEmissiveInfo = gEmissiveTexture.Sample(gsamLinearWrap, DistUV) * 2.f;
    float4 vSSaoInfo = gSSAOTexture.Sample(gsamLinearWrap, DistUV) * 3.0f;
    if(OptionList.x>0.5)
    OutColor = (vAlbedoInfo * vShadeInfo *(vSSaoInfo)) +( vSpecInfo* OptionList.y) + vBlurInfo + (vEmissiveInfo* OptionList.z);
    else
    OutColor = (vAlbedoInfo * vShadeInfo ) + (vSpecInfo * OptionList.y) + vBlurInfo + (vEmissiveInfo * OptionList.z);

   



    return OutColor;

};