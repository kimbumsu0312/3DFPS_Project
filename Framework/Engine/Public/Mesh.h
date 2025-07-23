#pragma once
#include "VIBuffer.h"
NS_BEGIN(Engine)

class ENGINE_DLL CMesh final : public CVIBuffer
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& Prototype);
	virtual ~CMesh() = default;

public:
	virtual HRESULT				Initialize_Prototype(MODELTYPE eType, const SAVE_MESH& pMesh, const vector<class CBone*>& Bones, const vector<SAVE_BONE>& BonesData);
	virtual HRESULT				Initialize(void* pArg) override;

public:
	HRESULT						Bind_BoneMatrices(class CShader* pShader, const _char* pConstantName, const vector<class CBone*>& Bones);
	_uint						Get_MaterialIndex() const { return m_iMaterialIndex; }

private:
	_wstring					m_szName = {};
	_uint						m_iMaterialIndex = {};
	_uint						m_iNumBones = {};
	vector<_int>				m_BoneIndices;
	_float4x4					m_BoneMatrices[g_iMaxNumBones] = {};

	vector<_float4x4>			m_OffsetMatrices;

private:
	HRESULT						Ready_Vertices_For_NonAnim(const SAVE_MESH& pMesh);
	HRESULT						Ready_Vertices_For_Anim(const SAVE_MESH& pMesh, const vector<CBone*>& Bones, const vector<SAVE_BONE>& BonesData);

public:
	static CMesh*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eType, const SAVE_MESH& pMesh, const vector<class CBone*>& Bones, const vector<SAVE_BONE>& BonesData);
	virtual CComponent*			Clone(void* pArg) override;
	virtual void				Free() override;
};

NS_END