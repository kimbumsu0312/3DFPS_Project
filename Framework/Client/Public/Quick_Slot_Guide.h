#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CQuick_Slot_Guide final : public CUIObject
{
private:
	CQuick_Slot_Guide(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CQuick_Slot_Guide(const CQuick_Slot_Guide& Prototype);
	virtual ~CQuick_Slot_Guide() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	_float4						m_vBackGroundColor = {};
	_int						m_iIndex = {};

private:
	HRESULT						Ready_Components();

public:
	static CQuick_Slot_Guide*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};
NS_END
