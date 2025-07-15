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
	virtual HRESULT					Initialize_Prototype(MODELTYPE eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
	virtual HRESULT					Initialize(void* pArg);

	HRESULT							Render(CShader* pShader, const _char* pConstantName, _uint iSRVIndex, _uint iShaderIndex);
private:
	const aiScene*					m_pAIScene = { nullptr };
	Assimp::Importer				m_Importer = {};
	MODELTYPE						m_eModelType = {};
	_float4x4						m_PreTransformMatrix = {};

	_uint							m_iNumMeshes = {};
	vector<class CMesh*>			m_Meshes;
	
	_uint							m_iNumMaterials = {};
	vector<class CMeshMaterial*>	m_Materials;

private:
	HRESULT							Ready_Meshes();
	HRESULT							Ready_Materials(const _char* pModelFilePath);

public:
	static CModel*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix);
	virtual CComponent*				Clone(void* pArg) override;
	virtual void					Free() override;
};

NS_END