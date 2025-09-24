#include "pch.h"
#include "Target_Manager.h"
#include "RenderTarget.h"

CTarget_Manager::CTarget_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : m_pDevice{pDevice}, m_pContext{pContext}
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CTarget_Manager::Initialize()
{
	return S_OK;
}

HRESULT CTarget_Manager::Add_RenderTarget(const _wstring& strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
	//·£ÅÍ Å¸°ÙÀ» »ý¼º ¹× ÀúÀå
	if (nullptr != Find_RenderTarget(strTargetTag))
		return E_FAIL;

	CRenderTarget* pRenderTarget = CRenderTarget::Create(m_pDevice, m_pContext, iSizeX, iSizeY, ePixelFormat, vClearColor);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	m_RenderTargets.emplace(strTargetTag, pRenderTarget);

	return S_OK;
}

HRESULT CTarget_Manager::Bind_ShaderResource(const _wstring& strTargetTag, CShader* pShader, const _char* pConstantName)
{
	//¼ÎÀÌ´õ¿¡ ¼ÎÀÌ´õ ¸®¼Ò½º ºä¸¦ ¹ÙÀÎµå
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Bind_ShaderResource(pShader, pConstantName);
}

HRESULT CTarget_Manager::Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag)
{
	//·£´õ Å¸°ÙÀ» ¸ÖÆ¼ ·£´õ Å¸°Ù¿¡ ¹­´Â´Ù.
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);
	if (pMRTList == nullptr)
	{
		list<CRenderTarget*> MRTList;
		MRTList.push_back(pRenderTarget);

		m_MRTs.emplace(strMRTTag, MRTList);
	}
	else
		pMRTList->push_back(pRenderTarget);

	Safe_AddRef(pRenderTarget);

	return S_OK;
}

HRESULT CTarget_Manager::Begin_MRT(const _wstring& strMRTTag, ID3D11DepthStencilView* pDSV, _bool isClear)
{
	ID3D11ShaderResourceView* pSRV[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
	m_pContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pSRV);

	//¸ÖÆ¼ ·£´õ Å¸°ÙÀ» Ã£¾Æ¼­
	list<CRenderTarget*>* pMRTList = Find_MRT(strMRTTag);
	if (pMRTList == nullptr)
		return E_FAIL;

	//ÇöÀç ·£´õ Å¸°Ù°ú ±íÀÌ ½ºÅÙ½Ç ºä¸¦ °¡Á®¿Â´Ù.
	m_pContext->OMGetRenderTargets(1, &m_pBackBuffer, &m_pOriginalDSV);

	_uint iNumRenderTargets = {};

	ID3D11RenderTargetView* RenderTargets[8] = { nullptr };
	
	for (auto& pRenderTarget : *pMRTList)
	{
		if(true == isClear)
			pRenderTarget->Clear();

		RenderTargets[iNumRenderTargets++] = pRenderTarget->Get_RTV();
	}
	
	if (nullptr != pDSV)
	{
		m_pContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0.f);
		m_pContext->OMSetRenderTargets(iNumRenderTargets, RenderTargets, pDSV);
	}
	else//·£´õ Å¸°Ù°ú ±íÀÌ ½ºÅÙ½Ç ºä¸¦ ¼ÂÆÃÇÑ´Ù.
	{
		m_pContext->OMSetRenderTargets(iNumRenderTargets, RenderTargets, m_pOriginalDSV);
	}
	return S_OK;
}

HRESULT CTarget_Manager::End_MRT()
{
	//ÀúÀåÇØµÐ ·£´õ Å¸°Ù°ú ±íÀÌ ½ºÅÙ½Ç ºä¸¦ ´Ù½Ã ¼ÂÆÃÇÑ´Ù.
	m_pContext->OMSetRenderTargets(1, &m_pBackBuffer, m_pOriginalDSV);

	Safe_Release(m_pOriginalDSV);
	Safe_Release(m_pBackBuffer);

	return S_OK;
}

HRESULT CTarget_Manager::Copy_Resource(const _wstring& strTargetTag, ID3D11Texture2D* pSourTexture)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	return pRenderTarget->Copy_Resource(pSourTexture);
}

#ifdef _DEBUG
HRESULT CTarget_Manager::Ready_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	CRenderTarget* pRenderTarget = Find_RenderTarget(strTargetTag);
	if (nullptr == pRenderTarget)
		return E_FAIL;

	return  pRenderTarget->Ready_Debug(fX, fY, fSizeX, fSizeY);
}

HRESULT CTarget_Manager::Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	for (auto& Pair : m_MRTs)
	{
		for (auto& pRenderTarget : Pair.second)
		{
			if (nullptr != pRenderTarget)
				pRenderTarget->Render(pShader, pVIBuffer);
		}
	}
	return S_OK;
}
#endif // _DEBUG

CRenderTarget* CTarget_Manager::Find_RenderTarget(const _wstring& strTargetTag)
{
	auto iter = m_RenderTargets.find(strTargetTag);

	if (iter == m_RenderTargets.end())
		return nullptr;

	return iter->second;
}

list<class CRenderTarget*>* CTarget_Manager::Find_MRT(const _wstring& strMRTTag)
{
	auto iter = m_MRTs.find(strMRTTag);

	if (iter == m_MRTs.end())
		return nullptr;

	return &iter->second;
}

CTarget_Manager* CTarget_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTarget_Manager* pInstance = new CTarget_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CTarget_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTarget_Manager::Free()
{
	__super::Free();

	for (auto& Pair : m_MRTs)
	{
		for (auto& pRenderTarget : Pair.second)
			Safe_Release(pRenderTarget);
		Pair.second.clear();
	}
	m_MRTs.clear();

	for (auto Pair : m_RenderTargets)
		Safe_Release(Pair.second);
	m_RenderTargets.clear();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
