#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CLogo_Button_Line final : public CUIObject
{
private:
	CLogo_Button_Line(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLogo_Button_Line(const CLogo_Button_Line& Prototype);
	virtual ~CLogo_Button_Line() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

private:
	CShader*					m_pShaderCom = { nullptr };
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	CTexture*					m_pTextureCom = { nullptr };

private:
	HRESULT						Ready_Components();

public:
	static CLogo_Button_Line*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};
NS_END
