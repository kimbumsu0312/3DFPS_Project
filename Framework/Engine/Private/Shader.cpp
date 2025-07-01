#include "pch.h"
#include "Shader.h"

CShader::CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CComponent { pDevice, pContext}
{
}

CShader::CShader(const CShader& Prototype) : CComponent (Prototype)
{
}

HRESULT CShader::Initialize_Prototype(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
	return S_OK;
}

HRESULT CShader::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CShader::Begin()
{
	return S_OK;
}

CShader* CShader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
	CShader* pInstance = new CShader(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath, pElements, iNumElements)))
	{
		MSG_BOX(TEXT("Failed To Created : CShader"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

CComponent* CShader::Clone(void* pArg)
{
	CShader* pInstace = new CShader(*this);

	if (FAILED(pInstace->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed To Clone : CShader"));
		Safe_Release(pInstace);
	}

	return pInstace;
}

void CShader::Free()
{
	__super::Free();
}
