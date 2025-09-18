#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CLoding_SideTex final : public CUIObject
{
private:
	CLoding_SideTex(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLoding_SideTex(const CLoding_SideTex& Prototype);
	virtual ~CLoding_SideTex() = default;

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

private:
	HRESULT						Ready_Components();


public:
	static CLoding_SideTex*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};
NS_END
