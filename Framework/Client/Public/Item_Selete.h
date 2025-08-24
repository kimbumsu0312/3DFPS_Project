#pragma once
#include "PoolUIObj.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CItem_Selete final : public CButton
{
public:
	typedef struct Slot_Desc {
		_int itemIndex;
		_float2 vPos;
	}ITEM_SELETE_DESC;
private:
	CItem_Selete(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem_Selete(const CItem_Selete& Prototype);
	virtual ~CItem_Selete() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta, CInvenItem* pItem);
	virtual void				Late_Update(_float fTimeDelta, _bool Selete);
	virtual HRESULT				Render();

public:
	HRESULT						Item_Init(const ITEM_SELETE_DESC& pDesc);
	void						Update_Pos(_float2 vPos);
	void						IsRotation(_float fAngle);
	void						IsSelete(_bool IsSelete);
	void						IsClick(_bool IsSelete);
	_bool						IsClick();
private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	class CItem_QuikSlot*		m_pItemGuide[5] = {nullptr};
	


	_uint						m_iPassIndex = {};
	_uint						m_iItemIndex = {};

	_float						m_fAlpha = {};
	_float4						m_fColor = {};

	_float						m_fAlphaTime = {};
	_bool						m_bIsSelete = { false };
	_bool						m_bIsClick = { false };
private:
	HRESULT						Ready_Components();
	HRESULT						Ready_Childeren();
	void						Default_Slot();


public:
	static CItem_Selete* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void				Free() override;
};
NS_END
