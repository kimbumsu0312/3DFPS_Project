#include "pch.h"
#include "MeshMaterial.h"

CMeshMaterial::CMeshMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : m_pDevice{ pDevice }, m_pContext{ pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CMeshMaterial::Initialize(const SAVE_MESHMATERIAL& pMaterialData)
{
	for (size_t i = 0; i < pMaterialData.Materials.size(); i++)
	{
		_uint		iNumTextures = pMaterialData.Materials[i].iTexCount;

		for (size_t j = 0; j < iNumTextures; j++)
		{
			ID3D11ShaderResourceView* pSRV = { nullptr };

			const _char* szFilePath = pMaterialData.Materials[i].szFullPath[j].c_str();

			_char			szExt[MAX_PATH] = {};
			_splitpath_s(szFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);


			_tchar			szFileFullPath[MAX_PATH] = {};
			MultiByteToWideChar(CP_ACP, 0, szFilePath, strlen(szFilePath), szFileFullPath, MAX_PATH);


			HRESULT		hr = {};

			if (false == strcmp(".tga", szExt))
				hr = E_FAIL;

			if (false == strcmp(".dds", szExt))
				hr = CreateDDSTextureFromFile(m_pDevice, szFileFullPath, nullptr, &pSRV);
			else
				hr = CreateWICTextureFromFile(m_pDevice, szFileFullPath, nullptr, &pSRV);

			if (FAILED(hr))
				return E_FAIL;

			m_SRVs[i].push_back(pSRV);
		}
	}

	return S_OK;
}

HRESULT CMeshMaterial::Bind_Resources(CShader* pShader, const _char* pConstantName, _int iTexIndex, _uint iIndex)
{
	if (iIndex >= m_SRVs[iTexIndex].size())
		return E_FAIL;

	return pShader->Bind_SRV(pConstantName, m_SRVs[iTexIndex][iIndex]);
}

CMeshMaterial* CMeshMaterial::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const SAVE_MESHMATERIAL& pMaterialData)
{
	CMeshMaterial* pInstance = new CMeshMaterial(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pMaterialData)))
	{
		MSG_BOX(TEXT("Failed to Created : CMeshMaterial"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMeshMaterial::Free()
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
