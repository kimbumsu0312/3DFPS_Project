#include "pch.h"
#include "Renderer.h"
#include "GameObject.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : m_pDevice { pDevice }, m_pContext { pContext }
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CRenderer::Initialize()
{
    //���� ���� ���ٽ��� �����´�.
    m_pContext->OMGetDepthStencilState(&m_pDepthStencil, 0);

    //���� ���ٽ� �İ�ü ����
    //���� ���� : ���� ���� �����ϴ� ����
    //���ٽ� ���� : ȭ�� �� �ȼ����� ������ �����ϴ� ����
    D3D11_DEPTH_STENCIL_DESC DepthStencil_Desc = {};
    DepthStencil_Desc.DepthEnable = FALSE;         
    DepthStencil_Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    DepthStencil_Desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
    DepthStencil_Desc.StencilEnable = FALSE;

    if (FAILED(m_pDevice->CreateDepthStencilState(&DepthStencil_Desc, &m_pDepthStencil_Off)))
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

    if (FAILED(m_pDevice->CreateBlendState(&Blend_Desc, &m_pAlphablend)))
        return E_FAIL;

    D3D11_BLEND_DESC NonBlend_Desc = {};
    NonBlend_Desc.RenderTarget[0].BlendEnable = FALSE;
    NonBlend_Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    if (FAILED(m_pDevice->CreateBlendState(&NonBlend_Desc, &m_pNonAlphablend)))
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
    m_pContext->OMSetDepthStencilState(m_pDepthStencil_Off, 0);
    m_pContext->OMSetBlendState(m_pAlphablend, nullptr, 0xffffffff);    //nullptr:float4�� Ư���� ������ ���� �� �ִ�, 0xffffffff: Ư�� ������ �������� ���� �� ����
    for (auto& pRenderObject : m_RenderObjects[ENUM_CLASS(RENDERGROUP::UI)])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();

        Safe_Release(pRenderObject);
    }

    m_RenderObjects[ENUM_CLASS(RENDERGROUP::UI)].clear();

    m_pContext->OMSetDepthStencilState(m_pDepthStencil, 0);
    m_pContext->OMSetBlendState(m_pNonAlphablend, nullptr, 0xffffffff);

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

    Safe_Release(m_pDepthStencil);
    Safe_Release(m_pDepthStencil_Off);

    Safe_Release(m_pAlphablend);
    Safe_Release(m_pNonAlphablend);

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
    
}
