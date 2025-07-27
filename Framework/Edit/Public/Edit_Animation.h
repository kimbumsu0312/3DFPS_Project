#pragma once
#include "Base.h"
#include "Edit_Defines.h"

NS_BEGIN(Edit)
class CEdit_Animation final : public CBase
{
private:
	CEdit_Animation();
	CEdit_Animation(const CEdit_Animation& Prototype);
	virtual ~CEdit_Animation() = default;

public:
	HRESULT							Initialize(const aiAnimation* pAIAnimation, const vector<class CEdit_Bone*>& Bones, SAVE_MODEL* pModelData);
	void							Update_TransformationMatrices(const vector<class CEdit_Bone*>& Bones, _float fTimeDelta, _bool isLoop, _bool* pFinished, _bool bIsAnimStop, _int iStartFrame, _int iEndFrame);
	void							Update_TransformationMatrices(const vector<class CEdit_Bone*>& Bones, _float fTimeDelta, _bool isLoop, _bool* pFinished, _bool bIsAnimStop);
	void							Set_TickPerSecond(_float fTickPerSecond) { m_fTickPerSecond = fTickPerSecond; }
	_float							Get_TickPerSecond() { return m_fTickPerSecond; }
	void							Set_CurTrackPositon(_float fCurrentTrackPosition) { m_fCurrentTrackPosition = fCurrentTrackPosition; }
	void							Add_CurTrackPositon(_int i) { m_fCurrentTrackPosition += i; }
	_float							Get_CurTrackPositon() { return m_fCurrentTrackPosition; }
	_float							Get_Duration() { return m_fDuration; }

private:
	_float							m_fDuration = {};
	_float							m_fTickPerSecond = {};

	_float							m_fPreTrackPosition = {};
	_float							m_fCurrentTrackPosition = {};
	_uint							m_iNumChannels = {};
	vector<class CEdit_Channel*>	m_Channels;
	vector<_uint>					m_CurrentKeyFrameIndices;

public:
	static CEdit_Animation*			Create(const aiAnimation* pAiAnimation, const vector<class CEdit_Bone*> Bones, SAVE_MODEL* pModelData);
	CEdit_Animation*				Clone();
	virtual void					Free() override;
};

NS_END