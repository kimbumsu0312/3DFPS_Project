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
	enum class Attack_Type {ZERO, SHORT, LONG, BEHIND, END};
	enum class ANIM_STATE { NORMAL, DAMAGE, ATTACK, END };
	enum ColliderType_Mon { Body = 0, Head, RESIST, ATTACK, End };
public:
	typedef struct Alchina_Data
	{
		//애니메이션 관련
		_uint*						iAnimState = {nullptr};
		string*						szAnimTag = { nullptr };
		_bool*						bIsAnimLoop = { nullptr };
		_bool*						bIsAnimFinsh = { nullptr };

		_wstring*					szCulStateTag = { nullptr };
		_int*						iHp = { nullptr };
		_int*						iDamage = { nullptr };

		_bool*						IsAttack = { nullptr };
		_bool*						IsChase = { nullptr };
		const _float4x4*			MonPos = { nullptr };
		Attack_Type					eAttackType = { Attack_Type::END };
	}ALCHINA_DATA;

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
	void						Switch_Anim(string szAnimTag, _bool IsLoop);
	CBlackBoard<ALCHINA_DATA>*	Get_BlackBoard() { return m_BlackBoard; }
	CTransform*					Get_TransForm();

	_bool						Get_HeadShot() { return m_bIsHeadShot; }
	void						Target_LookTurn(_float fTimeDelta);
	void						Target_LookAt();
	void						IsDamage();
	virtual void				OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision) override;
	

	void						SetUp_Node(_int iTargetCellIndex, _float3 vPos);
	void						Move_Node(_float fTimeDelta);

private:
	//컴포넌트 관련
	CNavigation*				m_pNavigationCom = { nullptr };
	CCollider*					m_pColliderCom[ColliderType_Mon::End] = { nullptr };
	_float4x4*					m_pColliderBone[ColliderType_Mon::End] = { nullptr };

	//스테이트 관련
	unordered_map<_wstring,
		class CMonState_Alcina*>m_StateObjects;
	class CMonState_Alcina*		m_pCulStateObject = { nullptr };
	_wstring					m_szPreStateTag = {};
	_wstring					m_szCulStateTag = {};

	//애니메이션 관련
	_uint						m_iAnimState = {};
	string						m_szAnimTag = {};
	_bool						m_bIsAnimLoop = { true };
	_bool						m_bIsAnimFinsh = { false };
	_bool						m_bIsHeadShot = { false };
	//파츠 관련
	class CBody_Alcina*			m_pBodyObject = { nullptr };
	
	//유틸 관련
	CBlackBoard<ALCHINA_DATA>*	m_BlackBoard = { nullptr };
	class CBehaviorTree_Alcina* m_pBehaviorTree = { nullptr };

	//상태 관련
	_int						m_iHp = {};
	_int						m_iDamage = {};
	_bool						m_bIsAttack = { false };
	_bool						m_bIsChase = { false };

private:
	HRESULT						Ready_Components();
	HRESULT						Ready_PartObjects();
	HRESULT						Ready_Utility();
	HRESULT						Ready_StateObjects();

	HRESULT						Add_StateObject(const _wstring& strStateObjectTag, CMonState_Alcina* pStateObject);
	class CMonState_Alcina*		Find_StateObject(const _wstring& strPartObjectTag);

	void						State_Change();
	void						Root_Move();
	void						Collider_Update();

public:
	static CAlcina*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free();


};

NS_END