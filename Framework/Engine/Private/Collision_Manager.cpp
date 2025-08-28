#include "pch.h"
#include "Collision_Manager.h"
#include "GameObject.h"
#include "Collider.h"

CCollision_Manager::CCollision_Manager() : m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CCollision_Manager::Initialize(_uint iNimLayerFilter)
{
    m_LayerFilter.resize(iNimLayerFilter);
    m_iNumLayerSize = iNimLayerFilter;

	return S_OK;
}

HRESULT CCollision_Manager::Update()
{
    for (auto iter_A = m_ColliderObject.begin(); iter_A != m_ColliderObject.end(); ++iter_A)
    {
        auto iter_B = iter_A;
        ++iter_B;
        
        for (; iter_B != m_ColliderObject.end(); ++iter_B)
        {
            _bool IsCheckAB = Layer_Check(*iter_A, *iter_B);
            _bool IsCheckBA = Layer_Check(*iter_B, *iter_A);

            if(!IsCheckAB && !IsCheckBA)
                continue;

            if((*iter_A).eCollider == COLLIDER::RAY && (*iter_B).eCollider == COLLIDER::RAY)
                continue;

            if ((*iter_A).eCollider == COLLIDER::RAY)
            {
                if ((*iter_B).pCollider->Intersect((*iter_A).RayDesc))
                {
                    if (IsCheckBA)
                        (*iter_B).pObject->OnCollision((*iter_B), (*iter_A));

                }
            }
            else if ((*iter_B).eCollider == COLLIDER::RAY)
            {
                if ((*iter_A).pCollider->Intersect((*iter_B).RayDesc))
                {
                    if (IsCheckAB)
                        (*iter_A).pObject->OnCollision((*iter_A), (*iter_B));
                }
            }
            else
            {
                if ((*iter_A).pCollider->Intersect((*iter_B).pCollider))
                {
                    if (IsCheckAB)
                        (*iter_A).pObject->OnCollision((*iter_A), (*iter_B));

                    if (IsCheckBA)
                        (*iter_B).pObject->OnCollision((*iter_B), (*iter_A));
                }
            }
        }
    }

    for (auto& pRenderObject : m_ColliderObject)
    {
        Safe_Release(pRenderObject.pObject);
        Safe_Release(pRenderObject.pCollider);
    }
    m_ColliderObject.clear();

	return S_OK;
}

HRESULT CCollision_Manager::Add_ColliderCheck(CGameObject* pObject, CCollider* pCollider)
{
    COLLISIONENTRY ObjectDesc{};

    if (pObject == nullptr || pCollider == nullptr)
        return E_FAIL;

    ObjectDesc.pObject = pObject;
    Safe_AddRef(ObjectDesc.pObject);

    ObjectDesc.pCollider = pCollider;
    Safe_AddRef(ObjectDesc.pCollider);
    
    ObjectDesc.iLayer = pCollider->Get_Layer();
    ObjectDesc.iObjType = pCollider->Get_ObjType();
    ObjectDesc.eCollider = pCollider->Get_ColliderType();

    m_ColliderObject.push_back(ObjectDesc);

	return S_OK;
}

HRESULT CCollision_Manager::Add_ColliderRay(_uint iLayLayer, _uint iObjType, RAY_DESC& RayDesc)
{
    COLLISIONENTRY ObjectDesc{};

    ObjectDesc.pObject = nullptr;
    ObjectDesc.pCollider = nullptr;

    ObjectDesc.iLayer = iLayLayer;
    ObjectDesc.iObjType = iObjType;
    ObjectDesc.eCollider = COLLIDER::RAY;

    ObjectDesc.RayDesc = RayDesc;
    m_ColliderObject.push_back(ObjectDesc);

    return S_OK;
}

HRESULT CCollision_Manager::Set_LayerFilter(_uint iLayerNum, _uint iLayerFilter)
{
    if (iLayerNum >= m_iNumLayerSize)
        return E_FAIL;

    m_LayerFilter[iLayerNum] = iLayerFilter;

    return S_OK;
}

_bool CCollision_Manager::Layer_Check(const COLLISIONENTRY& ObjectA, const COLLISIONENTRY& ObjectB)
{
    return (m_LayerFilter[ObjectA.iLayer] & (1 << ObjectB.iLayer)) != 0;
}

CCollision_Manager* CCollision_Manager::Create(_uint iNimLayerFilter)
{
    CCollision_Manager* pInstance = new CCollision_Manager();

    if (FAILED(pInstance->Initialize(iNimLayerFilter)))
    {
        MSG_BOX(TEXT("Failed to Created : CCollision_Manager"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCollision_Manager::Free()
{
    __super::Free();

    for (auto& pRenderObject : m_ColliderObject)
    {
        Safe_Release(pRenderObject.pObject);
        Safe_Release(pRenderObject.pCollider);
    }
    m_ColliderObject.clear();

    Safe_Release(m_pGameInstance);
}
