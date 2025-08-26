#pragma once
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
NS_END

NS_BEGIN(Client)
class CWorldItem final : public CGameObject
{
private:
	CWorldItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWorldItem(const CWorldItem& Prototype);
	virtual ~CWorldItem() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

private:
	CModel*					m_pModelCom = { nullptr };
	CShader*				m_pShaderCom = { nullptr };
	CCollider*				m_pColliderCom = { nullptr };

private:
	HRESULT					Ready_Components(_wstring szModelPath);
	HRESULT					Bind_ShaderResources();

public:
	static CWorldItem*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free();
};

NS_END