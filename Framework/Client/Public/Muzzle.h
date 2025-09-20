#pragma once
#include "PartObject.h"
#include "Client_Defines.h"
#include "Muzzle_Effect.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)
class CMuzzle final : public CPartObject
{
public:
	typedef struct Muzzle : public CPartObject::PARTOBJECT_DESC{
		CBlackBoard<CMuzzle_Effect::MUZZLE_EFFECT_DATA>* m_BlackBoard = { nullptr };
		CMuzzle_Effect::Gun_Type eGunType = {};

	}MUZZLE_DATA;
private:
	CMuzzle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMuzzle(const CMuzzle& Prototype);
	virtual ~CMuzzle() = default;

public:
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg);
	virtual void					Priority_Update(_float fTimeDelta);
	virtual void					Update(_float fTimeDelta);
	virtual void					Late_Update(_float fTimeDelta);
	virtual HRESULT					Render();

private:
	CShader*						m_pShaderCom = { nullptr };
	CTexture*						m_pTextureCom = { nullptr };
	CVIBuffer_Rect*					m_pVIBufferCom = { nullptr };

	
	_float2							m_vUVMin = {};
	_float2							m_vUVMax = {};
	_int							m_iCount = {};

	CBlackBoard<CMuzzle_Effect::MUZZLE_EFFECT_DATA>* m_BlackBoard = { nullptr };

	_float							m_fSize = {};
private:
	HRESULT							Ready_Components();
	HRESULT							Bind_ShaderResources();

public:
	static CMuzzle*					Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};

NS_END