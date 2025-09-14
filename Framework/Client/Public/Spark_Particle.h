#pragma once
#include "PartObject.h"
#include "BlackBoard.h"
#include "Spark_Effect.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Point_Instance;
NS_END

NS_BEGIN(Client)
class CSpark_Particle final : public CPartObject
{
public:
	typedef struct Spark_Particle : public CPartObject::PARTOBJECT_DESC {
		CBlackBoard<CSpark_Effect::SPARK_EFFECT_DATA>* m_BlackBoard = { nullptr };
	}SPARK_PARTICLE_DATA;
private:
	CSpark_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSpark_Particle(const CSpark_Particle& Prototype);
	virtual ~CSpark_Particle() = default;

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

	CBlackBoard<CSpark_Effect::SPARK_EFFECT_DATA>* m_BlackBoard = { nullptr };

	_float							m_fCount = {};

private:
	HRESULT							Ready_Components();
	HRESULT							Bind_ShaderResources();

public:
	static CSpark_Particle*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free() override;
};

NS_END