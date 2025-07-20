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

HRESULT CSaveLoader::File_Save(DATA_TYPE eData, string szFilename)
{
	string FilePath;
	switch (eData)
	{
	case Engine::DATA_TYPE::TERRAIN:
		FilePath = "../Bin/Data/Terrain/" + szFilename + ".json";
		break;
	case Engine::DATA_TYPE::OBJECT:
		FilePath = "../Bin/Data/Object/" + szFilename + ".json";
		break;
	case Engine::DATA_TYPE::CHARTER:
		FilePath = "../Bin/Data/Charter/" + szFilename + ".json";
		break;
	case Engine::DATA_TYPE::LEVEL:
		FilePath = "../Bin/Data/Level/" + szFilename + ".json";
		break;
	}

	switch (eData)
	{
	case Engine::DATA_TYPE::TERRAIN:
		if (Save_Terrain(FilePath))
			return E_FAIL;
		break;
	case Engine::DATA_TYPE::OBJECT:
		FilePath = "../Bin/Data/Object/" + szFilename + ".json";
		break;
	case Engine::DATA_TYPE::CHARTER:
		FilePath = "../Bin/Data/Charter/" + szFilename + ".json";
		break;
	case Engine::DATA_TYPE::LEVEL:
		FilePath = "../Bin/Data/Level/" + szFilename + ".json";
		break;
	}

	return S_OK;
}

