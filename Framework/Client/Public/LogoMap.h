#pragma once
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)
class CLogoMap final : public CGameObject
{
public:
	typedef struct tagLogoObject : public CGameObject::GAMEOBJECT_DESC
	{
		_float4 vPos;
		_float	fDegrees;
		_float3 vScale;
	}LOGOOBJECT_DESC;

private:
	CLogoMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLogoMap(const CLogoMap& Prototype);
	virtual ~CLogoMap() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();
	virtual HRESULT			Render_Shadow() override;
private:
	class CModel*			m_pModelCom = { nullptr };
	CShader*				m_pShaderCom = { nullptr };

private:
	HRESULT					Ready_Components(_wstring szModelPath);
	HRESULT					Bind_ShaderResources();

public:
	static CLogoMap*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free();
};

NS_END