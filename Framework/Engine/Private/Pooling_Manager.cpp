#include "pch.h"
#include "Pooling_Manager.h"
#include "GameInstance.h"
#include "PoolingObject.h"

CPooling_Manager::CPooling_Manager() : m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CPooling_Manager::Add_Object_ToPool(const _wstring& szPoolingPath, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	CPoolingObject* pGameObject = dynamic_cast<CPoolingObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pGameObject)
		return E_FAIL;

	auto iter = m_ObjectPool.find(szPoolingPath);

	if (iter != m_ObjectPool.end())
	{
		iter->second.push(pGameObject);
		return S_OK;
	}

	queue<CPoolingObject*> pObjects;

	pObjects.push(pGameObject);
	m_ObjectPool.emplace(szPoolingPath, pObjects);
	
	return S_OK;
}

HRESULT CPooling_Manager::Add_Pool_ToLayer(const _wstring& szPoolingPath, _uint iLayerLevelIndex, const _wstring& strLayerTag, void* pArg)
{
	auto iter = m_ObjectPool.find(szPoolingPath);
	if (iter != m_ObjectPool.end())
		return E_FAIL;
	
	if (FAILED(iter->second.front()->Initialize_Pool(pArg)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_PoolGameObject_ToLayer(iter->second.front(), iLayerLevelIndex, strLayerTag)))
		return E_FAIL;

	iter->second.pop();

	return S_OK;
}

HRESULT CPooling_Manager::Return_Object(CPoolingObject* pObject, const _wstring& szPoolingPath)
{
	pObject->Return_Pool();
	m_ObjectPool.at(szPoolingPath).push(pObject);

	return S_OK;
}

CPooling_Manager* CPooling_Manager::Create()
{
	return new CPooling_Manager();
}

void CPooling_Manager::Free()
{
	__super::Free();

	for (auto& pQueue : m_ObjectPool)
	{
		for (_int i = 0; i < pQueue.second.size(); ++i)
		{
			Safe_Release(pQueue.second.front());
			pQueue.second.pop();
		}
	}
	m_ObjectPool.clear();

	Safe_Release(m_pGameInstance);
}
