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
	typedef struct Fly_Effect_Init {
		_vector vPos = {};

	}FLY_EFFECT_INIT;
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

private:
	CShader*					m_pShaderCom = { nullptr };
	CModel_Instance*			m_pModel_InstanceCom = { nullptr };
private:
	HRESULT						Ready_Components();
	HRESULT						Bind_ShaderResources();

public:
	static CFly_Effect*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free();
};

NS_END