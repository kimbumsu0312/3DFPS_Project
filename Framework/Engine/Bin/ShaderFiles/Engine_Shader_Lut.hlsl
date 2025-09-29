#include "Engine_Shader_Defines.hlsli"

matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D g_DiffuseTexture;
Texture2D g_LutTexture;

const float size = 8.0f;

float2 CalcUV(float r, float g, float b, float size)
{
    // size = 8 (LUT 해상도)
    // R: X축, G: 세로 블록 안 위치, B: 블록 인덱스
    float slice = floor(b); // 현재 B 슬라이스
    float sliceNext = min(slice + 1, size - 1); // 다음 B 슬라이스

    // G,B를 세로(Y축)에 매핑
    float y0 = (g + slice * size + 0.5f) / (size * size);
    float y1 = (g + sliceNext * size + 0.5f) / (size * size);

    // R을 가로(X축)에 매핑
    float x = (r + 0.5f) / size;

    return float2(x, y0); // 기본 UV (Z축 보간 시 y0,y1 둘 다 사용)
}

float3 ApplyLUT(float3 color)
{
    const float size = 8.0f;

    // 입력 색상(0~1)을 LUT 좌표계(0~7)로 변환
    float3 coord = saturate(color) * (size - 1.0f);
    float3 indexFloor = floor(coord);
    float3 indexFrac = frac(coord);

    // 8개 코너 샘플링
    float3 c000 = g_LutTexture.Sample(DefaultSampler_CLAMP, CalcUV(indexFloor.x, indexFloor.y, indexFloor.z, size)).rgb;
    float3 c100 = g_LutTexture.Sample(DefaultSampler_CLAMP, CalcUV(indexFloor.x + 1, indexFloor.y, indexFloor.z, size)).rgb;
    float3 c010 = g_LutTexture.Sample(DefaultSampler_CLAMP, CalcUV(indexFloor.x, indexFloor.y + 1, indexFloor.z, size)).rgb;
    float3 c110 = g_LutTexture.Sample(DefaultSampler_CLAMP, CalcUV(indexFloor.x + 1, indexFloor.y + 1, indexFloor.z, size)).rgb;
    float3 c001 = g_LutTexture.Sample(DefaultSampler_CLAMP, CalcUV(indexFloor.x, indexFloor.y, indexFloor.z + 1, size)).rgb;
    float3 c101 = g_LutTexture.Sample(DefaultSampler_CLAMP, CalcUV(indexFloor.x + 1, indexFloor.y, indexFloor.z + 1, size)).rgb;
    float3 c011 = g_LutTexture.Sample(DefaultSampler_CLAMP, CalcUV(indexFloor.x, indexFloor.y + 1, indexFloor.z + 1, size)).rgb;
    float3 c111 = g_LutTexture.Sample(DefaultSampler_CLAMP, CalcUV(indexFloor.x + 1, indexFloor.y + 1, indexFloor.z + 1, size)).rgb;

    // 삼선형 보간 (X → Y → Z)
    float3 c00 = lerp(c000, c100, indexFrac.x);
    float3 c10 = lerp(c010, c110, indexFrac.x);
    float3 c01 = lerp(c001, c101, indexFrac.x);
    float3 c11 = lerp(c011, c111, indexFrac.x);

    float3 c0 = lerp(c00, c10, indexFrac.y);
    float3 c1 = lerp(c01, c11, indexFrac.y);

    return lerp(c0, c1, indexFrac.z);
}

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

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out;
    
    float4 vDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord);
    
        
    if (vDiffuse.r == 1.f && vDiffuse.g == 0.f && vDiffuse.b == 1.f)
    {
        discard;
    }
  
    float3 vLut = ApplyLUT(vDiffuse.rgb);
    
    float4 vColor;
    
    vColor.rgb = vDiffuse.rgb * 0.8f + vLut * 0.2f;
    vColor.a = 1.f;
    
    Out.vColor = vColor;
    return Out;
}

technique11 DefaultTechnique
{
    pass LutPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_None, 0);
        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
        
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
          
    }
}
