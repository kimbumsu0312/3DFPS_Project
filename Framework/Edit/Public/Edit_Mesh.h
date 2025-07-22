#pragma once
#include "VIBuffer.h"
#include "Engine_Defines.h"

NS_BEGIN(Edit)
class CEdit_Mesh final : public CVIBuffer
{
private:
	CEdit_Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEdit_Mesh(const CEdit_Mesh& Prototype);
	virtual ~CEdit_Mesh() = default;

public:
	virtual HRESULT			Initialize_Prototype(MODELTYPE eType, const aiMesh* pAIMesh, const vector<class CEdit_Bone*>& Bones, _fmatrix PreTransformMatrix, SAVE_MODEL* pModelData);
	virtual HRESULT			Initialize(void* pArg) override;

public:
	_uint					Get_MaterialIndex() { return m_iMaterialIndex; }
	HRESULT					Bind_BoneMatrices(class CShader* pShader, const _char* pConstantName, const vector<class CEdit_Bone*>& Bones);
	virtual _bool			IsPicked(MODELTYPE eType, class CTransform& pTransform, _float3& pOut);
private:
	_char					m_szName[MAX_PATH] = {};
	_uint					m_iMaterialIndex = {};
	vector<_int>			m_BoneIndices;
	_float4x4				m_BoneMatrices[g_iMaxNumBones] = {};

	vector<_float4x4>		m_OffsetMatrices;

	SAVE_MESH				m_MeshData = {};

private:
	HRESULT					Ready_Vertices_For_NonAnim(const aiMesh* pAiMesh, _fmatrix PreTransformMatrix);
	HRESULT					Ready_Vertices_For_Anim(const aiMesh* pAiMesh, const vector<class CEdit_Bone*>& Bones);

public:
	static CEdit_Mesh*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eType, const aiMesh* pAIMesh, const vector<class CEdit_Bone*>& Bones, _fmatrix PreTransformMatrix, SAVE_MODEL* pModelData);
	virtual CComponent*		Clone(void* pArg) override;
	virtual void			Free() override;
};

NS_END