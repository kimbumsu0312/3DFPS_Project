#include "pch.h"
#include "Edit_Mesh.h"

CEdit_Mesh::CEdit_Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{ pDevice, pContext }
{
}

CEdit_Mesh::CEdit_Mesh(const CEdit_Mesh& Prototype)
	: CVIBuffer( Prototype )
{
}

HRESULT CEdit_Mesh::Initialize_Prototype(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix, SAVE_MODEL* pModelData)
{
	SAVE_MESH Mesh;

	m_iMaterialIndex  = Mesh.iMaterialIndex = pAIMesh->mMaterialIndex;
	m_iNumVertices = Mesh.iNumVertices = pAIMesh->mNumVertices;
	m_iVertexStride = Mesh.iVertexStride = sizeof(VTXMESH);
	m_iNumIndices = Mesh.iNumIndices = pAIMesh->mNumFaces * 3;
	m_iIndexStride = 4;
	m_iNumVertexBuffers = 1;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	D3D11_BUFFER_DESC		VBDesc{};
	VBDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;
	VBDesc.StructureByteStride = m_iVertexStride;

	VTXMESH* pVertices = new VTXMESH[m_iNumVertices];
	m_pIndices = new _uint[m_iNumIndices];
	ZeroMemory(m_pIndices, sizeof(_uint) * m_iNumIndices);

	m_pVertexPositions = new _float3[m_iNumVertices];
	ZeroMemory(m_pVertexPositions, sizeof(_float3) * m_iNumVertices);


	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PreTransformMatrix));
		m_pVertexPositions[i] = pVertices[i].vPosition;
		Mesh.vPosition.push_back(pVertices[i].vPosition);

		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PreTransformMatrix));
		Mesh.vNormal.push_back(pVertices[i].vNormal);


		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
		Mesh.vTangent.push_back(pVertices[i].vTangent);

		memcpy(&pVertices[i].vBinormal, &pAIMesh->mBitangents[i], sizeof(_float3));
		Mesh.vBinormal.push_back(pVertices[i].vBinormal);

		memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		Mesh.vTexcoord.push_back(pVertices[i].vTexcoord);
	}

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

	_uint* pIndices = new _uint[m_iNumIndices];

	_uint	iNumIndices = {};

	Mesh.iNumFaces = pAIMesh->mNumFaces;

	for (size_t i = 0; i < pAIMesh->mNumFaces; i++)
	{
		aiFace AIFace = pAIMesh->mFaces[i];
		Face Indices;

		pIndices[iNumIndices++] = Indices.iIndices[0] = AIFace.mIndices[0];
		pIndices[iNumIndices++] = Indices.iIndices[1] = AIFace.mIndices[1];
		pIndices[iNumIndices++] = Indices.iIndices[2] = AIFace.mIndices[2];
		Mesh.iFaces.push_back(Indices);
	}

	D3D11_SUBRESOURCE_DATA	IBInitialData{};
	IBInitialData.pSysMem = pIndices;
	memcpy(m_pIndices, pIndices, sizeof(_uint) * m_iNumIndices);
	if (FAILED(m_pDevice->CreateBuffer(&IBDesc, &IBInitialData, &m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

	pModelData->Meshs.push_back(Mesh);

	return S_OK;
}

HRESULT CEdit_Mesh::Initialize(void* pArg)
{
	return S_OK;
}



CEdit_Mesh* CEdit_Mesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix, SAVE_MODEL* pModelData)
{
	CEdit_Mesh* pInstance = new CEdit_Mesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pAIMesh, PreTransformMatrix, pModelData)))
	{
		MSG_BOX(TEXT("Failed to Created : CEdit_Mesh"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CEdit_Mesh::Clone(void* pArg)
{
	CEdit_Mesh* pInstance = new CEdit_Mesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CEdit_Mesh"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEdit_Mesh::Free()
{
	__super::Free();


}
