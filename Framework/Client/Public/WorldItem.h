#pragma once
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CCollider;
NS_END

NS_BEGIN(Client)
class CWorldItem final : public CGameObject
{
private:
	enum class ITEM_COLLIDER {
		ITEM, DETACT_REAGE, END
	};

private:
	CWorldItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWorldItem(const CWorldItem& Prototype);
	virtual ~CWorldItem() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

	virtual void			OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision) override;

private:
	CModel*					m_pModelCom = { nullptr };
	CShader*				m_pShaderCom = { nullptr };
	CCollider*				m_pColliderCom[ENUM_CLASS(ITEM_COLLIDER::END)] = { nullptr };

	class CGet_UI*			m_pGuideUI = { nullptr };

	_int					m_iItemIndex = {};
	_wstring				m_szPoolPath = {};
	_bool					m_bisOn = { false };

private:
	HRESULT					Ready_Components(_wstring szModelPath);
	HRESULT					Ready_UI();
	HRESULT					Bind_ShaderResources();

public:
	static CWorldItem*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free();
};

NS_END