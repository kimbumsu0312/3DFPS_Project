#pragma once
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)
class CBaseMapObj final : public CGameObject
{
private:
	CBaseMapObj(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBaseMapObj(const CBaseMapObj& Prototype);
	virtual ~CBaseMapObj() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

private:
	class CModel*			m_pModelCom = { nullptr };
	CShader*				m_pShaderCom = { nullptr };

private:
	HRESULT					Ready_Components(_wstring szModelPath);
	HRESULT					Bind_ShaderResources();

public:
	static CBaseMapObj*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free();
};

NS_END