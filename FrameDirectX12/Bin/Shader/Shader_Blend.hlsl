Texture2D gAlbedoTexture : register(t0);
Texture2D gShadeTexture : register(t1);
Texture2D gSpecTexture : register(t2);
Texture2D gBlurTexture : register(t3);
Texture2D gEmissiveTexture : register(t4);
Texture2D gDestortionTexture : register(t5);

SamplerState gsamLinearWrap : register(s0);

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
    
    output.position.z = 0.01f;
    return output;
}


float4 PS_MAIN(VS_OUTPUT input) : SV_TARGET
{
    float4 OutColor;
    
    float4 vDistortion = gDestortionTexture.Sample(gsamLinearWrap, input.uv);
    
    float2 DistUV =  vDistortion.xy * 0.04f;

    DistUV = DistUV + input.uv;
    DistUV += vDistortion.z * 0.04;
    float4 vAlbedoInfo = gAlbedoTexture.Sample(gsamLinearWrap, DistUV);
    float4 vSpecInfo = gSpecTexture.Sample(gsamLinearWrap, DistUV);
    float4 vShadeInfo = gShadeTexture.Sample(gsamLinearWrap, DistUV);
    float4 vBlurInfo = gBlurTexture.Sample(gsamLinearWrap, DistUV);
    float4 vEmissiveInfo = gEmissiveTexture.Sample(gsamLinearWrap, DistUV);
    
    
    OutColor = (vAlbedoInfo * vShadeInfo) + vSpecInfo + vBlurInfo + vEmissiveInfo;


    return OutColor;

};