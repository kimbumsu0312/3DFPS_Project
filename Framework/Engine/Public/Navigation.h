#pragma once
#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	typedef struct tagNavigationDesc
	{
		_int		iCurrentCellIndex = { -1 };
	}NAVIGATION_DESC;
private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& Prototype);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize_Prototype(const string& pNavigationFilePath);
	virtual HRESULT					Initialize(void* pArg);
	virtual void					Update(_fmatrix WorldMatrix);

public:
	_bool							isMove(_fvector vPosition);
	_vector							Compute_OnCell(_fvector vPosition);

#ifdef _DEBUG
public:
	HRESULT							Add_Cell(const _float3* pPos);
	HRESULT							Render();
	HRESULT							Save_Cell(string szFilename);
	_bool							IsSnap(_float3& vPos, _float Radius );
#endif

private:
	_int							m_iCurrentCellIndex = { -1 };
	vector<class CCell*>			m_Cells;

	static _float4x4				m_WorldMatrix;

#ifdef _DEBUG
private:
	class CShader*					m_pShader = { nullptr };
	vector<SAVE_CELLDATA>			m_pCellPos;
#endif

private:
	void							SetUp_Neighbors();

public:
	static CNavigation*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const string& pNavigationFilePath);
	static CNavigation*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	virtual CComponent*				Clone(void* pArg) override;
	virtual void					Free() override;
};

NS_END