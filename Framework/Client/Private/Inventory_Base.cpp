#include "pch.h"
#include "Inventory_Base.h"

CInventory_Base::CInventory_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CInventory_Base::CInventory_Base(const CInventory_Base& Prototype) : CUIObject(Prototype)
{
}

HRESULT CInventory_Base::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CInventory_Base::Initialize(void* pArg)
{
    m_fOpenTexSpeed = 1.f;
    m_fOpenTexValueY = 0.15f;
   
    m_vLocalPos.x = 0.f;
    m_vLocalPos.y = 0.f;
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
}

HRESULT CInventory_Base::Render()
{
    if (FAILED(m_pShaderCom->Bind_RawValue("g_Vector", &m_vOpenTex, sizeof(_float4))))
         return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 5)))
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

HRESULT CInventory_Base::Ready_Children()
{
    CUIObject* pGameObject = nullptr;

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
