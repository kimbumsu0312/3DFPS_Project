#pragma once
#include "Engine_Defines.h"
#include "PoolingObject.h"
#include "Client_Struct.h"

NS_BEGIN(Engine)
class CNavigation;
class CCollider;
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)
class CPoolWorld_Item final : public CPoolingObject
{
private:
	enum class ITEM_COLLIDER {
		ITEM, DETACT_REAGE, END
	};
public:
	typedef struct PoolWorldItem_Desc
	{
		_vector vPos = { };
		_int	vNaviIndex = {};
		_wstring szPoolPath = {};
	}POOLITEM_DESC;
private:
	CPoolWorld_Item(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPoolWorld_Item(const CPoolWorld_Item& Prototype);
	virtual ~CPoolWorld_Item() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

	virtual void				OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision) override;

public:
	virtual HRESULT				Initialize_Pool(void* pArg) override;
	virtual void				Return_Pool() override;

private:
	CNavigation*				m_pNavigationCom = { nullptr };
	CCollider*					m_pColliderCom[ENUM_CLASS(ITEM_COLLIDER::END)] = {nullptr};
	CShader*					m_pShaderCom = { nullptr };
	CModel*						m_pModelCom = { nullptr };

	class CGet_UI*				m_pGuideUI = { nullptr };
	
	_int						m_iItemIndex = {};
	_wstring					m_szPoolPath = {};
	_bool						m_bisOn = { false };
private:
	HRESULT						Ready_Components(_wstring szModelPath);
	HRESULT						Ready_UI();
	HRESULT						Bind_ShaderResources();

public:
	static CPoolWorld_Item*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free();
};

NS_END