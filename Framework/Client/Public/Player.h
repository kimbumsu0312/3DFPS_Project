#pragma once
#include "Engine_Defines.h"
#include "ContainerObject.h"
#include "Client_Struct.h"
#include "BlackBoard.h"

NS_BEGIN(Engine)
class CNavigation;
class CCollider;
NS_END

NS_BEGIN(Client)
class CPlayer final : public CContainerObject
{
public:
	enum ColliderType_Player { BODY = 0, RESIST, PLAYER_VIEW, END };
	typedef struct PlayerData
	{
		//애니메이션 관련
		_uint*		iAnimState = { nullptr };
		string*		szAnimTag = { nullptr };
		_bool*		bIsAnimLoop = { nullptr };
		_bool*		bIsAnimFinsh = { nullptr };

		_wstring*	szCulStateTag = { nullptr };

		_bool		bIsDamage = { false };
		_float		fDamageCool = {};
		
		_bool		isJog;
		
		_bool		isMove;
		_bool		isAttack;
		_bool		isGuard;
		_bool		isReload;
		_bool		isAim;
		_bool		isWeaponSwap;

	}PLAYER_DATA;
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
	void						Switch_Anim(string szAnimTag, _bool IsLoop);
	_bool						IsAnimFinsh() { return m_bIsAnimFinsh; }
	CBlackBoard<PLAYER_DATA>*	Get_BlackBoard() { return m_BlackBoard;}


	virtual void				OnCollision(COLLISIONENTRY MyCollision, COLLISIONENTRY TargetCollision) override;
	void						WeaponSwap();
	void						AttackCollider();
private:
	CNavigation*				m_pNavigationCom = { nullptr };
	CCollider*					m_pColliderCom[ColliderType_Player::END] = {nullptr};
	_float4x4*					m_pColliderBone[ColliderType_Player::END] = { nullptr };

	_uint						m_iAnimState = {};
	_int						m_iNextAnim = {};

	_wstring					m_szPreStateTag = {};
	_wstring					m_szCulStateTag = {};

	string						m_szAnimTag;
	_bool						m_bIsAnimLoop = {true};
	_bool						m_bIsAnimFinsh = {false};

	class CCamera_Player*		m_pCamera = { nullptr };

	unordered_map<_wstring,
		class CPlayerState*>	m_StateObjects;

	CBlackBoard<PLAYER_DATA>*	m_BlackBoard = { nullptr };

	class CPlayerState*			m_CulStateObject = { nullptr };
	CPartObject*				m_pWeaponObject = { nullptr };
	class CBody_Player*			m_pBodyObject = { nullptr };

	_bool						m_bIsUIOpen = { false };
	_float						m_fYaw = {};
	_float						m_fPitch = {};

	_bool						m_bisCameraLock = { false };

	_int						m_iRayCount = {};
	_float						m_fRayRange = {};
private:
	HRESULT						Ready_Components();
	HRESULT						Ready_PartObjects();
	HRESULT						Ready_StateObjects();
	HRESULT						Ready_Utility();

	HRESULT						Add_StateObject(const _wstring& strStateObjectTag, class CPlayerState* pStateObject);
	class CPlayerState*			Find_StateObject(const _wstring& strPartObjectTag);

	void						InputKey_MoveState(_float fTimeDelta);
	void						InputKey_AttackState(_float fTimeDelta);
	void						InputKey_WeaponChange(_float fTimeDelta);
	_bool						InputKey_UI();

	void						Rotaion_Upper(_float fTimeDelta);
	void						Collider_Update();
	void						State_Check();
public:
	static CPlayer*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free();
};

NS_END