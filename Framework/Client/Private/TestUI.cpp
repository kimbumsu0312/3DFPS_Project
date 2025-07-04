#include "pch.h"
#include "TestUI.h"

CTestUI::CTestUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CTestUI::CTestUI(const CTestUI& Prototype) : CUIObject(Prototype)
{
}

HRESULT CTestUI::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CTestUI::Initialize(void* pArg)
{
    m_vLocalPos.x = g_iWinSizeX >> 1;
    m_vLocalPos.y = g_iWinSizeY >> 1;
    m_vLocalSize.x = g_iWinSizeX;
    m_vLocalSize.y = g_iWinSizeY;

    if (FAILED(__super::Initialize()))
        return E_FAIL;

    return S_OK;
}

void CTestUI::Priority_Update(_float fTimeDelta)
{
}

void CTestUI::Update(_float fTimeDelta)
{
}

void CTestUI::Late_Update(_float fTimeDelta)
{

}

HRESULT CTestUI::Render()
{
	return S_OK;
}

CTestUI* CTestUI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTestUI* pInstance = new CTestUI(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CTestUI"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CTestUI::Clone(void* pArg)
{
    CTestUI* pInstance = new CTestUI(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CTestUI"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CTestUI::Free()
{
    __super::Free();
}
