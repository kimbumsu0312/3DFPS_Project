#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
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
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };

	_bool						m_bIsOpen = {false};
	_float4						m_vOpenTex = {};
	_float						m_fOpenTexValueX = {};
	_float						m_fOpenTexValueY = {};
	_float						m_fOpenTexSpeed = {};
	_int						m_iSeletePenal_Index = {};

	class CItem_Penal*			m_pItemPenal = { nullptr };
	class CCreate_Penal*		m_pCreatePenal = { nullptr };

private:
	HRESULT						Ready_Components();
	HRESULT						Ready_Children_Prototype();
	HRESULT						Ready_Children();

	void						Open_UI(_float fTimeDelta);
	void						Change_Penal();
	void						Selete_Penal(_uint iIndex);
public:
	static CInventory_Base*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};

NS_END