#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CCollision_Manager final : public CBase
{
private:
	CCollision_Manager();
	virtual ~CCollision_Manager() = default;

public:
	HRESULT						Initialize(_uint iNimLayerFilter);
	HRESULT						Update();
	
	HRESULT						Add_ColliderCheck(class CGameObject* pObject, class CCollider* pCollider);
	HRESULT						Add_ColliderRay(_uint iLayLayer, _uint iObjType, RAY_DESC& RayDesc);

	HRESULT						Set_LayerFilter(_uint iLayerNum, _uint iLayerFilter);
private:
	class CGameInstance*		m_pGameInstance = { nullptr };
	list<COLLISIONENTRY>		m_ColliderObject;

	vector<_uint>				m_LayerFilter;
	_uint						m_iNumLayerSize = {};

private:
	_bool						Layer_Check(const COLLISIONENTRY& ObjectA, const COLLISIONENTRY& ObjectB);

public:
	static CCollision_Manager*	Create(_uint iNimLayerFilter);
	virtual void				Free() override;

};
NS_END
