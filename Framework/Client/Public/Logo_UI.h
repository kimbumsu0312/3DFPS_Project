#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CLogo_UI final : public CUIObject
{
private:
	CLogo_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CLogo_UI(const CLogo_UI& Prototype);
	virtual ~CLogo_UI() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	_uint						m_iNumSeleteButton = {};
	_uint						m_iNumMaxButton = {};
private:
	HRESULT						Ready_Components();
	HRESULT						Ready_Children_Prototype();
	HRESULT						Ready_Children();

	void						Button_Selete();
public:
	static CLogo_UI*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};
NS_END
