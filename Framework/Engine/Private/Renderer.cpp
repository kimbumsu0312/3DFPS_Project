#include "pch.h"
#include "Renderer.h"
#include "GameObject.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : m_pDevice { pDevice }, m_pContext { pContext }, m_pGameInstance{ CGameInstance::GetInstance() }
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
    Safe_AddRef(m_pGameInstance);
}

HRESULT CRenderer::Initialize()
{
    _uint iNumViewports = 1;
    D3D11_VIEWPORT ViewportDesc{};

    m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

    //랜더 타겟 뷰 생성
    if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Diffuse"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(1.f, 0.f, 1.f, 0.f))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Normal"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Depth"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 0.f, 1.f))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Shade"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Specular"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(0.f, 0.f, 0.f, 0.f))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Combined"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(1.f, 0.f, 1.f, 0.f))))
        return E_FAIL;

    //게임 오브젝트들의 정보를 저장 받음
    //각 랜더 타겟 뷰에 필요한 정보들을 담아두고 이후 필요한 연산에서 꺼내서 쓸수 있게한다.
    if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse"))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal"))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Depth"))))
        return E_FAIL;

    //빛 연산을 위한 랜더 타겟 뷰 셋팅
    //빛 연산 시, 빛 별로 연산 결과를 누적해서 저장함
    if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Combined"), TEXT("Target_Combined"))))
        return E_FAIL;


    m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
    if (nullptr == m_pVIBuffer)
        return E_FAIL;

    //각 랜더 타겟에 필요한 정보들을 저장하기 위한 셰이더
    m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Engine_Shader_Deferred.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
    if (nullptr == m_pShader)
        return E_FAIL;

    m_pFogShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Engine_Shader_Fog.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
    if (nullptr == m_pFogShader)
        return E_FAIL;

    XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(ViewportDesc.Width, ViewportDesc.Height, 1.f));
    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));

#ifdef _DEBUG
    if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Diffuse"), 100.0f, 100.0f, 200.f, 200.f)))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Normal"), 100.0f, 300.0f, 200.f, 200.f)))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Shade"), 300.0f, 100.0f, 200.f, 200.f)))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Specular"), 300.0f, 300.0f, 200.f, 200.f)))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Combined"), 500.0f, 100.0f, 200.f, 200.f)))
        return E_FAIL;
#endif // _DEBUG

    return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject* pRenderObject)
{
    if (nullptr == pRenderObject) 
        return E_FAIL;

    m_RenderObjects[ENUM_CLASS(eRenderGroup)].push_back(pRenderObject);

    Safe_AddRef(pRenderObject);
    
    return S_OK;
}

HRESULT CRenderer::Draw()
{
    if (FAILED(Render_Priority()))
        return E_FAIL;

    if (FAILED(Render_NonBlend()))
        return E_FAIL;

    if (FAILED(Render_Lights()))
        return E_FAIL;

    if (FAILED(Render_Combined()))
        return E_FAIL;

    if (FAILED(Render_NonLight()))
        return E_FAIL;

    if (FAILED(Render_Blend()))
        return E_FAIL;

    if (FAILED(Render_Effect()))
        return E_FAIL;

    if (FAILED(Render_Fog()))
        return E_FAIL;

    if (FAILED(Render_UI()))
        return E_FAIL;
    
    if (FAILED(Render_UI_Effect()))
        return E_FAIL;
    
    if (FAILED(Render_Last()))
        return E_FAIL;

#ifdef _DEBUG
    if (FAILED(Render_Debug()))
        return E_FAIL;
#endif // _DEBUG

    return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderer::Add_DebugComponent(CComponent* pComponent)
{
    m_DebugComponent.push_back(pComponent);

    Safe_AddRef(pComponent);

    return S_OK;
}

void CRenderer::IsDebugRender(DEBUG_RENDER eTag)
{
    switch (eTag)
    {
    case DEBUG_RENDER::RT:
        m_bRTRender ? m_bRTRender = false : m_bRTRender = true;
        break;
    case DEBUG_RENDER::COMPONET:
        m_bComponetRender ? m_bComponetRender = false : m_bComponetRender = true;
        break;
    }
}
#endif 
HRESULT CRenderer::Render_Priority()
{
    for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::PRIORITY)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();

        Safe_Release(pRenderObject);
    }

    m_RenderObjects[ENUM_CLASS(RENDERGROUP::PRIORITY)].clear();
    return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
    //게임 오브젝트 랜더 타겟들 바인드
    if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_GameObjects"))))
        return E_FAIL;

    for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::NONBLEND)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();

        Safe_Release(pRenderObject);
    }

    m_RenderObjects[ENUM_CLASS(RENDERGROUP::NONBLEND)].clear();

    //다시 원래 랜더 타겟으로 바꾼다.
    if (FAILED(m_pGameInstance->End_MRT()))
        return E_FAIL;
    return S_OK;
}

