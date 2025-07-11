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

	//이펙트 파일 가져오는 함수 (셰이더 파일 주소, 셰이더 관련 매크로 없으면 nullptr, 인클루드 처리 플래그, HIS 플래그, FX 플래그 = 0, 디바이스, 이펙트, 에러 메시지 메모리)
	if (FAILED(D3DX11CompileEffectFromFile(pShaderFilePath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iHlslFlag, 0, m_pDevice, &m_pEffect, nullptr)))
		return E_FAIL;

	//ID3DX11EffectTechnique : 셰이더, 변수, 텍스처 패스 등에 접근을 위한 컴객체
	//GetTechniqueByIndex(0) : 첫 번째 Technique
	ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
	if (nullptr == pTechnique)
		return E_FAIL;
	
	//테크니컬 정보 구조체 : 이름, 패스 개수
	D3DX11_TECHNIQUE_DESC TechniqueDesc{};
	pTechnique->GetDesc(&TechniqueDesc);

	m_iNumPasses = TechniqueDesc.Passes;
	
	for (_uint i = 0; i < m_iNumPasses; i++)
	{
		
		// i번 패스를 가져온다.
		ID3DX11EffectPass* pPass = pTechnique->GetPassByIndex(i);
		if (nullptr == pPass)
			return E_FAIL;

		// 가져온 패스에 정보를 보관한다.
		D3DX11_PASS_DESC PassDesc{};
		pPass->GetDesc(&PassDesc);

		// 가져온 패스를 인풋레이아웃에 담는다.
		// pElements, PassDesc를 이용해 인풋 시그니처를 가져오고 인풋 레이아웃을 만든다.
		// (POSITION, TEXCOORD 등 시멘트 들이 바이트 코드로 담겨 있음)
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

	//0번 테크니컬 접근 -> PassIndex번의 패스의 접근 -> pass에 정의된 모든 셰이더와 상태를 GPU에 적용
	if (FAILED(m_pEffect->GetTechniqueByIndex(0)->GetPassByIndex(iPassIndex)->Apply(0, m_pContext)))
		return E_FAIL;

	//인풋 레이아웃 셋팅
	m_pContext->IASetInputLayout(m_InputLayouts[iPassIndex]);

	return S_OK;
}

HRESULT CShader::Bind_Matrix(const _char* pConstantName, const _float4x4* pMatirx)
{
	//전역 변수의 이름을 찾아서 접근, 없으면 Nullptr 반환
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	//형변환으로 행렬로 바꿈
	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if(nullptr == pMatrixVariable)
		return E_FAIL;
		
	//매트릭스 셋팅
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
