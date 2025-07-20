
matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

vector g_vLightDir = vector(1.f, -1.f, 1.f, 0.f);
vector g_vLightDiffuse = vector(1.f, 1.f, 1.f, 1.f);
vector g_vLightAmbient = vector(0.4f, 0.4f, 0.4f, 1.f);
vector g_vLightSpecular = vector(1.f, 1.f, 1.f, 1.f);

vector g_vCamPosition;

texture2D   g_DiffuseTexture;
vector      g_vMtrlAmbient = 1.f;
vector      g_vMtrlSpecular = 1.f;

sampler DefaultSampler = sampler_state
{
    filter = min_mag_mip_linear;
    AddressU = wrap;
    AddressV = wrap;
};

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    float4x4 matWV, matWVP;
    
    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);
    
    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vNormal = mul(float4(In.vNormal, 0.f), g_WorldMatrix);
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    
    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal   : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;
};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
    
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord * 50.f);
    //vector vMtrlDiffuse = g_DiffuseTexture.Sample(DefaultSampler, In.vTexcoord * 50.f);
    
    //�� ������ ������ * ��ü�� ���� ���͸� �����ؼ� cos(��Ÿ)�� ���ؼ� ���� ���⸦ ����
    //float fShade = max(dot(normalize(g_vLightDir) * -1.f, normalize(In.vNormal)), 0.f);
    
    //���� �ݻ簢 : reflect 
    //���� �ݻ簢 ���� : ���� ���� ���� + (����(���� ���� ����(�븻) * -1.F, ��ü�� ����) * ���� * 2)
    //vector vReflect = reflect(normalize(g_vLightDir), In.vNormal);
    
    //vector vLightDirR = normalize(g_vLightDir);
    //vector vReflect = normalize(g_vLightDir) + abs(dot(vLightDirR, normalize(In.vNormal))) * (normalize(In.vNormal) * 2);
   
    //vector vReflect = normalize(g_vLightDir) + dot(vLightDirR, normalize(In.vNormal)) * (normalize(In.vNormal) * 2);
   
    //ī�޶� ���� ������ ���� ��ķ� ��ȯ
    //vector vLook = In.vWorldPos - g_vCamPosition;
    
    //����ŧ��
    //float fSpecular = pow(max(dot(normalize(vLook) * -1.f, normalize(vReflect)), 0.f), 10.f);
    
    //�� ��ǻ�� * ��ü ��ǻ��  = ��� ��ü�� ���� ������ ����
    //���� ��ǻ�� * (���� ���� + ���� ȯ�汤 * ��ü�� ȯ�汤) ���� ���� �� ���� ȯ�汤�� ��������
    //Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * saturate(fShade + (g_vLightAmbient * g_vMtrlAmbient))
    //                + (g_vLightSpecular * g_vMtrlSpecular) * fSpecular;
    
    
    //���� �� * ��ü�� �� * (��� �ڻ��� + ���� ȯ�汤 * ��ü ȯ�汤(1.f)) + ���� ����ŧ�� * ��ü�� ����ŧ�� * ����ŧ�� �ڻ���
    //��� = ���� ���� ������ * ���� 
    //����ŧ�� = ���� ���⿡ ������ * ���� �ݻ簢 
    
    return Out;

}



technique11 DefaultTechnique
{
    pass DefaultPass
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}

