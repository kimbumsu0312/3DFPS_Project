#pragma once
#include "Engine_Defines.h"
#include "ContainerObject.h"
#include "Client_Struct.h"

NS_BEGIN(Client)
class CPlayer final : public CContainerObject
{
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
private:
	_uint						m_iPreWeponState = { PLAYER_WEAPON::KNIFE };
	_uint						m_iCulWeponState = { PLAYER_WEAPON::KNIFE };

	_wstring					m_szPreStateTag = {};
	_wstring					m_szCulStateTag = {};

	string						m_szAnimTag;
	_bool						m_bIsAnimLoop = {true};
	_bool						m_bIsAnimFinsh = {false};

	class CCamera_Player*		m_pCamera = { nullptr };

	unordered_map<_wstring,
		class CStateObject*>	m_StateObjects;

	PLAYER_ATTACK_STATE			m_AttackState = {};
	PLAYER_MOVE_STATE			m_MoveState = {};

	CStateObject*				m_CulStateObject = { nullptr };
	CPartObject*				m_pWeaponObject = { nullptr };
	class CBody_Player*			m_pBodyObject = { nullptr };

	_bool						m_bIsUIOpen = { false };
	_float						m_fYaw = {};
	_float						m_fPitch = {};

	_bool						m_bisCameraLock = { false };
private:
	HRESULT						Ready_Components();
	HRESULT						Ready_PartObjects();
	HRESULT						Ready_StateObjects();

	HRESULT						Add_StateObject(const _wstring& strStateObjectTag, CStateObject* pStateObject);
	class CStateObject*			Find_StateObject(const _wstring& strPartObjectTag);

	void						InputKey_MoveState(_float fTimeDelta);
	void						InputKey_AttackState(_float fTimeDelta);
	void						InputKey_WeaponChange(_float fTimeDelta);
	_bool						InputKey_UI();

	void						Rotaion_Upper(_float fTimeDelta);
public:
	static CPlayer*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free();
};

NS_END