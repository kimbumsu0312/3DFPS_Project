#pragma once
#include "ContainerObject.h"
#include "Client_Struct.h"

NS_BEGIN(Engine)
class CCollider;
NS_END
NS_BEGIN(Client)
class CMonSpawner final : public CContainerObject
{
public:
	typedef struct MonSpawnerDesc{
		_vector	vSpawnerPostion;
		_float3 vExtents;
		_float3 vCenter;
		_wstring szPoolPath;
		vector<POOLMONDESC> MonDesc;
	}MONSPAWNERDESC;
private:
	CMonSpawner(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonSpawner(const CMonSpawner& Prototype);
	virtual ~CMonSpawner() = default;

public:
	virtual HRESULT					Initialize_Prototype() override;
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

	virtual void					OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision)override;
private:
	CCollider*						m_pColliderCom = { nullptr };
	vector<POOLMONDESC>				m_MonDesc;
	_wstring						m_szPoolPath = {};
	_bool							m_IsSpawn = {};
private:
	HRESULT							Ready_Components(void* pArg);
	HRESULT							Ready_PartObjects();

public:
	static CMonSpawner*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};

NS_END