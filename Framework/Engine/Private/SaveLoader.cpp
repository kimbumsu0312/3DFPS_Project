#include "pch.h"
#include "SaveLoader.h"
#include "GameObject.h"
#include "VIBuffer_Terrain.h"
#include "Model.h"
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

HRESULT CSaveLoader::File_Save_Object(string szFilename, MODELTYPE eType, const SAVE_MODEL& pData)
{
	if (MODELTYPE::NONANIM == eType)
		Save_NonAnimMesh(szFilename, pData);
	else
		Save_AnimMesh(szFilename, pData);

	return S_OK;
}

HRESULT CSaveLoader::File_Save_AnimData(string szFilename, const vector<SAVE_ANIMDATA>& AnimDatas)
{
	string FilePath = "../Bin/Data/AnimData/" + szFilename + ".json";


	json jAnimData;

	for (auto& AnimData : AnimDatas)
	{
		json jData;

		jData["AnimeName"] = AnimData.szAnimName;
		jData["StartFrame"] = AnimData.iStartFrame;
		jData["EndFrame"] = AnimData.iEndFrame;
		jData["TickPerSecond"] = AnimData.fTickPerSecond;

		jAnimData["AnimData"].push_back(jData);
	}

	ofstream file(FilePath);
	if (file.is_open())
	{
		file << jAnimData.dump(4);
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

HRESULT CSaveLoader::LevelSave(string szFilename)
{
	ofstream out(szFilename, ios::binary);

	for (_int i = 0; i < m_Objects.size(); ++i)
	{
		OBJCET_DATA Data = m_Objects[i]->Get_Data();

		size_t szNameSize = Data.szObject_Path.size();
		out.write(reinterpret_cast<const _char*>(&szNameSize), sizeof(size_t));
		out.write(reinterpret_cast<const _char*>(Data.szObject_Path.c_str()), szNameSize * sizeof(_tchar));

		szNameSize = Data.szModel_Path.size();
		out.write(reinterpret_cast<const _char*>(&szNameSize), sizeof(size_t));
		out.write(reinterpret_cast<const _char*>(Data.szModel_Path.c_str()), szNameSize * sizeof(_tchar));

		XMFLOAT4X4 matrix;
		XMStoreFloat4x4(&matrix, Data.objmat);
		out.write(reinterpret_cast<const _char*>(&matrix), sizeof(DirectX::XMFLOAT4X4));
	}

	out.close();
	MSG_BOX(TEXT("Save"));

	return S_OK;
}

HRESULT CSaveLoader::Save_NonAnimMesh(string szFilename, const SAVE_MODEL& pData)
{
	string FilePath = "../Bin/Data/Object/" + szFilename + ".json";
	string MeshFilePath = "../Bin/Data/Object/" + szFilename + "Data.dat";

	ofstream out(MeshFilePath, ios::binary);

	if (!out)
	{
		MSG_BOX(TEXT("저장 파일 열기 실패"));
		return E_FAIL;
	}

	out.write(reinterpret_cast<const _char*>(&pData.PreTransformMatrix), sizeof(XMFLOAT4X4));
	size_t iMeshCount = pData.iNumMeshes;
	out.write(reinterpret_cast<const _char*>(&iMeshCount), sizeof(iMeshCount));
	
	for (const auto& mesh : pData.Meshs) 
	{
		size_t iNameSize = mesh.szName.size();
		out.write(reinterpret_cast<const _char*>(&iNameSize), sizeof(iNameSize));
		out.write(reinterpret_cast<const _char*>(mesh.szName.c_str()), iNameSize * sizeof(_tchar));

		out.write(reinterpret_cast<const _char*>(&mesh.iMaterialIndex), sizeof(mesh.iMaterialIndex));
		out.write(reinterpret_cast<const _char*>(&mesh.iNumVertices), sizeof(mesh.iNumVertices));
		out.write(reinterpret_cast<const _char*>(&mesh.iVertexStride), sizeof(mesh.iVertexStride));
		out.write(reinterpret_cast<const _char*>(&mesh.iNumIndices), sizeof(mesh.iNumIndices));
		out.write(reinterpret_cast<const _char*>(&mesh.iNumFaces), sizeof(mesh.iNumFaces));
		size_t ifaceCount = mesh.iFaces.size();
		out.write(reinterpret_cast<const _char*>(&ifaceCount), sizeof(ifaceCount));
		out.write(reinterpret_cast<const _char*>(mesh.iFaces.data()), ifaceCount * sizeof(Face));

		size_t ivertexCount = mesh.NonAnimVertex.size();
		out.write(reinterpret_cast<const _char*>(&ivertexCount), sizeof(ivertexCount));
		out.write(reinterpret_cast<const _char*>(mesh.NonAnimVertex.data()), ivertexCount * sizeof(VTXMESH));
	}
	out.close();

	json jData;

	jData["Model_name"] = pData.szName;
	jData["Model_type"] = pData.eModel;
	jData["Data_Path"] = MeshFilePath;
	jData["iNumMeshes"] = pData.iNumMeshes;
	jData["iNumMaterials"] = pData.iNumMaterials;

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

HRESULT CSaveLoader::Save_AnimMesh(string szFilename, const SAVE_MODEL& pData)
{
	string FilePath = "../Bin/Data/Charter/" + szFilename + ".json";
	string DataFilePath = "../Bin/Data/Charter/" + szFilename + "Data.dat";

	ofstream out(DataFilePath, ios::binary);

	if (!out)
	{
		MSG_BOX(TEXT("저장 파일 열기 실패"));
		return E_FAIL;
	}
	//트랜스폼 매트릭스 저장
	out.write(reinterpret_cast<const _char*>(&pData.PreTransformMatrix), sizeof(_float4x4));
	
	//매쉬 저장
	size_t iMeshCount = pData.iNumMeshes;
	out.write(reinterpret_cast<const _char*>(&iMeshCount), sizeof(iMeshCount));

	for (const auto& mesh : pData.Meshs)
	{
		size_t iNameSize = mesh.szName.size();
		out.write(reinterpret_cast<const _char*>(&iNameSize), sizeof(iNameSize));
		out.write(reinterpret_cast<const _char*>(mesh.szName.c_str()), iNameSize * sizeof(_tchar));

		out.write(reinterpret_cast<const _char*>(&mesh.iMaterialIndex), sizeof(mesh.iMaterialIndex));
		out.write(reinterpret_cast<const _char*>(&mesh.iNumVertices), sizeof(mesh.iNumVertices));
		out.write(reinterpret_cast<const _char*>(&mesh.iVertexStride), sizeof(mesh.iVertexStride));
		out.write(reinterpret_cast<const _char*>(&mesh.iNumIndices), sizeof(mesh.iNumIndices));
		out.write(reinterpret_cast<const _char*>(&mesh.iNumFaces), sizeof(mesh.iNumFaces));
		size_t ifaceCount = mesh.iFaces.size();
		out.write(reinterpret_cast<const _char*>(&ifaceCount), sizeof(ifaceCount));
		out.write(reinterpret_cast<const _char*>(mesh.iFaces.data()), ifaceCount * sizeof(Face));

		size_t ivertexCount = mesh.AnimVertex.size();
		out.write(reinterpret_cast<const _char*>(&ivertexCount), sizeof(ivertexCount));
		out.write(reinterpret_cast<const _char*>(mesh.AnimVertex.data()), ivertexCount * sizeof(VTXANIMMESH));

		out.write(reinterpret_cast<const _char*>(&mesh.iNumBones), sizeof(mesh.iNumBones));

		size_t iBoneIndicesCount = mesh.BoneIndices.size();
		out.write(reinterpret_cast<const _char*>(&iBoneIndicesCount), sizeof(iBoneIndicesCount));
		out.write(reinterpret_cast<const _char*>(mesh.BoneIndices.data()), iBoneIndicesCount * sizeof(_int));
	}

	//본 저장
	size_t iBoneCount = pData.iNumBone;
	out.write(reinterpret_cast<const _char*>(&iBoneCount), sizeof(iBoneCount));

	for (const auto& Bone : pData.Bones)
	{
		size_t iNameSize = Bone.szName.size();
		out.write(reinterpret_cast<const _char*>(&iNameSize), sizeof(iNameSize));
		out.write(reinterpret_cast<const _char*>(Bone.szName.c_str()), iNameSize * sizeof(_tchar));

		out.write(reinterpret_cast<const _char*>(&Bone.TransformationMatrix), sizeof(_float4x4));
		out.write(reinterpret_cast<const _char*>(&Bone.matOffset), sizeof(_float4x4));
		
		out.write(reinterpret_cast<const _char*>(&Bone.iParentBoneIndex),sizeof(Bone.iParentBoneIndex));
	}

	//애니메이션 저장
	size_t iAnimCount = pData.iNumAnimations;
	out.write(reinterpret_cast<const _char*>(&iAnimCount), sizeof(iAnimCount));

	for (const auto& Anim : pData.Animations)
	{
		out.write(reinterpret_cast<const _char*>(&Anim.iNumChannels), sizeof(Anim.iNumChannels));

		out.write(reinterpret_cast<const _char*>(&Anim.fDuration), sizeof(Anim.fDuration));
		out.write(reinterpret_cast<const _char*>(&Anim.fTickPerSecond), sizeof(Anim.fTickPerSecond));

		for (const auto& Channel : Anim.Channels)
		{
			out.write(reinterpret_cast<const _char*>(&Channel.iBoneIndex), sizeof(_int));
			out.write(reinterpret_cast<const _char*>(&Channel.iNumKeyFrames), sizeof(Channel.iNumKeyFrames));

			for (const auto& Keyframe : Channel.KeyFrames)
			{
				out.write(reinterpret_cast<const _char*>(&Keyframe.vScale), sizeof(_float3));
				out.write(reinterpret_cast<const _char*>(&Keyframe.vRotation), sizeof(_float4));
				out.write(reinterpret_cast<const _char*>(&Keyframe.vTranslation), sizeof(_float3));
				out.write(reinterpret_cast<const _char*>(&Keyframe.fTrackPosition), sizeof(_float));

			}
		}
	}
	out.close();

	json jData;

	jData["Model_name"] = pData.szName;
	jData["Model_type"] = pData.eModel;
	jData["Data_Path"] = DataFilePath;
	jData["iNumMaterials"] = pData.iNumMaterials;

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
	file.read((_char*)pOut.pVertexPositions.data(), sizeof(_float3) * iPosSize);

	_uint iTexSize = 0;
	file.read((_char*)&iTexSize, sizeof(_uint));
	pOut.pVertexTex.resize(iTexSize);
	file.read((_char*)pOut.pVertexTex.data(), sizeof(_float2) * iTexSize);

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
		file.read(reinterpret_cast<_char*>(&szNameSize), sizeof(size_t));
		if (szNameSize > 0)
		{
			wstring szObjPath;
			szObjPath.resize(szNameSize);
			file.read(reinterpret_cast<_char*>(&szObjPath[0]), szNameSize * sizeof(_tchar));
			Data.szObject_Path = szObjPath;
		}

		file.read(reinterpret_cast<_char*>(&szNameSize), sizeof(size_t));
		if (szNameSize > 0)
		{
			wstring szModelPath;
			szModelPath.resize(szNameSize);
			file.read(reinterpret_cast<_char*>(&szModelPath[0]), szNameSize * sizeof(_tchar));
			Data.szModel_Path = szModelPath;
		}

		_float4x4 matrix;
		file.read(reinterpret_cast<_char*>(&matrix), sizeof(_float4x4));
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

HRESULT CSaveLoader::Load_Objcet(string FilePath, _uint iPrototypeLevelIndex, _wstring szPrototypeTag)
{
	string DataFilePath = {};
	SAVE_MODEL ModelData = {};
	
	ifstream file(FilePath);
	if (!file.is_open())
	{
		MSG_BOX(TEXT("불러오기 실패"));
		return E_FAIL;
	}

	json jData;
	file >> jData;
	file.close();

	ModelData.szName = jData["Model_name"];
	ModelData.eModel = jData["Model_type"];
	ModelData.iNumMaterials = jData["iNumMaterials"];
	DataFilePath = jData["Data_Path"];

	for (int i = 0; i < ModelData.iNumMaterials; ++i)
	{
		SAVE_MESHMATERIAL MeshMaterial;
		const auto& jMeshMaterial = jData["MeshMaterial"][i];

		for (const auto& jMaterial : jMeshMaterial["Materials"])
		{
			SAVE_MATERIAL material;
			material.iTexCount = jMaterial["iTexCount"];

			for (int k = 0; k < material.iTexCount; ++k)
			{
				string path = jMaterial["szFullPath"][k]["Path"];
				material.szFullPath.push_back(path);
			}

			MeshMaterial.Materials.push_back(material);
		}

		ModelData.MeshMaterials.push_back(MeshMaterial);
	}
	
	ifstream DatFile(DataFilePath, ios::binary);
	if (!DatFile)
	{
		MSG_BOX(TEXT("데이터 파일 불러오기 실패"));
		return E_FAIL;
	}

	DatFile.read(reinterpret_cast<_char*>(&ModelData.PreTransformMatrix), sizeof(_float4x4));

	size_t iMeshCount = 0;
	DatFile.read(reinterpret_cast<_char*>(&iMeshCount), sizeof(size_t));
	ModelData.iNumMeshes = static_cast<_int>(iMeshCount);
	ModelData.Meshs.resize(iMeshCount);
	//매쉬 로드
	for (auto& mesh : ModelData.Meshs)
	{
		size_t nameSize = 0;
		DatFile.read(reinterpret_cast<_char*>(&nameSize), sizeof(size_t));

		_wstring name(nameSize, L'\0');
		DatFile.read(reinterpret_cast<_char*>(&name[0]), nameSize * sizeof(_tchar));
		mesh.szName = name;

		DatFile.read(reinterpret_cast<_char*>(&mesh.iMaterialIndex), sizeof(_uint));
		DatFile.read(reinterpret_cast<_char*>(&mesh.iNumVertices), sizeof(_uint));
		DatFile.read(reinterpret_cast<_char*>(&mesh.iVertexStride), sizeof(_uint));
		DatFile.read(reinterpret_cast<_char*>(&mesh.iNumIndices), sizeof(_uint));
		DatFile.read(reinterpret_cast<_char*>(&mesh.iNumFaces), sizeof(_uint));

		size_t faceCount = 0;
		DatFile.read(reinterpret_cast<_char*>(&faceCount), sizeof(size_t));
		mesh.iFaces.resize(faceCount);
		DatFile.read(reinterpret_cast<_char*>(mesh.iFaces.data()), faceCount * sizeof(Face));

		if (MODELTYPE::ANIM == ModelData.eModel)
		{
			size_t vertexCount = 0;
			DatFile.read(reinterpret_cast<_char*>(&vertexCount), sizeof(size_t));
			mesh.AnimVertex.resize(vertexCount);
			DatFile.read(reinterpret_cast<_char*>(mesh.AnimVertex.data()), vertexCount * sizeof(VTXANIMMESH));

			DatFile.read(reinterpret_cast<_char*>(&mesh.iNumBones), sizeof(_uint));

			size_t boneIndexCount = 0;
			DatFile.read(reinterpret_cast<_char*>(&boneIndexCount), sizeof(size_t));
			mesh.BoneIndices.resize(boneIndexCount);
			DatFile.read(reinterpret_cast<_char*>(mesh.BoneIndices.data()), boneIndexCount * sizeof(_int));
		}
		else
		{
			size_t vertexCount = 0;
			DatFile.read(reinterpret_cast<_char*>(&vertexCount), sizeof(size_t));
			mesh.NonAnimVertex.resize(vertexCount);
			DatFile.read(reinterpret_cast<_char*>(mesh.NonAnimVertex.data()), vertexCount * sizeof(VTXMESH));
		}
	}

	if (ModelData.eModel != MODELTYPE::ANIM)
	{
		DatFile.close();
		if (FAILED(m_pGameInstance->Add_Prototype(iPrototypeLevelIndex, szPrototypeTag, CModel::Create(m_pDevice, m_pContext, ModelData))))
		{
			MSG_BOX(TEXT("모델 로드 실패"));
			return E_FAIL;
		}
		return S_OK;
	}

	//뼈 로드
	size_t iBoneCount = 0;
	DatFile.read(reinterpret_cast<_char*>(&iBoneCount), sizeof(size_t));
	ModelData.iNumBone = static_cast<_int>(iBoneCount);
	ModelData.Bones.resize(iBoneCount);
	
	for (auto& Bone : ModelData.Bones)
	{
		size_t nameSize = 0;
		DatFile.read(reinterpret_cast<_char*>(&nameSize), sizeof(size_t));

		vector<_tchar> buffer(nameSize);
		DatFile.read(reinterpret_cast<_char*>(buffer.data()), nameSize * sizeof(_tchar));
		Bone.szName.assign(buffer.data(), nameSize);

		DatFile.read(reinterpret_cast<_char*>(&Bone.TransformationMatrix), sizeof(_float4x4));
		DatFile.read(reinterpret_cast<_char*>(&Bone.matOffset), sizeof(_float4x4));
		DatFile.read(reinterpret_cast<_char*>(&Bone.iParentBoneIndex), sizeof(_int));
	}

	//애니메이션 로드
	size_t iAnimCount = 0;
	DatFile.read(reinterpret_cast<_char*>(&iAnimCount), sizeof(size_t));
	ModelData.iNumAnimations = static_cast<_int>(iAnimCount);
	ModelData.Animations.resize(iAnimCount);

	for (auto& Anim : ModelData.Animations)
	{
		DatFile.read(reinterpret_cast<_char*>(&Anim.iNumChannels), sizeof(_int));
		DatFile.read(reinterpret_cast<_char*>(&Anim.fDuration), sizeof(_float));
		DatFile.read(reinterpret_cast<_char*>(&Anim.fTickPerSecond), sizeof(_float));

		Anim.Channels.resize(Anim.iNumChannels);
		for (auto& Channel : Anim.Channels)
		{
			DatFile.read(reinterpret_cast<_char*>(&Channel.iBoneIndex), sizeof(_int));
			DatFile.read(reinterpret_cast<_char*>(&Channel.iNumKeyFrames), sizeof(_int));

			Channel.KeyFrames.resize(Channel.iNumKeyFrames);
			for (auto& Keyframe : Channel.KeyFrames)
			{
				DatFile.read(reinterpret_cast<_char*>(&Keyframe.vScale), sizeof(_float3));
				DatFile.read(reinterpret_cast<_char*>(&Keyframe.vRotation), sizeof(_float4));
				DatFile.read(reinterpret_cast<_char*>(&Keyframe.vTranslation), sizeof(_float3));
				DatFile.read(reinterpret_cast<_char*>(&Keyframe.fTrackPosition), sizeof(_float));
			}
		}
	}

	DatFile.close();

	if (FAILED(m_pGameInstance->Add_Prototype(iPrototypeLevelIndex, szPrototypeTag, CModel::Create(m_pDevice, m_pContext, ModelData))))
	{
		MSG_BOX(TEXT("모델 데이터 로딩 실패"));
		return E_FAIL;
	}
	return S_OK;
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
