#include "Engine_Shader_Defines.hlsli"

float           g_blurValue = 2.f;

matrix          g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

//랜더 타겟 텍스처에 그려진 객체의 월드 위치를 계산하기 위해 필요
matrix          g_ViewMatrixInv, g_ProjMatrixInv;     
matrix          g_LightViewMatrix, g_LightProjMatrix;
texture2D       g_Texture;

vector          g_vCamPosition;

vector          g_vLightDir;
vector          g_vLightPos;
float           g_fRange;

vector          g_vLightDiffuse;
vector          g_vLightAmbient;
vector          g_vLightSpecular;

texture2D       g_DiffuseTexture;
vector          g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
vector          g_vMtrlSpecular = vector(0.f, 0.f, 0.f, 1.f);

//각 랜더 타겟에서 생성한 텍스처를 저장
texture2D       g_NormalTexture;
texture2D       g_DepthTexture;
texture2D       g_ShadeTexture;
texture2D       g_SpecularTexture;
texture2D       g_LightDepthTexture;
texture2D       g_StillLightDepthTexture;
texture2D       g_BackBufferTexture;

texture2D       g_EmissiveTexture;
texture2D       g_BlurXTexture;
texture2D       g_BlurYTexture;

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vTexcoord : TEXCOORD0;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    
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

struct PS_OUT_BACKBUFFER
{
    float4 vColor : SV_TARGET0;
};

PS_OUT_BACKBUFFER PS_MAIN_DEBUG(PS_IN In)
{
    PS_OUT_BACKBUFFER Out = (PS_OUT_BACKBUFFER) 0;
    
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    
    return Out;
}

struct PS_OUT_LIGHT
{
    vector vShade : SV_TARGET0;
    vector vSpecular : SV_TARGET1;
};

PS_OUT_LIGHT PS_MAIN_DIRECTIONAL(PS_IN In)
{
    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;
    
    vector vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexcoord);
    //텍스처로 변환된 0,0 ~ 1,1 좌표를 화면 좌표의 노말로 복원
    vector vNormal = normalize(vector(vNormalDesc.xyz * 2.f - 1.f, 0.f));
    
    float fShade = max(dot(vNormal * -1.f, normalize(g_vLightDir)), 0.f);
    
    vector vReflect = reflect(normalize(g_vLightDir), vNormal);
    vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexcoord);
    
    vector vWorldPos;
    
    //NDC 좌표로 변환
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;
    
    //투영 스페이스로 내리기
    vWorldPos = vWorldPos * vDepthDesc.y;
    
    //뷰 스페이스로 내리기
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
    
    //월드로 내리기
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    
    vector vLook = vWorldPos - g_vCamPosition;
    float fSpecular = pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 50.f);
    
    Out.vShade = g_vLightDiffuse * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient));
    Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;
    
    return Out;
}
PS_OUT_LIGHT PS_MAIN_POINT(PS_IN In)
{
    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;
    
    vector vNormalDesc = g_NormalTexture.Sample(DefaultSampler, In.vTexcoord);
    vector vNormal = normalize(vector(vNormalDesc.xyz * 2.f - 1.f, 0.f));
    
    vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexcoord);
    
    vector vWorldPos;
    
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;
    
    vWorldPos = vWorldPos * vDepthDesc.y;
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    
    vector vLightDir = vWorldPos - g_vLightPos;
    float fDistance = length(vLightDir);
    
    float fAtt = saturate((g_fRange - fDistance) / g_fRange);
    float fShade = max(dot(vNormal * -1.f, normalize(vLightDir)), 0.f);
    
    vector vReflect = reflect(normalize(vLightDir), vNormal);
    
   
    vector vLook = vWorldPos - g_vCamPosition;
    
    float fSpecular = pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 50.f);
    
    Out.vShade = g_vLightDiffuse * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient)) * fAtt;
    Out.vSpecular = (g_vLightSpecular * g_vMtrlSpecular) * fSpecular * fAtt;
    
    return Out;
}

