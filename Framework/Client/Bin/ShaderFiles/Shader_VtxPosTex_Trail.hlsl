#include "Engine_Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D g_DiffuseTexture;

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vLifeTime : TEXCOORD1;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vLifeTime : TEXCOORD1;
};


VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    
    float4x4 matVP;

    matVP = mul(g_ViewMatrix, g_ProjMatrix);
    
    Out.vPosition = mul(float4(In.vPosition, 1.f), matVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vLifeTime = In.vLifeTime;
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vLifeTime : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET;
};

struct PS_EMISSIVE
{
    float4 vColor : SV_TARGET0;
    float4 vEmissive : SV_TARGET1;
};

PS_EMISSIVE PS_MAIN(PS_IN In)
{
    PS_EMISSIVE Out = (PS_EMISSIVE) 0;
    float4 vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
    Out.vColor.rgb = float3(0.3f, 0.05f, 0.05f);
    Out.vColor.r = Out.vColor.r * vColor.r;
    Out.vColor.a = vColor.r;
    float fColor = saturate(In.vLifeTime.y - In.vLifeTime.x);
   
    Out.vEmissive = float4(0.7f, 0.05f, 0.05f, vColor.r);
    
    Out.vColor = Out.vColor * fColor;
    Out.vEmissive = Out.vEmissive * fColor;
    return Out;
}

technique11 DefaultTechnique
{
    pass DefaultPass
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }

}