#include "pch.h"
#include "Player_Hp_Tex.h"

CPlayer_Hp_Tex::CPlayer_Hp_Tex(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CPlayer_Hp_Tex::CPlayer_Hp_Tex(const CPlayer_Hp_Tex& Prototype) : CUIObject(Prototype)
{
}

HRESULT CPlayer_Hp_Tex::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CPlayer_Hp_Tex::Initialize(void* pArg)
{
    if (pArg == nullptr)
        return E_FAIL;

    UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

    m_vLocalPos = Desc->vPos;
    m_vLocalSize = Desc->vSize;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CPlayer_Hp_Tex::Priority_Update(_float fTimeDelta)
{

}

void CPlayer_Hp_Tex::Update(_float fTimeDelta)
{

}

void CPlayer_Hp_Tex::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
        return;
}

HRESULT CPlayer_Hp_Tex::Render()
{

    Bind_ShaderTransform_Resourc(4);

    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CPlayer_Hp_Tex::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

CPlayer_Hp_Tex* CPlayer_Hp_Tex::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPlayer_Hp_Tex* pInstance = new CPlayer_Hp_Tex(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CPlayer_Hp_Tex"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPlayer_Hp_Tex::Clone(void* pArg)
{
    CPlayer_Hp_Tex* pInstance = new CPlayer_Hp_Tex(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CPlayer_Hp_Tex"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPlayer_Hp_Tex::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
}
