#pragma once
#include "ContainerObject.h"

NS_BEGIN(Engine)
NS_END

NS_BEGIN(Client)
class CPlayer final : public CContainerObject
{
public:
	enum class PLAYER_STATE{IDLE, JOG_F, JOG_R, JOG_L, WALK_B, ATTACK, DIE, END};
private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

public:
	void					Move(_float fMoveValue);
private:
	_uint					m_iState = { };
	class CCamera_Player*	m_pCamera = { nullptr };
	_float3*				m_pMovePos = { nullptr };

private:
	HRESULT					Ready_Components();
	HRESULT					Ready_PartObjects();
	HRESULT					Ready_StateObjects();

	void					Update_RootMove();
public:
	static CPlayer*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free();
};

NS_END