#include "pch.h"
#include "Logo_Button_Line.h"

CLogo_Button_Line::CLogo_Button_Line(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CLogo_Button_Line::CLogo_Button_Line(const CLogo_Button_Line& Prototype) : CUIObject(Prototype)
{
}

HRESULT CLogo_Button_Line::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CLogo_Button_Line::Initialize(void* pArg)
{
   if (pArg == nullptr)
        return E_FAIL;

   UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

   m_vLocalPos = pDesc->vPos;
   m_vLocalSize = pDesc->vSize;

   if (FAILED(__super::Initialize(pArg)))
       return E_FAIL;

   if (FAILED(Ready_Components()))
       return E_FAIL;

   return S_OK;
}

void CLogo_Button_Line::Priority_Update(_float fTimeDelta)
{
}

void CLogo_Button_Line::Update(_float fTimeDelta)
{
}

void CLogo_Button_Line::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CLogo_Button_Line::Render()
{
    __super::Bind_ShaderTransform_Resourc();

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CLogo_Button_Line::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;
    
    return S_OK;
}

CLogo_Button_Line* CLogo_Button_Line::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLogo_Button_Line* pInstance = new CLogo_Button_Line(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CLogo_Button_Line"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CLogo_Button_Line::Clone(void* pArg)
{
    CLogo_Button_Line* pInstance = new CLogo_Button_Line(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CLogo_Button_Line"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLogo_Button_Line::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
