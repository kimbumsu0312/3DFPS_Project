#pragma once
#include "PartObject.h"
#include "BlackBoard.h"
#include "Monster_Normal.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CAnimatio_Controller;
class CTexture;
NS_END

NS_BEGIN(Client)
class CBody_NorMon final : public CPartObject
{
public:
	typedef struct tagBodyNormalDesc : public CPartObject::PARTOBJECT_DESC {
		CBlackBoard<CMonster_Normal::NORMALMON_DATA>* BlackBoard = { nullptr };
	}BODY_DESC;

private:
	CBody_NorMon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_NorMon(const CBody_NorMon& Prototype);
	virtual ~CBody_NorMon() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

public:

	_float4x4*				Get_BoneMatrix(const _wstring pBoneName);
	_float3*				Get_MovePos();
	_float4*				Get_MoveRot();

	HRESULT					Set_Animation(_int iAnimIndex, string szAnimName, _bool IsLoop);
private:
	CTexture*				m_pNoiesTexCom = {nullptr};
	CShader*				m_pShaderCom = { nullptr };
	CModel*					m_pModelCom = { nullptr };
	CAnimatio_Controller*	m_pAnimCom = { nullptr };

	_uint					m_iRootLodeIndex = {};

	CBlackBoard<CMonster_Normal
		::NORMALMON_DATA>*	m_BlackBoard = { nullptr };
private:
	HRESULT					Ready_Components();
	HRESULT					Bind_ShaderResources();

public:
	static CBody_NorMon*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;


};

NS_END