#pragma once
#include "Button.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CLogo_Button final : public CButton
{
private:
	CLogo_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLogo_Button(const CLogo_Button& Prototype);
	virtual ~CLogo_Button() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	_uint						m_iIndex = {};
	_uint						m_iMaxIndex = {};
	_bool						m_bIsSelete = { false };
	_float4						m_vBackGroundColor = {};
private:
	HRESULT						Ready_Components();
	HRESULT						Ready_Children_Prototype();
	HRESULT						Ready_Children();

	void						Button_Event();
	void						Button_Selete();
	
public:
	static CLogo_Button*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};
NS_END
