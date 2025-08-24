#include "pch.h"
#include "Inventory_Base.h"
#include "UI_Tex.h"
#include "Inventory_Node.h"
#include "Inventory_Coin.h"
#include "Item_Penal.h"
#include "Create_Penal.h"
#include "Item_Info.h"
CInventory_Base::CInventory_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CInventory_Base::CInventory_Base(const CInventory_Base& Prototype) : CUIObject(Prototype)
{
}

HRESULT CInventory_Base::Initialize_Prototype()
{
    if (FAILED(Ready_Children_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CInventory_Base::Initialize(void* pArg)
{
    m_fOpenTexSpeed = 1.f;
    m_fOpenTexValueY = 0.15f;
   
    m_vLocalPos = { 0.f, 0.f };
    m_vLocalSize.x = g_iWinSizeX * 0.65f;
    m_vLocalSize.y = g_iWinSizeY;

    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_Children()))
        return E_FAIL;

    m_pGameInstance->Subscribe<Event_Inventory_Open>([&](const Event_Inventory_Open& e) { Open_UI(e.bIsOpen); });

    return S_OK;
}

void CInventory_Base::Priority_Update(_float fTimeDelta)
{
    //Change_Penal();
    __super::Priority_Update(fTimeDelta);
}

void CInventory_Base::Update(_float fTimeDelta)
{
    if(m_bIsOpen)
        Opening(fTimeDelta);

    __super::Update(fTimeDelta);
}

void CInventory_Base::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
    __super::Late_Update(fTimeDelta);

}

HRESULT CInventory_Base::Render()
{
    
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Vector", &m_vOpenTex, sizeof(_float4))))
         return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 2)))
        return E_FAIL;

    __super::Bind_ShaderTransform_Resourc(2);

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();


    return S_OK;
}

HRESULT CInventory_Base::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

HRESULT CInventory_Base::Ready_Children_Prototype()
{
    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Node"),
        CInventory_Node::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Coin"),
        CInventory_Coin::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Item_Penal"),
        CItem_Penal::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Create_Penal"),
        CCreate_Penal::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_ItemInfo"),
        CItem_Info::Create(m_pDevice, m_pContext))))
        return E_FAIL;

    return S_OK;
}

HRESULT CInventory_Base::Ready_Children()
{
    CUIObject* pGameObject = nullptr;
    CUI_Tex::UI_TEX_DESC Desc;

    _float fTexSizeX = 512.f;
    _float fTexSizeY = 256.f;

    Desc.vPos = { 0.f, -315.f };

    Desc.vSize = { 200.f * 1.5f, 27.f * 2.f };
    Desc.vMinUV = { 239.f / fTexSizeX, 95.f / 256.f };
    Desc.vMaxUV = { 438.f / fTexSizeX , 121.f / 256.f };

    Desc.iIndex = 0;
    Desc.fRot = 0.f;
    Desc.iTexIndex = 3;
    Desc.iPassIndex = 2;
    Desc.IsFont = true;
    Desc.szText = TEXT("æ∆¿Ã≈€");

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Tex"), &Desc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);

    Desc.IsFont = false;
    Desc.szText = {};


    /*
    Desc.vPos = { -125.f, -317.f };
    Desc.vSize = { 34.f, 38.f };
    Desc.vMinUV = { 36 / fTexSizeX, 84 / fTexSizeY };
    Desc.vMaxUV = { 53 / fTexSizeX , 103 / fTexSizeY };
    Desc.iIndex = 1;
    Desc.fRot = 0.f;
    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Tex"), &Desc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);

    Desc.vPos = { 125.f, -317.f };
    Desc.vSize = { 34.f, 38.f };
    Desc.vMinUV = { 36 / fTexSizeX, 84 / fTexSizeY };
    Desc.vMaxUV = { 53 / fTexSizeX , 103 / fTexSizeY };
    Desc.iIndex = 2;
    Desc.fRot = 180.f;

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_UI_Tex"), &Desc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);
     
    Desc.vPos = { 0.f, -290.f };
    Desc.vSize = { 25.f, 25.f };
    Desc.vMinUV = { 118 / fTexSizeX, 84 / fTexSizeY };
    Desc.vMaxUV = { 138 / fTexSizeX , 104 / fTexSizeY };
    Desc.iMaxIndex = 2;
    Desc.OffsetX = 20.f;
    for (_uint i = 0; i < Desc.iMaxIndex ; i++)
    {
        Desc.iIndex = i;
        pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Node"), &Desc));
        if (nullptr == pGameObject)
            return E_FAIL; 
        Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);
    }
     */

    Desc.vPos = { 300.f, -320.f };
    Desc.vSize = { 40.f, 40.f };
    Desc.vMinUV = { 589.f / 2048.f, 13.f / 2048.f };
    Desc.vMaxUV = { 659.f / 2048.f , 86.f / 2048.f };
    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Coin"), &Desc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);

    Desc.iIndex = 0;
    Desc.vPos = { 0.f, 0.f };
    
    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Item_Penal"), &Desc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);

    m_pItemPenal = static_cast<CItem_Penal*>(pGameObject);
    Safe_AddRef(m_pItemPenal);

    Desc.iIndex = 1;
    Desc.vPos = { 0.f, 0.f };

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Create_Penal"), &Desc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);
    
    m_pCreatePenal = static_cast<CCreate_Penal*>(pGameObject);
    Safe_AddRef(m_pCreatePenal);

    return S_OK;
}

