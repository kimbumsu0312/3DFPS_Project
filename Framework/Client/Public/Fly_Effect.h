#pragma once
#include "PoolingContainer.h"
NS_BEGIN(Engine)
class CShader;
class CModel_Instance;
NS_END

NS_BEGIN(Client)
class CFly_Effect final : public CPoolingContainer
{
public:
	enum class Fly_Type{ SPREAD,RETURN, SPIN };
public:
	typedef struct Fly_Effect_Init {
		_vector vPos = {};
	}FLY_EFFECT_INIT;

	typedef struct Fly_Effect_Desc : CPoolingObject::POOLOBJECT_DESC {
		Fly_Type	eType = {};
		_bool		isDead = {};
	}FLY_EFFECT_DESC;

	
private:
	CFly_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFly_Effect(const CFly_Effect& Prototype);
	virtual ~CFly_Effect() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

public:
	virtual HRESULT				Initialize_Pool(void* pArg) override;
	virtual void				Return_Pool() override;

	void						Set_Potion(_vector vPos);
	void						Reset();
private:
	CShader*					m_pShaderCom = { nullptr };
	CModel_Instance*			m_pModel_InstanceCom = { nullptr };

	_float						m_fAccTime = {};
	Fly_Type					m_eFly_Type = {};

	_bool						m_isDeadOn = {};
private:
	HRESULT						Ready_Components();
	HRESULT						Bind_ShaderResources();

	void						Spread_Effect(_float fTimeDelta);
	void						Spin_Effect(_float fTimeDelta);
public:
	static CFly_Effect*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free();
};

NS_END