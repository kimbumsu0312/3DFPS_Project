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
	_bool							isMove(_fvector vCulPosition);
	_bool							isOutNormal(_fvector vPosition, _float3& pOut);
	_vector							Compute_OnCell(_fvector vPosition);
	void							Set_CellIndex(_int iCellIndex) { m_iCurrentCellIndex = iCellIndex; }
	_int							Get_CulIndex() { return m_iCurrentCellIndex; }

	void							SetUp_Node(_int TargetIndex, _float3 vLastPos);
	_bool							IsNaviNode(_vector vPos, _float3 vNextPos);

	//void							Search_MovePos(_int TargetIndex);
#ifdef _DEBUG
public:
	HRESULT							Add_Cell(const _float3* pPos, _uint iCellType);
	HRESULT							Render();
	HRESULT							Save_Cell(string szFilename);
	_bool							IsSnap(_float3& vPos, _float Radius );
	_uint							Count_Cell() { return m_pCellPos.size(); }
	void							Chage_Color(_float4 vColor);
	_int							Selete_CellIndex(CTransform& pTransformCom);
	_bool							Erease_Cell(CTransform& pTransformCom);
	void							Erase_LastCell();
#endif

private:
	static _float4x4				m_WorldMatrix;
	_int							m_iCurrentCellIndex = { -1 };
	vector<class CCell*>			m_Cells;

	vector<Node>					m_CompleteList;
	vector<_int>					m_NodePath;
	vector<_float3>					m_NaviPos;
	//vector<_float3>					m_Portal;

	_int							m_iNaviMoveIndex = {};
#ifdef _DEBUG
private:
	class CShader*					m_pShader = { nullptr };
	vector<SAVE_CELLDATA>			m_pCellPos;
	_float4							m_vColor = {};
#endif

private:
	void							SetUp_Neighbors();
	_bool							FindNodeInClosed(_int cellIndex, Node& pOut);
	//_bool							SetUp_Portal(vector<Portal> &PortalPath);

public:
	static CNavigation*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const string& pNavigationFilePath);
	static CNavigation*				Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	virtual CComponent*				Clone(void* pArg) override;
	virtual void					Free() override;
};

NS_END