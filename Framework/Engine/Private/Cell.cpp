#include "pch.h"
#include "Cell.h"
#include "VIBuffer_Cell.h"

CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : m_pDevice {pDevice}, m_pContext {pContext}, m_pGameInstance{ CGameInstance::GetInstance()}
{
	Safe_AddRef(pDevice);
	Safe_AddRef(pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CCell::Initialize(const _float3* pPoints, _int iIndex, _uint iCellType)
{
	m_iIndex = iIndex;

	memcpy(m_vPoints, pPoints, sizeof(_float3) * ENUM_CLASS(CELL_POINT::END));
	
	_vector vLine = {};
	vLine = XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::B)]) - XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::A)]);
	m_vNormals[ENUM_CLASS(CELL_LINE::AB)] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

	vLine = XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::C)]) - XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::B)]);
	m_vNormals[ENUM_CLASS(CELL_LINE::BC)] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

	vLine = XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::A)]) - XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::C)]);
	m_vNormals[ENUM_CLASS(CELL_LINE::CA)] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

#ifdef _DEBUG
	m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, pPoints);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;
#endif

	return S_OK;
}

HRESULT CCell::Initialize_Load(const _float3* pPoints, _int iIndex, _uint iCellType)
{
	m_iIndex = iIndex;
	m_iCellType = iCellType;
	memcpy(m_vPoints, pPoints, sizeof(_float3) * ENUM_CLASS(CELL_POINT::END));

	_vector vLine = {};
	vLine = XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::B)]) - XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::A)]);
	m_vNormals[ENUM_CLASS(CELL_LINE::AB)] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

	vLine = XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::C)]) - XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::B)]);
	m_vNormals[ENUM_CLASS(CELL_LINE::BC)] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

	vLine = XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::A)]) - XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::C)]);
	m_vNormals[ENUM_CLASS(CELL_LINE::CA)] = _float3(XMVectorGetZ(vLine) * -1.f, 0.f, XMVectorGetX(vLine));

#ifdef _DEBUG
	m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, pPoints);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;
#endif
	Center_Update();
	return S_OK;
}

_bool CCell::isIn(_fvector vPosition, _int* pNeighborIndex)
{
	for (_uint i = 0; i < ENUM_CLASS(CELL_LINE::END); ++i)
	{
		//각 점 별로 플레이어의 방향과 노말을 내적한다.
		_vector vDir = XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPoints[i]));
		_vector vNormal = XMVector3Normalize(XMLoadFloat3(&m_vNormals[i]));

		if (0 < XMVectorGetX(XMVector3Dot(vDir, vNormal)))
		{
			//내적해서 양의 수인 경우 다음 셀 인덱스를 넘겨준다.
			//모두 음의 수인 경우 셀 내의 있는 것으로 판단 
			*pNeighborIndex = m_iNeighborIndices[i];
			return false;
		}

	}

	return true;
}

_bool CCell::isIn(_fvector vPosition, _float3& pOut)
{
	for (_uint i = 0; i < ENUM_CLASS(CELL_LINE::END); ++i)
	{

		_vector vDir = XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPoints[i]));
		_vector vNormal = XMVector3Normalize(XMLoadFloat3(&m_vNormals[i]));

		if (0 < XMVectorGetX(XMVector3Dot(vDir, vNormal)))
		{
			XMStoreFloat3(&pOut, vNormal);
			return false;
		}

	}
	return true;
}


_bool CCell::Compare_Points(_fvector vSourPoint, _fvector vDestPoint)
{
	// 같은 라인인지 판단한다.
	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::A)]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::B)]), vDestPoint))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::C)]), vDestPoint))
			return true;
	}
	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::B)]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::C)]), vDestPoint))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::A)]), vDestPoint))
			return true;
	}

	if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::C)]), vSourPoint))
	{
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::A)]), vDestPoint))
			return true;
		if (true == XMVector3Equal(XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::B)]), vDestPoint))
			return true;
	}

	return false;
}

