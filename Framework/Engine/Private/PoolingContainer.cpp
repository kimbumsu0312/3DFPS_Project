#include "pch.h"
#include "PoolingContainer.h"
#include "PartObject.h"

CPoolingContainer::CPoolingContainer(ID3D11Device* pDeivec, ID3D11DeviceContext* pContext) : CGameObject{ pDeivec, pContext }
{
}

CPoolingContainer::CPoolingContainer(const CPoolingContainer& Prototype) : CGameObject(Prototype)
{
}

HRESULT CPoolingContainer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPoolingContainer::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CPoolingContainer::Priority_Update(_float fTimeDelta)
{
	for (auto& Pair : m_PartObjects)
		Pair.second->Priority_Update(fTimeDelta);
}

void CPoolingContainer::Update(_float fTimeDelta)
{
	for (auto& Pair : m_PartObjects)
		Pair.second->Update(fTimeDelta);
}

void CPoolingContainer::Late_Update(_float fTimeDelta)
{
	for (auto& Pair : m_PartObjects)
		Pair.second->Late_Update(fTimeDelta);
}

HRESULT CPoolingContainer::Render()
{
	return S_OK;
}

void CPoolingContainer::On_Dead()
{
	__super::On_Dead();
}

HRESULT CPoolingContainer::Initialize_Pool(void* pArg)
{
	return S_OK;
}

HRESULT CPoolingContainer::Add_PartObject(const _wstring& strPartObjectTag, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	if (nullptr != Find_PartObject(strPartObjectTag))
		return E_FAIL;

	CPartObject* pPartObject = dynamic_cast<CPartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pPartObject)
		return E_FAIL;

	m_PartObjects.emplace(strPartObjectTag, pPartObject);

	return S_OK;
}

CPartObject* CPoolingContainer::Find_PartObject(const _wstring& strPartObjectTag)
{
	auto    iter = m_PartObjects.find(strPartObjectTag);
	if (iter == m_PartObjects.end())
		return nullptr;

	return iter->second;
}

void CPoolingContainer::Free()
{
	__super::Free();

	for (auto& Pair : m_PartObjects)
		Safe_Release(Pair.second);

	m_PartObjects.clear();

}
