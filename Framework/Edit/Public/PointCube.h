#pragma once
#include "PartObject.h"
NS_BEGIN(Engine)
class CShader;
class CVIBuffer_Cube;
NS_END

NS_BEGIN(Edit)
class CPointCube final : public CPartObject
{
private:
	CPointCube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPointCube(const CPointCube& Prototype);
	virtual ~CPointCube() = default;

public:
	void					Set_Transform(_float3 vPos);
	void					IsSelete(_bool IsSelete) { m_bIsSelete = IsSelete; }
public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

private:
	CShader*				m_pShaderCom = { nullptr };
	CVIBuffer_Cube*			m_pVIBufferCom = { nullptr };

	_bool					m_bIsSelete = { false };

private:
	HRESULT					Ready_Components();
	HRESULT					Bind_ShaderResources();

public:
	static CPointCube*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

NS_END