_float CCell::Compute_Height(_fvector vLocalPos)
{
	_vector		vPlane = XMPlaneFromPoints(
		XMVectorSetW(XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::A)]), 1.f),
		XMVectorSetW(XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::B)]), 1.f),
		XMVectorSetW(XMLoadFloat3(&m_vPoints[ENUM_CLASS(CELL_POINT::C)]), 1.f)
	);

	return (XMVectorGetX(vPlane) * -1.f * XMVectorGetX(vLocalPos) - vPlane.m128_f32[2] * vLocalPos.m128_f32[2] - vPlane.m128_f32[3]) / vPlane.m128_f32[1];

}

_float CCell::Distance(_vector vCenterPos)
{
	return XMVectorGetX(XMVector3Length(XMLoadFloat3(&m_vCenter) - vCenterPos));
}

#ifdef _DEBUG
HRESULT CCell::Render()
{
	m_pVIBuffer->Bind_Resources();
	m_pVIBuffer->Render();

	return S_OK;
}
_bool CCell::IsSnap(_float3& vPos, _float Radius)
{
	for (_int i = 0; i < 3; ++i)
	{
		if (m_vPoints[i].x + Radius < vPos.x || m_vPoints[i].x - Radius > vPos.x)
			continue;
		
		if (m_vPoints[i].z + Radius < vPos.z || m_vPoints[i].z - Radius > vPos.z)
			continue;

		if (m_vPoints[i].y + Radius < vPos.y || m_vPoints[i].y - Radius > vPos.y)
			continue;
		
		vPos = m_vPoints[i];
		return true;
	}
	return false;
}
_bool CCell::IsPick(_float& fDist, _int& iIndex)
{
	_float fDis = {};
	if (m_pGameInstance->isPickedInLocalSpace(m_vPoints[ENUM_CLASS(CELL_POINT::A)], m_vPoints[ENUM_CLASS(CELL_POINT::B)], m_vPoints[ENUM_CLASS(CELL_POINT::C)], fDis))
	{
		if (fDis < fDist)
		{
			fDist = fDis;
			iIndex = m_iIndex;
		}
		return true;
	}
	return false;
}
#endif

void CCell::Center_Update()
{
	m_vCenter.x = (m_vPoints[ENUM_CLASS(CELL_POINT::A)].x + m_vPoints[ENUM_CLASS(CELL_POINT::B)].x + m_vPoints[ENUM_CLASS(CELL_POINT::C)].x) / 3;
	m_vCenter.y = (m_vPoints[ENUM_CLASS(CELL_POINT::A)].y + m_vPoints[ENUM_CLASS(CELL_POINT::B)].y + m_vPoints[ENUM_CLASS(CELL_POINT::C)].y) / 3;
	m_vCenter.z = (m_vPoints[ENUM_CLASS(CELL_POINT::A)].z + m_vPoints[ENUM_CLASS(CELL_POINT::B)].z + m_vPoints[ENUM_CLASS(CELL_POINT::C)].z) / 3;
}

CCell* CCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex, _uint iCellType, _bool IsLoad)
{
	CCell* pInstance = new CCell(pDevice, pContext);

	if (IsLoad)
	{
		if (FAILED(pInstance->Initialize_Load(pPoints, iIndex, iCellType)))
		{
			MSG_BOX(TEXT("Failed to Created : CCell"));
			Safe_Release(pInstance);
		}
	}
	else 
	{
		if (FAILED(pInstance->Initialize(pPoints, iIndex, iCellType)))
		{
			MSG_BOX(TEXT("Failed to Created : CCell"));
			Safe_Release(pInstance);
		}
	}
	return pInstance;
}

void CCell::Free()
{
	__super::Free();

#ifdef _DEBUG
	Safe_Release(m_pVIBuffer);
#endif
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
