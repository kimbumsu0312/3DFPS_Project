#pragma once
#include "Edit_Defines.h"
#include "Camera.h"

class CCamera_Free final : public CCamera 
{
public:
	typedef struct tagCameraFreeDesc : public CCamera::CAMERA_DESC
	{
		_float	fMouseSensor;
	}CAMERA_FREE_DESC;

public:
	CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Free(const CCamera_Free& Prototype);
	virtual ~CCamera_Free() = default;

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

	

private:
	void					Zoom_In(_float fTimeDelta);
	void					Zoom_Out(_float fTimeDelta);
	void					KeyInput(_float fTimeDelta);
public:
	static CCamera_Free*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg)override;
	virtual void			Free() override;

};

