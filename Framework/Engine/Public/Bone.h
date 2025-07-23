#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CBone final : public CBase
{
private:
	CBone();
	virtual ~CBone() = default;

public:
	_matrix Get_CombinedTransformationMatrix() const {
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix); }

	void Set_TransformationMatrix(_fmatrix Matrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, Matrix); }

public:

	HRESULT Initialize(const SAVE_BONE& pBone);
	void Update_CombinedTransformationMatrix(const _float4x4& PreTransformMatrix, const vector<CBone*>& Bones);
	_bool				Compare_Name(_wstring pName) { return pName == m_szName; }
private:
	_wstring			m_szName = {};
	_float4x4			m_TransformationMatrix = {};
	_float4x4			m_CombinedTransformationMatrix = {};

	_int				m_iParentBoneIndex = { -1 };

public:
	static CBone*		Create(const SAVE_BONE& pBone);
	virtual void		Free() override;
};
NS_END

