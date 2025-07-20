#pragma once
#include "Transform.h"
#include "Engine_Defines.h"

NS_BEGIN(Engine)
class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef struct tagGameObject : public CTransform::TRANSFORM_DESC
	{

	}GAMEOBJECT_DESC;

protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;
	//123
public:
	class CComponent*		Get_Component(const _wstring& strComponentTag);
	_bool					IsDead() { return m_bIsDead; }
	OBJECTTYPE				IsObjectType() { return m_eObjectType; }
public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

	virtual void			On_Dead();
	virtual void*			GetDesc() { return nullptr; }
protected:
	OBJECTTYPE				m_eObjectType = {};
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	class CGameInstance*	m_pGameInstance = { nullptr };
	class CTransform*		m_pTransformCom = { nullptr };

	map<const _wstring, 
		class CComponent*>	m_Components;
	
	_bool					m_bIsDead = {};
protected:
	HRESULT					Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag,
											const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);
public:
	virtual CGameObject*	Clone(void* pArg) = 0;
	virtual void			Free() override;
};
NS_END
