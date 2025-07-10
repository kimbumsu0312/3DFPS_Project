#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CAim_Shotgun_Line final : public CUIObject
{
private:
	CAim_Shotgun_Line(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAim_Shotgun_Line(const CAim_Shotgun_Line& Prototype);
	virtual ~CAim_Shotgun_Line() = default;

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

	_bool						m_bZoom = { false };
	_uint						m_iIndex = {};

	_float						m_fRotation = {};
	_float2						m_vMovePos = {};
	_float2						m_vZoomUV = {};

private:
	HRESULT						Ready_Components();
	void						Aim_Zoom();
public:
	static CAim_Shotgun_Line*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};
NS_END
