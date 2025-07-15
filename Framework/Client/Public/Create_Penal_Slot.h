#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CCreate_Penal_Slot final : public CUIObject
{

private:
	CCreate_Penal_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCreate_Penal_Slot(const CCreate_Penal_Slot& Prototype);
	virtual ~CCreate_Penal_Slot() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	_int						m_iIndex = {};
	_float						m_fRotation = {};

private:
	HRESULT						Ready_Components();

public:
	static CCreate_Penal_Slot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void				Free() override;
};
NS_END
