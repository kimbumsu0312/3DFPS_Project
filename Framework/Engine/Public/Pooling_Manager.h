#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CPooling_Manager final : public CBase
{
private:
	CPooling_Manager();
	virtual ~CPooling_Manager() = default;

public:
	HRESULT								Add_Object_ToPool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iValue, void* pArg);
	HRESULT								Add_Pool_ToLayer(const _wstring& szPoolingPath, _uint iLayerLevelIndex, const _wstring& strLayerTag, void* pArg);
	HRESULT								Return_Object(class CPoolingObject* pObject, const _wstring& szPoolingPath);

private:
	class CGameInstance*				m_pGameInstance = { nullptr };

	unordered_map<_wstring,
		queue<class CPoolingObject*>>	m_ObjectPool;

public:
	static CPooling_Manager*			Create();
	virtual void						Free();
};

NS_END