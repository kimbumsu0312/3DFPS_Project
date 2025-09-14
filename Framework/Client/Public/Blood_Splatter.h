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
class CBlood_Splatter final : public CPartObject
{
public:
	typedef struct Blood_Splatter : public CPartObject::PARTOBJECT_DESC {
		CBlackBoard<CBlood_Effect::BLODE_EFFECT_DATA>* m_BlackBoard = { nullptr };
	}BLODE_SPLATTER_DATA;
private:
	CBlood_Splatter(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBlood_Splatter(const CBlood_Splatter& Prototype);
	virtual ~CBlood_Splatter() = default;

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
	static CBlood_Splatter* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void					Free() override;
};

NS_END