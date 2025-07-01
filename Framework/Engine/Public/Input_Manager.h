#pragma once
#include "GameInstance.h"

NS_BEGIN(Engine)
class CInput_Manager final : public CBase
{
private:
	CInput_Manager();
	virtual ~CInput_Manager() = default;

public:
	HRESULT						Initialize();
	void						Update(float fTimeDelta);

public:
	void						AddTrackIngKey(int iKey);
	_bool						IsKeyDown(int iKey) const;
	_bool						IsKeyUp(int iKey) const;
	_bool						IsKeyHold(int iKey) const;
	_float						GetKeyHoldTime(int iKey) const;

private:
	unordered_map<_int, _bool>	m_mapPreState;
	unordered_map<_int, _bool>	m_mapCurState;
	unordered_map<_int, _float>	m_mapKeyHoldTime;
	vector<_int>				m_vecTrackingKeys;

public:
	static CInput_Manager*		Create();
	virtual void				Free() override;
};

NS_END