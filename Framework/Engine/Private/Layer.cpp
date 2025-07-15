#include "pch.h"
#include "Layer.h"
#include "GameObject.h"

CLayer::CLayer()
{
}

CComponent* CLayer::Get_Component(const _wstring& strComponentTag, _uint iIndex)
{
	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iIndex; i++)
		++iter;

	return (*iter)->Get_Component(strComponentTag);
}

HRESULT CLayer::Add_GameObject(CGameObject* pGameObject)
{
	m_GameObjects.push_back(pGameObject);
	return S_OK;
}

void CLayer::Priority_Update(_float fTimeDelta)
{
	for (auto pGameObject = m_GameObjects.begin(); pGameObject != m_GameObjects.end();)
	{		
		if ((*pGameObject)->IsDead())
		{
			switch ((*pGameObject)->IsObjectType())
			{
			case OBJECTTYPE::GAMEOBJECT:
				(*pGameObject)->On_Dead();
				break;
			case OBJECTTYPE::POLLINGOBJECT:
				(*pGameObject)->On_Dead();
				pGameObject = m_GameObjects.erase(pGameObject);
				break;
			}
		}
		else {
			(*pGameObject)->Priority_Update(fTimeDelta);
			++pGameObject;
		}
	}
}

void CLayer::Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Update(fTimeDelta);
	}
}

void CLayer::Late_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Late_Update(fTimeDelta);
	}
}

CLayer* CLayer::Create()
{
	return new CLayer();
}

void CLayer::Free()
{
	__super::Free();

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();
}
