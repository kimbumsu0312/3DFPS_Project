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
	void							Update_TransformationMatrix(const vector<class CBone*>& Bones, _float fCurrentTrackPosition, _float fPreTrackPosition, _uint* m_iCurrentKeyFrameIndex, const ANIMEFRAME& pAnimFrameData);
	void							Update_TransformationMatirx_Transition(const vector<class CBone*>& Bones, _uint pStartKeyFrameIndex, _float fRatio);
	
private:
	_char							m_szName[MAX_PATH] = { };
	_uint							m_iBoneIndex = {};
	_uint							m_iNumKeyFrames = {};
	vector<KEYFRAME>				m_KeyFrames;

private:
	_uint							Finde_KeyFrameIndex(_float fTrackPotion);

public:
	static CChannel*				Create(const SAVE_CHANNEL& pChannel, const vector<class CBone*>& Bones);
	virtual void					Free() override;
};

NS_END

