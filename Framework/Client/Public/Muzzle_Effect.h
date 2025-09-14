#pragma once
#include "PoolingContainer.h"
#include "BlackBoard.h"

NS_BEGIN(Client)
class CMuzzle_Effect final : public CPoolingContainer
{
public:
	typedef struct Muzzle_Effect {
		_float fEffectTime = {};
		_bool  isEndEffect1 = false;
		_bool  isEndEffect2 = false;
	}MUZZLE_EFFECT_DATA;
	
	typedef struct Muzzle_Effect_Init {
		_vector vPos = {};

	}MUZZLE_EFFECT_INIT;

private:
	CMuzzle_Effect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMuzzle_Effect(const CMuzzle_Effect& Prototype);
	virtual ~CMuzzle_Effect() = default;

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
	CBlackBoard<MUZZLE_EFFECT_DATA>* m_BlackBoard = { nullptr };

private:
	HRESULT						Ready_PartObjects();
	HRESULT						Ready_BlackBoard();

public:
	static CMuzzle_Effect*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg);
	virtual void				Free();
};

NS_END