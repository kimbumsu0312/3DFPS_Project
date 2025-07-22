#include "pch.h"
#include "SaveLoader.h"
#include "GameObject.h"
#include "VIBuffer_Terrain.h"

CSaveLoader::CSaveLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : m_pDevice(pDevice), m_pContext(pContext), m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CSaveLoader::File_Save_TerrainLevel(DATA_TYPE eData, string szFilename, CVIBuffer* pVIBuffer)
{
	string FilePath;
	switch (eData)
	{
	case Engine::DATA_TYPE::TERRAIN:
		FilePath = "../Bin/Data/Terrain/" + szFilename + ".dat";
		break;
	case Engine::DATA_TYPE::LEVEL:
		FilePath = "../Bin/Data/Level/" + szFilename + ".dat";
		break;
	}

	switch (eData)
	{
	case Engine::DATA_TYPE::TERRAIN:
		if (FAILED(TerrainSave(FilePath, pVIBuffer)))
			return E_FAIL;
		break;
	case Engine::DATA_TYPE::LEVEL:
		if (FAILED(LevelSave(FilePath)))
			return E_FAIL;
		break;
	}

	return S_OK;
}

void CSaveLoader::Clear_Object()
{
	for (auto& pObject : m_Objects)
	{
		pObject->SetDead();
		Safe_Release(pObject);
	}
	m_Objects.clear();
}

HRESULT CSaveLoader::TerrainSave(string FileName, CVIBuffer* pVIBuffer)
{
	SAVE_TERRAIN SaveData = static_cast<CVIBuffer_Terrain*>(pVIBuffer)->Save_Terrain();

	ofstream out(FileName, ios::binary);

	if (!out)
	{
		MSG_BOX(TEXT("로드 실패"));
		return E_FAIL;
	}
	out.write((_char*)&SaveData.iNumverticesX, sizeof(_uint));
	out.write((_char*)&SaveData.iNumverticesZ, sizeof(_uint));

	_uint	iPosSize = SaveData.pVertexPositions.size();
	out.write((_char*)&iPosSize, sizeof(_uint));
	out.write((_char*)SaveData.pVertexPositions.data(), sizeof(XMFLOAT3) * iPosSize);

	_uint	iTexSize = SaveData.pVertexTex.size();
	out.write((_char*)&iTexSize, sizeof(_uint));
	out.write((_char*)SaveData.pVertexTex.data(), sizeof(XMFLOAT2) * iTexSize);

	out.close();
	MSG_BOX(TEXT("Save"));

	return S_OK;
}

HRESULT CSaveLoader::File_Save_Object(string szFilename, MODELTYPE eType, SAVE_MODEL* pData)
{
	if (MODELTYPE::NONANIM == eType)
		Save_NonAnimMesh(szFilename, pData);
	else
		Save_AnimMesh(szFilename, pData);

	return S_OK;
}

HRESULT CSaveLoader::LevelSave(string szFilename)
{
	ofstream out(szFilename, ios::binary);

	for (_int i = 0; i < m_Objects.size(); ++i)
	{
		OBJCET_DATA Data = m_Objects[i]->Get_Data();

		size_t szNameSize = Data.szObject_Path.size();
		out.write(reinterpret_cast<const char*>(&szNameSize), sizeof(size_t));
		out.write(reinterpret_cast<const char*>(Data.szObject_Path.c_str()), szNameSize * sizeof(wchar_t));

		szNameSize = Data.szModel_Path.size();
		out.write(reinterpret_cast<const char*>(&szNameSize), sizeof(size_t));
		out.write(reinterpret_cast<const char*>(Data.szModel_Path.c_str()), szNameSize * sizeof(wchar_t));

		XMFLOAT4X4 matrix;
		XMStoreFloat4x4(&matrix, Data.objmat);
		out.write(reinterpret_cast<const char*>(&matrix), sizeof(DirectX::XMFLOAT4X4));
	}

	out.close();
	MSG_BOX(TEXT("Save"));

	return S_OK;
}

