#pragma once
#include "ContainerObject.h"

NS_BEGIN(Engine)
class CNavigation;
NS_END

NS_BEGIN(Edit)
class CNeviMesh : public CContainerObject
{
private:
	CNeviMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNeviMesh(const CNeviMesh& Prototype);
	virtual ~CNeviMesh() = default;
public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

public:
	_float3					Get_SeletePoint(_int i) { return m_fCellPoint[i]; }
	_int					Get_SeleteNum() { return m_iSeletePoint; }
	void					Set_Objcets();

	void					Save_Nevi(string szFileName);
private:
	CNavigation*			m_pNevigationCom = { nullptr };
	vector<CGameObject*>*	m_fObjects = { nullptr };
	_float3					m_fCellPoint[3] = {};
	_int					m_iSeletePoint = {};
private:
	HRESULT					Ready_Components();
	HRESULT					Ready_PartObjects();
public:
	static CNeviMesh*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg);
	virtual void			Free();
};

NS_END