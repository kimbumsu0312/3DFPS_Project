#pragma once
#include "Base.h"

NS_BEGIN(Engine)

class CChannel final : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT							Initialize(const SAVE_CHANNEL& pChannel, const vector<class CBone*>& Bones);
	void							Update_TransformationMatrix(const vector<class CBone*>& Bones, _float fCurrentTrackPosition, _float fPreTrackPosition, _uint* m_iCurrentKeyFrameIndex);
private:
	_char							m_szName[MAX_PATH] = { };
	_uint							m_iBoneIndex = {};
	_uint							m_iNumKeyFrames = {};
	vector<KEYFRAME>				m_KeyFrames;

public:
	static CChannel*				Create(const SAVE_CHANNEL& pChannel, const vector<class CBone*>& Bones);
	virtual void					Free() override;
};

NS_END

