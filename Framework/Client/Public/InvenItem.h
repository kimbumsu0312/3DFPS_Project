#pragma once
#include "PoolUIObj.h"
#include "Client_Struct.h"

NS_BEGIN(Client)
class CInvenItem final : public CPoolUIObj
{
public:
	typedef struct tagItem_Desc : public CPoolUIObj::UIPOOLOBJECT_DESC
	{
		INVEN_ITEM				ItemData = {};
		_bool					IsRender = {false};
	}ITEM_DESC;

private:
	CInvenItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CInvenItem(const CInvenItem& Prototype);
	virtual ~CInvenItem() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg = nullptr) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

public:
	virtual HRESULT			Initialize_Pool(void* pArg) override;
	virtual void			Return_Pool() override;

	void					Change_Slot(_float2 vPos, _int iGridX, _int iGridY );
	const INVEN_ITEM&		Get_ItemData() {return m_ItemData;}
	_bool					Get_ItemSelete() { return m_bIsSelete; }
	void					IsSelete(_bool IsSelete);
	void					IsClick(_bool IsClick);

	void					IsRotation(_bool IsRotation);
	
	_bool					Get_Rtation() { return m_bIsCulRotation; }
	void					Set_Render(_bool IsRender) { m_bIsRender = IsRender;}
	void					Add_ItemValue(_int i) { m_ItemData.iItemCount += i; }
private:
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };
	
	class CItem_Slot*		m_pItemSlot = { nullptr };

	INVEN_ITEM				m_ItemData = {};
	_uint					m_iPassIndex = { 2 };
	_uint					m_iTexIndex = { 1 };

	_bool					m_bIsSelete = { false };
	_bool					m_bIsClick = { false };
	_bool					m_bIsPreRotation = { false };
	_bool					m_bIsCulRotation = { false };
	_bool					m_bIsRender = { false };


	INVENRECT				m_ItemRect = {};
	INVENRECT				m_FontPos = {};

	_float					m_fAlpha = {};
private:
	HRESULT					Ready_Components();
	HRESULT					Ready_Children();

	void					State_Selete();
	void					Render_Font();
public:
	static	CInvenItem*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free();
};

NS_END