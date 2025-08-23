#pragma once
#include "PoolUIObj.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CItem_Slot final : public CButton
{
public:
	typedef struct Slot_Desc {
		_int itemIndex;
		_float2 vPos;
	}ITEM_SLOT_DESC;

private:
	CItem_Slot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem_Slot(const CItem_Slot& Prototype);
	virtual ~CItem_Slot() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta, CInvenItem* pItem);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

public:
	HRESULT						Item_Init(const ITEM_SLOT_DESC& pDesc);
	void						Update_Pos(_float2 vPos);

private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	_uint						m_iPassIndex = {};
	_uint						m_iItemIndex = {};
private:
	HRESULT						Ready_Components();
	void						Default_Slot();

public:
	static CItem_Slot*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};
NS_END
