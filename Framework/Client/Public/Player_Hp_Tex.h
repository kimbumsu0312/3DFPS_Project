#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CPlayer_Hp_Tex final : public CUIObject
{

private:
	CPlayer_Hp_Tex(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer_Hp_Tex(const CPlayer_Hp_Tex& Prototype);
	virtual ~CPlayer_Hp_Tex() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };

private:
	HRESULT						Ready_Components();

public:
	static CPlayer_Hp_Tex* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void				Free() override;
};
NS_END
