Texture2D gNormaltexture : register(t0);
Texture2D gDepthtexture : register(t1);
Texture2D gSpecTexture : register(t2);
SamplerState gsamLinearWrap       : register(s0);



cbuffer cbLightInfo	: register(b0)
{
	float4 g_vLightDir		: packoffset(c0);
	float4 g_vLightDiffuse  : packoffset(c1);
	float4 g_vLightAmibient : packoffset(c2);
	float4 g_vLightSpecular : packoffset(c3);
    float4 g_vLightPos      : packoffset(c4);
    float1 g_fRange         : packoffset(c5);
};

cbuffer cbLightMatrixInfo : register(b1)
{
    float4x4 g_matViewInv : packoffset(c0);
    float4x4 g_matProjInv : packoffset(c4);
    float4 g_vCampos : packoffset(c8);
};


struct VS_INPUT
{
    float3 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 uv       : TEXCOORD;
};

VS_OUTPUT VS_MAIN(VS_INPUT Input)
{
 
    VS_OUTPUT output;
    
    output.position = float4(Input.position, 1.f);
    output.uv = Input.uv;
    
    
    return output;
}

struct ps_output
{
    float4 shade : SV_TARGET0;
    float4 spec : SV_TARGET1;
};

ps_output PS_MAIN(VS_OUTPUT input) : SV_TARGET
{
    ps_output output;
    
    float4 vNormalInfo = gNormaltexture.Sample(gsamLinearWrap, input.uv);
    float4 vDepthInfo = gDepthtexture.Sample(gsamLinearWrap, input.uv);
    float4 vMaterial = gSpecTexture.Sample(gsamLinearWrap, input.uv);
    
    float4 vWorldNormal = vector(vNormalInfo.xyz * 2.f - 1.f, 0.f);
    float fViewZ = vDepthInfo.y * 700.f;
    
    float4 vshade = saturate(dot(normalize(-g_vLightDir), vWorldNormal));
    
    output.shade = (g_vLightAmibient) +(g_vLightDiffuse * vshade);
    output.shade.a = 1.f;
    
   
    float4 vPosition;
    vPosition.x = (input.uv.x * 2.f - 1.f) * fViewZ;
    vPosition.y = (input.uv.y * -2.f + 1.f) * fViewZ;
    vPosition.z = vDepthInfo.x * fViewZ;
    vPosition.w = 1.0f * fViewZ;
    
    vPosition = mul(vPosition, g_matProjInv);
    vPosition = mul(vPosition, g_matViewInv);
    
    float3 reflection = normalize(reflect(g_vLightDir, vWorldNormal));
    float3 vLook = normalize(vPosition.xyz - g_vCampos.xyz);
    


    
    output.spec = pow(saturate(dot(reflection, -vLook)), 3.f) * g_vLightSpecular * vMaterial.r * step(0, vWorldNormal.r + vWorldNormal.g + vWorldNormal.b);
    output.spec.rgb *= 1.5f;
    output.spec.a = 0;





    return output;

};

ps_output PS_POINTMAIN(VS_OUTPUT input) : SV_TARGET
{
    ps_output output;
    
    float4 vNormalInfo = gNormaltexture.Sample(gsamLinearWrap, input.uv);
    float4 vDepthInfo = gDepthtexture.Sample(gsamLinearWrap, input.uv);
    float4 vMaterial = gSpecTexture.Sample(gsamLinearWrap, input.uv);
    
    float fViewZ = vDepthInfo.y * 700.f;
    
    float4 vWorldNormal = float4(vNormalInfo.xyz * 2.f - 1.f, 0.f);
    
    float4 vPosition;
    vPosition.x = (input.uv.x * 2.f - 1.f) * fViewZ;
    vPosition.y = (input.uv.y * -2.f + 1.f) * fViewZ;
    vPosition.z = vDepthInfo.x * fViewZ;
    vPosition.w = 1.0f * fViewZ;
    
    vPosition = mul(vPosition, g_matProjInv);
    vPosition = mul(vPosition, g_matViewInv);
    
    float4 vLightDir = vPosition - g_vLightPos;







    float fDistance = length(vLightDir);
    float4 vShade = saturate(dot(normalize(-vLightDir), vWorldNormal));
    float fAtt = saturate((g_fRange - fDistance) / g_fRange);
    float3 reflection = normalize(reflect(g_vLightDir, vWorldNormal));
    float3 vLook = normalize(vPosition.xyz - g_vCampos.xyz);
    
    output.shade = g_vLightDiffuse * (vShade + g_vLightAmibient) * fAtt;
    output.shade.a = 1.f;




    output.spec = pow(saturate(dot(reflection, -vLook)), 3.f) * g_vLightSpecular * vMaterial.r * fAtt;
    output.spec += output.shade * 0.05f;
    output.spec.a = 0;


    return output;
}
ps_output PS_SPOTMAIN(VS_OUTPUT input) : SV_TARGET
{

    ps_output output;

    float4 vNormalInfo = gNormaltexture.Sample(gsamLinearWrap, input.uv);
    float4 vDepthInfo = gDepthtexture.Sample(gsamLinearWrap, input.uv);
    float4 vMaterial = gSpecTexture.Sample(gsamLinearWrap, input.uv);

    float fViewZ = vDepthInfo.y * 700.f;

    float4 vWorldNormal = float4(vNormalInfo.xyz * 2.f - 1.f, 0.f);

    float4 vPosition;
    vPosition.x = (input.uv.x * 2.f - 1.f) * fViewZ;
    vPosition.y = (input.uv.y * -2.f + 1.f) * fViewZ;
    vPosition.z = vDepthInfo.x * fViewZ;
    vPosition.w = 1.0f * fViewZ;

    vPosition = mul(vPosition, g_matProjInv);
    vPosition = mul(vPosition, g_matViewInv);

    float4 vLightDir = vPosition - g_vLightPos;
    float4 vSpotDir = normalize(g_vLightDir);

    float fAngle = dot(normalize(vLightDir), -vSpotDir);

    float fDistance = length(vLightDir);
   



    if (fAngle > 0.6)
    {
        float4 vShade = saturate(dot(normalize(vLightDir), vWorldNormal));
        float fAtt = saturate((g_fRange - fDistance) / g_fRange); 
        float3 reflection = normalize(reflect(g_vLightDir, vWorldNormal));
        float3 vLook = normalize(vPosition.xyz - g_vCampos.xyz);
        float Cutoff = (1.0 - (1.0 - fAngle) * 1.0 / (1.0 - 0.5));
        output.shade = g_vLightDiffuse * (vShade + g_vLightAmibient) * (fAtt+ fAtt*Cutoff);
        output.shade.a = 1.f;




        output.spec = pow(saturate(dot(reflection, -vLook)), 3.f) * g_vLightSpecular * vMaterial.r * fAtt;
        output.spec += output.shade * 0.05f;
        output.spec.a = 0;
        output.spec = float4(0.f, 0.f, 0.f, 0.f);
    }
    else
    {
        output.shade = float4(0.f, 0.f, 0.f, 1.f);
        output.spec = float4(0.f, 0.f, 0.f, 0.f);

    }
 

    return output;


}