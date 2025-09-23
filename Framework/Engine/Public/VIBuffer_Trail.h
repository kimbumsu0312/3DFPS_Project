#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Trail final : public CVIBuffer
{
private:
	CVIBuffer_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Trail(const CVIBuffer_Trail& Prototype);
	virtual ~CVIBuffer_Trail() = default;

public:
	virtual HRESULT			Initialize_Prototype() override;
	virtual HRESULT			Initialize(void* pArg) override;

	virtual HRESULT			Render();
	virtual HRESULT			Bind_Resources() override;

	void					Update_Trail(_float fTimeDelta);
	void					Add_Postion(_float3 vTopPos, _float3 vBottomPos, _float fTimeDelta);
	void					Reset();

private:
	_float					m_fDeleyTime = {};
	_float					m_fAccTime = {};
	_int					m_iMaxNumVertices = {};

	D3D11_BUFFER_DESC		m_VBDesc = {};
	void*					m_pVertices = {};
public:
	static CVIBuffer_Trail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent*		Clone(void* pArg) override;
	virtual void			Free() override;
};
NS_END
