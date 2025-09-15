#pragma once
#include "PartObject.h"
#include "Bela.h"
#include "BlackBoard.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CAnimatio_Controller;
class CTexture;
NS_END

NS_BEGIN(Client)
class CBody_Bela final : public CPartObject
{
public:
	typedef struct tagBodyNormalDesc : public CPartObject::PARTOBJECT_DESC {
		CBlackBoard<CBela::BELA_DATA>* pBlackBoard = { nullptr };
	}BODY_DESC;

private:
	CBody_Bela(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBody_Bela(const CBody_Bela& Prototype);
	virtual ~CBody_Bela() = default;

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
private:
	CTexture*				m_pNoiesTexCom = { nullptr };
	CShader*				m_pShaderCom = { nullptr };
	CModel*					m_pModelCom = { nullptr };
	CAnimatio_Controller*	m_pAnimCom = { nullptr };

	CBlackBoard<CBela
		::BELA_DATA>*		m_BlackBoard = { nullptr };
	_uint					m_iRootLodeIndex = {};
private:
	HRESULT					Ready_Components();
	HRESULT					Bind_ShaderResources();

public:
	static CBody_Bela*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

NS_END