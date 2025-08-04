#include "pch.h"
#include "Navigation.h"
#include "Cell.h"

#include "GameInstance.h"

_float4x4 CNavigation::m_WorldMatrix = {};


CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CComponent { pDevice, pContext }
{
}

CNavigation::CNavigation(const CNavigation& Prototype) : CComponent ( Prototype ), m_Cells{ Prototype.m_Cells },
#ifdef _DEBUG
m_pShader{ Prototype.m_pShader }
#endif
{
	for (auto& pCell : m_Cells)
		Safe_AddRef(pCell);

#ifdef _DEBUG	
	Safe_AddRef(m_pShader);
#endif

}

HRESULT CNavigation::Initialize_Prototype(const string& pNavigationFilePath)
{
	ifstream file(pNavigationFilePath, ios::binary);
	if (!file.is_open())
	{
		MSG_BOX(TEXT("네비게이션 로드 실패"));
		return E_FAIL;
	}

	vector<VTXPOS> Sells;
	_uint iSize = 0;
	file.read(reinterpret_cast<char*>(&iSize), sizeof(_uint));
	Sells.resize(iSize);

	file.read(reinterpret_cast<char*>(Sells.data()), sizeof(VTXPOS) * iSize);

	file.close();
	
	for (_int i = 0; i < iSize; ++i)
	{
		CCell* pCell = CCell::Create(m_pDevice, m_pContext, &Sells[i].vPosition, m_Cells.size());
		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.push_back(pCell);
	}

	SetUp_Neighbors();

#ifdef _DEBUG
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Engine_Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;
#endif

	return S_OK;
}

HRESULT CNavigation::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return S_OK;

	NAVIGATION_DESC* pDesc = static_cast<NAVIGATION_DESC*>(pArg);

	// 클론 생성 시, 현재 객체의 셀 인덱스를 가져온다.
	m_iCurrentCellIndex = pDesc->iCurrentCellIndex;

	return S_OK;
}

void CNavigation::Update(_fmatrix WorldMatrix)
{
	XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);
}

_bool CNavigation::isMove(_fvector vPosition)
{
	//현재 월드 매트릭스를 객체의 로컬로 변경
	_vector vLocalPos = XMVector3TransformCoord(vPosition, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));

	_int iNeighborIndex = { -1 };

	//로컬 셀의 위치 내의 객체가 있으면 트루 
	if (true == m_Cells[m_iCurrentCellIndex]->isIn(vLocalPos, &iNeighborIndex))
		return true;
	else
	{
		//이웃이 없으면 못 넘어가게 한다.
		if (-1 != iNeighborIndex)
		{
			while (true)
			{
				if (-1 == iNeighborIndex)
					return false;

				//인접한 셀에 객체가 있는지 판단 후, 없으면 다음 인접셀로 넘김
				//있는 경우 셀 인덱스를 반환
				if (true == m_Cells[iNeighborIndex]->isIn(vLocalPos, &iNeighborIndex))
					break;
			}

			m_iCurrentCellIndex = iNeighborIndex;
			return true;
		}
		else
			return false;
	}
}

_vector CNavigation::Compute_OnCell(_fvector vPosition)
{
	//객체를 셀의 로컬 위치로 변경
	_vector vLocalPos = XMVector3TransformCoord(vPosition, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));

	//현재 셀의 높이를 가져온다.
	_float		fHeight = m_Cells[m_iCurrentCellIndex]->Compute_Height(vLocalPos);

	//객체의 Y값을 변경해준다.
	vLocalPos = XMVectorSetY(vLocalPos, fHeight);

	//월드 값으로 변환해서 반환
	return XMVector3TransformCoord(vLocalPos, XMLoadFloat4x4(&m_WorldMatrix));

}

HRESULT CNavigation::Add_Cell(const _float3* pPos)
{

	CCell* pCell = CCell::Create(m_pDevice, m_pContext, pPos, m_Cells.size());

	if (pCell == nullptr)
	{
		MSG_BOX(TEXT("Failed to Add Cell"));
		Safe_Release(pCell);
	}
	m_Cells.push_back(pCell);
	_float3 vPos = *pPos;
	m_CellPos.push_back(vPos);
	return S_OK;
}

#ifdef _DEBUG
HRESULT CNavigation::Render()
{
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	_float4		vColor = {};

	_float4x4	WorldMatrix = m_WorldMatrix;

	if (-1 == m_iCurrentCellIndex)
	{
		vColor = _float4(0.f, 1.f, 0.f, 1.f);

		if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &WorldMatrix)))
			return E_FAIL;

		m_pShader->Bind_RawValue("g_vColor", &vColor, sizeof(_float4));

		m_pShader->Begin(0);

		for (auto& pCell : m_Cells)
			pCell->Render();
	}

	else
	{
		vColor = _float4(1.f, 0.f, 0.f, 1.f);

		WorldMatrix._42 += 0.1f;

		if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &WorldMatrix)))
			return E_FAIL;

		m_pShader->Bind_RawValue("g_vColor", &vColor, sizeof(_float4));

		m_pShader->Begin(0);

		m_Cells[m_iCurrentCellIndex]->Render();
	}



	return S_OK;
}
HRESULT CNavigation::Save_Cell(string szFilename)
{
	string FilePath = "../Bin/Data/NavigationData/" + szFilename + ".dat";

	ofstream out(FilePath, ios::binary);

	if (!out)
	{
		MSG_BOX(TEXT("Save 실패"));
		return E_FAIL;
	}

	_uint	iSize = m_CellPos.size();
	out.write(reinterpret_cast<const _char*>(&iSize), sizeof(_uint));
	out.write(reinterpret_cast<const _char*>(m_CellPos.data()), sizeof(VTXPOS) * iSize);

	out.close();
	MSG_BOX(TEXT("Save"));

	return S_OK;
}
#endif

void CNavigation::SetUp_Neighbors()
{
	//현재 셀의 이웃을 셋팅해준다.
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CELL_POINT::A), pSourCell->Get_Point(CELL_POINT::B)))
			{
				pSourCell->Set_Neighbor(CELL_LINE::AB, pDestCell);
			}
			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CELL_POINT::B), pSourCell->Get_Point(CELL_POINT::C)))
			{
				pSourCell->Set_Neighbor(CELL_LINE::BC, pDestCell);
			}
			if (true == pDestCell->Compare_Points(pSourCell->Get_Point(CELL_POINT::C), pSourCell->Get_Point(CELL_POINT::A)))
			{
				pSourCell->Set_Neighbor(CELL_LINE::CA, pDestCell);
			}
		}
	}
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const string& pNavigationFilePath)
{
	CNavigation* pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pNavigationFilePath)))
	{
		MSG_BOX(TEXT("Failed to Created : CNavigation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CNavigation::Clone(void* pArg)
{
	CNavigation* pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CNavigation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);

	m_Cells.clear();

#ifdef _DEBUG	
	Safe_Release(m_pShader);
#endif
}
