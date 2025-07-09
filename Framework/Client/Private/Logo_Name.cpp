#include "pch.h"
#include "Logo_Name.h"

CLogo_Name::CLogo_Name(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CLogo_Name::CLogo_Name(const CLogo_Name& Prototype) : CUIObject(Prototype)
{
}

HRESULT CLogo_Name::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLogo_Name::Initialize(void* pArg)
{
    UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

    m_vLocalPos.x = 0.f;
    m_vLocalPos.y = Desc->OffsetY;
   
    m_vLocalSize = Desc->vSize;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;
    

    return S_OK;
}

void CLogo_Name::Priority_Update(_float fTimeDelta)
{
}

void CLogo_Name::Update(_float fTimeDelta)
{
}

void CLogo_Name::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CLogo_Name::Render()
{
    Bind_ShaderTransform_Resourc();


    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CLogo_Name::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

CLogo_Name* CLogo_Name::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLogo_Name* pInstance = new CLogo_Name(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CLogo_Name"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CLogo_Name::Clone(void* pArg)
{
    CLogo_Name* pInstance = new CLogo_Name(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CLogo_Name"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLogo_Name::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