HRESULT CSaveLoader::Save_NonAnimMesh(string szFilename, SAVE_MODEL* pData)
{
	string FilePath = "../Bin/Data/Object/" + szFilename + ".json";
	string MeshFilePath = "../Bin/Data/Object/" + szFilename + "Data.dat";

	ofstream out(MeshFilePath, ios::binary);

	if (!out)
	{
		MSG_BOX(TEXT("저장 파일 열기 실패"));
		return E_FAIL;
	}

	out.write(reinterpret_cast<const char*>(&pData->PreTransformMatrix), sizeof(XMFLOAT4X4));
	size_t iMeshCount = pData->iNumMeshes;
	out.write(reinterpret_cast<const char*>(&iMeshCount), sizeof(iMeshCount));
	
	for (const auto& mesh : pData->Meshs) 
	{
		size_t iNameSize = mesh.szName.size();
		out.write(reinterpret_cast<const char*>(&iNameSize), sizeof(iNameSize));
		out.write(reinterpret_cast<const char*>(mesh.szName.c_str()), iNameSize * sizeof(wchar_t));

		out.write(reinterpret_cast<const char*>(&mesh.iMaterialIndex), sizeof(mesh.iMaterialIndex));
		out.write(reinterpret_cast<const char*>(&mesh.iNumVertices), sizeof(mesh.iNumVertices));
		out.write(reinterpret_cast<const char*>(&mesh.iVertexStride), sizeof(mesh.iVertexStride));
		out.write(reinterpret_cast<const char*>(&mesh.iNumIndices), sizeof(mesh.iNumIndices));
		out.write(reinterpret_cast<const char*>(&mesh.iNumFaces), sizeof(mesh.iNumFaces));
		size_t ifaceCount = mesh.iFaces.size();
		out.write(reinterpret_cast<const char*>(&ifaceCount), sizeof(ifaceCount));
		out.write(reinterpret_cast<const char*>(mesh.iFaces.data()), ifaceCount * sizeof(Face));

		size_t ivertexCount = mesh.NonAnimVertex.size();
		out.write(reinterpret_cast<const char*>(&ivertexCount), sizeof(ivertexCount));
		out.write(reinterpret_cast<const char*>(mesh.NonAnimVertex.data()), ivertexCount * sizeof(VTXMESH));
	}
	out.close();

	json jData;

	jData["Model_name"] = pData->szName;
	jData["Model_path"] = pData->szModelPath;
	jData["Model_type"] = pData->eModel;
	jData["Mesh_Path"] = MeshFilePath;
	jData["iNumMeshes"] = pData->iNumMeshes;
	jData["iNumMaterials"] = pData->iNumMaterials;

	for (_int i = 0; i < pData->iNumMaterials; ++i)
	{
		json jMeshMaterial;
		for (_uint j = 0; j < pData->MeshMaterials[i].Materials.size(); ++j)
		{
			json jMaterial;
			jMaterial["iTexCount"] = pData->MeshMaterials[i].Materials[j].iTexCount;

			for (_uint k = 0; k < pData->MeshMaterials[i].Materials[j].iTexCount; ++j)
			{
				json szFullPath;
				szFullPath["Path"] = pData->MeshMaterials[i].Materials[j].szFullPath[k];

				jMaterial["szFullPath"].push_back(szFullPath);
			}

			jMeshMaterial["Materials"].push_back(jMaterial);
		}

		jData["MeshMaterial"].push_back(jMeshMaterial);
	}

	ofstream file(FilePath);
	if (file.is_open())
	{
		file << jData.dump(4);
		file.close();
		MSG_BOX(TEXT("데이터 저장 완료"));
	}
	else
	{
		MSG_BOX(TEXT("데이터 저장 실패"));
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSaveLoader::Save_AnimMesh(string szFilename, SAVE_MODEL* pData)
{
	string FilePath = "../Bin/Data/Object/" + szFilename + ".json";
	string MeshFilePath = "../Bin/Data/Object/" + szFilename + "Data.dat";

	ofstream out(MeshFilePath, ios::binary);

	if (!out)
	{
		MSG_BOX(TEXT("저장 파일 열기 실패"));
		return E_FAIL;
	}

	out.write(reinterpret_cast<const char*>(&pData->PreTransformMatrix), sizeof(XMFLOAT4X4));
	size_t iMeshCount = pData->iNumMeshes;
	out.write(reinterpret_cast<const char*>(&iMeshCount), sizeof(iMeshCount));

	for (const auto& mesh : pData->Meshs)
	{
		size_t iNameSize = mesh.szName.size();
		out.write(reinterpret_cast<const char*>(&iNameSize), sizeof(iNameSize));
		out.write(reinterpret_cast<const char*>(mesh.szName.c_str()), iNameSize * sizeof(wchar_t));

		out.write(reinterpret_cast<const char*>(&mesh.iMaterialIndex), sizeof(mesh.iMaterialIndex));
		out.write(reinterpret_cast<const char*>(&mesh.iNumVertices), sizeof(mesh.iNumVertices));
		out.write(reinterpret_cast<const char*>(&mesh.iVertexStride), sizeof(mesh.iVertexStride));
		out.write(reinterpret_cast<const char*>(&mesh.iNumIndices), sizeof(mesh.iNumIndices));
		out.write(reinterpret_cast<const char*>(&mesh.iNumFaces), sizeof(mesh.iNumFaces));
		size_t ifaceCount = mesh.iFaces.size();
		out.write(reinterpret_cast<const char*>(&ifaceCount), sizeof(ifaceCount));
		out.write(reinterpret_cast<const char*>(mesh.iFaces.data()), ifaceCount * sizeof(Face));

		size_t ivertexCount = mesh.AnimVertex.size();
		out.write(reinterpret_cast<const char*>(&ivertexCount), sizeof(ivertexCount));
		out.write(reinterpret_cast<const char*>(mesh.AnimVertex.data()), ivertexCount * sizeof(VTXANIMMESH));
	}
	out.close();

	json jData;

	jData["Model_name"] = pData->szName;
	jData["Model_path"] = pData->szModelPath;
	jData["Model_type"] = pData->eModel;
	jData["Mesh_Path"] = MeshFilePath;
	jData["iNumMeshes"] = pData->iNumMeshes;
	jData["iNumMaterials"] = pData->iNumMaterials;

	for (_int i = 0; i < pData->iNumMaterials; ++i)
	{
		json jMeshMaterial;
		for (_uint j = 0; j < pData->MeshMaterials[i].Materials.size(); ++j)
		{
			json jMaterial;
			jMaterial["iTexCount"] = pData->MeshMaterials[i].Materials[j].iTexCount;

			for (_uint k = 0; k < pData->MeshMaterials[i].Materials[j].iTexCount; ++j)
			{
				json szFullPath;
				szFullPath["Path"] = pData->MeshMaterials[i].Materials[j].szFullPath[k];

				jMaterial["szFullPath"].push_back(szFullPath);
			}

			jMeshMaterial["Materials"].push_back(jMaterial);
		}

		jData["MeshMaterial"].push_back(jMeshMaterial);
	}

	ofstream file(FilePath);
	if (file.is_open())
	{
		file << jData.dump(4);
		file.close();
		MSG_BOX(TEXT("데이터 저장 완료"));
	}
	else
	{
		MSG_BOX(TEXT("데이터 저장 실패"));
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CSaveLoader::Load_Terrain(string FilePath, SAVE_TERRAIN& pOut)
{
	string szFilePath;

	szFilePath = "../Bin/Data/Terrain/" + FilePath + ".dat";

	ifstream file(szFilePath, ios::binary);

	file.read((_char*)&pOut.iNumverticesX, sizeof(_uint));
	file.read((_char*)&pOut.iNumverticesZ, sizeof(_uint));

	_uint iPosSize = 0;
	file.read((_char*)&iPosSize, sizeof(_uint));
	pOut.pVertexPositions.resize(iPosSize);
	file.read((_char*)pOut.pVertexPositions.data(), sizeof(XMFLOAT3) * iPosSize);

	_uint iTexSize = 0;
	file.read((_char*)&iTexSize, sizeof(_uint));
	pOut.pVertexTex.resize(iTexSize);
	file.read((_char*)pOut.pVertexTex.data(), sizeof(XMFLOAT2) * iTexSize);

	file.close();

	return S_OK;
}

HRESULT CSaveLoader::Load_Level(string FilePath, _uint LevelIndex, _wstring szLayerTag, _uint iPrototypeLevelIndex)
{
	string szFilePath;

	szFilePath = "../Bin/Data/Level/" + FilePath + ".dat";

	ifstream file(szFilePath, ios::binary);
	if (!file.is_open())
		return E_FAIL;

	vector<OBJCET_DATA> OBjects_Data;

	while (!file.eof())
	{
		OBJCET_DATA Data;
		size_t szNameSize = 0;
		file.read(reinterpret_cast<char*>(&szNameSize), sizeof(size_t));
		if (szNameSize > 0)
		{
			wstring szObjPath;
			szObjPath.resize(szNameSize);
			file.read(reinterpret_cast<char*>(&szObjPath[0]), szNameSize * sizeof(wchar_t));
			Data.szObject_Path = szObjPath;
		}

		file.read(reinterpret_cast<char*>(&szNameSize), sizeof(size_t));
		if (szNameSize > 0)
		{
			wstring szModelPath;
			szModelPath.resize(szNameSize);
			file.read(reinterpret_cast<char*>(&szModelPath[0]), szNameSize * sizeof(wchar_t));
			Data.szModel_Path = szModelPath;
		}

		XMFLOAT4X4 matrix;
		file.read(reinterpret_cast<char*>(&matrix), sizeof(XMFLOAT4X4));
		Data.objmat = XMLoadFloat4x4(&matrix);

		OBjects_Data.push_back(Data);
	}
	file.close();

	for (_int i = 0; i < OBjects_Data.size(); ++i)
	{
		CGameObject::GAMEOBJECT_DESC Desc{};

		Desc.isLoad = true;
		Desc.szObject_Path = OBjects_Data[i].szObject_Path;
		Desc.szModel_Path = OBjects_Data[i].szModel_Path;
		XMStoreFloat4x4(&Desc.WolrdMatrix, OBjects_Data[i].objmat);

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LevelIndex, szLayerTag,
			iPrototypeLevelIndex, OBjects_Data[i].szObject_Path, &Desc)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CSaveLoader::Load_Objcet(string FilePath, _uint iLevelIndex, _wstring szLayerTag, _uint iPrototypeLevelIndex)
{

	return E_NOTIMPL;
}

HRESULT CSaveLoader::Add_SaveObject(CGameObject* pObject, _int& pOut)
{
	if (pObject == nullptr)
		return E_FAIL;

	m_Objects.push_back(pObject);
	Safe_AddRef(pObject);
	
	pOut = m_Objects.size() - 1;
	return S_OK;
}

HRESULT CSaveLoader::Erase_SaveObject(_int Index)
{
	Safe_Release(m_Objects[Index]);

	if (Index >= 0 && Index < m_Objects.size())
		m_Objects.erase(m_Objects.begin() + Index);
	else
		return E_FAIL;

	return S_OK;
}

CSaveLoader* CSaveLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new CSaveLoader(pDevice, pContext);
}

void CSaveLoader::Free()
{
	__super::Free();

	for (auto& pObject : m_Objects)
		Safe_Release(pObject);

	m_Objects.clear();
	
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);

}
