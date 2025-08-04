#pragma once
#include "Client_Defines.h"
#include "Camera.h"

NS_BEGIN(Client)
class CCamera_Player final : public CCamera
{
public:
	typedef struct tagCameraPlayerDesc : public CCamera::CAMERA_DESC
	{
		_float				fMouseSensor;
		const _float4x4*	pSocketMatrix1 = { nullptr };
		const _float4x4*	pSocketMatrix2 = { nullptr };
		const _float4x4*	pParentMatrix = { nullptr };
	}CAMERA_PLAYER_DESC;

public:
	CCamera_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Player(const CCamera_Player& Prototype);
	virtual ~CCamera_Player() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;
	virtual void			Priority_Update(_float fTimeDelta) override;
	virtual void			Update(_float fTimeDelta) override;
	virtual void			Late_Update(_float fTimeDelta) override;
	virtual HRESULT			Render() override;

private:
	_float					m_fMouseSensor = { };

	_float					m_fMoveFovy = {};

	_float					m_fResetFovy = {};
	_float					m_fZoomSpeed = {};

	_bool					m_bIsZoomIn = { false };
	_bool					m_bIsZoomout = { false };

	const _float4x4*		m_pParentMatrix = { nullptr };
	const _float4x4*		m_pSocketMatrix1 = { nullptr };
	const _float4x4*		m_pSocketMatrix2 = { nullptr };
	_matrix					m_DefultWorldMatrix = {};
	_float4x4				m_CombinedWorldMatrix = {};

	_int					m_iMousePosX = {};
	_int					m_iMousePosY = {};

	_float					m_fYaw = {};
	_float					m_fPitch = {};

private:
	void					Zoom_In(_float fTimeDelta);
	void					Zoom_Out(_float fTimeDelta);
	void					Update_CameraPos();
	void					Update_CameraRot(_float fTimeDelta);
public:
	static CCamera_Player*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg)override;
	virtual void			Free() override;

};

NS_END