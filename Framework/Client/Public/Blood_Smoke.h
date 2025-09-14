#pragma once
#include "PartObject.h"
#include "BlackBoard.h"
#include "Blood_Effect.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Point_Instance;
NS_END

NS_BEGIN(Client)

class CBlood_Smoke final : public CPartObject
{
public:
	typedef struct Muzzle_Smoke : public CPartObject::PARTOBJECT_DESC {
		CBlackBoard<CBlood_Effect::BLODE_EFFECT_DATA>* m_BlackBoard = { nullptr };
	}BLODE_SMOKE_DATA;
private:
	CBlood_Smoke(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBlood_Smoke(const CBlood_Smoke& Prototype);
	virtual ~CBlood_Smoke() = default;

public:
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg);
	virtual void					Priority_Update(_float fTimeDelta);
	virtual void					Update(_float fTimeDelta);
	virtual void					Late_Update(_float fTimeDelta);
	virtual HRESULT					Render();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Point_Instance* m_pVIBufferCom = { nullptr };

	_int							m_iTexSizeX = {};
	_int							m_iTexSizeY = {};

	CBlackBoard<CBlood_Effect::BLODE_EFFECT_DATA>* m_BlackBoard = { nullptr };

	_float							m_fCount = {};

private:
	HRESULT							Ready_Components();
	HRESULT							Bind_ShaderResources();

public:
	static CBlood_Smoke* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void					Free() override;
};

NS_END