PS_OUT_BACKBUFFER PS_MAIN_COMBINED(PS_IN In)
{
    PS_OUT_BACKBUFFER Out = (PS_OUT_BACKBUFFER) 0;
    
    vector vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
    if (vDiffuse.r == 1.f && vDiffuse.g == 0.f && vDiffuse.b == 1.f)
        discard;
    
    vector vShade = g_ShadeTexture.Sample(DefaultSampler, In.vTexcoord);
    vector vSpecular = g_SpecularTexture.Sample(DefaultSampler, In.vTexcoord);
    vector vDepthDesc = g_DepthTexture.Sample(DefaultSampler, In.vTexcoord);
    vector vWorldPos;
    
    vWorldPos.x = In.vTexcoord.x * 2.f - 1.f;
    vWorldPos.y = In.vTexcoord.y * -2.f + 1.f;
    vWorldPos.z = vDepthDesc.x;
    vWorldPos.w = 1.f;
    
    vWorldPos = vWorldPos * vDepthDesc.y;
    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);
    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);
    
    vector vPosition = mul(vWorldPos, g_LightViewMatrix);
    vPosition = mul(vPosition, g_LightProjMatrix);
    
    float2 vTexcoord;
    vTexcoord.x = (vPosition.x / vPosition.w) * 0.5f + 0.5f;
    vTexcoord.y = (vPosition.y / vPosition.w) * -0.5f + 0.5f;
    
    vector vLightDepth = g_LightDepthTexture.Sample(DefaultSampler, vTexcoord);
    vector vStillLightDepth = g_StillLightDepthTexture.Sample(DefaultSampler, vTexcoord);
    
    float fViewZ = vLightDepth.x * 1000.f;
    float fViewStillZ = vStillLightDepth.x * 1000.f;
    
    float ShadowFactor = 1.0f;
    if (vPosition.w - 0.1f > fViewStillZ)
        ShadowFactor = 0.4f;

    if (vPosition.w - 0.1f > fViewZ)
        ShadowFactor *= 0.7f;

    Out.vColor = (vDiffuse * vShade + vSpecular) * ShadowFactor;
    
    return Out;
}

float g_fWeights[13] =
{
    0.0561, 0.1353, 0.278, 0.4868, 0.7261, 0.9231, 1.f, 0.9231, 0.7261, 0.4868, 0.278, 0.1353, 0.0561
};
//float g_fWeights[13] =
//{
//    0.0022, 0.0086, 0.0270, 0.0656, 0.1209, 0.1747, 0.1974,
//    0.1747, 0.1209, 0.0656, 0.0270, 0.0086, 0.0022
//};


struct PS_OUT_BLUR_X
{
    vector vBlurX : SV_TARGET0;
};

PS_OUT_BLUR_X PS_MAIN_BLUR_X(PS_IN In)
{
    PS_OUT_BLUR_X Out;
    
    float2 vTexcoord = {0.f,0.f };
    vector vColor = { 0.f, 0.f, 0.f, 0.f };
    
    for (int i = -6; i < 7; ++i)
    {
        vTexcoord.x = In.vTexcoord.x + i / 1920.0f;
        vTexcoord.y = In.vTexcoord.y;
        
        vColor += g_fWeights[i + 6] * g_EmissiveTexture.Sample(DefaultSampler_CLAMP, vTexcoord);
    }
    
    Out.vBlurX = vColor / g_blurValue;
    
    return Out;
}

struct PS_OUT_BLUR_Y
{
    vector vBlurY : SV_TARGET0;
};

PS_OUT_BLUR_Y PS_MAIN_BLUR_Y(PS_IN In)
{
    PS_OUT_BLUR_Y Out;
    
    float2 vTexcoord = { 0.f, 0.f };
    vector vColor = {0.f,0.f,0.f,0.f };
    
    for (int i = -6; i < 7; ++i)
    {
        vTexcoord.x = In.vTexcoord.x;
        vTexcoord.y = In.vTexcoord.y + i / 1080.0f;
        
        vColor += g_fWeights[i + 6] * g_BlurXTexture.Sample(DefaultSampler_CLAMP, vTexcoord);
    }
    
    vector vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
    Out.vBlurY = vDiffuse + (vColor / g_blurValue);
    return Out;
}

technique11 DefaultTechnique
{
    pass Debug //0
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DEBUG();
    }

    pass Directional //1
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_DIRECTIONAL();
    }

    pass Point //2
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_POINT();
    }


    pass COMBINED //3
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_COMBINED();
    }

    pass BlurX //4
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLUR_X();
    }

    pass BlurY //5
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BLUR_Y();
    }

}