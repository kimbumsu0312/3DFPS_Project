#include "pch.h"
#include "VIBuffer_Terrain.h"
#include "Transform.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& Prototype) : CVIBuffer(Prototype), m_iNumverticesX (Prototype.m_iNumverticesX), m_iNumverticesZ(Prototype.m_iNumverticesZ)
{

}

HRESULT CVIBuffer_Terrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const _tchar* pHeightMapFilePath)
{
	_ulong dwByte = {};
	HANDLE hFile = CreateFile(pHeightMapFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	//읽는 순서 중요
	BITMAPFILEHEADER fh{};
	BITMAPINFOHEADER ih{};

	//앞에 이미지 파일들의 기본 정보를 읽어옴
	//14바이트를 읽어옴 : 비트맵 기본정보 (bfType, bfSize, bfofffBits)
	if (!ReadFile(hFile, &fh, sizeof fh, &dwByte, nullptr))
		return E_FAIL;
	//40바이트를 읽어옴 -> 14바이트 이후 정보를 읽어옴 : 해상도, 픽셀 수, 색상 깊이 등
	if (!ReadFile(hFile, &ih, sizeof ih, &dwByte, nullptr))
		return E_FAIL;

	m_iNumverticesX = ih.biWidth;
	m_iNumverticesZ = ih.biHeight;
	m_iNumVertices = m_iNumverticesX * m_iNumverticesZ;

	_uint* pPixels = new _uint[m_iNumVertices];

	if (!ReadFile(hFile, pPixels, sizeof(_uint) * m_iNumVertices, &dwByte, nullptr))
		return E_FAIL;

	CloseHandle(hFile);

	m_iVertexStride = sizeof(VTXNORTEX);
	m_iNumIndices = (m_iNumverticesX - 1) * (m_iNumverticesZ - 1) * 2 * 3;
	m_iIndexStride = 4;
	m_iNumVertexBuffers = 1;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_pIndices = new _uint[m_iNumIndices];
	ZeroMemory(m_pIndices, sizeof(_uint) * m_iNumIndices);
	m_pVertexPositions = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);
	m_pVertexTex = new _float2[m_iNumVertices];
	ZeroMemory(m_pVertexTex, sizeof(_float2) * m_iNumVertices);

	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];

	for (_uint i = 0; i < m_iNumverticesZ; i++)
	{
		for (_uint j = 0; j < m_iNumverticesX; j++)
		{
			_uint	iIndex = i * m_iNumverticesX + j;

			pVertices[iIndex].vPosition = m_pVertexPositions[iIndex] = _float3((float)j, (pPixels[iIndex] & 0x000000ff) / 10.0f, (float)i);
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexcoord = m_pVertexTex[iIndex] = _float2((float)j / (m_iNumverticesX - 1.f), i / (m_iNumverticesZ - 1.f));
		}
	}

	_uint* pIndices = new _uint[m_iNumIndices];
	_uint iNumIndices = {};

	for (_uint i = 0; i < m_iNumverticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumverticesX - 1; ++j)
		{
			_uint iIndex = i * m_iNumverticesX + j;
			_uint iIndices[] = {
				iIndex + m_iNumverticesX,
				iIndex + m_iNumverticesX + 1,
				iIndex + 1,
				iIndex
			};

			_vector	vSour, vDest, vNormal;

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			vSour = XMLoadFloat3(&pVertices[iIndices[1]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
			vDest = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[1]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[1]].vNormal, XMLoadFloat3(&pVertices[iIndices[1]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];

			vSour = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
			vDest = XMLoadFloat3(&pVertices[iIndices[3]].vPosition) - XMLoadFloat3(&pVertices[iIndices[2]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[3]].vNormal, XMLoadFloat3(&pVertices[iIndices[3]].vNormal) + vNormal);
		}
	}
	memcpy(m_pIndices, pIndices, sizeof(_uint) * m_iNumIndices);
	//pVertices에 담기 vNormal 값을 모두 노말라이즈 시킨다.
	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));
	}

	D3D11_BUFFER_DESC		VBDesc{};
	VBDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	VBDesc.Usage = D3D11_USAGE_DYNAMIC;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VBDesc.MiscFlags = 0;
	VBDesc.StructureByteStride = m_iVertexStride;

	D3D11_SUBRESOURCE_DATA	VBInitialData{};
	VBInitialData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &VBInitialData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	D3D11_BUFFER_DESC		IBDesc{};
	IBDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
	IBDesc.Usage = D3D11_USAGE_DEFAULT;
	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBDesc.CPUAccessFlags = 0;
	IBDesc.MiscFlags = 0;
	IBDesc.StructureByteStride = m_iIndexStride;

	D3D11_SUBRESOURCE_DATA	IBInitialData{};
	IBInitialData.pSysMem = pIndices;

	if (FAILED(m_pDevice->CreateBuffer(&IBDesc, &IBInitialData, &m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pPixels);

	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize(void* pArg)
{
	VIBUFFER_TERRAIN_DESC* pDesc = static_cast<VIBUFFER_TERRAIN_DESC*>(pArg);

	_ulong dwByte = {};

	if (pDesc->Terrain_Data == nullptr)
	{
		m_iNumverticesX = pDesc->iNumverticesX;
		m_iNumverticesZ = pDesc->iNumverticesZ;
	}
	else
	{
		m_iNumverticesX = pDesc->Terrain_Data->iNumverticesX;
		m_iNumverticesZ = pDesc->Terrain_Data->iNumverticesZ;
	}

	m_iNumVertices = m_iNumverticesX * m_iNumverticesZ;

	m_iVertexStride = sizeof(VTXNORTEX);
	m_iNumIndices = (m_iNumverticesX - 1) * (m_iNumverticesZ - 1) * 2 * 3;
	
	m_iIndexStride = 4;
	m_iNumVertexBuffers = 1;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];
	m_pIndices = new _uint[m_iNumIndices];
	ZeroMemory(m_pIndices, sizeof(_uint) * m_iNumIndices);
	m_pVertexPositions = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);
	m_pVertexTex = new _float2[m_iNumVertices];
	ZeroMemory(m_pVertexTex, sizeof(_float2) * m_iNumVertices);

	for (_uint i = 0; i < m_iNumverticesZ; i++)
	{
		for (_uint j = 0; j < m_iNumverticesX; j++)
		{
			_uint	iIndex = i * m_iNumverticesX + j;
			if (pDesc->Terrain_Data == nullptr)
				pVertices[iIndex].vPosition = m_pVertexPositions[iIndex] = _float3((float)j, 0.f, (float)i);
			else
				pVertices[iIndex].vPosition = m_pVertexPositions[iIndex] = _float3((float)j, pDesc->Terrain_Data->pVertexData[iIndex].y, (float)i);
	
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexcoord = m_pVertexTex[iIndex] = _float2((float)j / (m_iNumverticesX - 1.f), i / (m_iNumverticesZ - 1.f));
		}
	}

	_uint* pIndices = new _uint[m_iNumIndices];
	_uint iNumIndices = {};

	for (_uint i = 0; i < m_iNumverticesZ - 1; ++i)
	{
		for (_uint j = 0; j < m_iNumverticesX - 1; ++j)
		{
			_uint iIndex = i * m_iNumverticesX + j;
			_uint iIndices[] = {
				iIndex + m_iNumverticesX,
				iIndex + m_iNumverticesX + 1,
				iIndex + 1,
				iIndex
			};

			_vector	vSour, vDest, vNormal;

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[1];
			pIndices[iNumIndices++] = iIndices[2];

			vSour = XMLoadFloat3(&pVertices[iIndices[1]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
			vDest = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[1]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[1]].vNormal, XMLoadFloat3(&pVertices[iIndices[1]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];

			vSour = XMLoadFloat3(&pVertices[iIndices[2]].vPosition) - XMLoadFloat3(&pVertices[iIndices[0]].vPosition);
			vDest = XMLoadFloat3(&pVertices[iIndices[3]].vPosition) - XMLoadFloat3(&pVertices[iIndices[2]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMLoadFloat3(&pVertices[iIndices[0]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMLoadFloat3(&pVertices[iIndices[2]].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[3]].vNormal, XMLoadFloat3(&pVertices[iIndices[3]].vNormal) + vNormal);
		}
	}
	memcpy(m_pIndices, pIndices, sizeof(_uint) * m_iNumIndices);
	//pVertices에 담기 vNormal 값을 모두 노말라이즈 시킨다.
	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));
	}

	D3D11_BUFFER_DESC		VBDesc{};
	VBDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	VBDesc.Usage = D3D11_USAGE_DYNAMIC;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	VBDesc.MiscFlags = 0;
	VBDesc.StructureByteStride = m_iVertexStride;

	D3D11_SUBRESOURCE_DATA	VBInitialData{};
	VBInitialData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &VBInitialData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	D3D11_BUFFER_DESC		IBDesc{};
	IBDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
	IBDesc.Usage = D3D11_USAGE_DEFAULT;
	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBDesc.CPUAccessFlags = 0;
	IBDesc.MiscFlags = 0;
	IBDesc.StructureByteStride = m_iIndexStride;

	D3D11_SUBRESOURCE_DATA	IBInitialData{};
	IBInitialData.pSysMem = pIndices;

	if (FAILED(m_pDevice->CreateBuffer(&IBDesc, &IBInitialData, &m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CVIBuffer_Terrain::Initialize()
{
	return S_OK;
}


void CVIBuffer_Terrain::Terrain_Hight(bool raise, _float brushRadius, _float intensity, class CTransform& pTransformCom, _float2 vMinMax)
{
	if (D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST != m_ePrimitiveType)
		return;

	_float3 fMousePos{};

	if (!(IsPicked(pTransformCom, fMousePos)))
		return;

	for (_int z = -brushRadius; z <= brushRadius; ++z) {
		for (_int x = -brushRadius; x <= brushRadius; ++x) {
			// 거리 계산 (XZ 평면)
			_float distance = sqrt(x * x + z * z);
			if (distance > brushRadius) continue;

			// 부드러운 감쇠 (cosine falloff)
			_float falloff = 0.5f * (1 + cos(XM_PI * distance / brushRadius));

			// 현재 정점의 월드 위치 계산
			_float3 worldPos = _float3(x, 0.f, z);
			worldPos.x = fMousePos.x + worldPos.x;
			worldPos.y = fMousePos.y + worldPos.y;
			worldPos.z = fMousePos.z + worldPos.z;

			// 터레인 그리드 인덱스 계산
			_int gridX = (_int)((worldPos.x) / 1.f);
			_int gridZ = (_int)((worldPos.z) / 1.f);

			// 경계 체크
			if (gridX < 0 || gridX >= m_iNumverticesX || gridZ < 0 || gridZ >= m_iNumverticesZ)
				continue;

			// 정점 인덱스 계산
			_uint idx = gridZ * m_iNumverticesX + gridX;

			// 높이 변경 (Y축만 조정)
			_float fHeight = (raise ? 1.0f : -1.0f) * intensity * falloff;
			if (m_pVertexPositions[idx].y + fHeight <= vMinMax.x )
			{
				m_pVertexPositions[idx].y = vMinMax.x;
			}
			else if (m_pVertexPositions[idx].y + fHeight >= vMinMax.y)
			{
				m_pVertexPositions[idx].y = vMinMax.y;
			}
			else
			{
				m_pVertexPositions[idx].y += fHeight;
			}
		}
	}

	UpdateTerrain();
}

void CVIBuffer_Terrain::Save_Terrain(SAVE_TERRAIN& pArg)
{
	pArg.pVertexData.clear();
	pArg.pTexcoordData.clear();

	pArg.iNumverticesX = m_iNumverticesX;
	pArg.iNumverticesZ = m_iNumverticesZ;

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		_float3 Pos = {};
		_float2 Tex = {};

		Pos = m_pVertexPositions[i];
		Tex = m_pVertexTex[i];
		pArg.pVertexData.push_back(Pos);
		pArg.pTexcoordData.push_back(Tex);
	}
}

void CVIBuffer_Terrain::UpdateTerrain()
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT hr = m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(hr))
		return;

	VTXNORTEX* pVertexData = reinterpret_cast<VTXNORTEX*>(mappedResource.pData);

	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		pVertexData[i].vPosition = m_pVertexPositions[i];
		pVertexData[i].vTexcoord = m_pVertexTex[i];
	}

	m_pContext->Unmap(m_pVB, 0);

}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pHeightMapFilePath)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pHeightMapFilePath)))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CVIBuffer_Terrain* CVIBuffer_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Terrain::Clone(void* pArg)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(*this);

	if (pArg == nullptr)
	{
		if (FAILED(pInstance->Initialize()))
		{
			MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_Terrain"));
			Safe_Release(pInstance);
		}
	}
	else
	{
		if (FAILED(pInstance->Initialize(pArg)))
		{
			MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_Terrain"));
			Safe_Release(pInstance);
		}
	}
	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();
}
