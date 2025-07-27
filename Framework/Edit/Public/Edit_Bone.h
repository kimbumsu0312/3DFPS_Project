#pragma once
#include "Base.h"
#include "Edit_Defines.h"

NS_BEGIN(Edit)
class CEdit_Bone final : public CBase
{
private:
	CEdit_Bone();
	virtual ~CEdit_Bone() = default;

public:
	_matrix				Get_CombinedTransformationMatrix() const {
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);}

	void Set_TransformationMatrix(_fmatrix Matrix) {
		XMStoreFloat4x4(&m_TransformationMatrix, Matrix);
	}

public:
	HRESULT				Initialize(const aiNode* pAINode, _int iParentBoneIndex, SAVE_MODEL* pModelData);
	void				Update_CombinedTransformationMatrix(const _float4x4& PreTransformMatrix, const vector<CEdit_Bone*>& Bones);

	_bool				Compare_Name(const _char* pName) {	return !strcmp(pName, m_szName);}
private:
	_char				m_szName[MAX_PATH] = {};
	_float4x4			m_TransformationMatrix = {};
	_float4x4			m_CombinedTransformationMatrix = {};

	_int				m_iParentBoneIndex = { -1 };
	SAVE_BONE			m_Bone;
public:
	static CEdit_Bone*	Create(const aiNode* pAINode, _int iParentBoneIndex, SAVE_MODEL* pModelData);
	CEdit_Bone*			Clone();
	virtual void		Free() override;
};
NS_END
