#include "pch.h"
#include "Logo_UI.h"

#include "Logo_Name.h"
#include "Logo_Button.h"
CLogo_UI::CLogo_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CLogo_UI::CLogo_UI(const CLogo_UI& Prototype) : CUIObject(Prototype)
{
}

HRESULT CLogo_UI::Initialize_Prototype()
{
    if (FAILED(Ready_Children_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CLogo_UI::Initialize(void* pArg)
{
    m_vLocalPos.x = g_iWinSizeX >> 1;
    m_vLocalPos.y = g_iWinSizeY >> 1;
    m_vLocalSize.x = g_iWinSizeX;
    m_vLocalSize.y = g_iWinSizeY;

    if (FAILED(__super::Initialize()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_Children()))
        return E_FAIL;

    m_pGameInstance->Subscribe<Event_Selete_LogoButton_Index>([&](const Event_Selete_LogoButton_Index& e) { m_iNumSeleteButton = e.iIndex; });


    return S_OK;
}

void CLogo_UI::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CLogo_UI::Update(_float fTimeDelta)
{
    Button_Selete();
    __super::Update(fTimeDelta);
}

void CLogo_UI::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
    __super::Late_Update(fTimeDelta);
}

HRESULT CLogo_UI::Render()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    return S_OK;
}

HRESULT CLogo_UI::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Component_Texture_Logo"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

HRESULT CLogo_UI::Ready_Children_Prototype()
{ 
     if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Logo_Name"),
        CLogo_Name::Create(m_pDevice, m_pContext))))
        return E_FAIL;

   if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Logo_Button"),
       CLogo_Button::Create(m_pDevice, m_pContext))))
       return E_FAIL;

    return S_OK;
}

HRESULT CLogo_UI::Ready_Children()
{
    CUIObject* pGameObject = nullptr;

    UIOBJECT_DESC Desc;
    Desc.OffsetY = -150.f;
   
    Desc.vMinUV.x = 0;
    Desc.vMinUV.y = 150.f / 512.f;
    Desc.vMaxUV.x = 440.f / 1024.f;
    Desc.vMaxUV.y = 320.f / 512.f;
    Desc.vSize = { 1024.f * 0.3f, 512.f * 0.3f };

    pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Logo_Name"), &Desc));
    if (nullptr == pGameObject)
        return E_FAIL;
    Add_Child(this, pGameObject, m_pShaderCom);
    
    m_iNumMaxButton = 4;
    Desc.iMaxIndex = m_iNumMaxButton;
    Desc.vSize = { 200.f, 50.f };
    Desc.vPos = {0.f, 100.f };
    Desc.OffsetY = 60.f;

    m_iNumSeleteButton = 1;
    for (_uint i = 0; i < Desc.iMaxIndex; i++)
    {
        Desc.iIndex = i + 1;
      
        pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Logo_Button"), &Desc));
        if (nullptr == pGameObject)
            return E_FAIL;
        Add_Child(this, pGameObject, m_pShaderCom);
    }

    
    return S_OK;
}

void CLogo_UI::Button_Selete()
{
    if (m_pGameInstance->Get_DIKeyState(DIK_S))
    {
        m_pGameInstance->Publish(Event_NonSelete_LogoButton{});

        m_iNumSeleteButton += 1;
        if (m_iNumSeleteButton > m_iNumMaxButton)
        {
            m_iNumSeleteButton = 1;
            m_pGameInstance->Publish(Event_Selete_LogoButton{ { 1 } });
        }
        else
            m_pGameInstance->Publish(Event_Selete_LogoButton{ { m_iNumSeleteButton } });

    }
    
    if (m_pGameInstance->Get_DIKeyState(DIK_W))
    {
        m_pGameInstance->Publish(Event_NonSelete_LogoButton{});

        m_iNumSeleteButton -= 1;

        if (m_iNumSeleteButton == 0)
        {
            m_iNumSeleteButton = m_iNumMaxButton;
            m_pGameInstance->Publish(Event_Selete_LogoButton{ { m_iNumMaxButton } });
        }
        else
            m_pGameInstance->Publish(Event_Selete_LogoButton{ { m_iNumSeleteButton } });

    }
}

CLogo_UI* CLogo_UI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLogo_UI* pInstance = new CLogo_UI(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CLogo_UI"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CLogo_UI::Clone(void* pArg)
{
    CLogo_UI* pInstance = new CLogo_UI(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CLogo_UI"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLogo_UI::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
}
