#pragma once
#include "GameObject.h"

NS_BEGIN(Engine)
class CShader;
NS_END

NS_BEGIN(Edit)
class CMapObject final : public CGameObject
{
public:

	typedef struct tagObject : public CGameObject::GAMEOBJECT_DESC
	{
		_float4 vPos = {0.f, 0.f, 0.f, 1.f};
	}MODEL_OBJECT_DESC;

private:
	CMapObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMapObject(const CMapObject& Prototype);
	virtual ~CMapObject() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

	void					SetDead() { m_bIsDead = true; }
	void					SetSelete(_bool bSelete) { m_bIsSelete = bSelete; }
	
	_int&					Get_Index() {return m_iIndex;}
private:
	class CEdit_Model*		m_pModelCom = { nullptr };
	CShader*				m_pShaderCom = { nullptr };
	_wstring				m_szModelPath = {};
	_bool					m_bIsSelete = { false };

	_int					m_iIndex = {0};
private:
	HRESULT					Ready_Components();

	HRESULT					Bind_ShaderResources();
public:
	static CMapObject*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free();
};

NS_END