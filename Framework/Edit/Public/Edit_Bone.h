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
	HRESULT				Initialize(const aiNode* pAINode);
	
private:
	_char				m_szName[MAX_PATH] = {};
	_float4x4			m_TransformationMatrix = {};
	_float4x4			m_CombinedTransformationMatrix = {};

public:
	static CEdit_Bone*	Create(const aiNode* pAINode);
	virtual void		Free() override;
};
NS_END
