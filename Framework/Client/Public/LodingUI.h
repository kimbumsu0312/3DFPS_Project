#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CLodingUI final : public CUIObject
{
private:
	enum class FadeTag{ FADE_IN, FADE_OUT, END };
private:
	CLodingUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLodingUI(const CLodingUI& Prototype);
	virtual ~CLodingUI() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };

	_float						m_fTime = {};
	_float						m_fFadeOut = {};
	FadeTag						m_eFade = { FadeTag::END };
	_uint						m_iTexIndex = {};

	_bool						m_bisNext = { false };
private:
	HRESULT						Ready_Components();

public:
	static CLodingUI*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};

NS_END