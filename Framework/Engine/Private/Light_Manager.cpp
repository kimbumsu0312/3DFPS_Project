#include "pch.h"
#include "Light_Manager.h"
#include "Light.h"

CLight_Manager::CLight_Manager()
{
}

const LIGHT_DESC* CLight_Manager::Get_LightDesc(_uint iIndex)
{
    auto iter = m_Lights.begin();

    for (size_t i = 0; i < iIndex; i++)
        ++iter;

    return (*iter)->Get_LightDesc();
}

HRESULT CLight_Manager::Add_Light(LIGHT_DESC& LightDesc)
{
    CLight* pLight = CLight::Create(LightDesc);

    if (nullptr == pLight)
        return E_FAIL;
    
    m_Lights.push_back(pLight);

    return S_OK;
}

CLight_Manager* CLight_Manager::Create()
{
    return new CLight_Manager();
}

void CLight_Manager::Free()
{
    __super::Free();

    for (auto& pLight : m_Lights)
        Safe_Release(pLight);

    m_Lights.clear();
}
