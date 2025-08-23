#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	typedef struct tagColliderDesc
	{
		_uint						iLayer;
		_uint						iObjType;

	}COLLIDER_DESC;

private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	virtual HRESULT							Initialize_Prototype(COLLIDER eType);
	virtual HRESULT							Initialize(void* pArg) override;
	void									Update(_fmatrix WorldMatrix);

public:
	_bool									Intersect(CCollider* pTarget);
	_bool									Intersect(RAY_DESC& RayDesc);
	_float3&								Get_Intersect_Normal() { return m_vIntersect_Nromal;}
	COLLIDER								Get_ColliderType() { return m_eType; }
	_uint									Get_Layer() { return m_iLayer; }
	_uint									Get_ObjType() { return m_iObjType; }

#ifdef _DEBUG
	HRESULT									Render();
#endif

private:
	COLLIDER								m_eType = { COLLIDER::END };
	class CBounding*						m_pBounding = { nullptr };
	_bool									m_isColl = { false };
	_uint									m_iLayer;
	_uint									m_iObjType;
	_float3									m_vIntersect_Nromal = {};

#ifdef _DEBUG
	PrimitiveBatch<VertexPositionColor>*	m_pBatch = { nullptr };
	BasicEffect*							m_pEffect = { nullptr };
	ID3D11InputLayout*						m_pInputLayout = { nullptr };

#endif
public:
	static CCollider*						Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, COLLIDER eType);
	virtual CComponent*						Clone(void* pArg) override;
	virtual void							Free() override;
};

NS_END