#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CAnnounce final : public CUIObject
{
private:
	CAnnounce(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAnnounce(const CAnnounce& Prototype);
	virtual ~CAnnounce() = default;

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

private:
	HRESULT						Ready_Components();
	HRESULT						Ready_Children();

	void						UIOpen();
	void						UIAlpha_OnOff(_float fTimeDelta);
public:
	static CAnnounce* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void				Free() override;
};

NS_END