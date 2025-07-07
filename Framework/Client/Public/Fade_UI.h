#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CFade_UI final : public CUIObject
{
private:
	CFade_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFade_UI(const CFade_UI& Prototype);
	virtual ~CFade_UI() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

private:
	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_bool						m_bIsFadeIn = { false };
	_bool						m_bIsFadeOut = { true };

	_float4						m_vFadeValue = {};
	_float						m_fFadeTime = {};
	_float						m_fAlpah = {};
private:
	HRESULT						Ready_Components();
	HRESULT						Ready_Children();

public:
	static CFade_UI*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};

NS_END