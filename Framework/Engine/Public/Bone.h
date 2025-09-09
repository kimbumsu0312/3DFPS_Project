#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CBone final : public CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public:
	_matrix				Get_CombinedTransformationMatrix() const { return XMLoadFloat4x4(&m_CombinedTransformationMatrix); }
	_matrix				Get_TransformationMatrix() const { return XMLoadFloat4x4(&m_TransformationMatrix); }
	_float4x4*			Get_PtrCombinedTransformationMatrix() { return &m_CombinedTransformationMatrix; }

	void				Set_TransformationMatrix(_fmatrix Matrix) { XMStoreFloat4x4(&m_TransformationMatrix, Matrix); }
	void				Set_CombinedTransformationMatrix(_fmatrix Matrix) { XMStoreFloat4x4(&m_CombinedTransformationMatrix, Matrix); }

	void				Set_CombindMationMatinMatrix_PosReset();
	void				Set_CombindMationMatinMatrix_RotReset(const _float4x4& PreTransformMatrix);

	void				Set_TransformationToCombind() { m_CombinedTransformationMatrix = m_TransformationMatrix; }
	void				Set_RotBonePitch(_float fPitch, _bool IsTrans);

	_fvector			Get_XQut();
public:
	HRESULT				Initialize(const SAVE_BONE& pBone);
	void				Update_CombinedTransformationMatrix(const _float4x4& PreTransformMatrix, const vector<CBone*>& Bones);
	void				Update_CombinedTransformationMatrix_Transition(const _float4x4& PreTransformMatrix, const vector<CBone*>& Bones);
	_bool				Compare_Name(_wstring pName) { return pName == m_szName; }

private:
	_wstring			m_szName = {};
	_float4x4			m_TransformationMatrix = {};
	_float4x4			m_CombinedTransformationMatrix = {};
	_float				m_fPitch = {};

	_int				m_iParentBoneIndex = { -1 };
public:
	static CBone*		Create(const SAVE_BONE& pBone);
	CBone*				Clone();
	virtual void		Free() override;
};
NS_END

