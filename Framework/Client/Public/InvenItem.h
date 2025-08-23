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

	const INVEN_ITEM&		Get_ItemData() {return m_ItemData;}

private:
	CVIBuffer_Rect*			m_pVIBufferCom = { nullptr };
	INVEN_ITEM				m_ItemData = {};

private:
	HRESULT					Ready_Components();
	HRESULT					Ready_Children();

public:
	static	CInvenItem*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free();
};

NS_END