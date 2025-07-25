#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CInventory final : public CUIObject
{
private:
	CInventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInventory(const CInventory& Prototype);
	virtual ~CInventory() = default;

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

	_bool						m_bIsOpen = { false };

private:
	HRESULT						Ready_Components();
	HRESULT						Ready_Children_Prototype();
	HRESULT						Ready_Children();

public:
	static CInventory*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};
NS_END
