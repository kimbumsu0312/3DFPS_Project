#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CAim_Line final : public CUIObject
{
public:
	typedef struct CAimLineDesc : public CUIObject::UIOBJECT_DESC
	{
		WEAPON_TYPE eType;
	}AIM_LINE_DESC;

private:
	CAim_Line(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAim_Line(const CAim_Line& Prototype);
	virtual ~CAim_Line() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	_float						m_fRotation = {};
	_bool						m_bZoom = { false };
	_uint						m_iIndex = {};
	_float2						m_vMovePos = {};
	_float2						m_vZoomUV = {};

private:
	HRESULT						Ready_Components();

	void						Aim_Zoom();
public:
	static CAim_Line*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};
NS_END
