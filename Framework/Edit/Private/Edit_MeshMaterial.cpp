#include "pch.h"
#include "Edit_MeshMaterial.h"

CEdit_MeshMaterial::CEdit_MeshMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : m_pDevice { pDevice }, m_pContext { pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CEdit_MeshMaterial::Initialize(const _char* pModelFilePath, const aiMaterial* pAIMaterial, SAVE_MODEL* ModelData)
{

	SAVE_MESHMATERIAL MeshMateriial;

	for (_uint i = 1; i < AI_TEXTURE_TYPE_MAX; ++i)
	{
		SAVE_MATERIAL Materail;
		_uint iNumTextures = Materail.iTexCount = pAIMaterial->GetTextureCount(static_cast<aiTextureType>(i));

		for (_uint j = 0; j < iNumTextures; ++j)
		{
			ID3D11ShaderResourceView* pSRV = { nullptr };

			aiString	strTexturePath;
			if (FAILED(pAIMaterial->GetTexture(static_cast<aiTextureType>(i), j, &strTexturePath)))
				break;
		
			_char szFullPath[MAX_PATH] = {};
			_char szDrive[MAX_PATH] = {};
			_char szDir[MAX_PATH] = {};
			_char szName[MAX_PATH] = {};
			_char szExt[MAX_PATH] = {};
			
			_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDir, MAX_PATH, nullptr, 0, nullptr, 0);
			_splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szName, MAX_PATH, szExt, MAX_PATH);
			
			strcpy_s(szFullPath, szDrive);
			strcat_s(szFullPath, szDir);
			strcat_s(szFullPath, szName);
			strcat_s(szFullPath, szExt);

			Materail.szFullPath.push_back(szFullPath);
			_tchar szFullPath_W[MAX_PATH]{};
			MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szFullPath_W, MAX_PATH);


			HRESULT hr = {};
			if (false == strcmp(szExt, ".tga"))
				return E_FAIL;
			else if (false == strcmp(szExt, ".dds"))
				hr = CreateDDSTextureFromFile(m_pDevice, szFullPath_W, nullptr, &pSRV);	//DDS 파일 로드
			else
				hr = CreateWICTextureFromFile(m_pDevice, szFullPath_W, nullptr, &pSRV);	//윈도우 이미지 확장자 파일 로그

			if (FAILED(hr))
				return E_FAIL;

			m_SRVs[i].push_back(pSRV);
		}
		MeshMateriial.Materials.push_back(Materail);
	}
	ModelData->MeshMaterials.push_back(MeshMateriial);
	return S_OK;
}

HRESULT CEdit_MeshMaterial::Bind_Shader_Resource(CShader* pShader, const _char* pConstantName, aiTextureType eTextureType, _uint iIndex)
{
	if (iIndex >= m_SRVs[eTextureType].size())
		return E_FAIL;

	return pShader->Bind_SRV(pConstantName, m_SRVs[eTextureType][iIndex]);
}

CEdit_MeshMaterial* CEdit_MeshMaterial::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath, const aiMaterial* pAIMaterial, SAVE_MODEL* ModelData)
{
	CEdit_MeshMaterial* pInstance = new CEdit_MeshMaterial(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pModelFilePath, pAIMaterial, ModelData)))
	{
		MSG_BOX(TEXT("Failed to Created : CEdit_MeshMaterial"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEdit_MeshMaterial::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	for (auto& SRVs : m_SRVs)
	{
		for (auto& pSRV : SRVs)
			Safe_Release(pSRV);
		SRVs.clear();
	}
}