HRESULT CSaveLoader::Save_Object(string szFilename, const SAVE_MODEL& pData)
{
	string FilePath = "../Bin/Data/Object/" + szFilename + ".json";
	json jData;

	jData["MODELTYPE"] = pData.eModel;
	jData["iNumMeshes"] = pData.iNumMeshes;
	jData["iNumMaterials"] = pData.iNumMaterials;

	for (_int i = 0; i < pData.iNumMeshes; ++i)
	{
		json jMesh;
		jMesh["iMaterialIndex"] = pData.Meshs[i].iMaterialIndex;
		jMesh["iNumVertices"] = pData.Meshs[i].iNumVertices;
		jMesh["iVertexStride"] = pData.Meshs[i].iVertexStride;
		jMesh["iNumIndices"] = pData.Meshs[i].iNumIndices;
		jMesh["iNumFaces"] = pData.Meshs[i].iNumFaces;
		for (_int j = 0; j < pData.Meshs[i].iNumFaces; ++j)
		{
			json Face;
			Face["iIndices"] = { {"0", pData.Meshs[i].iFaces[j].iIndices[0]}, {"1", pData.Meshs[i].iFaces[j].iIndices[1]}, {"2", pData.Meshs[i].iFaces[j].iIndices[2]} };
			jMesh["iFaces"].push_back(Face);
		}

		for (_uint j = 0; j < pData.Meshs[i].iNumVertices; ++j)
		{
			json jPos, jNor, jTan, jBinor, jTex;
			jPos["Pos"] = { {"x", pData.Meshs[i].vPosition[j].x}, {"y", pData.Meshs[i].vPosition[j].y}, {"z", pData.Meshs[i].vPosition[j].z}};
			jNor["Nor"] = { {"x", pData.Meshs[i].vNormal[j].x}, {"y", pData.Meshs[i].vNormal[j].y}, {"z", pData.Meshs[i].vNormal[j].z} };
			jTan["Tan"] = { {"x", pData.Meshs[i].vTangent[j].x}, {"y", pData.Meshs[i].vTangent[j].y}, {"z", pData.Meshs[i].vTangent[j].z} };
			jBinor["BiNor"] = { {"x", pData.Meshs[i].vBinormal[j].x}, {"y", pData.Meshs[i].vBinormal[j].y}, {"z", pData.Meshs[i].vBinormal[j].z} };
			jTex["Tex"] = { {"x", pData.Meshs[i].vTexcoord[j].x}, {"y", pData.Meshs[i].vTexcoord[j].y} };

			jMesh["vPosition"].push_back(jPos);
			jMesh["vNormal"].push_back(jNor);
			jMesh["vTangent"].push_back(jTan);
			jMesh["vBinormal"].push_back(jBinor);
			jMesh["vTexcoord"].push_back(jTex);
		}
		jData["Meshs"].push_back(jMesh);
	}
	for (_int i = 0; i < pData.iNumMaterials; ++i)
	{
		json jMeshMaterial;
		for (_uint j = 0; j < pData.MeshMaterials[i].Materials.size(); ++j)
		{
			json jMaterial;
			jMaterial["iTexCount"] = pData.MeshMaterials[i].Materials[j].iTexCount;

			for (_uint k = 0; k < pData.MeshMaterials[i].Materials[j].iTexCount; ++j)
			{
				json szFullPath;
				szFullPath["Path"] = pData.MeshMaterials[i].Materials[j].szFullPath[k];

				jMaterial["szFullPath"].push_back(szFullPath);
			}

			jMeshMaterial["Materials"].push_back(jMaterial);
		}

		jData["MeshMaterials"].push_back(jMeshMaterial);
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

void CSaveLoader::Add_OBjcet(DATA_TYPE eData, CGameObject* pGameObject)
{
	if (pGameObject == nullptr)
		return;

	m_vecObject[ENUM_CLASS(eData)].push_back(pGameObject);
	Safe_AddRef(pGameObject);
}

void CSaveLoader::Clear_Object(DATA_TYPE eData)
{
	for (auto& pObject : m_vecObject[ENUM_CLASS(eData)])
		Safe_Release(pObject);

	m_vecObject[ENUM_CLASS(eData)].clear();
}

HRESULT CSaveLoader::Save_Terrain(string FilePath)
{
	if (m_vecObject[ENUM_CLASS(DATA_TYPE::TERRAIN)].size() <= 0)
		return E_FAIL;
	
	SAVE_TERRAIN* SaveData = static_cast<SAVE_TERRAIN*>(m_vecObject[ENUM_CLASS(DATA_TYPE::TERRAIN)].front()->GetDesc());

	json jData2;
	jData2["iNumverticesX"] = SaveData->iNumverticesX;
	jData2["iNumverticesZ"] = SaveData->iNumverticesZ;

	_uint iNumVertices = SaveData->iNumverticesX * SaveData->iNumverticesZ;

	for (_uint i = 0; i < iNumVertices; ++i)
	{

		json jVertex;
		jVertex["Position"] = { {"x", SaveData->pVertexData[i].x}, {"y", SaveData->pVertexData[i].y}, {"z", SaveData->pVertexData[i].z} };
		jVertex["Texcoord"] = { {"u", SaveData->pTexcoordData[i].x}, {"v", SaveData->pTexcoordData[i].y}};

		jData2["VertexPos"].push_back(jVertex);
	}
	
	ofstream file(FilePath);
	if (file.is_open())
	{
		file << jData2.dump(4);
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
	ifstream file(FilePath);
	if (!file.is_open())
	{
		MSG_BOX(TEXT("파일 열기 실패"));
		return E_FAIL;
	}
	
	string szData((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
	file.close();

	try
	{
		json jData = json::parse(szData);

		pOut.iNumverticesX = jData["iNumverticesX"];
		pOut.iNumverticesZ = jData["iNumverticesZ"];

		_uint iNumVertices = pOut.iNumverticesX * pOut.iNumverticesZ;
		
		if (jData["VertexPos"].size() != iNumVertices)
		{
			MSG_BOX(TEXT("사이즈 불일치"));
			return E_FAIL;
		}

		pOut.pVertexData.clear();
		pOut.pTexcoordData.clear();
		pOut.pVertexData.reserve(iNumVertices);
		pOut.pTexcoordData.reserve(iNumVertices);

		for (_uint i = 0; i < iNumVertices; ++i)
		{
			const json& vertex = jData["VertexPos"][i];

			_float3 Pos = {};
			Pos.x = vertex["Position"]["x"];
			Pos.y = vertex["Position"]["y"];
			Pos.z = vertex["Position"]["z"];
			pOut.pVertexData.push_back(Pos);

			_float2 Tex = {};
			Tex.x = vertex["Texcoord"]["u"];
			Tex.y = vertex["Texcoord"]["v"];
			pOut.pTexcoordData.push_back(Tex);
		}
		
		return S_OK;
	}
	catch (json::parse_error& e)
	{
		MSG_BOX(TEXT("Json 파싱 오류"));
		return E_FAIL;
	}
	
}

CSaveLoader* CSaveLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return new CSaveLoader(pDevice, pContext);
}

void CSaveLoader::Free()
{
	__super::Free();
	for (size_t i = 0; i < ENUM_CLASS(DATA_TYPE::END); i++)
	{
		for (auto& pObject : m_vecObject[i])
			Safe_Release(pObject);

		m_vecObject[i].clear();
	}
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);

}
