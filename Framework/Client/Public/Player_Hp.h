#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CPlayer_Hp final : public CUIObject
{
private:
	CPlayer_Hp(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_Hp(const CPlayer_Hp& Prototype);
	virtual ~CPlayer_Hp() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	_float						m_fAlpha = {};
	_bool						m_bIsOpen = {};
	_bool						m_bIsClose = {};
	_float						m_fIsOpenTime = {};

	_bool						m_bIsInven = {};

private:
	HRESULT						Ready_Components();
	HRESULT						Ready_Children_Prototype();
	HRESULT						Ready_Children();

	void						UIOpen_Damage();
	void						UIOpen_Inventory(_bool bIsOpen);
	void						UIAlpha_OnOff(_float fTimeDelta);
public:
	static CPlayer_Hp*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};

NS_END