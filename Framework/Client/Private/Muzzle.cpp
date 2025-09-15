#include "pch.h"
#include "Muzzle.h"
#include "Player_Manager.h"
CMuzzle::CMuzzle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPartObject { pDevice, pContext }
{
}

CMuzzle::CMuzzle(const CMuzzle& Prototype) : CPartObject(Prototype)
{
}

HRESULT CMuzzle::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CMuzzle::Initialize(void* pArg)
{
    MUZZLE_DATA* pDesc = static_cast<MUZZLE_DATA*>(pArg);

    m_BlackBoard = pDesc->m_BlackBoard;
    Safe_AddRef(m_BlackBoard);

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    //m_pTransformCom->Set_State(STATE::POSITION, XMVectorSetW(CPlayer_Manager::GetInstance()->Get_PlayerPos(), 1.f));
    m_iCount = 0;

    return S_OK;
}

void CMuzzle::Priority_Update(_float fTimeDelta)
{

}

void CMuzzle::Update(_float fTimeDelta)
{
}

void CMuzzle::Late_Update(_float fTimeDelta)
{
    XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * XMLoadFloat4x4(m_pParentMatrix));
    ++m_iCount;
    switch (m_iCount)
    {
    case 1:
        m_pTransformCom->Scale(_float3(0.5f, 0.5f, 1.f));
        m_vUVMin = { 0.f, 0.f };
        m_vUVMax = { 0.5f, 1.f };
        break;
    case 2:
        m_pTransformCom->Scale(_float3(0.3f, 0.3f, 1.f));
        m_vUVMin = { 0.5f, 0.f };
        m_vUVMax = { 1.f, 1.f };
        break;
    case 3:
        m_iCount = 0;
        m_BlackBoard->Set_Data().isEndEffect1 = true;
        break;
    }

	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::EFFECT, this)))
		return;
}

HRESULT CMuzzle::Render()
{
    m_pShaderCom->Bind_RawValue("g_UVMin", &m_vUVMin, sizeof(_float2));
    m_pShaderCom->Bind_RawValue("g_UVMax", &m_vUVMax, sizeof(_float2));
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(2);
    m_pVIBufferCom->Bind_Resources();
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CMuzzle::Ready_Components()
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Muzzle"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

HRESULT CMuzzle::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    return S_OK;
}

CMuzzle* CMuzzle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMuzzle* pInstance = new CMuzzle(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CMuzzle"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMuzzle::Clone(void* pArg)
{
    CMuzzle* pInstance = new CMuzzle(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CMuzzle"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMuzzle::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pShaderCom);

    Safe_Release(m_BlackBoard);
}
