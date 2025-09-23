#include "Engine_Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
texture2D   g_DiffuseTexture;

vector  g_vCamPosition;
int     g_iTexValueX;
int     g_iTexValueY;

float g_Alpha;
struct VS_IN
{
    float3 vPosition : POSITION;
    row_major float4x4 TransformMatrix : WORLD;   
    float2 vLifeTime : TEXCOORD0;
    float vSplat : TEXCOORD1;
};

struct VS_OUT
{
    float4 vPosition : POSITION;
    float fSize : PSIZE;
    float2 vLifeTime : TEXCOORD0;
    float vSplat : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    vector vPosition = mul(float4(In.vPosition, 1.f), In.TransformMatrix);
    
    Out.vPosition = mul(vPosition, g_WorldMatrix);
    Out.fSize = length(In.TransformMatrix._11_22_33);
    Out.vLifeTime = In.vLifeTime;
    Out.vSplat = In.vSplat;
    
    return Out;
}

//기하 셰이더
struct GS_IN
{
    float4 vPosition : POSITION;
    float fSize : PSIZE;
    float2 vLifeTime : TEXCOORD0;
    float vSplat : TEXCOORD1;
};

struct GS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vLifeTime : TEXCOORD1;
    float vSplat : TEXCOORD2;
};

[maxvertexcount(6)]//삼각형 2개를 만들기 위해 6으로 지정
void GS_MAIN(point GS_IN In[1], inout TriangleStream<GS_OUT> Vertices)
{
    GS_OUT Out[4];
    
    vector vRight, vUp, vLook;
    
    vLook = g_vCamPosition - In[0].vPosition;
    vRight = normalize(vector(cross(float3(0.f, 1.f, 0.f), vLook.xyz), 0.f)) * In[0].fSize * 0.5f;
    vUp = normalize(vector(cross(vLook.xyz, vRight.xyz), 0.f)) * In[0].fSize * 0.5f;
    
    matrix matVP = mul(g_ViewMatrix, g_ProjMatrix);
    
    Out[0].vPosition = mul(In[0].vPosition + vRight + vUp, matVP);
    Out[0].vTexcoord = float2(0.f, 0.f);
    Out[0].vLifeTime = In[0].vLifeTime;
    Out[0].vSplat = In[0].vSplat;
    
    Out[1].vPosition = mul(In[0].vPosition - vRight + vUp, matVP);
    Out[1].vTexcoord = float2(1.f, 0.f);
    Out[1].vLifeTime = In[0].vLifeTime;
    Out[1].vSplat = In[0].vSplat;
    
    Out[2].vPosition = mul(In[0].vPosition - vRight - vUp, matVP);
    Out[2].vTexcoord = float2(1.f, 1.f);
    Out[2].vLifeTime = In[0].vLifeTime;
    Out[2].vSplat = In[0].vSplat;
    
    Out[3].vPosition = mul(In[0].vPosition + vRight - vUp, matVP);
    Out[3].vTexcoord = float2(0.f, 1.f);
    Out[3].vLifeTime = In[0].vLifeTime;
    Out[3].vSplat = In[0].vSplat;
    
    Vertices.Append(Out[0]);
    Vertices.Append(Out[1]);
    Vertices.Append(Out[2]);
    Vertices.RestartStrip();
    
    Vertices.Append(Out[0]);
    Vertices.Append(Out[2]);
    Vertices.Append(Out[3]);
    Vertices.RestartStrip();
}
[maxvertexcount(6)]
void GS_MAIN_UI(point GS_IN In[1], inout TriangleStream<GS_OUT> Vertices)
{
    GS_OUT Out[4];
    
    vector vRight = normalize(float4(g_ViewMatrix._11, g_ViewMatrix._12, g_ViewMatrix._13, 0.f)) * In[0].fSize * 0.5f;
    vector vUp = normalize(float4(g_ViewMatrix._21, g_ViewMatrix._22, g_ViewMatrix._23, 0.f)) * In[0].fSize * 0.5f;

    matrix matVP = mul(g_ViewMatrix, g_ProjMatrix);

    Out[0].vPosition = mul(In[0].vPosition + vRight + vUp, matVP);
    Out[0].vTexcoord = float2(0.f, 0.f);
    Out[0].vLifeTime = In[0].vLifeTime;
    Out[0].vSplat = In[0].vSplat;
    
    Out[1].vPosition = mul(In[0].vPosition - vRight + vUp, matVP);
    Out[1].vTexcoord = float2(1.f, 0.f);
    Out[1].vLifeTime = In[0].vLifeTime;
    Out[1].vSplat = In[0].vSplat;
    
    Out[2].vPosition = mul(In[0].vPosition - vRight - vUp, matVP);
    Out[2].vTexcoord = float2(1.f, 1.f);
    Out[2].vLifeTime = In[0].vLifeTime;
    Out[2].vSplat = In[0].vSplat;
    
    Out[3].vPosition = mul(In[0].vPosition + vRight - vUp, matVP);
    Out[3].vTexcoord = float2(0.f, 1.f);
    Out[3].vLifeTime = In[0].vLifeTime;
    Out[3].vSplat = In[0].vSplat;
    
    Vertices.Append(Out[0]);
    Vertices.Append(Out[1]);
    Vertices.Append(Out[2]);
    Vertices.RestartStrip();
    
    Vertices.Append(Out[0]);
    Vertices.Append(Out[2]);
    Vertices.Append(Out[3]);
    Vertices.RestartStrip();
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float2 vLifeTime : TEXCOORD1;
    float vSplat : TEXCOORD2;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
    
};

