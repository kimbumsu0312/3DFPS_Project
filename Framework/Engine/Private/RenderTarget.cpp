#include "pch.h"
#include "RenderTarget.h"

#include "Shader.h"
#include "VIBuffer_Rect.h"

CRenderTarget::CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : m_pDevice { pDevice }, m_pContext { pContext }
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CRenderTarget::Initailize(_uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
    //텍스처 구조체 셋팅
    D3D11_TEXTURE2D_DESC TextureDesc{};

    //텍스처 너비 / 높이
    TextureDesc.Width = iSizeX;
    TextureDesc.Height = iSizeY;

    //밉맵 사용 여부 ( 0 밉맵 자동 생성, 1 밉맵 사용 안함,  2이상 특정 수의 밉맵 레벨 사용)
    TextureDesc.MipLevels = 1;

    //텍스처 배열 크기 // 포맷 셋팅
    TextureDesc.ArraySize = 1;
    TextureDesc.Format = ePixelFormat;

    //샘플링 
    TextureDesc.SampleDesc.Quality = 0;
    TextureDesc.SampleDesc.Count = 1;

    TextureDesc.Usage = D3D11_USAGE_DEFAULT;

    //바인딩 방식 -> 랜더 타겟, 셰이더 리소스로 사용
    TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    TextureDesc.CPUAccessFlags = 0;
    TextureDesc.MiscFlags = 0;

    //텍스처 구조체로 텍스처 생성
    if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D)))
        return E_FAIL;

    //랜더 타겟 뷰 생성, 셋팅한 텍스처로 뷰를 생성
    if (FAILED(m_pDevice->CreateRenderTargetView(m_pTexture2D, nullptr, &m_pRTV)))
        return E_FAIL;

    //셰이더 리소스 뷰 생성, 셋팅한 텍스처로 뷰를 생성
    if (FAILED(m_pDevice->CreateShaderResourceView(m_pTexture2D, nullptr, &m_pSRV)))
        return E_FAIL;

    m_vClearColor = vClearColor;

    return S_OK;
}

HRESULT CRenderTarget::Bind_ShaderResource(CShader* pShader, const _char* pConstantName)
{
    return pShader->Bind_SRV(pConstantName, m_pSRV);
}

void CRenderTarget::Clear()
{
    m_pContext->ClearRenderTargetView(m_pRTV, reinterpret_cast<_float*>(&m_vClearColor));
}

HRESULT CRenderTarget::Copy_Resource(ID3D11Texture2D* pSourTexture)
{
    m_pContext->CopyResource(pSourTexture, m_pTexture2D);

    return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderTarget::Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY)
{
    _uint iNumViewports = { 1 };
    D3D11_VIEWPORT ViewportDesc{ };

    m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);
    XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(fSizeX, fSizeY, 1.f));
    m_WorldMatrix._41 = fX - ViewportDesc.Width * 0.5f;
    m_WorldMatrix._42 = -fY + ViewportDesc.Height * 0.5f;

    return S_OK;
}

HRESULT CRenderTarget::Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
    if (FAILED(pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
        return E_FAIL;

    if (FAILED(pShader->Bind_SRV("g_Texture", m_pSRV)))
        return E_FAIL;

    pShader->Begin(0);
    
    pVIBuffer->Bind_Resources();
    pVIBuffer->Render();

    return S_OK;
}
#endif // DEBUG

CRenderTarget* CRenderTarget::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
    CRenderTarget* pInstance = new CRenderTarget(pDevice, pContext);

    if (FAILED(pInstance->Initailize(iSizeX, iSizeY, ePixelFormat, vClearColor)))
    {
        MSG_BOX(TEXT("Filed to Created : CRenderTarget"));
        Safe_Release(pInstance);
    }
    return pInstance;
}

void CRenderTarget::Free()
{
    __super::Free();

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    Safe_Release(m_pSRV);
    Safe_Release(m_pRTV);
    Safe_Release(m_pTexture2D);
}
