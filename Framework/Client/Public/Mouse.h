#pragma once
#include "UIObject.h"

NS_BEGIN(Client)
class CMouse final : public CUIObject
{
private:
	CMouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMouse(const CMouse& Prototype);
	virtual ~CMouse() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual HRESULT			Render() override;
	
private:
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };
	_float2					m_vDefaultMinUV = {};
	_float2					m_vDefaultMaxUV = {};
	
	_float2					m_vSeleteMinUV = {};
	_float2					m_vSeleteMaxUV = {};

	_bool					m_bIsFPS = { false };

private:
	HRESULT					Ready_Components();
	void					FPS_Mode();
	void					Mouse_Input();

public:
	static CMouse*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};
NS_END
