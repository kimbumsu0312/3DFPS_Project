//����� �����͸� ���� ������ ����
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
    //���� ��ǥ�� �ؽ�ó ������ ����
    float4 vWorldPos : TEXCOORD1;
};


VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    
    float4x4 matWV, matWVP;
    
    // ���� * ���� * �� * ����
    // ���� * �� * ���� 
    
    //�� �����̽� ��ȯ ���
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    //Ŭ�� �����̽� ��ȯ ���
    matWVP = mul(matWV, g_ProjMatrix);
   
    //���� �����̽��� Ŭ�� �����̽��� ��ȯ�ؼ� ���� ���ش�.
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    
    return Out;
}

// SV_POSITION�� �̿��ؼ� ��ǻ������ ������ ���۽�Ų��.
// ��ǻ�Ͱ� W �����⸦ �����ϰ� ���� �����̽��� ��ȯ
// ��ǻ�Ͱ� ����Ʈ ��ȯ
// ������ ����� ���ش�.


// �ȼ� ���̴�
// �ȼ��� ���� �������ش�.
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
    
    //�ؽ�ó ���� - �ؽ�ó Ÿ��, �ؽ�ó �ȼ� ���� ��
    //Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord * 2.f);
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    
    return Out;
}

// � ���̴��� ����� ���� �����Ѵ�.
// ������ ������ ���� �н��� �߰��� �� �ִ�.
technique11 DefaultTechnique
{
    pass DefaultPass
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();

    }
}