#include "pch.h"
#include "Lut.h"

CLut::CLut()
{
}

HRESULT CLut::Set_TexPass(_int iPass)
{
	if (m_iMaxPass <= iPass)
		return E_FAIL;

	m_iTexPass = iPass;

	return S_OK;
}

HRESULT CLut::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTextureFilePath, _uint iNumTextures)
{
	m_iMaxPass = iNumTextures;
	m_pTexture = CTexture::Create(pDevice, pContext, pTextureFilePath, iNumTextures);
	
	if (m_pTexture == nullptr)
		E_FAIL;
		
	return S_OK;
}

HRESULT CLut::Bind_LutTexture(CShader* pShader, const _char* pConstantName)
{
	return m_pTexture->Bind_Shader_Resource(pShader, pConstantName, m_iTexPass);
}

CLut* CLut::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTextureFilePath, _uint iNumTextures)
{
	CLut* pInstance = new CLut();

	if (FAILED(pInstance->Initialize(pDevice, pContext, pTextureFilePath, iNumTextures)))
	{
		MSG_BOX(TEXT("Failed to Created : CLut"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLut::Free()
{
	__super::Free();
	Safe_Release(m_pTexture);
}
