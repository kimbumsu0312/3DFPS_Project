#include "pch.h"
#include "BackGround.h"
#include "GameInstance.h"

CBackGround::CBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CBackGround::CBackGround(const CBackGround& Prototype) : CUIObject( Prototype )
{
}

HRESULT CBackGround::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBackGround::Initialize(void* pArg)
{
    m_vLocalPos.x = g_iWinSizeX >> 1;
    m_vLocalPos.y = g_iWinSizeY >> 1;
    m_vLocalSize.x = g_iWinSizeX;
    m_vLocalSize.y = g_iWinSizeY;
    
    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CBackGround::Priority_Update(_float fTimeDelta)
{
}

void CBackGround::Update(_float fTimeDelta)
{

}

void CBackGround::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CBackGround::Render()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    __super::Bind_ShaderTransform_Resourc(3);

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CBackGround::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Texture_BackGround"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

CBackGround* CBackGround::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBackGround* pInstance = new CBackGround(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CBackGround"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBackGround::Clone(void* pArg)
{
    CBackGround* pInstance = new CBackGround(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CBackGround"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBackGround::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
