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
	_float4x4*			Get_PtrCombinedTransformationMatrix() { return &m_CombinedTransformationMatrix; }

	_matrix				Get_OldTransitionTransformationMatrix() const { return XMLoadFloat4x4(&m_OldTransitionTransformationMatrix); }
	_matrix				Get_NewTransitionTransformationMatrix() const { return XMLoadFloat4x4(&m_NewTransitionTransformationMatrix); }

	void				Set_NewTransitionTransformationMatrix(_matrix NewTransitionTransformationMatrix);

	void				Set_TransformationMatrix(_fmatrix Matrix) { XMStoreFloat4x4(&m_TransformationMatrix, Matrix); }
	void				Set_CombindMationMatinMatrix_PosReset() {
							_matrix Combined = XMLoadFloat4x4(&m_CombinedTransformationMatrix);
							Combined.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f);
							XMStoreFloat4x4(&m_CombinedTransformationMatrix, Combined);	}

public:
	HRESULT				Initialize(const SAVE_BONE& pBone);
	void				Update_CombinedTransformationMatrix(const _float4x4& PreTransformMatrix, const vector<CBone*>& Bones);
	_bool				Compare_Name(_wstring pName) { return pName == m_szName; }
	void				Anim_Change_Set();

private:
	_wstring			m_szName = {};
	_float4x4			m_DefaultTransformationMatrix = {};
	_float4x4			m_OldTransitionTransformationMatrix = {};
	_float4x4			m_NewTransitionTransformationMatrix = {};

	_float4x4			m_TransformationMatrix = {};
	_float4x4			m_CombinedTransformationMatrix = {};

	_int				m_iParentBoneIndex = { -1 };
public:
	static CBone*		Create(const SAVE_BONE& pBone);
	CBone*				Clone();
	virtual void		Free() override;
};
NS_END

