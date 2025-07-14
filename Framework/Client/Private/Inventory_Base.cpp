#include "pch.h"
#include "Inventory_Base.h"
#include "Inventory_Tex.h"
#include "Inventory_Node.h"
#include "Inventory_Coin.h"
#include "Item_Penal.h"
#include "Create_Penal.h"

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
    m_vLocalSize.x = 1280 * 0.7;
    m_vLocalSize.y = 1080 * 0.7;

    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_Children()))
        return E_FAIL;

    m_pGameInstance->Subscribe<Event_Inventory_Open>([&](const Event_Inventory_Open& e) { m_bIsOpen = e.bIsOpen; m_vOpenTex = {}; m_fOpenTexValueY = 0.15f; m_fOpenTexValueX = 0.f; });

    return S_OK;
}

void CInventory_Base::Priority_Update(_float fTimeDelta)
{
    if (m_pGameInstance->IsKeyDown(DIK_Q))
    {
        m_iSeletePenal_Index--;

        if (m_iSeletePenal_Index < 0)
            m_iSeletePenal_Index = 1;

        m_pGameInstance->Publish(Event_Inven_Selete_penal{ m_iSeletePenal_Index });
    }

    if (m_pGameInstance->IsKeyDown(DIK_E))
    {
        m_iSeletePenal_Index++;

        if (m_iSeletePenal_Index > 1)
            m_iSeletePenal_Index = 0;

        m_pGameInstance->Publish(Event_Inven_Selete_penal{ m_iSeletePenal_Index });
    }
}

void CInventory_Base::Update(_float fTimeDelta)
{
    if(m_bIsOpen)
        Open_UI(fTimeDelta);
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

    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 3)))
        return E_FAIL;

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

    return S_OK;
}

HRESULT CInventory_Base::Ready_Children()
{
    CUIObject* pGameObject = nullptr;
    CUIObject::UIOBJECT_DESC Desc;

    _float fTexSizeX = 512.f;
    _float fTexSizeY = 256.f;

    Desc.vPos = { 0.f, -225.f };
    Desc.vSize = { 43.f * 3.f, 32.f * 1.4f};
    Desc.vMinUV = {67/ fTexSizeX, 83/ fTexSizeY };
    Desc.vMaxUV = {110/ fTexSizeX , 115/ fTexSizeY };
    Desc.iIndex = 0;
    Desc.fRot = 0.f;
    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Tex"), &Desc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);

    Desc.vPos = { -85.f, -225.f };
    Desc.vSize = { 17.f, 19.f };
    Desc.vMinUV = { 36 / fTexSizeX, 84 / fTexSizeY };
    Desc.vMaxUV = { 53 / fTexSizeX , 103 / fTexSizeY };
    Desc.iIndex = 1;
    Desc.fRot = 0.f;
    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Tex"), &Desc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);

    Desc.vPos = { 85.f, -225.f };
    Desc.vSize = { 17.f, 19.f };
    Desc.vMinUV = { 36 / fTexSizeX, 84 / fTexSizeY };
    Desc.vMaxUV = { 53 / fTexSizeX , 103 / fTexSizeY };
    Desc.iIndex = 2;
    Desc.fRot = 180.f;

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Tex"), &Desc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);

    Desc.vPos = { 0.f, -200.f };
    Desc.vSize = { 20.f, 20.f };
    Desc.vMinUV = { 118 / fTexSizeX, 84 / fTexSizeY };
    Desc.vMaxUV = { 138 / fTexSizeX , 104 / fTexSizeY };
    Desc.iMaxIndex = 4;
    Desc.OffsetX = 20.f;
    for (_uint i = 0; i < Desc.iMaxIndex ; i++)
    {
        Desc.iIndex = i;
        pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Node"), &Desc));
        if (nullptr == pGameObject)
            return E_FAIL;
        Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);
    }

    Desc.vPos = { 250.f, -225.f };
    Desc.vSize = { 35.f, 35.f };

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

    Desc.iIndex = 1;
    Desc.vPos = { 0.f, 0.f };

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven_Create_Penal"), &Desc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);

    return S_OK;
}

void CInventory_Base::Open_UI(_float fTimeDelta)
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
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
