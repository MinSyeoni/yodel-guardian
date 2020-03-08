Texture2D g_DownSample : register(t0);

SamplerState gsamLinearWrap : register(s0);

cbuffer cbDownSampleInfo : register(b0)
{
    float2 PixelKernelH[13];
    float2 PixelKernelV[13];
};

cbuffer cbBlurInfo : register(b1)
{
    float BlurWeights[13];
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
    
    output.position.z = 0.01f;
    return output;
}


float4 PS_MAIN(VS_OUTPUT input) : SV_TARGET
{
    float2 rcpres = { 0.0025, 0.00333333333333333333333 };
    float4 gauss = { 0.f, 0.f, 0.f, 0.f };

    for (int i = 0; i < 13; i++)
    {
        gauss += g_DownSample.Sample(gsamLinearWrap, input.uv*4.f + (PixelKernelH[i] * rcpres))*BlurWeights[i];
        gauss += g_DownSample.Sample(gsamLinearWrap, input.uv*4.f + (PixelKernelV[i] * rcpres))*BlurWeights[i];
    }
    

    float4 OutColor = gauss*2.0;
 
    return OutColor;

};