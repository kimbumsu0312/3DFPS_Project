#pragma once
#include "Engine_Defines.h"
#include "ContainerObject.h"
#include "Client_Struct.h"

NS_BEGIN(Engine)
class CCollider;
class CNavigation;
NS_END

NS_BEGIN(Client)
class CDaniela final : public CContainerObject
{
public:
	enum class ANIM_STATE { NORMAL, ATTACK, DAMAGE, OTHER, END };
	enum class ColliderType_Mon { Body = 0, Head, L_ARM, R_ARM, RESIST, End };

	typedef struct Daniela_Data
	{
		//애니메이션 관련
		_uint*				iAnimState = { nullptr };
		string*				szAnimTag = { nullptr };
		_bool*				bIsAnimLoop = { nullptr };
		_bool*				bIsAnimFinsh = { nullptr };

		_wstring*			szCulStateTag = { nullptr };

		BOSS_SISTER_HIT		IsHitPoint = {};
		_bool				IsIdle = { false };
		_bool				IsAttack = { false };
		_bool				IsCriticalAttack = { false };
		const _float4x4*	MonPos = { nullptr };

		_int				iHp = {};
		_int				iDamage = {};
		_float				fDamage_Cool = {};
		_float				fCriAttackCool = {};
		_float				fAttackCool = {};
		_int				iWeapon = {};
		_int				iStartMotion = {};
		_int				iDropItemIndex = {};

		_float				fNoies = {};
		_bool				isBogan = {};
	}DANIELA_DATA;

private:
	CDaniela(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDaniela(const CDaniela& Prototype);
	virtual ~CDaniela() = default;

public:
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg);
	virtual void					Priority_Update(_float fTimeDelta);
	virtual void					Update(_float fTimeDelta);
	virtual void					Late_Update(_float fTimeDelta);
	virtual HRESULT					Render();

public:
	void							Switch_AnimState(_uint iAnimState) { if (iAnimState < ENUM_CLASS(ANIM_STATE::END)) { m_iAnimState = iAnimState; } }
	void							Switch_Anim(string szAnimTag, _bool IsLoop);
	void							Switch_State(_wstring szStateTag) { m_szCulStateTag = szStateTag; }

	CTransform*						Get_Transform() { return m_pTransformCom; }
	CBlackBoard<DANIELA_DATA>*		Get_BlackBoard() { return m_BlackBoard; }
	CNavigation*					Get_Navi() {return m_pNavigationCom;}

	void							Target_LookTurn(_float fTimeDelta);
	void							Target_LookTurn_Navi(_float fTimeDelta);
	void							SetUp_Node();
	void							Attack_Collision();
	virtual void					OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision) override;

	void							Daniela_Start();
private:
	//컴포넌트 관련
	CNavigation*					m_pNavigationCom = { nullptr };
	CCollider*						m_pColliderCom[ENUM_CLASS(ColliderType_Mon::End)] = {nullptr};
	_float4x4*						m_pColliderBone[ENUM_CLASS(ColliderType_Mon::End)] = {nullptr};

	//파츠 오브젝트 관련
	class CBoss_Shotel*				m_pWeaponObject = { nullptr };
	class CBody_Daniela*			m_pBodyObject = { nullptr };
		
	//상태 관련
	unordered_map<_wstring,
		class CMonState_Daniela*>	m_StateObjects;
	
	class CMonState_Daniela*		m_pCulStateObject = { nullptr };
	_wstring						m_szPreStateTag = {};
	_wstring						m_szCulStateTag = {};

	//유틸 관련
	CBlackBoard<DANIELA_DATA>*		m_BlackBoard = { nullptr };
	class CBehaviorTree_Daniela*	m_pBehaviorTree = { nullptr };

	//애니메이션 관련
	_uint							m_iAnimState = {};
	string							m_szAnimTag;
	_bool							m_bIsAnimLoop = { true };
	_bool							m_bIsAnimFinsh = { false };
	
	_bool							m_bIsStart = { false };
private:
	HRESULT							Ready_Components();
	HRESULT							Ready_PartObjects();
	HRESULT							Ready_Utility();
	HRESULT							Ready_StateObjects();
	HRESULT							Ready_TriggerEvent();

	HRESULT							Add_StateObject(const _wstring& strStateObjectTag, class CMonState_Daniela* pStateObject);
	class CMonState_Daniela*		Find_StateObject(const _wstring& strPartObjectTag);

	void							State_Change();
	void							Root_Move();
	void							Collider_Update();
	 
public:
	static CDaniela*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free();
};

NS_END