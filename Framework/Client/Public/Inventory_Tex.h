#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CInventory_Tex final : public CUIObject
{

private:
	CInventory_Tex(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInventory_Tex(const CInventory_Tex& Prototype);
	virtual ~CInventory_Tex() = default;

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

private:
	HRESULT						Ready_Components();

public:
	static CInventory_Tex* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void				Free() override;
};
NS_END