HRESULT CRenderer::Render_Lights()
{
    //라이트 랜더 타겟들 바인드
    if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_LightAcc"))))
        return E_FAIL;

    if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;
    if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrixInv", m_pGameInstance->Get_Transform_Float4x4_Inverse(D3DTS::VIEW))))
        return E_FAIL;
    if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrixInv", m_pGameInstance->Get_Transform_Float4x4_Inverse(D3DTS::PROJ))))
        return E_FAIL;

    if (FAILED(m_pShader->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
        return E_FAIL;

    //노말 랜더 타겟 텍스처를 셰이더에 바인드
    if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Normal"), m_pShader, "g_NormalTexture")))
        return E_FAIL;

    //뎁스 랜더 타겟 텍스처를 셰이더에 바인드
    if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Depth"), m_pShader, "g_DepthTexture")))
        return E_FAIL;

    //빛 정보들을 셰이더에 바인드
    m_pGameInstance->Render_Lights(m_pShader, m_pVIBuffer);
  
    if (FAILED(m_pGameInstance->End_MRT()))
        return E_FAIL;

    return S_OK;
}

HRESULT CRenderer::Render_Combined()
{
    if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Combined"))))
        return E_FAIL;

    if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    //각 랜더 타겟들을 셰이더에 바인딩
    if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Diffuse"), m_pShader, "g_DiffuseTexture")))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Shade"), m_pShader, "g_ShadeTexture")))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Specular"), m_pShader, "g_SpecularTexture")))
        return E_FAIL;

    m_pShader->Begin(3);

    m_pVIBuffer->Bind_Resources();
    m_pVIBuffer->Render();
    return S_OK;
}

HRESULT CRenderer::Render_NonLight()
{
    for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::NONLIGHT)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();

        Safe_Release(pRenderObject);
    }

    m_RenderObjects[ENUM_CLASS(RENDERGROUP::NONLIGHT)].clear();

    return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
    for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::BLEND)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();

        Safe_Release(pRenderObject);
    }

    m_RenderObjects[ENUM_CLASS(RENDERGROUP::BLEND)].clear();
    
    return S_OK;
}

HRESULT CRenderer::Render_Fog()
{
    //실제 랜더
    if (FAILED(m_pGameInstance->End_MRT()))
        return E_FAIL;

    if (FAILED(m_pFogShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pFogShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pFogShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Combined"), m_pFogShader, "g_DiffuseTexture")))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(TEXT("Target_Depth"), m_pFogShader, "g_DepthTexture")))
        return E_FAIL;

    m_pFogShader->Begin(0);
    m_pVIBuffer->Bind_Resources();
    m_pVIBuffer->Render();

    return S_OK;
}

HRESULT CRenderer::Render_Effect()
{
    for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::EFFECT)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();

        Safe_Release(pRenderObject);
    }

    m_RenderObjects[ENUM_CLASS(RENDERGROUP::EFFECT)].clear();

    return S_OK;
}

HRESULT CRenderer::Render_UI()
{
    for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::PRIORITY_UI)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();

        Safe_Release(pRenderObject);
    }

    for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::UI)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();

        Safe_Release(pRenderObject);
    }

    for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::LATE_UI)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();

        Safe_Release(pRenderObject);
    }
    m_RenderObjects[ENUM_CLASS(RENDERGROUP::PRIORITY_UI)].clear();
    m_RenderObjects[ENUM_CLASS(RENDERGROUP::UI)].clear();
    m_RenderObjects[ENUM_CLASS(RENDERGROUP::LATE_UI)].clear();
    return S_OK;
}

HRESULT CRenderer::Render_UI_Effect()
{

    for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::UI_EFFECT)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();

        Safe_Release(pRenderObject);
    }

    m_RenderObjects[ENUM_CLASS(RENDERGROUP::UI_EFFECT)].clear();

    return S_OK;
}

HRESULT CRenderer::Render_Last()
{

    for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::LAST)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();

        Safe_Release(pRenderObject);
    }

    m_RenderObjects[ENUM_CLASS(RENDERGROUP::LAST)].clear();

    return S_OK;
}
#ifdef _DEBUG
HRESULT CRenderer::Render_Debug()
{
    if (m_bComponetRender)
    {
        for (auto& pDebugCom : m_DebugComponent)
        {
            if (nullptr != pDebugCom)
                pDebugCom->Render();

            Safe_Release(pDebugCom);
        }
        m_DebugComponent.clear();
    }
    else
    {
        for (auto& pDebugCom : m_DebugComponent)
        {
            Safe_Release(pDebugCom);
        }
        m_DebugComponent.clear();
    }
    if (m_bRTRender)
    {
        if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
            return E_FAIL;

        if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
            return E_FAIL;

        m_pGameInstance->Render_RT_Debug(m_pShader, m_pVIBuffer);
    }
    return S_OK;
}
#endif
CRenderer* CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CRenderer* pInstance = new CRenderer(pDevice, pContext);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed to Created : CRenderer"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CRenderer::Free()
{
    __super::Free();
#ifdef _DEBUG

    for (auto& pDebugComponent : m_DebugComponent)
        Safe_Release(pDebugComponent);
    m_DebugComponent.clear();
#endif // 

    for (size_t i = 0; i < ENUM_CLASS(RENDERGROUP::END); i++)
    {
        for (auto& pRenderObject : m_RenderObjects[i])
            Safe_Release(pRenderObject);

        m_RenderObjects[i].clear();
    }

    Safe_Release(m_pGameInstance);
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
    
    Safe_Release(m_pShader);
    Safe_Release(m_pFogShader);
    Safe_Release(m_pVIBuffer);
}
