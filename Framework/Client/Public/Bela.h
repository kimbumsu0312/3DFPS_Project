#pragma once
#include "Engine_Defines.h"
#include "ContainerObject.h"
#include "Client_Struct.h"

NS_BEGIN(Engine)
class CCollider;
class CNavigation;
NS_END

NS_BEGIN(Client)
class CBela final : public CContainerObject
{
private:
	enum ColliderType_Mon { Body = 0, Head, L_ARM, R_ARM, End };
private:
	CBela(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBela(const CBela& Prototype);
	virtual ~CBela() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

public:
	void						Switch_AnimState(_uint iAnimState) { if (iAnimState < ENUM_CLASS(NORMAL_MON_STATE::END)) { m_iAnimState = iAnimState; } }
	void						Switch_Anim(string szAnimTag, _bool IsLoop);
	void						Switch_State(_wstring szStateTag) { m_szCulStateTag = szStateTag; }
	_bool						IsAnimFinsh() { return m_bIsAnimFinsh; }
	BOSS_SISTER_HIT				IsHitPoint() { return m_IsHitPoint; }

	const BOSS_SISTER_STATE&	Get_State() { return m_State;}
	const _vector				Get_TransformState(STATE eState) { return m_pTransformCom->Get_State(eState); }

	void						Target_LookTurn(_float fTimeDelta);
	void						Target_LookAt();
	void						Reset_DamageCheck() { m_bIsDamage = false; m_IsHitPoint = {}; }
	void						Attack_Collision();
	void						IsDamage();
	virtual void				OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision) override;

private:
	CNavigation*				m_pNavigationCom = { nullptr };
	//콜리전
	CCollider*					m_pColliderCom[ColliderType_Mon::End] = {nullptr};
	_float4x4*					m_pColliderBone[ColliderType_Mon::End] = {nullptr};

	//파츠 오브젝트 관련
	class CWeaponObject*		m_pWeaponObject = { nullptr };
	class CBody_Bela*			m_pBodyObject = { nullptr };

	_int						m_iWeaponType = {};
	
	//상태 관련
	unordered_map<_wstring,
		class CMonState_Bela*>m_StateObjects;
	
	class CMonState_Bela*	m_pCulStateObject = { nullptr };
	_wstring					m_szPreStateTag = {};
	_wstring					m_szCulStateTag = {};

	BOSS_SISTER_STATE			m_State = {};
	BOSS_SISTER_HIT				m_IsHitPoint = { false };
	_bool						m_bIsDamage = { false };

	_int						m_iHp = {};
	//애니메이션 관련
	_uint						m_iAnimState = { ENUM_CLASS(NORMAL_MON_STATE::NORMAL) };
	string						m_szAnimTag;
	_bool						m_bIsAnimLoop = { true };
	_bool						m_bIsAnimFinsh = { false };
private:
	HRESULT						Ready_Components();
	HRESULT						Ready_PartObjects();
	HRESULT						Ready_StateObjects();

	HRESULT						Add_StateObject(const _wstring& strStateObjectTag, CMonState_Bela* pStateObject);
	class CMonState_Bela*		Find_StateObject(const _wstring& strPartObjectTag);

	void						State_Check();
	void						State_Change();
	void						Root_Move();
	void						Collider_Update();

public:
	static CBela*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free();


};

NS_END