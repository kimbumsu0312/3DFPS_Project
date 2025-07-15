#pragma once
#include "Button.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CUI_Slot final : public CButton
{
public:
	typedef struct tagInven_Slot_Desc : public CUIObject::UIOBJECT_DESC
	{
		_uint iTexIndex = {};
		_uint iPassIndex{};
	}UI_SLOT_DESC;
private:
	CUI_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Slot(const CUI_Slot& Prototype);
	virtual ~CUI_Slot() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	_uint						m_iIndex = {};
	_uint						m_iMaxIndex = {};
	_uint						m_iTexIndex = {};
	_uint						m_iPassIndex{};
private:
	HRESULT						Ready_Components();
	HRESULT						Ready_Children_Prototype();
	HRESULT						Ready_Children();

public:
	static CUI_Slot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void				Free() override;
};
NS_END
