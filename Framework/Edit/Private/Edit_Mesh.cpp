#include "pch.h"
#include "Edit_Mesh.h"
#include "Edit_Bone.h"
#include "Shader.h"

CEdit_Mesh::CEdit_Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{ pDevice, pContext }
{
}

CEdit_Mesh::CEdit_Mesh(const CEdit_Mesh& Prototype)
	: CVIBuffer( Prototype )
{
}

HRESULT CEdit_Mesh::Initialize_Prototype(MODELTYPE eType, const aiMesh* pAIMesh, const vector<class CEdit_Bone*>& Bones, _fmatrix PreTransformMatrix, SAVE_MODEL* pModelData)
{
	//매쉬 이름 복사
	strcpy_s(m_szName, pAIMesh->mName.data);
	m_PreTransformMatrix = PreTransformMatrix;
	int iLength = MultiByteToWideChar(CP_ACP, 0, m_szName, -1, NULL, 0);

	wstring szMeshName(iLength, L'\0');
	MultiByteToWideChar(CP_ACP, 0, m_szName, -1, &szMeshName[0], iLength);
	m_MeshData.szName = szMeshName;

	//매쉬 재질 인덱스 가져오기
	m_iMaterialIndex  = m_MeshData.iMaterialIndex = pAIMesh->mMaterialIndex;
	m_iNumVertices = m_MeshData.iNumVertices = pAIMesh->mNumVertices;
	m_iNumIndices = m_MeshData.iNumIndices = pAIMesh->mNumFaces * 3;
	m_iIndexStride = 4;
	m_iNumVertexBuffers = 1;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	//모델 타입에 따라 버텍스 셋팅
	HRESULT	hr = MODELTYPE::NONANIM == eType ?
		Ready_Vertices_For_NonAnim(pAIMesh, PreTransformMatrix) :
		Ready_Vertices_For_Anim(pAIMesh, Bones);

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

	m_MeshData.iNumFaces = pAIMesh->mNumFaces;
	m_pIndices = new _uint[m_iNumIndices];
	ZeroMemory(m_pIndices, sizeof(_uint) * m_iNumIndices);

	for (size_t i = 0; i < pAIMesh->mNumFaces; i++)
	{
		aiFace AIFace = pAIMesh->mFaces[i];
		Face Indices;

		pIndices[iNumIndices++] = Indices.iIndices[0] = AIFace.mIndices[0];
		pIndices[iNumIndices++] = Indices.iIndices[1] = AIFace.mIndices[1];
		pIndices[iNumIndices++] = Indices.iIndices[2] = AIFace.mIndices[2];
		m_MeshData.iFaces.push_back(Indices);
	}

	D3D11_SUBRESOURCE_DATA	IBInitialData{};
	IBInitialData.pSysMem = pIndices;
	
	memcpy(m_pIndices, pIndices, sizeof(_uint) * m_iNumIndices);
	if (FAILED(m_pDevice->CreateBuffer(&IBDesc, &IBInitialData, &m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);
	pModelData->Meshs.push_back(m_MeshData);

	return S_OK;
}

HRESULT CEdit_Mesh::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CEdit_Mesh::Bind_BoneMatrices(CShader* pShader, const _char* pConstantName, const vector<class CEdit_Bone*>& Bones)
{
	for (size_t i = 0; i < m_MeshData.iNumBones; i++)
	{
		//매쉬에 연결된 뼈들의 행렬을 구해준다.
		//초기 오프셋 행렬 * 뼈의 콤바인드 행렬
		XMStoreFloat4x4(&m_BoneMatrices[i],	XMLoadFloat4x4(&m_OffsetMatrices[i]) * Bones[m_BoneIndices[i]]->Get_CombinedTransformationMatrix());
	}

	//계산 해준 뼈의 행렬들을 바인드시킨다.
	return pShader->Bind_Matrices(pConstantName, m_BoneMatrices, m_MeshData.iNumBones);
}

_bool CEdit_Mesh::IsPicked(MODELTYPE eType, CTransform& pTransform, _float3& pOut)
{
	_float3	vLocalPickPos = { pOut };

	_vector vWolrdPickPos = {};
	_bool	IsPicked = false;

	if (D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST != m_ePrimitiveType)
		return false;

	m_pGameInstance->TransformToLocalSpace(pTransform);
	_uint* pIndices = reinterpret_cast<_uint*>(m_pIndices);

	for (_int i = 0; i < m_iNumIndices / 3; i++)
	{
		_uint i0 = pIndices[i * 3 + 0];
		_uint i1 = pIndices[i * 3 + 1];
		_uint i2 = pIndices[i * 3 + 2];
		
		if(MODELTYPE::NONANIM == eType)
			IsPicked = m_pGameInstance->isPickedInLocalSpace(m_MeshData.NonAnimVertex[i0].vPosition, m_MeshData.NonAnimVertex[i1].vPosition, m_MeshData.NonAnimVertex[i2].vPosition, vLocalPickPos);
		else
		{
			_float3 vPos0{}, vPos1{}, vPos2{};
			
			XMStoreFloat3(&vPos0, XMVector3TransformCoord(XMLoadFloat3(&m_MeshData.AnimVertex[i0].vPosition), m_PreTransformMatrix));
			XMStoreFloat3(&vPos1, XMVector3TransformCoord(XMLoadFloat3(&m_MeshData.AnimVertex[i1].vPosition), m_PreTransformMatrix));
			XMStoreFloat3(&vPos2, XMVector3TransformCoord(XMLoadFloat3(&m_MeshData.AnimVertex[i2].vPosition), m_PreTransformMatrix));

			IsPicked = m_pGameInstance->isPickedInLocalSpace(vPos0, vPos1, vPos2, vLocalPickPos);
		}
		if (IsPicked)
		{
			XMVECTOR vWorldPos = XMVector3TransformCoord(XMLoadFloat3(&vLocalPickPos), pTransform.Get_WorldMatrix());
			XMStoreFloat3(&pOut, vWorldPos);
			break;
		}

	}

	return IsPicked;


}

HRESULT CEdit_Mesh::Ready_Vertices_For_NonAnim(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix)
{
	m_iVertexStride = m_MeshData.iVertexStride = sizeof(VTXMESH);

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
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
		memcpy(&pVertices[i].vBinormal, &pAIMesh->mBitangents[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));


		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PreTransformMatrix));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PreTransformMatrix));
		m_MeshData.NonAnimVertex.push_back(pVertices[i]);
	}

	D3D11_SUBRESOURCE_DATA	VBInitialData{};
	VBInitialData.pSysMem = pVertices;

	if (FAILED(m_pDevice->CreateBuffer(&VBDesc, &VBInitialData, &m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	return S_OK;
}

HRESULT CEdit_Mesh::Ready_Vertices_For_Anim(const aiMesh* pAIMesh, const vector<class CEdit_Bone*>& Bones)
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
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
		memcpy(&pVertices[i].vBinormal, &pAIMesh->mBitangents[i], sizeof(_float3));
		memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));

		m_MeshData.AnimVertex.push_back(pVertices[i]);
	}

	//현재 메쉬에 연결된 뼈 개수를 가져옴
	m_MeshData.iNumBones = pAIMesh->mNumBones;
	
	for (size_t i = 0; i < m_MeshData.iNumBones; i++)
	{
		aiBone* pAIBone = pAIMesh->mBones[i];

		//뼈의 오프셋 매트릭스를 가져온다
		_float4x4	OffsetMatrix;
		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));

		//뼈의 오프셋 매트릭스 전치
		XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));
		m_OffsetMatrices.push_back(OffsetMatrix);

		//부모에서 받아온 뼈 기준으로 인덱스를 받아옴
		_uint	iBoneIndex = { 0 };
		auto	iter = find_if(Bones.begin(), Bones.end(), [&](CEdit_Bone* pBone)->_bool
			{
				if (true == pBone->Compare_Name(pAIBone->mName.data))
					return true;

				iBoneIndex++;

				return false;
			});

		m_BoneIndices.push_back(iBoneIndex);

		//뼈의 인덱스와 가중치를 받아온다.
		for (size_t j = 0; j < pAIBone->mNumWeights; j++)
		{
			aiVertexWeight	AIVertexWeight = pAIBone->mWeights[j];

			if (0.f == pVertices[AIVertexWeight.mVertexId].vBlendWeight.x)
			{
				pVertices[AIVertexWeight.mVertexId].vBlendIndex.x = i;
				pVertices[AIVertexWeight.mVertexId].vBlendWeight.x = AIVertexWeight.mWeight;
			}
			else if (0.f == pVertices[AIVertexWeight.mVertexId].vBlendWeight.y)
			{
				pVertices[AIVertexWeight.mVertexId].vBlendIndex.y = i;
				pVertices[AIVertexWeight.mVertexId].vBlendWeight.y = AIVertexWeight.mWeight;
			}
			else if (0.f == pVertices[AIVertexWeight.mVertexId].vBlendWeight.z)
			{
				pVertices[AIVertexWeight.mVertexId].vBlendIndex.z = i;
				pVertices[AIVertexWeight.mVertexId].vBlendWeight.z = AIVertexWeight.mWeight;
			}
			else
			{
				pVertices[AIVertexWeight.mVertexId].vBlendIndex.w = i;
				pVertices[AIVertexWeight.mVertexId].vBlendWeight.w = AIVertexWeight.mWeight;
			}
		}
	}

	//매쉬에 뼈가 0개인 경우
	if (0 == m_MeshData.iNumBones)
	{
		//임의에 뼈를 1개 만들어줌
		m_MeshData.iNumBones = 1;

		_uint	iBoneIndex = { 0 };
		
		//현재 모델에 매쉬 이름과 동일한 이름의 뼈가 있으면 해당 뼈 인덱스를 부여해준다.
		auto	iter = find_if(Bones.begin(), Bones.end(), [&](CEdit_Bone* pBone)->_bool
			{
				if (true == pBone->Compare_Name(m_szName))
					return true;

				iBoneIndex++;

				return false;
			});

		m_BoneIndices.push_back(iBoneIndex);
		
		//오프셋 행렬을 항등 행렬로 넣어준다.
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

CEdit_Mesh* CEdit_Mesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, MODELTYPE eType, const aiMesh* pAIMesh, const vector<class CEdit_Bone*>& Bones, _fmatrix PreTransformMatrix, SAVE_MODEL* pModelData)
{
	CEdit_Mesh* pInstance = new CEdit_Mesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eType, pAIMesh, Bones, PreTransformMatrix, pModelData)))
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
