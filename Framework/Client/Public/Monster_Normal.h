#pragma once
#include "Engine_Defines.h"
#include "PoolingContainer.h"
#include "Client_Struct.h"
#include "BlackBoard.h"

NS_BEGIN(Engine)
class CCollider;
class CNavigation;
NS_END

NS_BEGIN(Client)
class CMonster_Normal final : public CPoolingContainer
{
public:
	enum class NORMAL_MON_STATE { NORMAL, FALL, DAMAGE, ATTACK, END };
	enum class NORMAL_MON_WEAPON { SWORD, HALBERD, SHOTEL, END };
public:
	typedef struct NormalMon_Data
	{
		//애니메이션 관련
		_uint*				iAnimState = { nullptr };
		string*				szAnimTag = { nullptr };
		_bool*				bIsAnimLoop = { nullptr };
		_bool*				bIsAnimFinsh = { nullptr };

		_wstring*			szCulStateTag = { nullptr };

		_bool				bIsHeadShot = { false };
		_bool				IsChase = {false};
		_bool				IsIdle = {false};
		_bool				IsAttack = { false };
		_bool				IsWeaponDamage = { false };
		const _float4x4*	MonPos = { nullptr };

		_int				iHp = {};
		_int				iDamage = {};
		_float				fAttackCool = {};
		_int				iWeapon = {};
		_int				iStartMotion = {};
		_int				iDropItemIndex = {};

		_float				fNoies = {};

	}NORMALMON_DATA;
private:
	enum ColliderType_Mon { BODY = 0, Head, Hand, RESIST, CHASE, END };
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
	void						Switch_Anim(string szAnimTag, _bool IsLoop);
	void						Switch_State(_wstring szStateTag) { m_szCulStateTag = szStateTag; }
	CBlackBoard<
		NORMALMON_DATA>*		Get_BlackBoard() { return m_BlackBoard;	}


	const _vector				Get_TransformState(STATE eState) { return m_pTransformCom->Get_State(eState); }
	const _int					Get_CulNaviIndex();
	void						Target_LookAt(_float fTimeDelta);
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

	//유틸 관련
	CBlackBoard<NORMALMON_DATA>* m_BlackBoard = { nullptr };
	class CBehaviorTree_Normon_1* m_pBehaviorTree = { nullptr };

	//스테이트 관련
	unordered_map<_wstring,
		class CMonState_Normal*>m_StateObjects;
	class CMonState_Normal*		m_pCulStateObject = { nullptr };
	_wstring					m_szPreStateTag = {};
	_wstring					m_szCulStateTag = {};
	
	//애니메이션 관련
	_uint						m_iAnimState = { ENUM_CLASS(NORMAL_MON_STATE::NORMAL) };
	string						m_szAnimTag;
	_bool						m_bIsAnimLoop = { true };
	_bool						m_bIsAnimFinsh = { false };

private:
	HRESULT						Ready_Components();
	HRESULT						Ready_PartObjects();
	HRESULT						Ready_Utility();
	HRESULT						Ready_StateObjects();

	HRESULT						Add_StateObject(const _wstring& strStateObjectTag, CMonState_Normal* pStateObject);
	class CMonState_Normal*		Find_StateObject(const _wstring& strPartObjectTag);
 	
	void						State_Change();
	void						Root_Move();
	void						Collider_Update();

public:
	static CMonster_Normal*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free();


};

NS_END