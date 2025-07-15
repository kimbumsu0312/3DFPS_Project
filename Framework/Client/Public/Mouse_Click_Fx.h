#pragma once
#include "PoolingObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END
NS_BEGIN(Client)
class CMouse_Click_Fx : public CPoolingObject
{
public:
	typedef struct tagClickfxDescClone : public CPoolingObject::POOLOBJECT_DESC
	{
		_float2 vMinUV{}, vMaxUV{}, vSize{};
	}CLICKFX_CLONE_DESC;

	typedef struct tagClickfxDescPool : public CGameObject::GAMEOBJECT_DESC
	{
		_float2 vPos{};
		_float2 vMinUV{}, vMaxUV{};
	}CLICKFX_POOL_DESC;
private:
	CMouse_Click_Fx(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMouse_Click_Fx(const CMouse_Click_Fx& Prototype);
	virtual ~CMouse_Click_Fx() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

public:
	virtual void			On_Dead() override;
	virtual HRESULT			Initialize_Pool(void* pArg) override;
	virtual void			Return_Pool() override ;

private:
	CShader*				m_pShaderCom = { nullptr };
	CTexture*				m_pTextureCom = { nullptr };
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };
	
	_float4x4				m_ViewMatrix = {};
	_float4x4				m_ProjMatrix = {};

	_float2					m_vPos = {};
	_float2					m_vSize = {};
	_float2					m_vMinUV = {};
	_float2					m_vMaxUV = {};

	_float					m_fAlpha = {};
	_float					m_fAccScale = {};

	_float					m_fAlphaTime = {};
	_float					m_fScaleTime = {};

private:
	HRESULT					Ready_Components();
public:
	static CMouse_Click_Fx* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

NS_END