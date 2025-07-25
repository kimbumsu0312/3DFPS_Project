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
		_float4 vPos;
		_wstring szModelPath;
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

	_wstring				Get_ModelPath() { return m_szModelPath; }
	void					SetDead() { m_bIsDead = true; }
	void					SetSelete(_bool bSelete) { m_bIsSelete = bSelete; }
	virtual	void*			GetDesc();
private:
	class CEdit_Model*		m_pModelCom = { nullptr };
	CShader*				m_pShaderCom = { nullptr };

	_wstring				m_szModelPath = {};
	_bool					m_bIsSelete = { false };
private:
	HRESULT					Ready_Components();

	HRESULT					Bind_ShaderResources();
public:
	static CMapObject*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free();
};

NS_END