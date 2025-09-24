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
class CMonster_WereWolf final : public CContainerObject
{
public:
	enum class ANIM_STATE { DAMAGE, NORMAL, ATTACK, END };
	enum class ColliderType_Mon { Body = 0, RESIST, ATTACK_L, ATTACK_R, End };

public:
	typedef struct WereWolfData
	{
		//애니메이션 관련
		_uint* iAnimState = { nullptr };
		string* szAnimTag = { nullptr };
		_bool* bIsAnimLoop = { nullptr };
		_bool* bIsAnimFinsh = { nullptr };

		_wstring* szCulStateTag = { nullptr };
		_int iHp = {};
		_int iDamage = {};

		_bool  IsAttack = {false};
		_float fAttackCool = {};
		_bool  IsChase = {false};

		_bool IsEvent_1 = { false };
		_bool IsEvent_2 = { false };
		_bool IsEvent_3 = { false };

		const _float4x4* MonPos = { nullptr };

		_float fNoies = {};
	}WEREWOLF_DATA;

private:
	CMonster_WereWolf(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster_WereWolf(const CMonster_WereWolf& Prototype);
	virtual ~CMonster_WereWolf() = default;

public:
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg);
	virtual void					Priority_Update(_float fTimeDelta);
	virtual void					Update(_float fTimeDelta);
	virtual void					Late_Update(_float fTimeDelta);
	virtual HRESULT					Render();

	void							Event3_Create();
public:
	void							Switch_Anim(string szAnimTag, _bool IsLoop);
	CBlackBoard<WEREWOLF_DATA>*		Get_BlackBoard() { return m_BlackBoard; }
	CTransform*						Get_Transform() {return m_pTransformCom; }
	CNavigation*					Get_Navi() { return m_pNavigationCom; }

	virtual void					OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision) override;

	void							Target_LookTurn(_float fTimeDelta);
	void							Attack_Collision();

	void							Event_1();
	void							Event_2();
	void							Event_3();
private:
	//컴포넌트 관련
	CNavigation*					m_pNavigationCom = { nullptr };
	CCollider*						m_pColliderCom[ENUM_CLASS(ColliderType_Mon::End)] = { nullptr };
	_float4x4*						m_pColliderBone[ENUM_CLASS(ColliderType_Mon::End)] = { nullptr };

	//스테이트 관련
	unordered_map<_wstring,
		class CMonState_WereWolf*>	m_StateObjects;
	class CMonState_WereWolf*		m_pCulStateObject = { nullptr };
	_wstring						m_szPreStateTag = {};
	_wstring						m_szCulStateTag = {};

	//애니메이션 관련
	_uint							m_iAnimState = {};
	string							m_szAnimTag = {};
	_bool							m_bIsAnimLoop = { true };
	_bool							m_bIsAnimFinsh = { false };

	//파츠 관련
	class CBody_WereWolf*			m_pBodyObject = { nullptr };
	
	//유틸 관련
	CBlackBoard<WEREWOLF_DATA>*		m_BlackBoard = { nullptr };
	class CBehaviorTree_WereWolf*	m_pBehaviorTree = { nullptr };

	_bool							m_bIsStart = { false };
private:
	HRESULT							Ready_Components();
	HRESULT							Ready_PartObjects();
	HRESULT							Ready_Utility();
	HRESULT							Ready_StateObjects();
	HRESULT							Ready_TriggerEvent();

	HRESULT							Add_StateObject(const _wstring& strStateObjectTag, CMonState_WereWolf* pStateObject);
	class CMonState_WereWolf*		Find_StateObject(const _wstring& strStateObjectTag);

	void							State_Change();
	void							Root_Move();
	void							Collider_Update();

public:
	static CMonster_WereWolf*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*			Clone(void* pArg);
	virtual void					Free();

};

NS_END