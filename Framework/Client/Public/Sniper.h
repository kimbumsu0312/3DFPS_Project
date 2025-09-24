#pragma once
#include "WeaponObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
NS_END

NS_BEGIN(Client)
class CSniper final : public CWeaponObject
{
public:
	typedef struct tagWeaponSniperDesc : public CWeaponObject::WEAPON_DESC
	{
		CBlackBoard<CPlayer::PLAYER_DATA>* m_BlackBoard = { nullptr };
	}SNIPER_DESC;
private:
	CSniper(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSniper(const CSniper& Prototype);
	virtual ~CSniper() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

private:
	CShader*				m_pShaderCom = { nullptr };
	CModel*					m_pModelCom = { nullptr };
	CAnimatio_Controller*	m_pAnimCom = { nullptr };
	string					m_AnimTag = {};
	
	_bool					m_bEffect = {};
	_float					m_fRange = { -0.86f };
	CBlackBoard<CPlayer::PLAYER_DATA>* m_pBlackBoard = { nullptr };

	_float					m_fAccTime = {};
	_bool					m_IsSounde = {};

private:
	HRESULT					Ready_Components();
	HRESULT					Bind_ShaderResources();

public:
	static CSniper*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

NS_END