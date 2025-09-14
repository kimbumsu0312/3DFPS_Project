#pragma once
#include "PartObject.h"
#include "BlackBoard.h"
#include "Muzzle_Effect.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Point_Instance;
NS_END

NS_BEGIN(Client)

class CMuzzle_Smoke final : public CPartObject
{
public:
	typedef struct Muzzle_Smoke : public CPartObject::PARTOBJECT_DESC {
		CBlackBoard<CMuzzle_Effect::MUZZLE_EFFECT_DATA>* m_BlackBoard = { nullptr };
	}MUZZLE_SMOKE_DATA;
private:
	CMuzzle_Smoke(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMuzzle_Smoke(const CMuzzle_Smoke& Prototype);
	virtual ~CMuzzle_Smoke() = default;

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
	CVIBuffer_Point_Instance*		m_pVIBufferCom = { nullptr };

	_int							m_iTexSizeX = {};
	_int							m_iTexSizeY = {};

	CBlackBoard<CMuzzle_Effect::MUZZLE_EFFECT_DATA>* m_BlackBoard = { nullptr };

	_float							m_fCount = {};

private:
	HRESULT							Ready_Components();
	HRESULT							Bind_ShaderResources();

public:
	static CMuzzle_Smoke*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};

NS_END