struct PS_EMISSIVE
{
    float4 vColor : SV_TARGET0;
    float4 vEmissive : SV_TARGET1;
};


PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
        
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
    if (Out.vColor.a < 0.3f)
        discard;
    
    Out.vColor = 1.f;
    
    return Out;
}

PS_OUT PS_SNOW(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
       
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
    Out.vColor.a = Out.vColor.r * 0.6f;

    Out.vColor.rgb = 0.5f;
    
    return Out;
}

PS_OUT PS_SMOKE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    int iTotalFrame = g_iTexValueX * g_iTexValueY;
    int iTexIndex = In.vLifeTime.x * iTotalFrame;
    
    //0.5 
    int iX = (iTexIndex % g_iTexValueX);
    int iY = (iTexIndex / g_iTexValueX);
    
    float2 vUVMin = { 0.f, 0.f };
    float2 vUVMax = { 0.f, 0.f };
    
    vUVMin.x = (float)iX / g_iTexValueX;
    vUVMin.y = (float) iY / g_iTexValueY;
    vUVMax.x = (float) (iX + 1) / g_iTexValueX;
    vUVMax.y = (float) (iY + 1) / g_iTexValueY;
    

    In.vTexcoord = vUVMin + (vUVMax - vUVMin) * In.vTexcoord;
    
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler_CLAMP, In.vTexcoord);
    if (Out.vColor.a < 0.01f)
        discard;
    
    float fColor = saturate(In.vLifeTime.y - In.vLifeTime.x);
    Out.vColor.a = Out.vColor.a * fColor * 0.25f;
    
    return Out;
}

PS_OUT PS_BLODE(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    int iTotalFrame = g_iTexValueX * g_iTexValueY;
    int iTexIndex = In.vLifeTime.x * iTotalFrame;
    
    //0.5 
    int iX = (iTexIndex % g_iTexValueX);
    int iY = (iTexIndex / g_iTexValueX);
    
    float2 vUVMin = { 0.f, 0.f };
    float2 vUVMax = { 0.f, 0.f };
    
    vUVMin.x = (float) iX / g_iTexValueX;
    vUVMin.y = (float) iY / g_iTexValueY;
    vUVMax.x = (float) (iX + 1) / g_iTexValueX;
    vUVMax.y = (float) (iY + 1) / g_iTexValueY;
    

    In.vTexcoord = vUVMin + (vUVMax - vUVMin) * In.vTexcoord;
    
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler_CLAMP, In.vTexcoord);
    if (Out.vColor.a < 0.01f)
        discard;
    Out.vColor.r = Out.vColor.r * 0.545f;
    Out.vColor.g = Out.vColor.g * 0.01f;
    Out.vColor.b = Out.vColor.b * 0.01f;
    
    float fColor = saturate(In.vLifeTime.y - In.vLifeTime.x);
    Out.vColor.a = Out.vColor.a * fColor * 0.5f;
    
    return Out;
}

