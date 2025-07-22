#pragma once
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Terrain;
NS_END

NS_BEGIN(Edit)

class CTerrain final : public CGameObject
{
public:
	typedef struct tagCTerrain : public CGameObject::GAMEOBJECT_DESC
	{
		_uint	iNumverticesX = {};
		_uint	iNumverticesZ = {};
		SAVE_TERRAIN* Terrain_Data = { nullptr };
	}TERRAIN_DESC;

private:
	CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrain(const CTerrain& Prototype);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize_Prototype(void* pArg);
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

public:
	CVIBuffer_Terrain*		Get_Buffer() { return m_pVIBufferCom; }
private:
	CShader*				m_pShaderCom = { nullptr };
	CTexture*				m_pTextureCom = { nullptr };
	CVIBuffer_Terrain*		m_pVIBufferCom = { nullptr };
	_float3					vSetModelPos = {};

private:
	HRESULT					Ready_Components(void* pArg);
	HRESULT					Bind_ShaderResources();

public:
	static CTerrain*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	static CTerrain*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, void* pArg);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

NS_END