void CInventory_Base::Open_UI(_bool bOpen)
{
    m_bIsOpen = bOpen;
    m_vOpenTex = {};
    m_fOpenTexValueY = 0.15f;
    m_fOpenTexValueX = 0.f;
    m_iSeletePenal_Index = 0;
   
}

void CInventory_Base::Opening(_float fTimeDelta)
{
    if (m_fOpenTexValueY > 1.f)
    {
        m_bIsOpen = false;
        m_vOpenTex.x = (m_vPos.x - m_vSize.x * 0.5f);
        m_vOpenTex.y = (m_vPos.x + m_vSize.x * 0.5f);
        m_vOpenTex.z = 0.f;
        m_vOpenTex.w = m_vSize.y * 1.f;
    }
    else if (m_fOpenTexValueX >= 0.5f)
    {
        m_vOpenTex.x = (m_vPos.x - m_vSize.x * 0.5f);
        m_vOpenTex.y = (m_vPos.x + m_vSize.x * 0.5f);
        m_vOpenTex.z = 0.f;
        m_vOpenTex.w = m_vSize.y * m_fOpenTexValueY;
        m_fOpenTexValueY += m_fOpenTexSpeed * fTimeDelta;
    }
    else
    {
        m_vOpenTex.x = (m_vPos.x - m_vSize.x * m_fOpenTexValueX);
        m_vOpenTex.y = (m_vPos.x + m_vSize.x * m_fOpenTexValueX);
        m_vOpenTex.z = 0.f;
        m_vOpenTex.w = m_vSize.y * m_fOpenTexValueY;
        m_fOpenTexValueX += m_fOpenTexSpeed * fTimeDelta;
    }
}

void CInventory_Base::Change_Penal()
{
    if (m_pGameInstance->IsKeyDown(DIK_Q))
    {
        m_iSeletePenal_Index--;

        if (m_iSeletePenal_Index < 0)
            m_iSeletePenal_Index = 1;
        
        Selete_Penal(m_iSeletePenal_Index);
    }

    if (m_pGameInstance->IsKeyDown(DIK_E))
    {
        m_iSeletePenal_Index++;

        if (m_iSeletePenal_Index > 1)
            m_iSeletePenal_Index = 0;

        Selete_Penal(m_iSeletePenal_Index);
    }
}

void CInventory_Base::Selete_Penal(_uint iIndex)
{
    m_pItemPenal->Selete_Penal(m_iSeletePenal_Index);
    m_pCreatePenal->Selete_Penal(m_iSeletePenal_Index);
}

CInventory_Base* CInventory_Base::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CInventory_Base* pInstance = new CInventory_Base(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CInventory_Base"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CInventory_Base::Clone(void* pArg)
{
    CInventory_Base* pInstance = new CInventory_Base(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CInventory_Base"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CInventory_Base::Free()
{
    Safe_Release(m_pItemPenal);
    Safe_Release(m_pCreatePenal);
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
