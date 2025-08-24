#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CQuick_Slot_Item final : public CUIObject
{
private:
	CQuick_Slot_Item(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CQuick_Slot_Item(const CQuick_Slot_Item& Prototype);
	virtual ~CQuick_Slot_Item() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

	void						Set_Item(_int iItemIndex);
private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	_int						m_iIndex = {};
	_int						m_iITemType = {};
	_int						m_iItemIndex = {};
private:
	HRESULT						Ready_Components();
public:
	static CQuick_Slot_Item*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};
NS_END
