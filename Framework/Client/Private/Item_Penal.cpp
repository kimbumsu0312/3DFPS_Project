#include "pch.h"
#include "Item_Penal.h"
#include "UI_Slot.h"
CItem_Penal::CItem_Penal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CItem_Penal::CItem_Penal(const CItem_Penal& Prototype) : CUIObject(Prototype)
{
}

HRESULT CItem_Penal::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CItem_Penal::Initialize(void* pArg)
{
    UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);
    m_bIsSelete = true;
    m_iIndex = pDesc->iIndex;
    m_vLocalPos = pDesc->vPos;
    m_vLocalSize = { g_iWinSizeX, g_iWinSizeY };
    
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_Children()))
        return E_FAIL;

    return S_OK;
}

void CItem_Penal::Priority_Update(_float fTimeDelta)
{
}

void CItem_Penal::Update(_float fTimeDelta)
{
}

void CItem_Penal::Late_Update(_float fTimeDelta)
{
    if (!m_bIsSelete)
        return;

    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
    __super::Late_Update(fTimeDelta);

}

HRESULT CItem_Penal::Render()
{
    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 1)))
        return E_FAIL;

    return S_OK;
}

void CItem_Penal::Selete_Penal(_uint iIndex)
{
    if (iIndex == m_iIndex)
        m_bIsSelete = true;
    else
        m_bIsSelete = false;
}

HRESULT CItem_Penal::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

HRESULT CItem_Penal::Ready_Children()
{
    CUIObject* pGameObject = nullptr;
    CUI_Slot::UI_SLOT_DESC Desc;

    _float fTexSizeX = 512.f;
    _float fTexSizeY = 512.f;
    Desc.iPassIndex = 2;
    Desc.vPos = { 0.f, -140.f };
    Desc.vSize = { 55.f, 55.f };
    Desc.vMinUV = { 0.f, 0.f };
    Desc.vMaxUV = { 60.f / fTexSizeX , 60.f / fTexSizeY };
    Desc.OffsetX = 65.f;
    Desc.iIndex = 0;
    Desc.iMaxIndex = 10;
    Desc.iTexIndex = 1;

    for (_uint i = 0; i < 6; ++i)
    {
        Desc.OffsetY = (_float)i;
        for (_uint j = 0; j < Desc.iMaxIndex; ++j)
        {

            pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Slot"), &Desc));
            if (nullptr == pGameObject)
                return E_FAIL;
            Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);
            Desc.iIndex += 1;
        }
    }

    return S_OK;
}

CItem_Penal* CItem_Penal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CItem_Penal* pInstance = new CItem_Penal(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CItem_Penal"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CItem_Penal::Clone(void* pArg)
{
    CItem_Penal* pInstance = new CItem_Penal(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CItem_Penal"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CItem_Penal::Free()
{
    __super::Free();
    Safe_Release(m_pVIBufferCom);
}
