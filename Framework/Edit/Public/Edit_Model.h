#pragma once
#include "Component.h"
#include "Engine_Defines.h"

NS_BEGIN(Edit)
class CEdit_Model final : public CComponent
{
private:
	CEdit_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEdit_Model(const CEdit_Model& Prototype);
	virtual ~CEdit_Model() = default;

public:
	virtual HRESULT						Initialize_Prototype(MODELTYPE eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
	virtual HRESULT						Initialize(void* pArg);

	virtual HRESULT						Render(_uint iMeshIndex);
	
public:
	HRESULT								Bind_Materials(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex, aiTextureType eTextureType, _uint iIndex);
	_uint								Get_NumMeshes() const {	return m_iNumMeshes;	}
	_bool								Selete_Model(CTransform& pTransform, _float3& pOut);
private:
	const aiScene*						m_pAIScene = { nullptr };
	Assimp::Importer					m_Importer = {};
	MODELTYPE							m_eModelType = {};
	_float4x4							m_PreTransformMatrix = {};
	
	
	_uint								m_iNumMeshes = {};
	vector<class CEdit_Mesh*>			m_Meshes;
	
	_uint								m_iNumMaterials = {};
	vector<class CEdit_MeshMaterial*>	m_Materials;

private:
	HRESULT								Ready_Meshes();
	HRESULT								Ready_Materials(const _char* pModelFilePath);

public:
	static CEdit_Model*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
	virtual CComponent*					Clone(void* pArg) override;
	virtual void						Free() override;
};

NS_END