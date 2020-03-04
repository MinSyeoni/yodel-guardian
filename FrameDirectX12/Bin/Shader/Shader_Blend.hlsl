Texture2D gAlbedoTexture : register(t0);
Texture2D gShadeTexture : register(t1);
Texture2D gSpecTexture : register(t2);


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
    
    float4 vAlbedoInfo = gAlbedoTexture.Sample(gsamLinearWrap, input.uv);
    float4 vSpecInfo = gSpecTexture.Sample(gsamLinearWrap, input.uv);
    float4 vShadeInfo = gShadeTexture.Sample(gsamLinearWrap, input.uv);
     
    OutColor = (vAlbedoInfo * vShadeInfo)+vSpecInfo;


    return OutColor;

};