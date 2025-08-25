#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CAnnounce_Icon final : public CUIObject
{
private:
	CAnnounce_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAnnounce_Icon(const CAnnounce_Icon& Prototype);
	virtual ~CAnnounce_Icon() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

	void						Set_Item(_int iItemIndex);
	void						Set_RenderGroup(RENDERGROUP eRenderGroup);
private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	_int						m_iIndex = {};
	_int						m_iITemType = {};
	_int						m_iItemIndex = {};
	_bool						m_bIsRender = { false };
	RENDERGROUP					m_eRenderGroup = { RENDERGROUP::UI };
private:
	HRESULT						Ready_Components();
public:
	static CAnnounce_Icon*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};
NS_END
