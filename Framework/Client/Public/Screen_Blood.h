#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CScreen_Blood final : public CUIObject
{
private:
	CScreen_Blood(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CScreen_Blood(const CScreen_Blood& Prototype);
	virtual ~CScreen_Blood() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_float4						m_vColor = {};
	_float						m_fAlpha = {};
private:
	HRESULT						Ready_Components();

public:
	static CScreen_Blood* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void				Free() override;
};

NS_END