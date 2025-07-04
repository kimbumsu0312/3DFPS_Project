#pragma once
#include "GameObject.h"
#include "Client_Defines.h"

NS_BEGIN(Engine)
class CShader;
class CTexture;
NS_END

NS_BEGIN(Client)
class CUIObject abstract : public CGameObject
{
public:
	typedef struct tagUIObjectDesc : public CGameObject::GAMEOBJECT_DESC
	{
		_float fX{}, fY{}, fSizeX{}, fSizeY{};
	}UIOBJECT_DESC;

protected:
	CUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIObject(const CUIObject& Prototype);
	virtual ~CUIObject() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg = nullptr);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

protected:
	vector<CUIObject*>		m_vecChildren;

	_float2					m_vLocalPos = {};
	_float2					m_vPos = {};
	_float					m_fZOrder = {};

	_float2					m_vLocalSize = {};
	_float2					m_vSize = {};

	_float4x4				m_ViewMatrix = {};
	_float4x4				m_ProjMatrix = {};

	_float					m_iWinSizeX{}, m_iWinSizeY{};

protected:
	HRESULT					Bind_Shader_Resourec(CShader* pShader);
	HRESULT					Bind_Shader_Resourec(CShader* pShader, CTexture* pTexture);

	void					Update_Position(CUIObject* pParent = nullptr);
	void					Add_Child(CUIObject* pParent,CUIObject* pChild);

public:
	virtual CGameObject*	Clone(void* pArg) = 0;
	virtual void			Free() override;
};
NS_END
