#pragma once
#include "Engine_Defines.h"
#include "ContainerObject.h"
#include "Client_Struct.h"
#include "BlackBoard.h"

NS_BEGIN(Engine)
class CCollider;
class CNavigation;
NS_END

NS_BEGIN(Client)
class CAlcina final : public CContainerObject
{
public:
	typedef struct Alchina_Data
	{
		//애니메이션 관련
		_uint*						iAnimState = {nullptr};
		string*						szAnimTag = { nullptr };
		_bool*						bIsAnimLoop = { nullptr };
		_bool*						bIsAnimFinsh = { nullptr };

	}ALCHINA_DATA;

private:
	enum ColliderType_Mon { Body = 0, Head, L_ARM, R_ARM, RESIST, End };
private:
	CAlcina(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAlcina(const CAlcina& Prototype);
	virtual ~CAlcina() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

public:	
	const _vector				Get_TransformState(STATE eState) { return m_pTransformCom->Get_State(eState); }

	void						Target_LookTurn(_float fTimeDelta);
	void						Target_LookAt();
	void						IsDamage();
	virtual void				OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision) override;

private:
	CNavigation*				m_pNavigationCom = { nullptr };
	//콜리전
	CCollider*					m_pColliderCom[ColliderType_Mon::End] = { nullptr };
	_float4x4*					m_pColliderBone[ColliderType_Mon::End] = { nullptr };

	_uint						m_iAnimState = { ENUM_CLASS(NORMAL_MON_STATE::NORMAL) };
	string						m_szAnimTag;
	_bool						m_bIsAnimLoop = { true };
	_bool						m_bIsAnimFinsh = { false };

	class CBody_Alchina*		m_pBodyObject = { nullptr };

	class CSeletctorNode*		m_pRoot = {nullptr};


	CBlackBoard<ALCHINA_DATA>*	m_BlackBoard = { nullptr };
	class CBehaviorTree_Alcina* m_pBehaviorTree = { nullptr };

private:
	HRESULT						Ready_Components();
	HRESULT						Ready_PartObjects();
	HRESULT						Ready_Utility();
	HRESULT						Ready_StateObjects();

	void						Root_Move();
	void						Collider_Update();

public:
	static CAlcina*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free();


};

NS_END