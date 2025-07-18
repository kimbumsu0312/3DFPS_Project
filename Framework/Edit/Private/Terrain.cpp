#include "pch.h"
#include "Terrain.h"
#include "GameInstance.h"
#include "MapObject.h"
#include "Imgui_Manager.h"
CTerrain::CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CGameObject{pDevice, pContext}
{
}

CTerrain::CTerrain(const CTerrain& Prototype) : CGameObject (Prototype)
{
}

HRESULT CTerrain::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CTerrain::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components(pArg)))
        return E_FAIL;

    m_pGameInstance->Subscribe<Clear_Map>([&](const Clear_Map& e) {m_bIsDead = true; });


    return S_OK;
}

void CTerrain::Priority_Update(_float fTimeDelta)
{
}

void CTerrain::Update(_float fTimeDelta)
{
    if (g_CreateModel)
    {
        if (m_pGameInstance->IsMouseDown(MOUSEKEYSTATE::LB))
        {
            _float3 vSetModelPos = { 0.f, 0.f, 0.f };
            if (m_pVIBufferCom->IsPicked(*m_pTransformCom, vSetModelPos))
            {
                CMapObject::MODEL_OBJECT_DESC Desc{};
                Desc.vPos.x = vSetModelPos.x;
                Desc.vPos.y = vSetModelPos.y;
                Desc.vPos.z = vSetModelPos.z;
                Desc.vPos.w = 1.f;
                Desc.szModelPath = CImgui_Manger::GetInstance()->Get_ModelPath();

                if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::MAP), TEXT("Layer_Model"),
                    ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Player"), &Desc)))
                    return;
            
            }
        }
    }

}

void CTerrain::Late_Update(_float fTimeDelta)
{
    if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
        return;
}

HRESULT CTerrain::Render()
{
    TCHAR szChar[MAX_PATH];
    swprintf_s(szChar, MAX_PATH, L"%.1f %.1f %.1f", vSetModelPos.x, vSetModelPos.y, vSetModelPos.z);
    SetWindowText(g_hWnd, szChar);

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Bind_Resources();

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CTerrain::Ready_Components(void* pArg)
{
    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxNorTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::MAP), TEXT("Prototype_Component_VIBuffer_Terrain"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), pArg)))
        return E_FAIL;

    if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::MAP), TEXT("Prototype_Component_Texture_Terrain"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
        return E_FAIL;

    return S_OK;
}

HRESULT CTerrain::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_Shader_Resource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
        return E_FAIL;
    
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_DiffuseTexture", 0)))
        return E_FAIL;

    const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);
    if (nullptr == pLightDesc)
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
        return E_FAIL;

    return S_OK;
}

CTerrain* CTerrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTerrain* pInstance = new CTerrain(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CTerrain"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTerrain::Clone(void* pArg)
{
    CTerrain* pInstance = new CTerrain(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CTerrain"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTerrain::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pShaderCom);
}
