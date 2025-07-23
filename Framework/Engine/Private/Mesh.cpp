#include "pch.h"
#include "Mesh.h"
#include "Bone.h"

CMesh::CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CVIBuffer{ pDevice, pContext}
{
}

CMesh::CMesh(const CMesh& Prototype) : CVIBuffer( Prototype )
{
}

HRESULT CMesh::Initialize_Prototype(MODELTYPE eType, const SAVE_MESH& pMesh, const vector<class CBone*>& Bones, const vector<SAVE_BONE>& BonesData)
{
	m_szName = pMesh.szName;

	m_iMaterialIndex = pMesh.iMaterialIndex;
	m_iNumVertices = pMesh.iNumVertices;
	m_iNumIndices = pMesh.iNumIndices;
	m_iIndexStride = 4;
	m_iNumVertexBuffers = 1;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	HRESULT			hr = MODELTYPE::NONANIM == eType ?
		Ready_Vertices_For_NonAnim(pMesh) :
		Ready_Vertices_For_Anim(pMesh, Bones, BonesData);

	if (FAILED(hr))
		return E_FAIL;


	D3D11_BUFFER_DESC		IBDesc{};
	IBDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
	IBDesc.Usage = D3D11_USAGE_DEFAULT;
	IBDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	IBDesc.CPUAccessFlags = 0;
	IBDesc.MiscFlags = 0;
	IBDesc.StructureByteStride = m_iIndexStride;

	_uint* pIndices = new _uint[m_iNumIndices];

	_uint	iNumIndices = {};

	for (size_t i = 0; i < pMesh.iNumFaces; i++)
	{
		Face Face = pMesh.iFaces[i];

		pIndices[iNumIndices++] = Face.iIndices[0];
		pIndices[iNumIndices++] = Face.iIndices[1];
		pIndices[iNumIndices++] = Face.iIndices[2];
	}

	D3D11_SUBRESOURCE_DATA	IBInitialData{};
	IBInitialData.pSysMem = pIndices;

	if (FAILED(m_pDevice->CreateBuffer(&IBDesc, &IBInitialData, &m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

	return S_OK;
}

HRESULT CMesh::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT CMesh::Bind_BoneMatrices(CShader* pShader, const _char* pConstantName, const vector<class CBone*>& Bones)
{
	for (size_t i = 0; i < m_iNumBones; i++)
	{
		XMStoreFloat4x4(&m_BoneMatrices[i],
			XMLoadFloat4x4(&m_OffsetMatrices[i]) * Bones[m_BoneIndices[i]]->Get_CombinedTransformationMatrix());
	}

	return pShader->Bind_Matrices(pConstantName, m_BoneMatrices, m_iNumBones);
}

HRESULT CMesh::Ready_Vertices_For_NonAnim(const SAVE_MESH& pMesh)
{
	m_iVertexStride = sizeof(VTXMESH);

	D3D11_BUFFER_DESC		VBDesc{};
	VBDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;
	VBDesc.StructureByteStride = m_iVertexStride;

	VTXMESH* pVertices = new VTXMESH[m_iNumVertices];

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		pVertices[i].vPosition = pMesh.NonAnimVertex[i].vPosition;
		pVertices[i].vNormal = pMesh.NonAnimVertex[i].vNormal;
		pVertices[i].vTangent = pMesh.NonAnimVertex[i].vTangent;
		pVertices[i].vBinormal = pMesh.NonAnimVertex[i].vBinormal;
		pVertices[i].vTexcoord = pMesh.NonAnimVertex[i].vTexcoord;
	}

	D3D11_SUBRESOURCE_DATA	VBInitialData{};
	VBInitialData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &VBInitialData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CMesh::Ready_Vertices_For_Anim(const SAVE_MESH& pMesh, const vector<CBone*>& Bones, const vector<SAVE_BONE>& BonesData)
{
	m_iVertexStride = sizeof(VTXANIMMESH);

	D3D11_BUFFER_DESC		VBDesc{};
	VBDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	VBDesc.Usage = D3D11_USAGE_DEFAULT;
	VBDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VBDesc.CPUAccessFlags = 0;
	VBDesc.MiscFlags = 0;
	VBDesc.StructureByteStride = m_iVertexStride;

	VTXANIMMESH* pVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		pVertices[i].vPosition = pMesh.AnimVertex[i].vPosition;
		pVertices[i].vNormal = pMesh.AnimVertex[i].vNormal;
		pVertices[i].vTangent = pMesh.AnimVertex[i].vTangent;
		pVertices[i].vBinormal = pMesh.AnimVertex[i].vBinormal;
		pVertices[i].vTexcoord = pMesh.AnimVertex[i].vTexcoord;

		pVertices[i].vBlendIndex = pMesh.AnimVertex[i].vBlendIndex;
		pVertices[i].vBlendWeight = pMesh.AnimVertex[i].vBlendWeight;
	}

	m_iNumBones = pMesh.iNumBones;

	/*이 메시에 영향을 주는 뼈들을 하나씩 순회한다. */
	for (size_t i = 0; i < m_iNumBones; i++)
	{
		m_BoneIndices.push_back(pMesh.BoneIndices[i]);
		m_OffsetMatrices.push_back(BonesData[pMesh.BoneIndices[i]].matOffset);
	}

	if (0 == m_iNumBones)
	{
		m_iNumBones = 1;

		_uint	iBoneIndex = { 0 };

		auto	iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pBone)->_bool
			{
				if (true == pBone->Compare_Name(m_szName))
					return true;

				iBoneIndex++;
				return false;
			});

		m_BoneIndices.push_back(iBoneIndex);

		_float4x4		OffsetMatrix;
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());

		m_OffsetMatrices.push_back(OffsetMatrix);
	}

	D3D11_SUBRESOURCE_DATA	VBInitialData{};
	VBInitialData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &VBInitialData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

CMesh* CMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eType, const SAVE_MESH& pMesh, const vector<class CBone*>& Bones, const vector<SAVE_BONE>& BonesData)
{
	CMesh* pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType, pMesh, Bones, BonesData)))
	{
		MSG_BOX(TEXT("Failed to Created : CMesh"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CMesh::Clone(void* pArg)
{
	CMesh* pInstance = new CMesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMesh"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMesh::Free()
{
	__super::Free();
}
