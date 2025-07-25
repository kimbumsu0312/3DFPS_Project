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
	void						Update_TransformationMatrices(const vector<class CBone*>& Bones, _float fTimeDelta, _bool isLoop, ANIM_STATUS eAnimStatus, _bool* pFinished, const ANIMEFRAME& pAnimFrameData);

private:
	_float						m_fDuration = {};
	_float						m_fTickPerSecond = {};
	
	_float						m_fPreTrackPosition = {};
	_float						m_fCurrentTrackPosition = {};
	
	_uint						m_iNumChannels = {};
	vector<class CChannel*>		m_Channels;
	vector<_uint>				m_CurrentKeyFrameIndices;
public:
	static CAnimation*			Create(const SAVE_ANIM& pAnimation, const vector<class CBone*>& Bones);
	CAnimation*					Clone();
	virtual void				Free() override;
};

NS_END

