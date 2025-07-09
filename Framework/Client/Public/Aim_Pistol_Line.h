#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CAim_Pistol_Line final : public CUIObject
{
private:
	CAim_Pistol_Line(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAim_Pistol_Line(const CAim_Pistol_Line& Prototype);
	virtual ~CAim_Pistol_Line() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	_float			m_fRotation = {};

private:
	HRESULT						Ready_Components();

public:
	static CAim_Pistol_Line* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void				Free() override;
};
NS_END
