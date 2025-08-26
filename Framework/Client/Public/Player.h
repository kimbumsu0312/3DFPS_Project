#pragma once
#include "Engine_Defines.h"
#include "ContainerObject.h"
#include "Client_Struct.h"
NS_BEGIN(Engine)
class CNavigation;
class CCollider;
NS_END

NS_BEGIN(Client)
class CPlayer final : public CContainerObject
{
private:
	enum ColliderType_Player { BODY = 0, RESIST, PLAYER_VIEW, END };
private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

public:
	void						Switch_WeaponState(_uint iWeaponState) { if (iWeaponState < ENUM_CLASS(PLAYER_WEAPON::END)) { m_iCulWeponState = iWeaponState; } }
	void						Switch_Anim(string szAnimTag, _bool IsLoop);
	void						Switch_State(_wstring szStateTag) { m_szCulStateTag = szStateTag; }
	_bool						IsAnimFinsh() { return m_bIsAnimFinsh; }
	void						WeaponSwap() { m_iCulWeponState = m_iNextWeponState; }

	const PLAYER_ATTACK_STATE&	Get_AttackState() { return m_AttackState; }
	const PLAYER_MOVE_STATE&	Get_MoveState() { return m_MoveState; }
	const _int&					Get_WeaponType() { return m_iCulWeponState; }
	virtual void				OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision) override;

private:
	CNavigation*				m_pNavigationCom = { nullptr };
	CCollider*					m_pColliderCom[ColliderType_Player::END] = {nullptr};
	_float4x4*					m_pColliderBone[ColliderType_Player::END] = { nullptr };

	_uint						m_iPreWeponState = {};
	_uint						m_iCulWeponState = {};
	_uint						m_iNextWeponState = {};

	_wstring					m_szPreStateTag = {};
	_wstring					m_szCulStateTag = {};

	string						m_szAnimTag;
	_bool						m_bIsAnimLoop = {true};
	_bool						m_bIsAnimFinsh = {false};

	class CCamera_Player*		m_pCamera = { nullptr };

	unordered_map<_wstring,
		class CPlayerState*>	m_StateObjects;

	PLAYER_ATTACK_STATE			m_AttackState = {};
	PLAYER_MOVE_STATE			m_MoveState = {};

	class CPlayerState*			m_CulStateObject = { nullptr };
	CPartObject*				m_pWeaponObject = { nullptr };
	class CBody_Player*			m_pBodyObject = { nullptr };

	_bool						m_bIsUIOpen = { false };
	_float						m_fYaw = {};
	_float						m_fPitch = {};

	_bool						m_bisCameraLock = { false };

	_bool						m_bIsDamage = { false };
	_float						m_fDamageCool = {};
private:
	HRESULT						Ready_Components();
	HRESULT						Ready_PartObjects();
	HRESULT						Ready_StateObjects();

	HRESULT						Add_StateObject(const _wstring& strStateObjectTag, class CPlayerState* pStateObject);
	class CPlayerState*			Find_StateObject(const _wstring& strPartObjectTag);

	void						InputKey_MoveState(_float fTimeDelta);
	void						InputKey_AttackState(_float fTimeDelta);
	void						InputKey_WeaponChange(_float fTimeDelta);
	_bool						InputKey_UI();

	void						Rotaion_Upper(_float fTimeDelta);
	void						Collider_Update();

	void						IsDmage(_float fTimeDelta);
public:
	static CPlayer*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free();
};

NS_END