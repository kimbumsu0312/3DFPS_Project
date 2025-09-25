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
public:
	enum class ANIM_STATE { NORMAL, ATTACK, DAMAGE, OTHER, END };
	enum class ColliderType_Mon { Body = 0, Head, L_ARM, R_ARM, End };

	typedef struct Bela_Data
	{
		//애니메이션 관련
		_uint* iAnimState = { nullptr };
		string* szAnimTag = { nullptr };
		_bool* bIsAnimLoop = { nullptr };
		_bool* bIsAnimFinsh = { nullptr };

		_wstring* szCulStateTag = { nullptr };

		BOSS_SISTER_HIT		IsHitPoint = {};
		_bool				IsChase = { false };
		_bool				IsAttack = { false };
		_bool				IsSpawn = { false };

		_bool				IsEvent_1 = { false };
		_bool				IsEvent_2 = { false };
		_bool				IsEvent_3 = { false };

		_bool				IsFreezes = { false };
		_bool				IsDamage = {};

		const _float4x4*	MonPos = { nullptr };

		_int				iHp = {};

		_float				fNoies = {};
		_float				fFreezes = {};

		_bool				bIsFly = {};
		_bool				bIsSpawnFly = {};

		_float				fMoveSpeed = {};

	}BELA_DATA;

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
	void						Switch_Anim(string szAnimTag, _bool IsLoop);
	CTransform*					Get_Transform() { return m_pTransformCom; }
	CNavigation*				Get_Navi() {return m_pNavigationCom;}
	CBlackBoard<BELA_DATA>*		Get_BlackBoard() { return m_BlackBoard; }

	void						Target_LookTurn(_float fTimeDelta);
	void						Target_LookAt();
	void						Attack_Collision();
	virtual void				OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision) override;

	void						Spawn_EffectReset();
	void						SetUp_Node();
	void						Target_LookTurn_Navi(_float fTimeDelta);
private:
	//컴포넌트 관련
	CNavigation*				m_pNavigationCom = { nullptr };
	CCollider*					m_pColliderCom[ENUM_CLASS(ColliderType_Mon::End)] = {nullptr};
	_float4x4*					m_pColliderBone[ENUM_CLASS(ColliderType_Mon::End)] = {nullptr};

	//파츠 오브젝트 관련
	class CBoss_Shotel*			m_pWeaponObject = { nullptr };
	class CBody_Bela*			m_pBodyObject = { nullptr };
	
	//상태 관련
	unordered_map<_wstring,
		class CMonState_Bela*>	m_StateObjects;
	
	class CMonState_Bela*		m_pCulStateObject = { nullptr };
	_wstring					m_szPreStateTag = {};
	_wstring					m_szCulStateTag = {};

	//유틸 관련
	CBlackBoard<BELA_DATA>*		m_BlackBoard = { nullptr };
	class CBehaviorTree_Bela*	m_pBehaviorTree = { nullptr };

	//애니메이션 관련
	_uint						m_iAnimState = {};
	string						m_szAnimTag;
	_bool						m_bIsAnimLoop = { true };
	_bool						m_bIsAnimFinsh = { false };

	//이펙트 관련
	class CFly_Effect*			m_pEffect = { nullptr };
	class CFly_Effect*			m_pSpawnEffect = { nullptr };
private:
	HRESULT						Ready_Components();
	HRESULT						Ready_PartObjects();
	HRESULT						Ready_Utility();
	HRESULT						Ready_StateObjects();
	HRESULT						Ready_TriggerEvent();

	HRESULT						Add_StateObject(const _wstring& strStateObjectTag, CMonState_Bela* pStateObject);
	class CMonState_Bela*		Find_StateObject(const _wstring& strPartObjectTag);

	void						State_Change();
	void						Root_Move();
	void						Collider_Update();

	void						Event_Spawn();

	void						Event_2();
	void						Event_3();
	void						Event_DIe();

public:
	static CBela*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free();


};

NS_END