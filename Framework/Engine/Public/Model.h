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
	HRESULT							Bind_Materials(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex, _int iTexIndex, _uint iIndex);
	HRESULT							Bind_BoneMatrices(class CShader* pShader, const _char* pConstantName, _uint iMeshIndex);
	
	_bool							Play_Animation(_float fTimeDelta, ANIM_STATUS eAnimStatus, const ANIMEFRAME& pAnimFrameData, _int RootNodeIndex);
	_bool							Play_Animation(_float fTimeDelta, ANIM_STATUS eAnimStatus, const ANIMEFRAME& pAnimFrameData, _int RootNodeIndex, _int UperBone);

	_uint							Get_NumMeshes() const { return m_iNumMeshes; }
	_float4x4*						Get_BoneMatrix(const _wstring pBoneName);
	_float3*						Get_PtrMovePos() { return &m_vMovePos; }
	_float4*						Get_PtrMoveRot() { return &m_vMoveRot; }
	void							Reset_MovePos();
	void							Set_TransitionTime();
	void							Set_Animations(_uint AnimiIndex, _bool IsLoop);
	void							Set_Upper(_int BoneIndex, _float fPeach, _bool IsUpperSet);
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

	_bool							m_bisLoop = {};
	_float3							m_vMovePos = {};
	_float4							m_vMoveRot = {};
	_vector							m_vPreRootPos = {};
	_vector							m_vPreRootRot = {};

	_float							m_fTransitionTime = { 1.f };
	_float							m_fTransitionDuration = { 0.2f };

	_bool							m_bIsUpperSet = { false };
	_float							m_fPitch = {};
	_int							m_iUpperBoneIndex = {};
	_bool							m_bIsAnimFished = { false };
	_bool							m_bPreRootSet = { true };
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