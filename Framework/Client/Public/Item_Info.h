#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CItem_Info final : public CUIObject
{
public:
	typedef struct tagInven_Tex_Desc : public CUIObject::UIOBJECT_DESC
	{
		_uint		iTexIndex{};
		_uint		iPassIndex{};
	}UI_TEX_DESC;
private:
	CItem_Info(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem_Info(const CItem_Info& Prototype);
	virtual ~CItem_Info() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	_int						m_iIndex = {};
	_float						m_fRotation = {};
	_uint						m_iTexIndex = {};
	_uint						m_iPassIndex = {};

	_int						m_iItemIndex = {};

private:
	HRESULT						Ready_Components();
	void						Render_Font();

public:
	static CItem_Info* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void				Free() override;
};
NS_END
