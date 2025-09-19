#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CBulletCount final : public CUIObject
{
private:
	CBulletCount(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBulletCount(const CBulletCount& Prototype);
	virtual ~CBulletCount() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

	void						Open_UI();
private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };

	_float						m_fAlpha = {};
	_float						m_fDelay = {};
	_bool						m_bIsOpen = {};
	_bool						m_bIsClose = {};

	_int						m_iItemIndex = {};
	_int						m_iCulInvenBullet = {};
	_int						m_iCulGunBullet = {};
private:
	HRESULT						Ready_Components();
	
	void						Render_Font();

public:
	static CBulletCount*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};

NS_END