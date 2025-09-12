#include "Engine_Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D g_DiffuseTexture;
Texture2D g_DepthTexture;

// 안개 관련 변수들
float3 g_vFogColor = float3(0.4f, 0.4f, 0.4f);
float g_ffogStart = 12.f; // 안개 시작 거리
float g_ffogEnd = 100.f; // 안개 끝 거리  
float g_ffogDensity = 0.0035f; // 안개 밀도 (지수 안개용)
float g_fnearPlane = 1.f; // 카메라 near plane
float g_fFarPlane = 1000.f; // 카메라 far plane

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0;

    float4x4 matWV, matWVP;
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;

    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
};

// 깊이를 선형 거리로 변환 (Near~Far 실제 거리)
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // NDC로 변환
    return (2.0 * g_fnearPlane * g_fFarPlane) / (g_fFarPlane + g_fnearPlane - z * (g_fFarPlane - g_fnearPlane));
}

// 정규화된 깊이를 사용한 Linear Fog
// float CalculateLinearFogFromDepth(float depth)
// {
//     return saturate((g_fg_ffogEnd - depth) / (g_fg_ffogEnd - g_fg_ffogStart));
// }

// 정규화된 깊이를 사용한 Exponential Fog
// float CalculateExpFogFromDepth(float depth)
// {
//     float distance = LinearizeDepth(depth);
//     // float distance = depth;
//     return exp(-g_fg_ffogDensity * distance / g_fg_fFarPlane);
// }

PS_OUT PS_MAIN(PS_IN In)
{
    // 원본 색상 샘플링
    float4 vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    if (vDiffuse.r == 1.f && vDiffuse.g == 0.f && vDiffuse.b == 1.f && vDiffuse.a == 0.f)
    {
        discard;
    }
    // 깊이값 샘플링 및 선형화
    float depth = g_DepthTexture.Sample(DefaultSampler, In.vTexcoord).r;
    
    
    float linearDepth = LinearizeDepth(depth);

    // 안개 계산 방법 선택
    float fogFactor = 1.0;

    // 1. 선형 안개 (Linear Fog)
    // fogFactor = saturate((g_ffogEnd - linearDepth) / (g_ffogEnd - g_ffogStart));

    // 2. 지수 안개 (Exponential Fog) - 주석 해제하여 사용
    // fogFactor = exp(-g_ffogDensity * linearDepth);

    // 3. 지수 제곱 안개 (Exponential Squared Fog) - 주석 해제하여 사용
    fogFactor = exp(-pow(g_ffogDensity * linearDepth, 2.0));

    float4 vColor;
    vColor.rgb = lerp(g_vFogColor, vDiffuse.rgb, fogFactor);
    vColor.a = vDiffuse.a;

    PS_OUT Out;
    
    Out.vColor = vColor;
    return Out;
    // // 최종 색상 블렌딩
    // return lerp(g_vFogColor, fOrignalColor, fogFactor);
}

technique11 DefaultTechnique
{
    pass FogPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
          
    }
}
