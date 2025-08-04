#pragma once
#include "Engine_Defines.h"
#include "PoolingContainer.h"
#include "Client_Struct.h"

NS_BEGIN(Client)
class CMonster_Normal final : public CPoolingContainer
{
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
	virtual void				On_Dead() override;
	virtual HRESULT				Initialize_Pool(void* pArg) override;
	virtual void				Return_Pool() override;

private:
	_uint						m_iPreAnimState = { ENUM_CLASS(NORMAL_MON_STATE::NORMAL) };
	_uint						m_iCulAnimState = { ENUM_CLASS(NORMAL_MON_STATE::NORMAL) };

	_wstring					m_szPreStateTag = {};
	_wstring					m_szCulStateTag = {};

	string						m_szAnimTag;
	_bool						m_bIsAnimLoop = { true };
	_bool						m_bIsAnimFinsh = { false };

	unordered_map<_wstring,
		class CStateObject*>	m_StateObjects;

	CStateObject*				m_pCulStateObject = { nullptr };
	CPartObject*				m_pWeaponObject = { nullptr };
	class CBody_NorMon*				m_pBodyObject = { nullptr };

	_int						m_iWeaponType = {};

	NORMON_STATE				m_NorMonState = {};

private:
	HRESULT						Ready_Components();
	HRESULT						Ready_PartObjects();
	HRESULT						Ready_StateObjects();

	HRESULT						Add_StateObject(const _wstring& strStateObjectTag, CStateObject* pStateObject);
	class CStateObject*			Find_StateObject(const _wstring& strPartObjectTag);

	void						State_Check();

public:
	static CMonster_Normal*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free();


};

NS_END