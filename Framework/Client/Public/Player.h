#pragma once
#include "Engine_Defines.h"
#include "ContainerObject.h"

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

public:
	void						Move(_float fTimeDelta);
private:
	_uint						m_iPreState = {};
	_uint						m_iCulState = { PLAYER_STATE::IDLE };
	_uint						m_iWeponState = { PLAYER_WEAPON::KNIFE };
	string						m_szAnimTag;
	_wstring					m_szStateTag = {};
	_bool						m_bIsAnimLoop = {true};
	_bool						m_bIsAnimFinsh = {false};


	class CCamera_Player*		m_pCamera = { nullptr };
	_float3*					m_pMovePos = { nullptr };
	unordered_map<_wstring,
		class CStateObject*>	m_StateObjects;

	CStateObject*				m_CulStateObject = { nullptr };
private:
	HRESULT						Ready_Components();
	HRESULT						Ready_PartObjects();
	HRESULT						Ready_StateObjects();

	void						Update_RootMove();
	HRESULT						Add_StateObject(const _wstring& strStateObjectTag, CStateObject* pStateObject);
	class CStateObject*			Find_StateObject(const _wstring& strPartObjectTag);

public:
	static CPlayer*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free();
};

NS_END