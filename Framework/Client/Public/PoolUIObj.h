#pragma once
#include "PoolingObject.h"
#include "Client_Defines.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
NS_END

NS_BEGIN(Client)
class CPoolUIObj abstract : public CPoolingObject
{
public:
	typedef struct tagUIObjectDesc
	{
		_float2 vPos{}, vSize{};
		_float2 vMinUV{}, vMaxUV{};
	}UIPOOLOBJECT_DESC;

protected:
	CPoolUIObj(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPoolUIObj(const CPoolUIObj& Prototype);
	virtual ~CPoolUIObj() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg = nullptr);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

public:
	virtual HRESULT			Initialize_Pool(void* pArg);
	virtual void			Return_Pool();

protected:
	vector<CPoolUIObj*>		m_vecChildren;
	CShader*				m_pShaderCom = { nullptr };
	CTexture*				m_pTextureCom = { nullptr };

	_float4x4				m_ViewMatrix = {};
	_float4x4				m_ProjMatrix = {};

	_float2					m_vLocalPos = {};
	_float2					m_vPos = {};

	_float2					m_vLocalSize = {};
	_float2					m_vSize = {};

	_float2					m_vMinUV = { 0.f,0.f }, m_vMaxUV = { 1.f,1.f };
protected:
	HRESULT					Bind_ShaderTransform_Resourc(_uint iPassIndex = 0);

	void					Update_Position(CPoolUIObj* pParent = nullptr);
	void					Update_Position_Children(CPoolUIObj* pParent);
	void					Add_Child(CPoolUIObj* pParent, CPoolUIObj* pChild);
	void					Add_Child(CPoolUIObj* pParent, CPoolUIObj* pChild, CShader* pShader);
	void					Add_Child(CPoolUIObj* pParent, CPoolUIObj* pChild, CShader* pShader, CTexture* pTexture);

	void					Update_Shader(CShader* pShader);
	void					Update_Texture(CTexture* pTexture);
	void					Correct_Position(_float2 pPos);

public:
	virtual CGameObject*	Clone(void* pArg) = 0;
	virtual void			Free() override;
};
NS_END
