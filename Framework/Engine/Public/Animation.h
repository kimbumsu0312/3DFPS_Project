#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CAnimation final : public CBase
{
private:
	CAnimation();
	virtual ~CAnimation() = default;

public:
	HRESULT						Initialize(const SAVE_ANIM& pAnimation, const vector<class CBone*>& Bones);
	void						Update_TransformationMatrices(const vector<class CBone*>& Bones, _float fTimeDelta);

private:
	_float						m_fDuration = {};
	_float						m_fTickPerSecond = {};
	_float						m_fCurrentTrackPosition = {};
	_uint						m_iNumChannels = {};

	vector<class CChannel*>		m_Channels;

public:
	static CAnimation*			Create(const SAVE_ANIM& pAnimation, const vector<class CBone*>& Bones);
	virtual void				Free() override;
};

NS_END

