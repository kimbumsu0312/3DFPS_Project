#include "pch.h"
#include "VIBuffer_Terrain.h"

CVIBuffer_Terrain::CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Terrain::CVIBuffer_Terrain(const CVIBuffer_Terrain& Prototype) : CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_Terrain::Initialize_Prototype(const _tchar* pHeightMapFilePath)
{
	_ulong dwByte = {};
	HANDLE hFile = CreateFile(pHeightMapFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	//�д� ���� �߿�
	BITMAPFILEHEADER fh{};
	BITMAPINFOHEADER ih{};

	//�տ� �̹��� ���ϵ��� �⺻ ������ �о��
	//14����Ʈ�� �о�� : ��Ʈ�� �⺻���� (bfType, bfSize, bfofffBits)
	if (!ReadFile(hFile, &fh, sizeof(fh), &dwByte, nullptr))
		return E_FAIL;
	//40����Ʈ�� �о�� -> 14����Ʈ ���� ������ �о�� : �ػ�, �ȼ� ��, ���� ���� ��
	if (!ReadFile(hFile, &ih, sizeof(ih), &dwByte, nullptr))
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

	VTXNORTEX* pVertices = new VTXNORTEX[m_iNumVertices];

	for (size_t i = 0; i < m_iNumverticesZ; i++)
	{
		for (size_t j = 0; j < m_iNumverticesX; j++)
		{
			_uint	iIndex = i * m_iNumverticesX + j;

			pVertices[iIndex].vPosition = _float3((_float)j, (pPixels[iIndex] & 0x000000ff) / 15.0f, (_float)i);
			pVertices[iIndex].vNormal = _float3(0.f, 0.f, 0.f);
			pVertices[iIndex].vTexcoord = _float2(j / (m_iNumverticesX - 1.f), (_float)i / (m_iNumverticesZ - 1.f));
		}
	}

	_uint* pIndices = new _uint[m_iNumIndices];
	_uint iNumIndices = {};

	for (size_t i = 0; i < m_iNumverticesZ - 1; ++i)
	{
		for (size_t j = 0; j < m_iNumverticesX - 1; ++j)
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

			vSour = XMLoadFloat3(&pVertices[pIndices[1]].vPosition) - XMLoadFloat3(&pVertices[pIndices[0]].vPosition);
			vDest = XMLoadFloat3(&pVertices[pIndices[2]].vPosition) - XMLoadFloat3(&pVertices[pIndices[1]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMLoadFloat3(&pVertices[0].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[1]].vNormal, XMLoadFloat3(&pVertices[1].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMLoadFloat3(&pVertices[2].vNormal) + vNormal);

			pIndices[iNumIndices++] = iIndices[0];
			pIndices[iNumIndices++] = iIndices[2];
			pIndices[iNumIndices++] = iIndices[3];

			vSour = XMLoadFloat3(&pVertices[pIndices[2]].vPosition) - XMLoadFloat3(&pVertices[pIndices[0]].vPosition);
			vDest = XMLoadFloat3(&pVertices[pIndices[3]].vPosition) - XMLoadFloat3(&pVertices[pIndices[2]].vPosition);
			vNormal = XMVector3Normalize(XMVector3Cross(vSour, vDest));

			XMStoreFloat3(&pVertices[iIndices[0]].vNormal, XMLoadFloat3(&pVertices[0].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[2]].vNormal, XMLoadFloat3(&pVertices[2].vNormal) + vNormal);
			XMStoreFloat3(&pVertices[iIndices[3]].vNormal, XMLoadFloat3(&pVertices[3].vNormal) + vNormal);
		}
	}

	//pVertices�� ��� vNormal ���� ��� �븻������ ��Ų��.
	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3Normalize(XMLoadFloat3(&pVertices[i].vNormal)));
	}

	D3D11_BUFFER_DESC		VBDesc{};
	VBDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = 0;
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
	return S_OK;
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

CComponent* CVIBuffer_Terrain::Clone(void* pArg)
{
	CVIBuffer_Terrain* pInstance = new CVIBuffer_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CVIBuffer_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Terrain::Free()
{
	__super::Free();
}
