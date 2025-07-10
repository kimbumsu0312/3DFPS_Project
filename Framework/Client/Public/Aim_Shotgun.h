#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CAim_Shotgun final : public CUIObject
{
private:
	CAim_Shotgun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAim_Shotgun(const CAim_Shotgun& Prototype);
	virtual ~CAim_Shotgun() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };

	WEAPON_TYPE					m_eWeapon_Type = {};
	_uint						m_iNumSeleteButton = {};
	_uint						m_iNumMaxButton = {};

	_bool						m_bIsSelete = { false };
	_bool						m_bZoom = { false };

	_float						m_fCool = {};
	_bool						m_bIsZoomIn = { false };

private:
	HRESULT						Ready_Components();
	HRESULT						Ready_Children_Prototype();
	HRESULT						Ready_Children();

	void						KeyInput();
	void						Shoting();
public:
	static CAim_Shotgun* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void				Free() override;
};
NS_END
