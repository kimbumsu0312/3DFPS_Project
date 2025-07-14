#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CItem_Penal final : public CUIObject
{
private:
	CItem_Penal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem_Penal(const CItem_Penal& Prototype);
	virtual ~CItem_Penal() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	_bool						m_bIsSelete = { false };
	_int						m_iIndex = {};

private:
	HRESULT						Ready_Components();
	HRESULT						Ready_Children();

public:
	static CItem_Penal*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};

NS_END