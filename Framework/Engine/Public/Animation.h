#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CAnimation final : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& Prototype);
	virtual ~CAnimation() = default;

public:
	HRESULT						Initialize(const SAVE_ANIM& pAnimation, const vector<class CBone*>& Bones);
	void						Update_TransformationMatrices(const vector<class CBone*>& Bones, _float fTimeDelta, _bool isLoop, ANIM_STATUS eAnimStatus, _bool* pFinished, _bool bAnimChange, const ANIMEFRAME& pAnimFrameData);
	void						Anim_Change_Set(const vector<class CBone*>& Bones, const ANIMEFRAME& pAnimFrameData);

private:
	_float						m_fDuration = {};
	_float						m_fTickPerSecond = {};
	
	_float						m_fPreTrackPosition = {};
	_float						m_fCurrentTrackPosition = {};
	
	_uint						m_iNumChannels = {};
	vector<class CChannel*>		m_Channels;
	vector<_uint>				m_PreKeyFrameIndices;
	vector<_uint>				m_CurrentKeyFrameIndices;

	_float						m_fTransitionTime = { 1.f };
	_float						m_fTransitionDuration = { 0.2f };
	vector<KEYFRAME>			m_KeyFrames;
	_bool						m_bIsAnimChange = { false };

private:
	void						Anim_Change_Reset();
public:
	static CAnimation*			Create(const SAVE_ANIM& pAnimation, const vector<class CBone*>& Bones);
	CAnimation*					Clone();
	virtual void				Free() override;
};

NS_END

