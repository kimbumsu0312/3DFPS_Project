#include "pch.h"
#include "Texture.h"
#include "Shader.h"

CTexture::CTexture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CComponent { pDevice, pContext }
{
}

CTexture::CTexture(const CTexture& Prototype) : CComponent (Prototype), m_iNumTextures { Prototype.m_iNumTextures }, m_SRVs { Prototype.m_SRVs }
{
	for (auto& pSRV : m_SRVs)
		Safe_AddRef(pSRV);
}

HRESULT CTexture::Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures)
{
	m_iNumTextures = iNumTextures;

	//패스를 분류한다.
	//1.드라이브 2.디렉토리 3.이름 4.확장자
	_tchar szExt[MAX_PATH];

	//패스 파일 분리 함수
	_wsplitpath_s(pTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szExt, MAX_PATH);
	
	for (size_t i = 0; i < iNumTextures; i++)
	{
		_tchar szFullPath[MAX_PATH] = {  };
		//파일 패스와 인덱스를 합침
		wsprintf(szFullPath, pTextureFilePath, i);
		
		ID3D11ShaderResourceView* pSRV = { nullptr };

		HRESULT hr = {};

		if (false == lstrcmp(szExt, TEXT(".tga")))
			return E_FAIL;
		else if (false == lstrcmp(szExt, TEXT(".dds")))
			hr = CreateDDSTextureFromFile(m_pDevice, szFullPath, nullptr, &pSRV);	//DDS 파일 로드
		else
			hr = CreateWICTextureFromFile(m_pDevice, szFullPath, nullptr, &pSRV);	//윈도우 이미지 확장자 파일 로그

		if (FAILED(hr))
			return E_FAIL;

		m_SRVs.push_back(pSRV);
	}

	return S_OK;
}

HRESULT CTexture::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CTexture::Bind_Shader_Resource(CShader* pShader, const _char* pConstantName, _uint iTextureIndex)
{
	if (iTextureIndex >= m_iNumTextures)
		return E_FAIL;
	
	m_iLastTexIndex = iTextureIndex;
	return pShader->Bind_SRV(pConstantName, m_SRVs[iTextureIndex]);

}

HRESULT CTexture::Bind_Shader_Resource_IndexCheck(CShader* pShader, const _char* pConstantName, _uint iTextureIndex)
{
	if (iTextureIndex >= m_iNumTextures)
		return E_FAIL;

	if (m_iNumTextures == m_iLastTexIndex)
		return S_OK;

	m_iLastTexIndex = iTextureIndex;
	return pShader->Bind_SRV(pConstantName, m_SRVs[iTextureIndex]);
}

CTexture* CTexture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTextureFilePath, _uint iNumTextures)
{
	CTexture* pInstance = new CTexture(pDevice, pContext);
	
	if(FAILED(pInstance->Initialize_Prototype(pTextureFilePath, iNumTextures)))
	{
		MSG_BOX(TEXT("Failed to Created : CTexture"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CTexture::Clone(void* pArg)
{
	CTexture* pInstance = new CTexture(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CTexture"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTexture::Free()
{
	__super::Free();
	for (auto& pSRV : m_SRVs)
		Safe_Release(pSRV);

	m_SRVs.clear();
}
