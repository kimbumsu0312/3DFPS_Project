#pragma once
#include "VIBuffer_Instance.h"

NS_BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Mesh_Instance final : public CVIBuffer_Instance
{
public:
	typedef struct tagMeshInstanceDesc : public CVIBuffer_Instance::INSTANCE_DESC
	{
		_float3			vPivot;
		_float2			vSpeed;
		_float2			vLifeTime;
		_bool			isLoop;
	}MESH_INSTANCE_DESC;

private:
	CVIBuffer_Mesh_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Mesh_Instance(const CVIBuffer_Mesh_Instance& Prototype);
	virtual ~CVIBuffer_Mesh_Instance() = default;

public:
	virtual HRESULT Initialize_Prototype(const INSTANCE_DESC* pDesc) override;
	virtual HRESULT Initialize(void* pArg);
	virtual HRESULT	Bind_Resources() override;

public:
	void Spread(_float fTimeDelta);
	void Drop(_float fTimeDelta);

private:
	_float3					m_vPivot = {};
	_float*					m_pSpeeds = {};
	_bool					m_isLoop = {};

public:
	static CVIBuffer_Mesh_Instance* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const INSTANCE_DESC* pDesc);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END