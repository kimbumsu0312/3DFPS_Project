#pragma once
#include "PoolingContainer.h"
#include "BlackBoard.h"

NS_BEGIN(Client)
class CSpark_Effect final : public CPoolingContainer
{
public:
	typedef struct Spark_Effect {
		_float fEffectTime = {};
		_bool  isEndEffect1 = false;
	}SPARK_EFFECT_DATA;

	typedef struct SPARK_Effect_Init {
		_vector vPos = {};
	}SPARK_EFFECT_INIT;

private:
	CSpark_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSpark_Effect(const CSpark_Effect& Prototype);
	virtual ~CSpark_Effect() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

public:
	virtual HRESULT				Initialize_Pool(void* pArg) override;
	virtual void				Return_Pool() override;

private:
	CBlackBoard<SPARK_EFFECT_DATA>* m_BlackBoard = { nullptr };

private:
	HRESULT						Ready_PartObjects();
	HRESULT						Ready_BlackBoard();

public:
	static CSpark_Effect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void				Free();
};

NS_END