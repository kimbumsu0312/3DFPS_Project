#pragma once
#include "Base.h"
#include "Edit_Defines.h"
#include "Engine_Defines.h"

NS_BEGIN(Edit)
class CEdit_Channel final : public CBase
{
private:
	CEdit_Channel();
	virtual ~CEdit_Channel() = default;

public:
	HRESULT							Initialize(const aiNodeAnim* pAIChannel, const vector<class CEdit_Bone*>& Bones, SAVE_ANIM* pAnim);
	void							Update_TransformationMatrix(const vector<class CEdit_Bone*>& Bones, _float fCurrentTrackPosition);
private:
	_char							m_szName[MAX_PATH] = { };

	_uint							m_iBoneIndex = {};
	_uint							m_iNumKeyFrames = {};
	vector<KEYFRAME>				m_KeyFrames;

public:
	static CEdit_Channel*			Create(const aiNodeAnim* pAIChannel, const vector<class CEdit_Bone*>& Bones, SAVE_ANIM* pAnim);
	virtual void					Free() override;
};

NS_END