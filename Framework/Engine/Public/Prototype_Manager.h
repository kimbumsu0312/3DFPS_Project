#pragma once
#include "Component.h"

NS_BEGIN(Engine)
class CPrototype_Manager final : public CBase
{
private:
	CPrototype_Manager();
	virtual ~CPrototype_Manager() = default;

public:
	HRESULT						Initialize(_uint iNumLevels);
	HRESULT						Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	class CBase*				Clone_Prototype(PROTOTYPE ePrototype, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg);
	void						Clear(_uint iLevelIndex);
private:
	_uint						m_iNumLevels = {  };
	
	map<const _wstring,
		class CBase*>*			m_pPrototypes = { nullptr };
	
	typedef map<const _wstring, 
		class CBase*>			PROTOTYPES;


private:
	class CBase*				Find_Prototype(_uint iPrototpyeLevelIndex, const _wstring& strPrototypeTag);


public:
	static CPrototype_Manager*	Create(_uint iNumLevels);
	virtual void				Free() override;
};

NS_END