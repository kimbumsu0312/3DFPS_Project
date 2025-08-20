#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CCell final : public CBase
{
private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCell() = default;

public:
	_vector					Get_Point(CELL_POINT ePoint) { return XMLoadFloat3(&m_vPoints[ENUM_CLASS(ePoint)]);	}
	void					Set_Neighbor(CELL_LINE eLine, CCell* pNeighborCell) { m_iNeighborIndices[ENUM_CLASS(eLine)] = pNeighborCell->m_iIndex; }

public:
	HRESULT					Initialize(const _float3* pPoints, _int iIndex, _uint iCellType);
	HRESULT					Initialize_Load(const _float3* pPoints, _int iIndex, _uint iCellType);
	_bool					isIn(_fvector vPosition, _int* pNeighborIndex);
	_bool					isSlide(_fvector vCulPosition, _fvector vPrePosition, _float3& pOut);

	_bool					Compare_Points(_fvector vSourPoint, _fvector vDestPoint);
	_float					Compute_Height(_fvector vLocalPos);

	
#ifdef _DEBUG
public:
	HRESULT					Render();
	_bool					IsSnap(_float3& vPos, _float Radius);
	_bool					IsPick(_float& fDist, _int& iIndex);
#endif

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	CGameInstance*			m_pGameInstance = { nullptr };
	_float3					m_vPoints[ENUM_CLASS(CELL_POINT::END)] = {};
	_float3					m_vNormals[ENUM_CLASS(CELL_LINE::END)] = {};
	_int					m_iIndex = {};
	_int					m_iNeighborIndices[ENUM_CLASS(CELL_LINE::END)] = { -1, -1, -1 };

	_uint					m_iCellType = {};
#ifdef _DEBUG
private:
	class CVIBuffer_Cell*	m_pVIBuffer = { nullptr };

#endif

public:
	static CCell*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex, _uint iCellType, _bool IsLoad);
	virtual void			Free() override;
};

NS_END