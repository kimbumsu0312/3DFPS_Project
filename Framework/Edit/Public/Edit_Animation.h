#pragma once
#include "Base.h"
#include "Edit_Defines.h"

NS_BEGIN(Edit)
class CEdit_Animation final : public CBase
{
private:
	CEdit_Animation();
	virtual ~CEdit_Animation() = default;

public:
	HRESULT							Initialize(const aiAnimation* pAIAnimation, const vector<class CEdit_Bone*>& Bones);
	void							Update_TransformationMatrices(const vector<class CEdit_Bone*>& Bones, _float fTimeDelta);

private:
	_float							m_fDuration = {};
	_float							m_fTickPerSecond = {};
	_float							m_fCurrentTrackPosition = {};
	_uint							m_iNumChannels = {};
	vector<class CEdit_Channel*>	m_Channels;

public:
	static CEdit_Animation*			Create(const aiAnimation* pAiAnimation, const vector<class CEdit_Bone*> Bones);
	virtual void					Free() override;
};

NS_END