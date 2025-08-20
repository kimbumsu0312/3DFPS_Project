#pragma once
#include "PartObject.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CAnimatio_Controller;
NS_END

NS_BEGIN(Client)
class CBody_Bela final : public CPartObject
{
public:
	typedef struct tagBodyNormalDesc : public CPartObject::PARTOBJECT_DESC {
		_uint* pState = { nullptr };
		_uint* pAnimState = { nullptr };
		string* pAnimTag = { nullptr };
		_bool* pIsAnimLoop = { nullptr };
		_bool* pIsAnimFinsh = { nullptr };
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
	CShader*				m_pShaderCom = { nullptr };
	CModel*					m_pModelCom = { nullptr };
	CAnimatio_Controller*	m_pAnimCom = { nullptr };

	_uint*					m_pAnimState = { nullptr };
	string*					m_pAnimTag = { nullptr };
	_bool*					m_pIsAnimLoop = { nullptr };
	_bool*					m_pIsAnimFinsh = { nullptr };
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