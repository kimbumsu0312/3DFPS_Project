#pragma once
#include "PartObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)
class CHandGun final : public CPartObject
{
public:
	typedef struct tagWeaponHandGunDesc : public CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* pSocketMatrix = { nullptr };
		_uint* pState = { nullptr };
	}HANDGUN_DESC;

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

private:
	const _float4x4*		m_pSocketMatrix = { nullptr };
	_uint*					m_pParentState = { nullptr };
	string					m_AnimTag = {};
private:
	HRESULT					Ready_Components();
	HRESULT					Bind_ShaderResources();

public:
	static CHandGun*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

NS_END