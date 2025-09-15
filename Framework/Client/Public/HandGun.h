#pragma once
#include "WeaponObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CAnimatio_Controller;
NS_END

NS_BEGIN(Client)
class CHandGun final : public CWeaponObject
{
private:
	CHandGun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CHandGun(const CHandGun& Prototype);
	virtual ~CHandGun() = default;

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
	CAnimatio_Controller*	m_pAnimCom = { nullptr };
	string					m_AnimTag = {};

	_bool					m_bEffect = {};
	_float					m_fRange = { -0.16f };
	
private:
	HRESULT					Ready_Components();
	HRESULT					Bind_ShaderResources();

public:
	static CHandGun*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

NS_END