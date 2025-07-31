#pragma once
#include "PartObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)
class CShotGun final : public CPartObject
{
public:
	typedef struct tagWeaponShotGunDesc : public CPartObject::PARTOBJECT_DESC
	{
		const _float4x4* pSocketMatrix = { nullptr };
		_uint* pState = { nullptr };
	}SHOTGUN_DESC;

private:
	CShotGun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShotGun(const CShotGun& Prototype);
	virtual ~CShotGun() = default;

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
private:
	const _float4x4*		m_pSocketMatrix = { nullptr };
	_uint*					m_pParentState = { nullptr };

private:
	HRESULT					Ready_Components();
	HRESULT					Bind_ShaderResources();

public:
	static CShotGun*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

NS_END