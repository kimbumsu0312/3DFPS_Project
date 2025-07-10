#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CAim_Pistol final : public CUIObject
{
private:
	CAim_Pistol(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAim_Pistol(const CAim_Pistol& Prototype);
	virtual ~CAim_Pistol() = default;

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

	void						Shoting();
	void						KeyInput();
public:
	static CAim_Pistol*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};
NS_END
