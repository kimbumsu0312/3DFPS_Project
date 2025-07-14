#include "pch.h"
#include "Create_Penal.h"
#include "Inventory_Slot.h"
#include "Inventory_Tex.h"
CCreate_Penal::CCreate_Penal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CCreate_Penal::CCreate_Penal(const CCreate_Penal& Prototype) : CUIObject(Prototype)
{
}

HRESULT CCreate_Penal::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCreate_Penal::Initialize(void* pArg)
{
    UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

    m_vLocalPos = pDesc->vPos;
    m_vLocalSize = { g_iWinSizeX, g_iWinSizeY };
    m_iIndex = pDesc->iIndex;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_Children()))
        return E_FAIL;
    
    return S_OK;
}

void CCreate_Penal::Priority_Update(_float fTimeDelta)
{
}

void CCreate_Penal::Update(_float fTimeDelta)
{

}

void CCreate_Penal::Late_Update(_float fTimeDelta)
{
    if (!m_bIsSelete)
        return;

    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
    __super::Late_Update(fTimeDelta);

}

HRESULT CCreate_Penal::Render()
{
    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 1)))
        return E_FAIL;

    return S_OK;
}

void CCreate_Penal::Selete_Penal(_uint iIndex)
{
    if (iIndex == m_iIndex)
        m_bIsSelete = true;
    else
        m_bIsSelete = false;
}

HRESULT CCreate_Penal::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

HRESULT CCreate_Penal::Ready_Children()
{
    CUIObject* pGameObject = nullptr;
    CInventory_Slot::INVEN_SLOT_DESC SlotDesc;
    CInventory_Tex::INVEN_TEX_DESC  TexDesc;

    _float fTexSizeX = 512.f;
    _float fTexSizeY = 512.f;
    SlotDesc.iTexIndex = 1;
    SlotDesc.vPos = { 0.f, 150.f };
    SlotDesc.vSize = { 65.f, 65.f };
    SlotDesc.vMinUV = { 0.f, 0.f };
    SlotDesc.vMaxUV = { 60.f / fTexSizeX , 60.f / fTexSizeY };
    SlotDesc.OffsetX = 75.f;
    SlotDesc.iIndex = 0;
    SlotDesc.iMaxIndex = 6;

    for (_int i = 0; i < 2; ++i)
    {
        SlotDesc.OffsetY = (float)i;
        for (_uint j = 0; j < SlotDesc.iMaxIndex; ++j)
        {

            pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Slot"), &SlotDesc));
            if (nullptr == pGameObject)
                return E_FAIL;
            Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);
            SlotDesc.iIndex += 1;
        }
    }
    
    fTexSizeX = 512.f;
    fTexSizeY = 128.f;
    SlotDesc.iTexIndex = 5;
    SlotDesc.vPos = { 0.f, -120.f };
    SlotDesc.vSize = { 80.f, 80.f };
    SlotDesc.vMinUV = { 0.f, 0.f };
    SlotDesc.vMaxUV = { 128.f / fTexSizeX , 128.f / fTexSizeY };
    SlotDesc.iIndex = 0;
    SlotDesc.iMaxIndex = 1;
    SlotDesc.OffsetX = 0.f;
    SlotDesc.OffsetY = 0.f;

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Slot"), &SlotDesc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);

    TexDesc.vPos = { -50.f, -10.f };
    TexDesc.vSize = { 70.f, 70.f };
    TexDesc.vMinUV = { 142.f / fTexSizeX , 2.f / fTexSizeY };
    TexDesc.vMaxUV = { 217.f / fTexSizeX , 77.f / fTexSizeY };
    TexDesc.iTexIndex = 5;

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Tex"), &TexDesc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);

    TexDesc.vPos = { 50.f, -10.f };
    
    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Tex"), &TexDesc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);

    fTexSizeX = 512.f;
    fTexSizeY = 256.f;

    TexDesc.vPos = { 0.f, 70.f };
    TexDesc.vSize = { 500.f, 100.f };
    TexDesc.vMinUV = { 64.f / fTexSizeX , 81.f / fTexSizeY };
    TexDesc.vMaxUV = { 113.f / fTexSizeX , 116.f / fTexSizeY };
    TexDesc.iTexIndex = 3;

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Tex"), &TexDesc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);


    fTexSizeX = 2048.f;
    fTexSizeY = 2048.f;
    SlotDesc.iTexIndex = 4;
    SlotDesc.vPos = { 0.f, 65.f };
    SlotDesc.vSize = { 40.f, 40.f };
    SlotDesc.vMinUV = { 75.f / fTexSizeX, 0.f };
    SlotDesc.vMaxUV = { 175.f / fTexSizeX , 100.f / fTexSizeY };
    SlotDesc.iIndex = 0;
    SlotDesc.iMaxIndex = 5;
    SlotDesc.OffsetX = 80.f;
    SlotDesc.OffsetY = 0.f;

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Slot"), &SlotDesc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);
    SlotDesc.iIndex += 1;

    SlotDesc.vMinUV = { 380.f / fTexSizeX, 0.f };
    SlotDesc.vMaxUV = { 470.f / fTexSizeX , 100.f / fTexSizeY };

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Slot"), &SlotDesc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);
    SlotDesc.iIndex += 1;

    SlotDesc.vMinUV = { 475.f / fTexSizeX, 0.f };
    SlotDesc.vMaxUV = { 575.f / fTexSizeX , 100.f / fTexSizeY };

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Slot"), &SlotDesc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);
    SlotDesc.iIndex += 1;

    SlotDesc.vMinUV = { 200.f / fTexSizeX, 0.f };
    SlotDesc.vMaxUV = { 300.f / fTexSizeX , 100.f / fTexSizeY };

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Slot"), &SlotDesc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);
    SlotDesc.iIndex += 1;

    SlotDesc.vMinUV = { 285.f / fTexSizeX, 0.f };
    SlotDesc.vMaxUV = { 385.f / fTexSizeX , 100.f / fTexSizeY };

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Slot"), &SlotDesc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);
    SlotDesc.iIndex += 1;

    return S_OK;
}

CCreate_Penal* CCreate_Penal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCreate_Penal* pInstance = new CCreate_Penal(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CCreate_Penal"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCreate_Penal::Clone(void* pArg)
{
    CCreate_Penal* pInstance = new CCreate_Penal(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CCreate_Penal"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCreate_Penal::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
