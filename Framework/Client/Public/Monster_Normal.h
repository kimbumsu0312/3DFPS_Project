#pragma once
#include "Engine_Defines.h"
#include "PoolingContainer.h"
#include "Client_Struct.h"

NS_BEGIN(Engine)
class CCollider;
class CNavigation;
NS_END

NS_BEGIN(Client)
class CMonster_Normal final : public CPoolingContainer
{
private:
	enum ColliderType_Mon { BODY = 0, Head, Hand, RESIST, END };
private:
	CMonster_Normal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_Normal(const CMonster_Normal& Prototype);
	virtual ~CMonster_Normal() = default;

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
	_bool						IsHeadShot() { return m_bIsHeadShot; }

	const NORMON_STATE&			Get_State() { return m_NorMonState;}
	const _int&					Get_WeaponType() { return m_iWeaponType; }
	const _vector				Get_TransformState(STATE eState) { return m_pTransformCom->Get_State(eState); }

	void						Target_LookAt(_float fTimeDelta);
	void						Reset_DamageCheck() { m_bIsDamage = false; m_bIsHeadShot = false; }
	void						Attack_Collision();

	virtual void				OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision) override;

public:
	virtual HRESULT				Initialize_Pool(void* pArg) override;
	virtual void				Return_Pool() override;

private:
	CNavigation*				m_pNavigationCom = { nullptr };
	//콜리전
	CCollider*					m_pColliderCom[ColliderType_Mon::END] = {nullptr};
	_float4x4*					m_pColliderBone[ColliderType_Mon::END] = {nullptr};

	//파츠 오브젝트 관련
	class CWeaponObject*		m_pWeaponObject = { nullptr };
	class CBody_NorMon*			m_pBodyObject = { nullptr };

	_int						m_iWeaponType = {};
	
	//상태 관련
	unordered_map<_wstring,
		class CMonState_Normal*>m_StateObjects;
	
	class CMonState_Normal*		m_pCulStateObject = { nullptr };
	_wstring					m_szPreStateTag = {};
	_wstring					m_szCulStateTag = {};

	NORMON_STATE				m_NorMonState = {};
	_bool						m_bIsDamage = { false };
	_bool						m_bIsHeadShot = { false };

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

	HRESULT						Add_StateObject(const _wstring& strStateObjectTag, CMonState_Normal* pStateObject);
	class CMonState_Normal*		Find_StateObject(const _wstring& strPartObjectTag);

	void						State_Check();

	
	void						State_Change();
	void						Root_Move();
	void						Collider_Update();

public:
	static CMonster_Normal*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free();


};

NS_END