Texture2D gAlbedoTexture : register(t0);
Texture2D gShadeTexture : register(t1);
Texture2D gSpecTexture : register(t2);
Texture2D gBlurTexture : register(t3);
Texture2D gEmissiveTexture : register(t4);
Texture2D gDestortionTexture : register(t5);
Texture2D gSSAOTexture : register(t6);
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

    float2 DistUV = vDistortion.xy * 0.02f;

    DistUV = DistUV + input.uv;
    DistUV += vDistortion.z * 0.02;
    float4 vAlbedoInfo = gAlbedoTexture.Sample(gsamLinearWrap, DistUV);
    float4 vSpecInfo = gSpecTexture.Sample(gsamLinearWrap, DistUV);
    float4 vShadeInfo = gShadeTexture.Sample(gsamLinearWrap, DistUV);
    float4 vBlurInfo = gBlurTexture.Sample(gsamLinearWrap, DistUV) * 1.5f;
    float4 vEmissiveInfo = gEmissiveTexture.Sample(gsamLinearWrap, DistUV) * 2.f;
    float4 vSSaoInfo = gSSAOTexture.Sample(gsamLinearWrap, DistUV) * 1.5f;

    OutColor = (vAlbedoInfo * vShadeInfo * vSSaoInfo) + vSpecInfo + vBlurInfo + vEmissiveInfo;



    float4 Color2 = OutColor;
    float A = 0.15f;
    float B = 0.5f;
    float C = 0.1f;
    float D = 0.2f;
    float E = 0.02f;
    float F = 0.3f;
    float3 linearWhite = float3(11.2f, 11.2f, 11.2f);

    float3 color = OutColor.rgb;

    color = ((color * (A * color + C * B) + D * E) / (color * (A * color + B) + D * F)) - (E / F);
    linearWhite = ((linearWhite * (A * linearWhite + C * B) + D * E) / (linearWhite * (A * linearWhite + B) + D * F)) - (E / F);

    OutColor = float4(color / linearWhite, Color2.a);

    return OutColor;

};