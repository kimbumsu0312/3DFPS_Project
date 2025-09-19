#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Point_Instance;
NS_END

NS_BEGIN(Client)

class CScreen_Blood_Particle final : public CUIObject
{
private:
	CScreen_Blood_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CScreen_Blood_Particle(const CScreen_Blood_Particle& Prototype);
	virtual ~CScreen_Blood_Particle() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

private:
	CVIBuffer_Point_Instance*	m_pVIBufferCom[5] = {nullptr};

	_int						m_iTexSizeX = {};
	_int						m_iTexSizeY = {};

	_float						m_fAlpha = {};

	_int						m_iIndex = {};
private:
	HRESULT						Ready_Components();

public:
	static CScreen_Blood_Particle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void				Free() override;
};

NS_END