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
    if (FAILED(Ready_RenderState()))
        return E_FAIL;

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

HRESULT CRenderer::Add_RenderState(_wstring szRenderTag, RENDERSTATE eRenderStates, const void* pDesc)
{
    ID3D11DeviceChild* pRenderState = Find_RenderState(szRenderTag, eRenderStates);

    if (pRenderState != nullptr)
        return E_FAIL;

    if (eRenderStates == RENDERSTATE::BLEND)
    {
        ID3D11BlendState* pBlendState = nullptr;
        if (FAILED(m_pDevice->CreateBlendState(static_cast<const D3D11_BLEND_DESC*>(pDesc), &pBlendState)))
            return E_FAIL;

        m_pRenderState[ENUM_CLASS(eRenderStates)].emplace(szRenderTag, pBlendState);
    }
    else if (eRenderStates == RENDERSTATE::DEPTH_STENCIL)
    {
        ID3D11DepthStencilState* pDSState = nullptr;
        if (FAILED(m_pDevice->CreateDepthStencilState(static_cast<const D3D11_DEPTH_STENCIL_DESC*>(pDesc), &pDSState)))
            return E_FAIL;

        m_pRenderState[ENUM_CLASS(eRenderStates)].emplace(szRenderTag, pDSState);
    }
    else
    {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CRenderer::Switching_RenderState(_wstring szRenderTag, RENDERSTATE eRenderStates)
{
    ID3D11DeviceChild* pRenderState = Find_RenderState(szRenderTag, eRenderStates);

    if (pRenderState == nullptr)
        return E_FAIL;

    switch (eRenderStates)
    {
    case RENDERSTATE::BLEND:
        m_pContext->OMSetBlendState(static_cast<ID3D11BlendState*>(pRenderState), nullptr, 0xffffffff);
        break;
    case RENDERSTATE::DEPTH_STENCIL:
        m_pContext->OMSetDepthStencilState(static_cast<ID3D11DepthStencilState*>(pRenderState), 0);
        break;
    }
    return S_OK;
}

HRESULT CRenderer::Draw()
{
    if (FAILED(Render_Priority()))
        return E_FAIL;
    if (FAILED(Render_NonBlend()))
        return E_FAIL;
    if (FAILED(Render_Blend()))
        return E_FAIL;
    if (FAILED(Render_UI()))
        return E_FAIL;
    if (FAILED(Render_UI_Effect()))
        return E_FAIL;
    if (FAILED(Render_Last()))
        return E_FAIL;
    return S_OK;
}

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
    for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::NONBLEND)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();

        Safe_Release(pRenderObject);
    }

    m_RenderObjects[ENUM_CLASS(RENDERGROUP::NONBLEND)].clear();

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

HRESULT CRenderer::Render_UI()
{
    //m_pContext->OMSetDepthStencilState(m_pDepthStencil_Off, 0);
    //m_pContext->OMSetBlendState(m_pAlphablend, nullptr, 0xffffffff);    //nullptr:float4로 특정한 색상값을 곱할 수 있다, 0xffffffff: 특정 색상을 랜더하지 않을 수 있음
    
    Switching_RenderState(TEXT("NonDepthTest"), RENDERSTATE::DEPTH_STENCIL);
    Switching_RenderState(TEXT("AlphaBlend"), RENDERSTATE::BLEND);

    //m_RenderObjects[ENUM_CLASS(RENDERGROUP::UI)].sort([](const RenderObject& a, const RenderObject& b) {return a.vWorldPos.z > b.vWorldPos.z;  });

    for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::UI)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();

        Safe_Release(pRenderObject);
    }

    m_RenderObjects[ENUM_CLASS(RENDERGROUP::UI)].clear();

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

    Switching_RenderState(TEXT("Default_State"), RENDERSTATE::DEPTH_STENCIL);
    Switching_RenderState(TEXT("NonAlphaBlend"), RENDERSTATE::BLEND);
    return S_OK;
}

ID3D11DeviceChild* CRenderer::Find_RenderState(_wstring szRenderTag, RENDERSTATE eRenderStates)
{
    auto iter = m_pRenderState[ENUM_CLASS(eRenderStates)].find(szRenderTag);

    if (iter == m_pRenderState[ENUM_CLASS(eRenderStates)].end())
        return nullptr;

    return iter->second;
}

HRESULT CRenderer::Ready_RenderState()
{
    D3D11_DEPTH_STENCIL_DESC dsDesc = {};
    dsDesc.DepthEnable = TRUE;                  
    dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    dsDesc.DepthFunc = D3D11_COMPARISON_LESS;       
    dsDesc.StencilEnable = FALSE;

    if (FAILED(Add_RenderState(TEXT("Default_State"), RENDERSTATE::DEPTH_STENCIL, &dsDesc)))
        return E_FAIL;

    //뎁스 스텐실 컴객체 생성
    //뎁스 버퍼 : 깊이 값을 저장하는 버퍼
    //스텐실 버퍼 : 화면 각 픽셀마다 정보를 저장하는 버퍼
    D3D11_DEPTH_STENCIL_DESC DepthStencil_Desc = {};
    DepthStencil_Desc.DepthEnable = FALSE;
    DepthStencil_Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    DepthStencil_Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
    DepthStencil_Desc.StencilEnable = FALSE;

    if (FAILED(Add_RenderState(TEXT("NonDepthTest"), RENDERSTATE::DEPTH_STENCIL, &DepthStencil_Desc)))
        return E_FAIL;

    D3D11_BLEND_DESC Blend_Desc = {};
    Blend_Desc.RenderTarget[0].BlendEnable = TRUE;
    Blend_Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    Blend_Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    Blend_Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    Blend_Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    Blend_Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    Blend_Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    Blend_Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    if (FAILED(Add_RenderState(TEXT("AlphaBlend"), RENDERSTATE::BLEND, &Blend_Desc)))
        return E_FAIL;

    D3D11_BLEND_DESC NonBlend_Desc = {};
    NonBlend_Desc.RenderTarget[0].BlendEnable = FALSE;
    NonBlend_Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    if (FAILED(Add_RenderState(TEXT("NonAlphaBlend"), RENDERSTATE::BLEND, &NonBlend_Desc)))
        return E_FAIL;

    return S_OK;
}


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

    for (size_t i = 0; i < ENUM_CLASS(RENDERGROUP::END); i++)
    {
        for (auto& pRenderObject : m_RenderObjects[i])
            Safe_Release(pRenderObject);

        m_RenderObjects[i].clear();
    }
 
    for (size_t i = 0; i < ENUM_CLASS(RENDERSTATE::END); ++i)
    {
        for (auto Pair : m_pRenderState[i])
            Safe_Release(Pair.second);

        m_pRenderState[i].clear();
    }

    Safe_Release(m_pGameInstance);
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
