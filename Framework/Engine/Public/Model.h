#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class ENGINE_DLL CModel final : public CComponent
{
private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel(const CModel& Prototype);
	virtual ~CModel() = default;

public:
	virtual HRESULT					Initialize_Prototype(const SAVE_MODEL& pModelData);
	virtual HRESULT					Initialize(void* pArg);
	virtual HRESULT					Render(_uint iMeshIndex);

public:
	void							Set_Animation(_uint iIndex);

public:
	HRESULT							Bind_Materials(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex, _int iTexIndex, _uint iIndex);
	HRESULT							Bind_BoneMatrices(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex);
	void							Play_Animation(_float fTimeDelta);

	_uint							Get_NumMeshes() const { return m_iNumMeshes; }

private:

	MODELTYPE						m_eModelType = {};
	_float4x4						m_PreTransformMatrix = {};

	_uint							m_iNumMeshes = {};
	vector<class CMesh*>			m_Meshes;

	_uint							m_iNumMaterials = {};
	vector<class CMeshMaterial*>	m_Materials;

	vector<class CBone*>			m_Bones;

	_uint							m_iCurrentAnimIndex = { 0 };
	_uint							m_iNumAnimations = { 0 };
	vector<class CAnimation*>		m_Animations;

private:
	HRESULT							Ready_Meshes(const SAVE_MODEL& pModelData);
	HRESULT							Ready_Materials(const SAVE_MODEL& pModelData);
	HRESULT							Ready_Bones(const SAVE_MODEL& pModelData);
	HRESULT							Ready_Animations(const SAVE_MODEL& pModelData);

public:
	static CModel*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const SAVE_MODEL& pModelData);
	virtual CComponent*				Clone(void* pArg) override;
	virtual void					Free() override;
};

NS_END