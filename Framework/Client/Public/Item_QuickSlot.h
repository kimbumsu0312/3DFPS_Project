#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CItem_QuikSlot final : public CUIObject
{
private:
	CItem_QuikSlot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem_QuikSlot(const CItem_QuikSlot& Prototype);
	virtual ~CItem_QuikSlot() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta, _float2 vPos);
	virtual HRESULT				Render();


private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	_float4						m_vBackGroundColor = {};
	_int						m_iIndex = {};

	_wstring					m_szIndex = {};
private:
	HRESULT						Ready_Components();
	void						Render_Font();
public:
	static CItem_QuikSlot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void				Free() override;
};
NS_END
