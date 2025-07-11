#include "pch.h"
#include "PoolingObject.h"

CPoolingObject::CPoolingObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CGameObject{pDevice, pContext}
{
}

CPoolingObject::CPoolingObject(const CPoolingObject& Prototype) : CGameObject(Prototype)
{
}

HRESULT CPoolingObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPoolingObject::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	POOLOBJECT_DESC* pDesc = static_cast<POOLOBJECT_DESC*>(pArg);

	m_szPoolingPath = pDesc->szPoolingPath;
	m_eObjectType = OBJECTTYPE::POLLINGOBJECT;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CPoolingObject::Priority_Update(_float fTimeDelta)
{
}

void CPoolingObject::Update(_float fTimeDelta)
{
}

void CPoolingObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CPoolingObject::Render()
{
	return S_OK;
}

void CPoolingObject::OnDead()
{
	m_pGameInstance->Return_Object(this, m_szPoolingPath);
}

HRESULT CPoolingObject::Initialize_Pool(void* pArg)
{
	return S_OK;
}

void CPoolingObject::Return_Pool()
{
}


void CPoolingObject::Free()
{
	__super::Free();
}
