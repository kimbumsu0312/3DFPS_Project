matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix, g_PenalMatrix;
float g_Alpha;
float2 g_MinUV, g_MaxUV;
float4 g_Vector, g_Size, g_vColor;
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
    
    In.vTexcoord = g_MinUV + (g_MaxUV - g_MinUV) * In.vTexcoord;
     
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    return Out;
}

PS_OUT PS_Tex(PS_IN In)
{
    
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    
    return Out;
}

PS_OUT PS_Tex_UV_ApllyAlpha(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    if (In.vPosition.x <= g_Vector.r || In.vPosition.x >= g_Vector.g || In.vPosition.y <= g_Vector.b || In.vPosition.y >= g_Vector.a)
        discard;
    
    In.vTexcoord = g_MinUV + (g_MaxUV - g_MinUV) * In.vTexcoord;
     
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    Out.vColor.a = Out.vColor.a * g_Alpha;
    return Out;
}

PS_OUT PS_Color_ApllyAlpha(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    Out.vColor = g_Vector;
    Out.vColor.a = Out.vColor.a * g_Alpha;
    return Out;
}

PS_OUT PS_Hp_Flash(PS_IN In)
{
    
    PS_OUT Out = (PS_OUT) 0;
    if (In.vPosition.x <= g_Vector.r || In.vPosition.x >= g_Vector.g || In.vPosition.y <= g_Vector.b || In.vPosition.y >= g_Vector.a)
        discard;
    In.vTexcoord = g_MinUV + (g_MaxUV - g_MinUV) * In.vTexcoord;
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    
    Out.vColor.rgb = Out.vColor.rgb + g_vColor.rgb;
    Out.vColor.a = Out.vColor.a * g_vColor.w * g_Alpha;
    return Out;
}

PS_OUT PS_Hp_Beat(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    if (In.vPosition.x <= g_Vector.r || In.vPosition.x >= g_Vector.g || In.vPosition.y <= g_Vector.b || In.vPosition.y >= g_Vector.a)
        discard;
    
    if (g_Size.x >= In.vPosition.x || g_Size.z <= In.vPosition.x)
         discard;
     
    In.vTexcoord = g_MinUV + (g_MaxUV - g_MinUV) * In.vTexcoord;
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    Out.vColor.rgb = -Out.vColor.rgb * g_vColor.rgb + g_vColor.rgb;

    Out.vColor.a = Out.vColor.a * g_vColor.w * g_Alpha;

    return Out;
}

PS_OUT PS_Quick_Slot(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    In.vTexcoord = g_MinUV + (g_MaxUV - g_MinUV) * In.vTexcoord;
     
    Out.vColor = g_Texture.Sample(DefaultSampler, In.vTexcoord);
    Out.vColor.a = Out.vColor.a * g_Alpha;
    return Out;
}

technique11 DefaultTechnique
{
    pass UI_TexUV_Pass_0
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_Tex_UV();
    }

    pass UI_Color_Pass_1
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_Color();
    }

    pass UI_Inven_Pass_2
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_Inven_Bass();
    }

    pass UI_Tex_Pass_3
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_Tex();
    }

    pass UI_TexUV_AlphaAplly_Pass_4
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_Tex_UV_ApllyAlpha();
    }

    pass UI_Color_AlphaAplly_Pass_5
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_Color_ApllyAlpha();
    }

    pass UI_Hp_Flash_Pass_6
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_Hp_Flash();
    }

    pass UI_Hp_Beat_Pass_7
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_Hp_Beat();
    }

    pass UI_QuickSlot_Pass_8
    {
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_Quick_Slot();
    }
}