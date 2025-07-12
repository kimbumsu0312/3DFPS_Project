#include "pch.h"
#include "Item_2D_Render.h"

Item_2D_Render::Item_2D_Render(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : C2DItem{pDevice, pContext}
{
}

Item_2D_Render::Item_2D_Render(const Item_2D_Render& Prototype) : C2DItem(Prototype)
{
}

HRESULT Item_2D_Render::Initialize_Prototype()
{
	return S_OK;
}

HRESULT Item_2D_Render::Initialize(void* pArg)
{
    if (__super::Initialize(pArg))
        return E_FAIL;

	return S_OK;
}

void Item_2D_Render::Priority_Update(_float fTimeDelta)
{
}

void Item_2D_Render::Update(_float fTimeDelta)
{
}

void Item_2D_Render::Late_Update(_float fTimeDelta)
{
}

HRESULT Item_2D_Render::Render()
{
	return S_OK;
}

Item_2D_Render* Item_2D_Render::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    Item_2D_Render* pInstance = new Item_2D_Render(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CLogo_UI"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* Item_2D_Render::Clone(void* pArg)
{
    Item_2D_Render* pInstance = new Item_2D_Render(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : Item_2D_Render"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void Item_2D_Render::Free()
{
	__super::Free();
}
