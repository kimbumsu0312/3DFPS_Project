#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CGet_UI final : public CUIObject
{
private:
	CGet_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGet_UI(const CGet_UI& Prototype);
	virtual ~CGet_UI() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

	void						Update_WorldPos(_vector vPos);
	void						IsOn() { m_bIsOn = true; }
private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };

	_bool						m_bIsOn = { false };
private:
	HRESULT						Ready_Components();
	void						Render_Font();

public:
	static CGet_UI*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};

NS_END