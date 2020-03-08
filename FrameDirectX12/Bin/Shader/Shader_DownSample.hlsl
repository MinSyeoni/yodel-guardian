Texture2D g_EmissiveTexture : register(t0);
Texture2D g_SpecTexture : register(t1);

SamplerState gsamLinearWrap : register(s0);

cbuffer cbDownSampleInfo : register(b0)
{
    float4 DownSampleOffset[16];
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
    
    float4 average = { 0.f, 0.f, 0.f, 0.f };
    float4 average2 = { 0.f, 0.f, 0.f, 0.f };
    for (int i = 0; i < 16; i++)
    {
        average += g_EmissiveTexture.Sample(gsamLinearWrap, input.uv*4.f  + float2(DownSampleOffset[i].x, DownSampleOffset[i].y));    
        average2 += g_SpecTexture.Sample(gsamLinearWrap, input.uv * 4.f + float2(DownSampleOffset[i].x, DownSampleOffset[i].y));
    }
    
    average *= (1.0f / 16.f);
    average2 *= (1.0f / 16.f);
    average2.a = average2.r * 20.f;
    float4 OutColor = average + average2;
    
    return OutColor;

};