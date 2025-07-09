matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
float2 g_MinUV, g_MaxUV;
float4 g_Vector;
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
    float4 vWorldPos : TEXCOORD1;
};


VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    
    float4x4 matWV, matWVP;

    matWV = mul(g_WorldMatrix, g_ViewMatrix);
    matWVP = mul(matWV, g_ProjMatrix);

    Out.vPosition = mul(float4(In.vPosition, 1.f), matWVP);
    Out.vTexcoord = In.vTexcoord;
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    
    return Out;
}

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

PS_OUT PS_Tex_UV(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
       
    In.vTexcoord = g_MinUV + (g_MaxUV - g_MinUV) * In.vTexcoord;
     
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    
    return Out;
}

PS_OUT PS_Color(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor = g_Vector;
   
    return Out;
}

PS_OUT PS_Inven_Bass(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    if (In.vPosition.x <= g_Vector.r || In.vPosition.x >= g_Vector.g || In.vPosition.y <= g_Vector.b || In.vPosition.y >= g_Vector.a)
        discard;
    
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    Out.vColor.a += 0.1;
    return Out;
}

PS_OUT PS_Tex(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    
    return Out;
}

technique11 DefaultTechnique
{
    pass UI_TexUV_Pass
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_Tex_UV();
    }

    pass UI_Color_Pass
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_Color();
    }

    pass UI_Invent_Pass
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_Inven_Bass();
    }

    pass UI_Tex_Pass
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_Tex();
    }

}