PS_OUT PS_Blood_Splatter(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    int iTotalFrame = g_iTexValueX * g_iTexValueY;
    int iTexIndex = In.vSplat * iTotalFrame;
    
    int iX = (iTexIndex % g_iTexValueX);
    int iY = (iTexIndex / g_iTexValueX);
    
    float2 vUVMin = { 0.f, 0.f };
    float2 vUVMax = { 0.f, 0.f };
    
    vUVMin.x = (float) iX / g_iTexValueX;
    vUVMin.y = (float) iY / g_iTexValueY;
    vUVMax.x = (float) (iX + 1) / g_iTexValueX;
    vUVMax.y = (float) (iY + 1) / g_iTexValueY;
    

    In.vTexcoord = vUVMin + (vUVMax - vUVMin) * In.vTexcoord;
    
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler_CLAMP, In.vTexcoord);
    if (Out.vColor.a < 0.01f)
        discard;
    
    if (Out.vColor.r < 0.6f)
    {
        Out.vColor.r = Out.vColor.r * 1.0f;
        Out.vColor.g = Out.vColor.r * 0.1f;
        Out.vColor.b = Out.vColor.r * 0.1f;
    }
    else
    {
        Out.vColor.r = Out.vColor.r * 0.6f;
        Out.vColor.g = Out.vColor.r * 0.05f;
        Out.vColor.b = Out.vColor.r * 0.05f;
    }
    float fColor = saturate(In.vLifeTime.y - In.vLifeTime.x);
    Out.vColor.a = Out.vColor.a * (fColor * 1.5f);
    
    return Out;
}

PS_EMISSIVE PS_Spark(PS_IN In)
{
    PS_EMISSIVE Out = (PS_EMISSIVE) 0;

    int iTotalFrame = g_iTexValueX * g_iTexValueY;
    int iTexIndex = In.vLifeTime.x * iTotalFrame;
    
    //0.5 
    int iX = (iTexIndex % g_iTexValueX);
    int iY = (iTexIndex / g_iTexValueX);
    
    float2 vUVMin = { 0.f, 0.f };
    float2 vUVMax = { 0.f, 0.f };
    
    vUVMin.x = (float) iX / g_iTexValueX;
    vUVMin.y = (float) iY / g_iTexValueY;
    vUVMax.x = (float) (iX + 1) / g_iTexValueX;
    vUVMax.y = (float) (iY + 1) / g_iTexValueY;
    

    In.vTexcoord = vUVMin + (vUVMax - vUVMin) * In.vTexcoord;
    
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler_CLAMP, In.vTexcoord);
    if (Out.vColor.a < 0.01f)
        discard;
    if (Out.vColor.r > 0.65f && Out.vColor.g > 0.65f)
        Out.vColor.rg = Out.vColor.rg * 3.f;
    float fColor = saturate(In.vLifeTime.y - In.vLifeTime.x);
    Out.vColor.a = Out.vColor.a * fColor * 0.5f;
    Out.vEmissive = Out.vColor;
    
    return Out;
}

PS_OUT PS_ScreenBlood_Splatter(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    int iTotalFrame = g_iTexValueX * g_iTexValueY;
    int iTexIndex = In.vSplat * iTotalFrame;
    
    int iX = (iTexIndex % g_iTexValueX);
    int iY = (iTexIndex / g_iTexValueX);
    
    float2 vUVMin = { 0.f, 0.f };
    float2 vUVMax = { 0.f, 0.f };
    
    vUVMin.x = (float) iX / g_iTexValueX;
    vUVMin.y = (float) iY / g_iTexValueY;
    vUVMax.x = (float) (iX + 1) / g_iTexValueX;
    vUVMax.y = (float) (iY + 1) / g_iTexValueY;
    
    In.vTexcoord = vUVMin + (vUVMax - vUVMin) * In.vTexcoord;
    
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler_CLAMP, In.vTexcoord);
    if (Out.vColor.a < 0.01f)
        discard;
    
    if (Out.vColor.r < 0.6f)
    {
        Out.vColor.r = Out.vColor.r * 0.2f;
        Out.vColor.g = Out.vColor.r * 0.05f;
        Out.vColor.b = Out.vColor.r * 0.05f;
    }
    else
    {
        Out.vColor.r = Out.vColor.r * 0.2f;
        Out.vColor.g = Out.vColor.r * 0.05f;
        Out.vColor.b = Out.vColor.r * 0.05f;
    }
    float fColor = saturate(In.vLifeTime.y - In.vLifeTime.x);
    Out.vColor.a = Out.vColor.a * (fColor * 1.5f) * g_Alpha;

    return Out;
}

technique11 DefaultTechnique
{
    pass DefaultPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
       
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass SnowPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
       
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_SNOW();
    }

    pass SmokePass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Effect, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
       
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_SMOKE();
    }

    pass BlodePass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Effect, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
       
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_BLODE();
    }
    pass SplatterPass   //4
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Effect, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
       
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_Blood_Splatter();
    }
    pass SparkPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Effect, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
       
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN();
        PixelShader = compile ps_5_0 PS_Spark();
    }
    pass ScreenSplatterPass
    {
        SetRasterizerState(RS_Cull_None);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_AlphaBlend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
       
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = compile gs_5_0 GS_MAIN_UI();
        PixelShader = compile ps_5_0 PS_ScreenBlood_Splatter();
    }

}

