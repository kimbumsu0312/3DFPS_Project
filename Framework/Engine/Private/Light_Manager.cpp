#include "pch.h"
#include "Light_Manager.h"
#include "Light.h"

CLight_Manager::CLight_Manager()
{
}

const LIGHT_DESC* CLight_Manager::Get_LightDesc(_wstring LightTag)
{
    CLight* pLight = Find_Light(LightTag);

    if (pLight == nullptr)
        return nullptr;

    return pLight->Get_LightDesc();
}

HRESULT CLight_Manager::Add_Light(_wstring LightTag, LIGHT_DESC& LightDesc)
{
    CLight* pLight = Find_Light(LightTag);
    
    if (nullptr != pLight)
        return E_FAIL;

    pLight = CLight::Create(LightDesc);

    if (nullptr == pLight)
        return E_FAIL;

    m_Lights.emplace(LightTag, pLight);

    return S_OK;
}


HRESULT CLight_Manager::Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
    for (auto& pLight : m_Lights)
    {
        if(pLight.second->Get_LightOnOff() == true)
            pLight.second->Render(pShader, pVIBuffer);
    }
    return S_OK;
}

_bool CLight_Manager::Update_LightPotion(_wstring LightTag, _float4 LightPos)
{
    CLight* pLight = Find_Light(LightTag);

    if (pLight == nullptr)
        return false;

    pLight->Update_Postion(LightPos);
    return true;
}

_bool CLight_Manager::OnOff_Light(_wstring LightTag, _bool isOnoff)
{
    CLight* pLight = Find_Light(LightTag);

    if (pLight == nullptr)
        return false;

    pLight->On_Light(isOnoff);

    return true;
}

CLight* CLight_Manager::Find_Light(_wstring LightTag)
{
    auto iter = m_Lights.find(LightTag);
    if (iter == m_Lights.end())
        return nullptr;

    return iter->second;
}

CLight_Manager* CLight_Manager::Create()
{
    return new CLight_Manager();
}

void CLight_Manager::Free()
{
    __super::Free();

    for (auto& pLight : m_Lights)
        Safe_Release(pLight.second);

    m_Lights.clear();
}
