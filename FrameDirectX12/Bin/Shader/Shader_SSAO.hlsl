
Texture2D gNormalTexture : register(t0);
Texture2D gDepthTexture : register(t1);

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
    
    return output;
}


float4 PS_MAIN(VS_OUTPUT input) : SV_TARGET
{
    
    float4 OutColor;
    float4 gauss = 1;
    gauss = gDepthTexture.Sample(gsamLinearWrap, input.uv);
    float4 Normal = gNormalTexture.Sample(gsamLinearWrap, input.uv);
    
    float Color = gauss.r * (Normal.g) * 0.3f;
    
    OutColor = float4(Color.rrr, 1.f);
    
    return OutColor;

};