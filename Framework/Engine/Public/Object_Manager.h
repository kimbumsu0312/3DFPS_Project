#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CObject_Manager final : public CBase
{
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	class CComponent*			Get_Component(_uint iLayerLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex = 0);

public:
	HRESULT						Initialize(_uint iNumLevels);
	HRESULT						Add_GameObject_ToLayer(_uint iLayerLevelIndex, const _wstring& strLayerTag, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg);
	HRESULT						Add_GameObject_ToLayer(_uint iLayerLevelIndex, const _wstring& strLayerTag, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, CGameObject* pGameObject);

	HRESULT						Add_PoolGameObject_ToLayer(class CPoolingObject* pObject, _uint iLayerLevelIndex, const _wstring& strLayerTag);
	void						Priority_Update(_float fTimeDelta);
	void						Update(_float fTimeDelta);
	void						Late_Update(_float fTimeDelta);
	void						Clear(_uint iLevelIndex);

private:
	class CGameInstance*		m_pGameInstance = { nullptr };
	
	_uint						m_iNumLevels = {};

	map<const _wstring,
		class CLayer*>*			m_pLayers = {};

private:
	class CLayer*				Find_Layer(_uint iLayerLevelIndex, const _wstring& strLayerTag);

public:
	static CObject_Manager*		Create(_uint iNumLevels);
	virtual void				Free() override;
};
NS_END
