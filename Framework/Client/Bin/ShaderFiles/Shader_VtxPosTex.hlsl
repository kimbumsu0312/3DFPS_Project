//사용할 데이터를 전역 변수로 선언
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
Texture2D g_Texture;
sampler DefaultSampler = sampler_state
{
    filter = min_mag_mip_linear;
};

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    //월드 좌표를 텍스처 값으로 전달
    float4 vWorldPos : TEXCOORD1;
};


VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    
    float4x4 matWV, matWVP;
    
    // 로컬 * 월드 * 뷰 * 투영
    // 월드 * 뷰 * 투영 
    
    //뷰 스페이스 변환 행렬
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    //클립 스페이스 변환 행렬
    matWVP = mul(matWV, g_ProjMatrix);
   
    //로컬 스페이스를 클립 스페이스로 변환해서 저장 해준다.
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    
    return Out;
}

// SV_POSITION을 이용해서 컴퓨터한테 연산을 시작시킨다.
// 컴퓨터가 W 나누기를 수행하고 투영 스페이스로 변환
// 컴퓨터가 뷰포트 변환
// 레스터 라이즈를 해준다.


// 픽셀 셰이더
// 픽셀의 색을 결정해준다.
struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    //Out.vColor.rgb = In.vTexcoord.y;
    
    //텍스처 셋팅 - 텍스처 타입, 텍스처 픽셀 색상 값
    //Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord * 2.f);
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    
    return Out;
}

// 어떤 셰이더를 사용할 건지 정의한다.
// 정점의 정보에 따라 패스를 추가할 수 있다.
technique11 DefaultTechnique
{
    pass DefaultPass
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();

    }
}