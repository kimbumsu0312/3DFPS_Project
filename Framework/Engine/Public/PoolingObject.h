#pragma once
#include "GameObject.h"

NS_BEGIN(Engine)
class ENGINE_DLL CPoolingObject abstract : public CGameObject
{
public:
	typedef struct tagPoolingObject: public CGameObject::GAMEOBJECT_DESC
	{
		_wstring	szPoolingPath;
	}POOLOBJECT_DESC;

protected:
	CPoolingObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPoolingObject(const CPoolingObject& Prototype);
	virtual ~CPoolingObject() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

	virtual void			On_Dead() override;
public:
	virtual HRESULT			Initialize_Pool(void *pArg);
	virtual void			Return_Pool() = 0;
	
private:
	_wstring				m_szPoolingPath;

public:
	virtual CGameObject*	Clone(void* pArg) = 0;
	virtual void			Free() override;
};

NS_END