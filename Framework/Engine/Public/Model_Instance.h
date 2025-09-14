#pragma once
#include "Component.h"
#include "VIBuffer_Instance.h"

NS_BEGIN(Engine)
class ENGINE_DLL CModel_Instance final : public CComponent
{
private:
	CModel_Instance(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel_Instance(const CModel_Instance& Prototype);
	virtual ~CModel_Instance() = default;

public:
	HRESULT							Initialize_Prototype(const SAVE_MODEL& pModelData, const CVIBuffer_Instance::INSTANCE_DESC* pInstance);
	HRESULT							Initialize(void* pArg);

	HRESULT							Bind_Instance();
	HRESULT							Bind_Materials(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex, _int iTexIndex, _uint iIndex);
	HRESULT							Render(_uint iMeshIndex);
	_uint							Get_NumMeshes() const { return m_iNumMeshes; }

public:
	void							Spread(_float fTimeDelta);
private:
	class CVIBuffer_Mesh_Instance*	m_pInstance_Buffer = { nullptr };

	MODELTYPE						m_eModelType = {};
	_float4x4						m_PreTransformMatrix = {};

	_uint							m_iNumMeshes = {};
	vector<class CMesh*>			m_Meshes;

	_uint							m_iNumMaterials = {};
	vector<class CMeshMaterial*>	m_Materials;

private:
	HRESULT							Ready_Instance_Buffer(const CVIBuffer_Instance::INSTANCE_DESC* pDesc);
	HRESULT							Ready_Meshes(const SAVE_MODEL& pModelData);
	HRESULT							Ready_Materials(const SAVE_MODEL& pModelData);
public:
	static CModel_Instance*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const SAVE_MODEL& pModelData, const CVIBuffer_Instance::INSTANCE_DESC* pDesc);
	virtual CComponent*				Clone(void* pArg) override;
	virtual void					Free() override;
};

NS_END