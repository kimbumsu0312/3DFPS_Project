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
	void						Update_TransformationMatrices(const vector<class CBone*>& Bones, _float fTimeDelta, _bool isLoop, ANIM_STATUS eAnimStatus, _bool* pFinished, const ANIMEFRAME& pAnimFrameData, _bool IsAnimChange);
	void						Anim_Change_Reset();
	const _bool&				IsAnimChange() { return m_bIsAnimChange; }
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
	_float						m_fTransitionDuration = { 0.4f };
	vector<KEYFRAME>			m_KeyFrames;
	_bool						m_bIsAnimChange = { false };
public:
	static CAnimation*			Create(const SAVE_ANIM& pAnimation, const vector<class CBone*>& Bones);
	CAnimation*					Clone();
	virtual void				Free() override;
};

NS_END

