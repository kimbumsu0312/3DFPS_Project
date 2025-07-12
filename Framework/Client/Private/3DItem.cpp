#include "pch.h"
#include "3DItem.h"

C3DItem::C3DItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CItem{ pDevice, pContext }
{
}

C3DItem::C3DItem(const C3DItem& Prototype) : CItem (Prototype)
{
}

HRESULT C3DItem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT C3DItem::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void C3DItem::Priority_Update(_float fTimeDelta)
{
}

void C3DItem::Update(_float fTimeDelta)
{
}

void C3DItem::Late_Update(_float fTimeDelta)
{
}

HRESULT C3DItem::Render()
{
	return S_OK;
}

void C3DItem::Free()
{
	__super::Free();
}
