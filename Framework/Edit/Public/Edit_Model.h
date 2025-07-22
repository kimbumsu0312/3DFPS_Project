#pragma once
#include "Component.h"
#include "Edit_Defines.h"

NS_BEGIN(Engine)
class CTransform;
NS_END
NS_BEGIN(Edit)
class CEdit_Model final : public CComponent
{
public:
	typedef struct tagModelDesc
	{
		string szModelName;
		string szModelPath;
	}MODEl_DESC;

private:
	CEdit_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEdit_Model(const CEdit_Model& Prototype);
	virtual ~CEdit_Model() = default;

public:
	virtual HRESULT						Initialize_Prototype(MODELTYPE eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix, void* pArg);
	virtual HRESULT						Initialize(void* pArg);

	virtual HRESULT						Render(_uint iMeshIndex);
	
public:
	HRESULT								Bind_Materials(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex, aiTextureType eTextureType, _uint iIndex);
	HRESULT								Bind_BoneMatrices(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex);
	void								Play_Animation(_float fTimeDelta);

	_uint								Get_NumMeshes() const {	return m_ModelData.iNumMeshes;	}
	_bool								Selete_Model(CTransform& pTransform, _float3& pOut);
	SAVE_MODEL*							Get_ModelData() { return &m_ModelData;}
	MODELTYPE							Get_ModelType() { return m_ModelData.eModel; }
private:
	const aiScene*						m_pAIScene = { nullptr };
	Assimp::Importer					m_Importer = {};
	_float4x4							m_PreTransformMatrix = {};
	
	vector<class CEdit_Mesh*>			m_Meshes;
	vector<class CEdit_MeshMaterial*>	m_Materials;
	vector<class CEdit_Bone*>			m_Bones;

	SAVE_MODEL							m_ModelData = {};
private:
	HRESULT								Ready_Meshes();
	HRESULT								Ready_Materials(const _char* pModelFilePath);
	HRESULT								Ready_Bones(const aiNode* pAINode, _int iParentIndex);

public:
	static CEdit_Model*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix, void* pArg);
	virtual CComponent*					Clone(void* pArg) override;
	virtual void						Free() override;
};

NS_END