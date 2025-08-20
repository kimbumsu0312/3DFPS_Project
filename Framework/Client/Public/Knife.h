#pragma once
#include "WeaponObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)
class CKnife final : public CWeaponObject
{
private:
	CKnife(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CKnife(const CKnife& Prototype);
	virtual ~CKnife() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

private:
	CShader*				m_pShaderCom = { nullptr };
	CModel*					m_pModelCom = { nullptr };

private:
	HRESULT					Ready_Components();
	HRESULT					Bind_ShaderResources();

public:
	static CKnife*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

NS_END