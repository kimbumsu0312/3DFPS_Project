#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CInventory_Base final : public CUIObject
{
private:
	CInventory_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInventory_Base(const CInventory_Base& Prototype);
	virtual ~CInventory_Base() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

private:
	CShader*					m_pShaderCom = { nullptr };
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	CTexture*					m_pTextureCom = { nullptr };

	_bool						m_bIsOpen = {false};
	_float4						m_vOpenTex = {};
	_float						m_fOpenTexValueX = {};
	_float						m_fOpenTexValueY = {};
	_float						m_fOpenTexSpeed = {};
private:
	HRESULT						Ready_Components();
	HRESULT						Ready_Children();

	void						Open_UI();
public:
	static CInventory_Base*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};

NS_END