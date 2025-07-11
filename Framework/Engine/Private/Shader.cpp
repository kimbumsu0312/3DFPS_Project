#include "pch.h"
#include "Shader.h"

CShader::CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CComponent { pDevice, pContext}
{
}

CShader::CShader(const CShader& Prototype) : CComponent (Prototype) 
			, m_pEffect( Prototype.m_pEffect), m_iNumPasses( Prototype.m_iNumPasses)
			, m_InputLayouts { Prototype.m_InputLayouts }
{
	Safe_AddRef(m_pEffect);

	for (auto& pInputlayout : m_InputLayouts)
		Safe_AddRef(pInputlayout);
}

HRESULT CShader::Initialize_Prototype(const _tchar* pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
	_uint iHlslFlag = {};

#ifdef _DEBUG
	iHlslFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	iHlslFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1;
#endif

	//����Ʈ ���� �������� �Լ� (���̴� ���� �ּ�, ���̴� ���� ��ũ�� ������ nullptr, ��Ŭ��� ó�� �÷���, HIS �÷���, FX �÷��� = 0, ����̽�, ����Ʈ, ���� �޽��� �޸�)
	if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHlslFlag, 0, m_pDevice, &m_pEffect, nullptr)))
		return E_FAIL;

	//ID3DX11EffectTechnique : ���̴�, ����, �ؽ�ó �н� � ������ ���� �İ�ü
	//GetTechniqueByIndex(0) : ù ��° Technique
	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
		return E_FAIL;
	
	//��ũ���� ���� ����ü : �̸�, �н� ����
	D3DX11_TECHNIQUE_DESC TechniqueDesc{};
	pTechnique->GetDesc(&TechniqueDesc);

	m_iNumPasses = TechniqueDesc.Passes;
	
	for (_uint i = 0; i < m_iNumPasses; i++)
	{
		
		// i�� �н��� �����´�.
		ID3DX11EffectPass* pPass = pTechnique->GetPassByIndex(i);
		if (nullptr == pPass)
			return E_FAIL;

		// ������ �н��� ������ �����Ѵ�.
		D3DX11_PASS_DESC PassDesc{};
		pPass->GetDesc(&PassDesc);

		// ������ �н��� ��ǲ���̾ƿ��� ��´�.
		// pElements, PassDesc�� �̿��� ��ǲ �ñ״�ó�� �������� ��ǲ ���̾ƿ��� �����.
		// (POSITION, TEXCOORD �� �ø�Ʈ ���� ����Ʈ �ڵ�� ��� ����)
		ID3D11InputLayout* pInputLayout = { nullptr };
		if (FAILED(m_pDevice->CreateInputLayout(pElements, iNumElements, PassDesc.pIAInputSignature, PassDesc.IAInputSignatureSize, &pInputLayout)))
			return E_FAIL;
		
		m_InputLayouts.push_back(pInputLayout);
	}

	return S_OK;
}

HRESULT CShader::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CShader::Begin(_uint iPassIndex)
{
	if (iPassIndex >= m_iNumPasses)
		return E_FAIL;

	//0�� ��ũ���� ���� -> PassIndex���� �н��� ���� -> pass�� ���ǵ� ��� ���̴��� ���¸� GPU�� ����
	if (FAILED(m_pEffect->GetTechniqueByIndex(0)->GetPassByIndex(iPassIndex)->Apply(0, m_pContext)))
		return E_FAIL;

	//��ǲ ���̾ƿ� ����
	m_pContext->IASetInputLayout(m_InputLayouts[iPassIndex]);

	return S_OK;
}

HRESULT CShader::Bind_Matrix(const _char* pConstantName, const _float4x4* pMatirx)
{
	//���� ������ �̸��� ã�Ƽ� ����, ������ Nullptr ��ȯ
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	//����ȯ���� ��ķ� �ٲ�
	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if(nullptr == pMatrixVariable)
		return E_FAIL;
		
	//��Ʈ���� ����
	return pMatrixVariable->SetMatrix(reinterpret_cast<const _float*>(pMatirx));
}

HRESULT CShader::Bind_SRV(const _char* pConstantName, ID3D11ShaderResourceView* pSRV)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pSrvVariable = pVariable->AsShaderResource();
	if (nullptr == pSrvVariable)
		return E_FAIL;

	return pSrvVariable->SetResource(pSRV);
}

HRESULT CShader::Bind_RawValue(const _char* pConstantName, const void* pData, _uint iLength)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetRawValue(pData, 0, iLength);
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

	Safe_Release(m_pEffect);

	for (auto& pInputlayout : m_InputLayouts)
		Safe_Release(pInputlayout);
	m_InputLayouts.clear();
}
